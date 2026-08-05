#include "rafi.h"
#include "sadia.h"
#include "hira.h"
#include "prothoma.h"

/* ============================================================================
 * Section 1: RAFI (Storage, Portals & Core Operations)
 * ============================================================================ */

int get_int() {
    int val = 0;
    scanf("%d", &val);
    return val;
}

double get_dbl() {
    double val = 0.0;
    scanf("%lf", &val);
    return val;
}

void get_str(char *buf, int size) {
    (void)size;
    scanf(" %[^\n]", buf);
}

void initialize_files() {
    const char *files[] = {
        "students.txt", "rooms.txt", "fees.txt", "complaints.txt",
        "transfers.txt", "slips.txt", "leave_requests.txt",
        "staff.txt", "new_booking_requests.txt",
        "guests.txt", "meals.txt", "checkin.txt", "checkout.txt"
    };
    for (int i = 0; i < 13; i++) {
        FILE *f = fopen(files[i], "a");
        if (f) fclose(f);
    }
}

void pause_term() {
    printf("\nPress Enter to continue...");
    getchar(); getchar();
}

void clear_term() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void clear_screen() {
    clear_term();
}

void append_line(const char *file, const char *data) {
    FILE *f = fopen(file, "a");
    if (f) {
        fprintf(f, "%s\n", data);
        fclose(f);
    }
}

int get_next_id(const char *file) {
    FILE *f = fopen(file, "r");
    if (!f) return 1;
    char line[256];
    int max_id = 0, id = 0;
    while (fgets(line, 256, f)) {
        if (sscanf(line, "%d", &id) == 1 && id > max_id) {
            max_id = id;
        }
    }
    fclose(f);
    return max_id + 1;
}

void get_password_md5(const char *password, char hash_out[33]) {
    md5_hash(password, hash_out);
}

void admin_register_student() {
    int id, rm;
    char name[50], dept[50], phone[20], pass[20], hash[33], data[256];
    double fee;

    printf("Student ID: "); scanf("%d", &id);
    printf("Full Name: "); scanf(" %[^\n]", name);
    printf("Department: "); scanf("%s", dept);
    printf("Phone Number: "); scanf("%s", phone);
    printf("Password: "); scanf("%s", pass);
    printf("Room Number: "); scanf("%d", &rm);
    printf("Monthly Fee: "); scanf("%lf", &fee);

    md5_hash(pass, hash);

    sprintf(data, "%d;%s;%s;%s;%s;%d", id, name, dept, phone, hash, rm);
    append_line("students.txt", data);
    sprintf(data, "%d;%.2f;0.00;%.2f;Unpaid;%s", id, fee, fee, DEFAULT_DATE);
    append_line("fees.txt", data);
    printf("Student registered!\n");
}

void admin_search_student() {
    int search_id, id, rm, found = 0;
    char line[256], name[50], dept[50], phone[20], pass[33];

    printf("Enter Student ID: ");
    scanf("%d", &search_id);

    FILE *f = fopen("students.txt", "r");
    if (f) {
        while (fgets(line, 256, f)) {
            if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%32[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
                if (id == search_id) {
                    printf("\n--- Student Found ---\nID: %d | Name: %s | Dept: %s | Phone: %s | Room: %d\n", id, name, dept, phone, rm);
                    found = 1;
                    break;
                }
            }
        }
        fclose(f);
        if (!found) printf("Student ID #%d not found.\n", search_id);
    }
}

