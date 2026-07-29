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

void staff_view_complaints() {
    FILE *f = fopen("complaints.txt", "r");
    if (f) {
        printf("\n--- Complaints Log ---\n");
        char line[256], desc[100], status[20], date[30];
        int cid, sid;
        while (fgets(line, 256, f)) {
            if (sscanf(line, "%d;%d;%99[^;];%19[^;];%29[^\n]", &cid, &sid, desc, status, date) == 5) {
                printf("Ref #%d | Student #%d | %s | Status: %s\n", cid, sid, desc, status);
            }
        }
        fclose(f);
    }
}

void student_submit_complaint(int sid) {
    char desc[100], data[256];
    printf("Complaint Description: "); scanf("%s", desc);
    sprintf(data, "%d;%d;%s;Pending;%s", get_next_id("complaints.txt"), sid, desc, DEFAULT_DATE);
    append_line("complaints.txt", data);
    printf("Complaint logged!\n");
}

void staff_update_complaint() {
    int ref_id, cid, sid;
    char line[256], desc[100], status[20], date[30];
    printf("Complaint Ref ID: "); scanf("%d", &ref_id);
    FILE *src = fopen("complaints.txt", "r"), *tmp = fopen("temp.txt", "w");
    if (src && tmp) {
        while (fgets(line, 256, src)) {
            if (sscanf(line, "%d;%d;%99[^;];%19[^;];%29[^\n]", &cid, &sid, desc, status, date) == 5) {
                if (cid == ref_id) strcpy(status, "Resolved");
                fprintf(tmp, "%d;%d;%s;%s;%s\n", cid, sid, desc, status, date);
            }
        }
        fclose(src); fclose(tmp);
        remove("complaints.txt"); rename("temp.txt", "complaints.txt");
        printf("Complaint updated!\n");
    }
}

void admin_portal()
{
    int ch, sid, sub;

    while (1)
    {
        printf("\n=== Administrator Portal ===\n");

        printf("1. Register Student\n");
        printf("2. Delete Student\n");
        printf("3. Search Student\n");
        printf("4. Room Operations\n");
        printf("5. Fee Operations\n");
        printf("6. Register Staff\n");
        printf("7. Guest Request Operations\n");
        printf("8. Meal Request Operations\n");
        printf("9. View Late Entry Logs\n");
        printf("10. Executive Summary\n");
        printf("11. Logout\n");

        printf("Choice: ");
        if (scanf("%d", &ch) != 1) break;

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
                admin_room_ops();
                break;

            case 5:
                admin_fee_ops();
                break;

            case 6:
                admin_staff_ops();
                break;

            case 7:
                printf("\n--- Guest Request Operations ---\n");
                printf("1. View Guest Requests\n");
                printf("2. Approve Guest Request\n");
                printf("Choice: ");
                if (scanf("%d", &sub) == 1) {
                    if (sub == 1) admin_view_guest_requests();
                    else if (sub == 2) {
                        printf("Enter Student ID to Approve: ");
                        if (scanf("%d", &sid) == 1) admin_approve_guest(sid);
                    }
                }
                break;

            case 8:
                printf("\n--- Meal Request Operations ---\n");
                printf("1. View Meal Requests\n");
                printf("2. Approve Meal Request\n");
                printf("Choice: ");
                if (scanf("%d", &sub) == 1) {
                    if (sub == 1) admin_view_meal_requests();
                    else if (sub == 2) {
                        printf("Enter Student ID to Approve: ");
                        if (scanf("%d", &sid) == 1) admin_approve_meal(sid);
                    }
                }
                break;

            case 9:
                admin_view_late_entries();
                break;

            case 10:
                admin_executive_summary();
                break;

            case 11:
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

        printf("13. Logout\n");

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
        printf("\n=== Staff Portal (ID: %d) ===\n", staff_id);
        printf("1. View Complaints\n2. Resolve Complaint\n3. Logout\nChoice: ");
        int ch;
        if (scanf("%d", &ch) != 1 || ch == 3) break;

        if (ch == 1) staff_view_complaints();
        else if (ch == 2) staff_update_complaint();
        pause_term();
    }
}

void login_portal() {
    while (1) {
        printf("\n===========================================\n");
        printf("  UNIVERSITY HOSTEL MANAGEMENT SYSTEM CLI  \n");
        printf("===========================================\n");
        printf("1. Admin Portal\n");
        printf("2. Student Login\n");
        printf("3. Staff Login\n");
        printf("4. View Rooms & Prices (Public)\n");
        printf("5. See Facilities List\n");
        printf("6. Call Now / Contact Us\n");
        printf("7. Exit\n");
        printf("Choice: ");
        int ch;
        if (scanf("%d", &ch) != 1 || ch == 7) break;

        if (ch == 4) { public_view_rooms(); pause_term(); continue; }
        else if (ch == 5) { show_facilities_list(); pause_term(); continue; }
        else if (ch == 6) { show_call_now(); pause_term(); continue; }

        if (ch >= 1 && ch <= 3) {
            char u[32], p[32];
            printf("ID / Username: "); scanf("%s", u);
            printf("Password: "); scanf("%s", p);

            if (ch == 1) {
                if (strcmp(u, "admin") == 0 && strcmp(p, "admin") == 0) admin_portal();
                else { printf("Auth Failed!\n"); pause_term(); }
            } else if (ch == 2) student_portal(atoi(u));
            else if (ch == 3) staff_portal(atoi(u));
        }
    }
}