void admin_search_room() {
    int target_rm;
    printf("Enter Room Number: ");
    if (scanf("%d", &target_rm) != 1) {
        printf("Invalid Room Number!\n");
        return;
    }

    int capacity = -1;
    double price = 0.0;
    char line[256];

    FILE *rf = fopen("rooms.txt", "r");
    if (rf) {
        int rm, cap, occ;
        double p;
        while (fgets(line, sizeof(line), rf)) {
            p = 0.0;
            if (sscanf(line, "%d;%d;%d;%lf", &rm, &cap, &occ, &p) >= 3) {
                if (rm == target_rm) {
                    capacity = cap;
                    price = p;
                    break;
                }
            }
        }
        fclose(rf);
    }

    FILE *sf = fopen("students.txt", "r");
    int occupied_count = 0;

    struct StudentInfo {
        int id;
        char name[50];
        char dept[50];
        char phone[20];
    } students[100];

    if (sf) {
        int id, rm;
        char name[50], dept[50], phone[20], pass[33];
        while (fgets(line, sizeof(line), sf)) {
            if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%32[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
                if (rm == target_rm) {
                    if (occupied_count < 100) {
                        students[occupied_count].id = id;
                        strcpy(students[occupied_count].name, name);
                        strcpy(students[occupied_count].dept, dept);
                        strcpy(students[occupied_count].phone, phone);
                    }
                    occupied_count++;
                }
            }
        }
        fclose(sf);
    }

    printf("\n==================================================\n");
    printf("           ROOM DETAILS - ROOM #%d                 \n", target_rm);
    printf("==================================================\n");

    if (capacity != -1) {
        int free_beds = capacity - occupied_count;
        if (free_beds < 0) free_beds = 0;
        printf("Total Bed Capacity : %d\n", capacity);
        printf("Occupied Beds      : %d\n", occupied_count);
        printf("Free Beds Available: %d\n", free_beds);
        printf("Monthly Price      : BDT %.2f\n", price);
    } else {
        printf("Room #%d is not listed in rooms.txt.\n", target_rm);
        printf("Occupied Beds      : %d\n", occupied_count);
    }

    printf("\n--- Students Residing in Room #%d ---\n", target_rm);
    if (occupied_count == 0) {
        printf("No students currently assigned to Room #%d.\n", target_rm);
    } else {
        for (int i = 0; i < occupied_count && i < 100; i++) {
            printf("%d. ID: %d | Name: %s | Dept: %s | Phone: %s\n",
                   i + 1, students[i].id, students[i].name, students[i].dept, students[i].phone);
        }
    }
    printf("==================================================\n");
}

void admin_fee_ops() {
    int ch, target_id, id;
    double amt, monthly, extra, due;
    char line[256], status[20], date[30];

    printf("1. Record Payment\n2. View Defaulters\nChoice: ");
    scanf("%d", &ch);

    if (ch == 1) {
        printf("Student ID: "); scanf("%d", &target_id);
        printf("Amount Paid: "); scanf("%lf", &amt);
        FILE *src = fopen("fees.txt", "r"), *tmp = fopen("temp.txt", "w");
        if (src && tmp) {
            while (fgets(line, 256, src)) {
                if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &monthly, &extra, &due, status, date) == 6) {
                    if (id == target_id) {
                        due -= amt;
                        if (due <= 0) strcpy(status, "Paid");
                    }
                    fprintf(tmp, "%d;%.2f;%.2f;%.2f;%s;%s\n", id, monthly, extra, due, status, date);
                }
            }
            fclose(src); fclose(tmp);
            remove("fees.txt"); rename("temp.txt", "fees.txt");
        }
        printf("Payment recorded!\n");
    } else {
        FILE *f = fopen("fees.txt", "r");
        if (f) {
            printf("\n--- Fee Defaulters List ---\n");
            while (fgets(line, 256, f)) {
                if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &monthly, &extra, &due, status, date) == 6) {
                    if (due > 0) printf("Student #%d: Due BDT %.2f (%s)\n", id, due, status);
                }
            }
            fclose(f);
        }
    }
}

void admin_executive_summary() {
    int st = 0, rm = 0, comp = 0, def = 0, id;
    double m, e, due;
    char line[256], s[20], d[30];

    FILE *f = fopen("students.txt", "r"); if (f) { while (fgets(line, 256, f)) st++; fclose(f); }
    f = fopen("rooms.txt", "r"); if (f) { while (fgets(line, 256, f)) rm++; fclose(f); }
    f = fopen("complaints.txt", "r"); if (f) { while (fgets(line, 256, f)) comp++; fclose(f); }
    f = fopen("fees.txt", "r");
    if (f) {
        while (fgets(line, 256, f)) {
            if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &m, &e, &due, s, d) == 6 && due > 0) def++;
        }
        fclose(f);
    }
    
    printf("\n=== Executive Summary Dashboard ===\n");
    printf("Students: %d | Rooms: %d | Complaints: %d | Defaulters: %d\n", st, rm, comp, def);
}

void student_submit_slip(int sid) {
    char tx[32], data[256];
    double amt;
    printf("Transaction ID: "); scanf("%s", tx);
    printf("Amount: "); scanf("%lf", &amt);
    sprintf(data, "%d;%d;%s;%.2f;Approved", get_next_id("slips.txt"), sid, tx, amt);
    append_line("slips.txt", data);
    printf("Payment slip submitted!\n");
}

void student_request_leave(int sid) {
    char leave_date[30], reason[100], data[256];
    printf("Enter Intended Leave Date (YYYY-MM-DD): ");
    get_str(leave_date, sizeof(leave_date));
    printf("Enter Reason for Leave: ");
    get_str(reason, sizeof(reason));

    sprintf(data, "%d;%d;%s;%s;Pending", get_next_id("leave_requests.txt"), sid, leave_date, reason);
    append_line("leave_requests.txt", data);
    printf("Leave request for date '%s' logged successfully!\n", leave_date);
}

void admin_view_complaints() {
    FILE *f = fopen("complaints.txt", "r");
    if (!f) {
        printf("\nNo complaints logged yet.\n");
        return;
    }
    printf("\n==================================================\n");
    printf("            ALL STUDENT COMPLAINTS LOG            \n");
    printf("==================================================\n");
    char line[256], desc[100], status[30], date[30];
    int cid, sid, staff_id = 0, count = 0;
    while (fgets(line, sizeof(line), f)) {
        staff_id = 0;
        int parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%d;%29[^\n]", &cid, &sid, desc, status, &staff_id, date);
        if (parsed < 4) {
            parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%29[^\n]", &cid, &sid, desc, status, date);
        }
        if (parsed >= 4) {
            count++;
            if (staff_id > 0) {
                printf("Ref #%-4d | Student #%-8d | Staff #%-4d | Status: %-10s | Desc: %s\n",
                       cid, sid, staff_id, status, desc);
            } else {
                printf("Ref #%-4d | Student #%-8d | Staff: Unassigned    | Status: %-10s | Desc: %s\n",
                       cid, sid, status, desc);
            }
        }
    }
    if (count == 0) {
        printf("No complaints found.\n");
    }
    printf("==================================================\n");
    fclose(f);
}

void admin_assign_complaint() {
    FILE *sf = fopen("staff.txt", "r");
    printf("\n==================================================\n");
    printf("              REGISTERED STAFF MEMBERS            \n");
    printf("==================================================\n");
    int staff_count = 0;
    if (sf) {
        char sline[256], sname[50], sphone[20], spass[20];
        int st_id;
        while (fgets(sline, sizeof(sline), sf)) {
            if (sscanf(sline, "%d;%49[^;];%19[^;];%19[^\n]", &st_id, sname, sphone, spass) >= 3) {
                printf("Staff ID: %d | Name: %s | Phone: %s\n", st_id, sname, sphone);
                staff_count++;
            }
        }
        fclose(sf);
    }
    if (staff_count == 0) {
        printf("No staff members registered in staff.txt yet!\n");
        printf("==================================================\n");
        return;
    }
    printf("==================================================\n");

    printf("Enter Complaint Ref ID to Assign: ");
    int target_cid;
    if (scanf("%d", &target_cid) != 1) {
        printf("Invalid Complaint Ref ID!\n");
        return;
    }
    printf("Enter Staff ID to Assign to: ");
    int target_staff;
    if (scanf("%d", &target_staff) != 1) {
        printf("Invalid Staff ID!\n");
        return;
    }

    FILE *src = fopen("complaints.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");
    int found = 0;
    if (src && tmp) {
        char line[256], desc[100], status[30], date[30];
        int cid, sid, staff_id;
        while (fgets(line, sizeof(line), src)) {
            staff_id = 0;
            int parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%d;%29[^\n]", &cid, &sid, desc, status, &staff_id, date);
            if (parsed < 4) {
                parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%29[^\n]", &cid, &sid, desc, status, date);
            }
            if (parsed >= 4) {
                if (cid == target_cid) {
                    found = 1;
                    staff_id = target_staff;
                    strcpy(status, "Assigned");
                }
                fprintf(tmp, "%d;%d;%s;%s;%d;%s\n", cid, sid, desc, status, staff_id, date);
            }
        }
        fclose(src);
        fclose(tmp);
        remove("complaints.txt");
        rename("temp.txt", "complaints.txt");
        if (found) {
            printf("Complaint Ref #%d successfully assigned to Staff #%d!\n", target_cid, target_staff);
        } else {
            printf("Complaint Ref #%d not found.\n", target_cid);
        }
    }
}

void staff_view_complaints(int staff_id) {
    FILE *f = fopen("complaints.txt", "r");
    if (f) {
        printf("\n--- Complaints Log for Staff #%d ---\n", staff_id);
        char line[256], desc[100], status[30], date[30];
        int cid, sid, st_id = 0, count = 0;
        while (fgets(line, sizeof(line), f)) {
            st_id = 0;
            int parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%d;%29[^\n]", &cid, &sid, desc, status, &st_id, date);
            if (parsed < 4) {
                parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%29[^\n]", &cid, &sid, desc, status, date);
            }
            if (parsed >= 4) {
                if (st_id == staff_id || st_id == 0) {
                    printf("Ref #%d | Student #%d | %s | Status: %s\n", cid, sid, desc, status);
                    count++;
                }
            }
        }
        if (count == 0) {
            printf("No complaints assigned to Staff #%d.\n", staff_id);
        }
        fclose(f);
    }
}

void student_submit_complaint(int sid) {
    char desc[100], data[256];
    printf("Complaint Description: ");
    get_str(desc, sizeof(desc));
    sprintf(data, "%d;%d;%s;Pending;0;%s", get_next_id("complaints.txt"), sid, desc, DEFAULT_DATE);
    append_line("complaints.txt", data);
    printf("Complaint submitted to Admin successfully!\n");
}

void staff_update_complaint() {
    int ref_id, cid, sid, staff_id;
    char line[256], desc[100], status[30], date[30];
    printf("Complaint Ref ID: ");
    if (scanf("%d", &ref_id) != 1) return;
    FILE *src = fopen("complaints.txt", "r"), *tmp = fopen("temp.txt", "w");
    if (src && tmp) {
        while (fgets(line, sizeof(line), src)) {
            staff_id = 0;
            int parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%d;%29[^\n]", &cid, &sid, desc, status, &staff_id, date);
            if (parsed < 4) {
                parsed = sscanf(line, "%d;%d;%99[^;];%29[^;];%29[^\n]", &cid, &sid, desc, status, date);
            }
            if (parsed >= 4) {
                if (cid == ref_id) strcpy(status, "Resolved");
                fprintf(tmp, "%d;%d;%s;%s;%d;%s\n", cid, sid, desc, status, staff_id, date);
            }
        }
        fclose(src); fclose(tmp);
        remove("complaints.txt"); rename("temp.txt", "complaints.txt");
        printf("Complaint status updated to Resolved!\n");
    }
}

void admin_portal()
{
    int ch;
    int sid;
    int eventChoice;

    while (1)
    {
        clear_term();
        printf("\n=== Administrator Portal ===\n");

        printf("1. Register Student\n");
        printf("2. Delete Student\n");
        printf("3. Search Student\n");
        printf("4. Room Wise Search\n");
        printf("5. Room Operations\n");
        printf("6. Fee Operations\n");
        printf("7. Register Staff\n");
        printf("8. Executive Summary\n");

        printf("9. View Guest Requests\n");
        printf("10. Approve Guest Request\n");
        printf("11. View Meal Requests\n");
        printf("12. Approve Meal Request\n");
        printf("13. View Late Entries\n");
        printf("14. Event Management\n");
        printf("15. Complaint Management\n");

        printf("16. Logout\n");

        printf("Choice: ");
        if (scanf("%d", &ch) != 1 || ch == 16) break;

        switch(ch)
        {
            case 1:
                admin_register_student();
                break;

            case 2:
                admin_student_ops();
                break;

            case 3:
                admin_search_student();
                break;

            case 4:
                admin_search_room();
                break;

            case 5:
                admin_room_ops();
                break;

            case 6:
                admin_fee_ops();
                break;

            case 7:
                admin_staff_ops();
                break;

            case 8:
                admin_executive_summary();
                break;

            case 9:
                admin_view_guest_requests();
                break;

            case 10:
                printf("Enter Student ID: ");
                if (scanf("%d", &sid) != 1) {
                    printf("Invalid Input!\n");
                    pause_term();
                    break;
                }
                admin_approve_guest(sid);
                break;

            case 11:
                admin_view_meal_requests();
                break;

            case 12:
                printf("Enter Student ID: ");
                if (scanf("%d", &sid) != 1) {
                    printf("Invalid Input!\n");
                    pause_term();
                    break;
                }
                admin_approve_meal(sid);
                break;

            case 13:
                admin_view_late_entries();
                break;

            case 14:

                while(1)
                {
                    clear_term();
                    printf("\n===== Event Management =====\n");
                    printf("1. View Event Requests\n");
                    printf("2. Approve/Reject Event\n");
                    printf("3. Back\n");

                    printf("Choice: ");
                    scanf("%d", &eventChoice);

                    switch(eventChoice)
                    {
                        case 1:
                            admin_view_event_requests();
                            break;

                        case 2:
                            printf("Enter Student ID: ");
                            if (scanf("%d", &sid) != 1) {
                                printf("Invalid Input!\n");
                                pause_term();
                                break;
                            }
                            admin_approve_event(sid);
                            break;

                        case 3:
                            break;

                        default:
                            printf("Invalid Choice!\n");
                    }

                    if(eventChoice == 3)
                        break;

                    pause_term();
                }

                break;

            case 15:
                while(1) {
                    clear_term();
                    printf("\n===== Complaint Management =====\n");
                    printf("1. View All Complaints\n");
                    printf("2. Assign Complaint to Staff\n");
                    printf("3. Back\n");
                    printf("Choice: ");
                    int compChoice;
                    if (scanf("%d", &compChoice) != 1 || compChoice == 3) break;
                    if (compChoice == 1) admin_view_complaints();
                    else if (compChoice == 2) admin_assign_complaint();
                    else printf("Invalid Choice!\n");
                    pause_term();
                }
                break;

            case 16:
                printf("Logging out...\n");
                return;

            default:
                printf("Invalid Choice!\n");
        }

        pause_term();
    }
}

void student_portal(int sid)
{
    int ch;

    while (1)
    {
        clear_term();
        printf("\n=== Student Portal (ID: %d) ===\n", sid);

        printf("1. View Profile\n");
        printf("2. Fee Status\n");
        printf("3. Submit Payment Slip\n");
        printf("4. Request Room Transfer\n");
        printf("5. Lodge Complaint\n");
        printf("6. Request Leave\n");
        printf("7. Booking History\n");
        printf("8. Book for New Student\n");

        printf("9. Guest Registration\n");
        printf("10. Meal Registration\n");
        printf("11. Check In\n");
        printf("12. Check Out\n");
        printf("13. Event Request\n");

        printf("14. Logout\n");

        printf("Choice: ");
        if (scanf("%d", &ch) != 1) break;

        switch(ch)
        {
            case 1:
                student_view_profile(sid);
                break;

            case 2:
                student_view_fees(sid);
                break;

            case 3:
                student_submit_slip(sid);
                break;

            case 4:
                student_request_transfer(sid);
                break;

            case 5:
                student_submit_complaint(sid);
                break;

            case 6:
                student_request_leave(sid);
                break;

            case 7:
                student_view_booking_history(sid);
                break;

            case 8:
                student_book_for_new(sid);
                break;

            case 9:
                guest_register(sid);
                break;

            case 10:
                meal_register(sid);
                break;

            case 11:
                student_checkin(sid);
                break;

            case 12:
                student_checkout(sid);
                break;

            case 13:
                event_request(sid);
                break;

            case 14:
                printf("Logging out...\n");
                return;

            default:
                printf("Invalid Choice!\n");
        }

        pause_term();
    }
}
void staff_portal(int staff_id) {
    while (1) {
        clear_term();
        printf("\n=== Staff Portal (ID: %d) ===\n", staff_id);
        printf("1. View Complaints\n2. Resolve Complaint\n3. Logout\nChoice: ");
        int ch;
        if (scanf("%d", &ch) != 1 || ch == 3) break;

        if (ch == 1) staff_view_complaints(staff_id);
        else if (ch == 2) staff_update_complaint();
        pause_term();
    }
}

void provost_view_all_students() {
    FILE *f = fopen("students.txt", "r");
    if (!f) {
        printf("\nNo student records found.\n");
        return;
    }
    printf("\n==================================================\n");
    printf("            ALL REGISTERED STUDENTS LOG           \n");
    printf("==================================================\n");
    char line[256], name[50], dept[50], phone[20], pass[33];
    int id, rm, count = 0;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%32[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
            printf("ID: %-8d | Name: %-20s | Dept: %-8s | Phone: %-13s | Room: %d\n",
                   id, name, dept, phone, rm);
            count++;
        }
    }
    if (count == 0) printf("No students listed.\n");
    printf("==================================================\n");
    fclose(f);
}

void provost_view_all_staff() {
    FILE *sf = fopen("staff.txt", "r");
    if (!sf) {
        printf("\nNo staff records found.\n");
        return;
    }
    printf("\n==================================================\n");
    printf("            ALL REGISTERED STAFF MEMBERS          \n");
    printf("==================================================\n");
    char sline[256], sname[50], sphone[20], spass[20];
    int st_id, count = 0;
    while (fgets(sline, sizeof(sline), sf)) {
        if (sscanf(sline, "%d;%49[^;];%19[^;];%19[^\n]", &st_id, sname, sphone, spass) >= 3) {
            printf("Staff ID: %-5d | Name: %-20s | Phone: %s\n", st_id, sname, sphone);
            count++;
        }
    }
    if (count == 0) printf("No staff members listed.\n");
    printf("==================================================\n");
    fclose(sf);
}

void provost_view_fee_records() {
    FILE *f = fopen("fees.txt", "r");
    if (!f) {
        printf("\nNo fee records found.\n");
        return;
    }
    printf("\n==================================================\n");
    printf("            STUDENT FINANCIAL & FEE LOG           \n");
    printf("==================================================\n");
    char line[256], status[20], date[30];
    int id, count = 0;
    double monthly, extra, due;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &monthly, &extra, &due, status, date) == 6) {
            printf("Student #%-8d | Monthly: BDT %-8.2f | Due: BDT %-8.2f | Status: %-8s | Date: %s\n",
                   id, monthly, due, status, date);
            count++;
        }
    }
    if (count == 0) printf("No fee records listed.\n");
    printf("==================================================\n");
    fclose(f);
}

void provost_portal() {
    int ch;
    while (1) {
        clear_term();
        printf("\n===========================================\n");
        printf("     PROVOST / SUPERADMIN PORTAL (READ-ONLY) \n");
        printf("===========================================\n");
        printf("1. Executive Summary Dashboard\n");
        printf("2. Search Student Details\n");
        printf("3. View All Registered Students\n");
        printf("4. Room Wise Search & Occupancy\n");
        printf("5. View All Rooms & Pricing\n");
        printf("6. View All Complaint Logs\n");
        printf("7. View All Fee Records & Defaulters\n");
        printf("8. View Registered Staff Members\n");
        printf("9. View Guest Requests\n");
        printf("10. View Meal Requests\n");
        printf("11. View Event Requests\n");
        printf("12. View Late Entry Logs\n");
        printf("13. Logout\n");
        printf("Choice: ");
        if (scanf("%d", &ch) != 1 || ch == 13) break;

        switch(ch) {
            case 1: admin_executive_summary(); break;
            case 2: admin_search_student(); break;
            case 3: provost_view_all_students(); break;
            case 4: admin_search_room(); break;
            case 5: public_view_rooms(); break;
            case 6: admin_view_complaints(); break;
            case 7: provost_view_fee_records(); break;
            case 8: provost_view_all_staff(); break;
            case 9: admin_view_guest_requests(); break;
            case 10: admin_view_meal_requests(); break;
            case 11: admin_view_event_requests(); break;
            case 12: admin_view_late_entries(); break;
            case 13: printf("Logging out...\n"); return;
            default: printf("Invalid Choice!\n");
        }
        pause_term();
    }
}

void login_portal() {
    while (1) {
        clear_term();
        printf("\n===========================================\n");
        printf("  UNIVERSITY HOSTEL MANAGEMENT SYSTEM CLI  \n");
        printf("===========================================\n");
        printf("1. Admin Portal\n");
        printf("2. Provost / Superadmin Portal\n");
        printf("3. Student Login\n");
        printf("4. Staff Login\n");
        printf("5. View Rooms & Prices (Public)\n");
        printf("6. See Facilities List\n");
        printf("7. Call Now / Contact Us\n");
        printf("8. Exit\n");
        printf("Choice: ");
        int ch;
        if (scanf("%d", &ch) != 1 || ch == 8) break;

        if (ch == 5) { public_view_rooms(); pause_term(); continue; }
        else if (ch == 6) { show_facilities_list(); pause_term(); continue; }
        else if (ch == 7) { show_call_now(); pause_term(); continue; }

        if (ch >= 1 && ch <= 4) {
            char u[32], p[32];
            printf("ID / Username: "); scanf("%s", u);
            printf("Password: "); scanf("%s", p);

            if (ch == 1) {
                if (strcmp(u, "admin") == 0 && strcmp(p, "admin") == 0) admin_portal();
                else { printf("Auth Failed!\n"); pause_term(); }
            } else if (ch == 2) {
                if ((strcmp(u, "superadmin") == 0 && strcmp(p, "superadmin") == 0) ||
                    (strcmp(u, "superadmin") == 0 && strcmp(p, "superpass") == 0)) provost_portal();
                else { printf("Auth Failed!\n"); pause_term(); }
            } else if (ch == 3) student_portal(atoi(u));
            else if (ch == 4) staff_portal(atoi(u));
        }
    }
}
