#include "sadia.h"
#include "rafi.h"

/* ============================================================================
 * Section 2: SADIA (Room Operations, Pricing, Call Now & Booking History)
 * ============================================================================ */

void admin_room_ops() {
    printf("\n=== Room Operations ===\n");
    printf("1. Register Room\n");
    printf("2. View Rooms\n");
    printf("3. Room Wise Search\n");
    printf("4. View Transfer Requests\n");
    printf("5. Process / Approve Transfer Request\n");
    printf("Choice: ");
    int rch = get_int();
    char data[256], line[256];

    if (rch == 1) {
        printf("Room Number: ");
        int rm = get_int();
        printf("Bed Capacity: ");
        int cap = get_int();
        printf("Monthly Room Price (BDT): ");
        double price = get_dbl();
        sprintf(data, "%d;%d;0;%.2f", rm, cap, price);
        append_line("rooms.txt", data);
        printf("Room registered with price BDT %.2f!\n", price);
    } else if (rch == 3) {
        admin_search_room();
    } else if (rch == 4) {
        admin_view_transfer_requests();
    } else if (rch == 5) {
        admin_approve_transfer_request();
    } else {
        FILE *f = fopen("rooms.txt", "r");
        if (f) {
            int rm, cap, occ;
            double price;
            printf("\n--- Rooms Registry & Pricing ---\n");
            while (fgets(line, sizeof(line), f)) {
                price = 0.0;
                if (sscanf(line, "%d;%d;%d;%lf", &rm, &cap, &occ, &price) >= 3) {
                    printf("Room %d: Occupied %d / %d beds | Price: BDT %.2f\n", rm, occ, cap, price);
                }
            }
            fclose(f);
        }
    }
}

void admin_view_transfer_requests() {
    FILE *f = fopen("transfers.txt", "r");
    if (!f) {
        printf("\nNo transfer requests found.\n");
        return;
    }
    printf("\n==================================================\n");
    printf("            ROOM TRANSFER REQUESTS LOG            \n");
    printf("==================================================\n");
    char line[256], status[30];
    int req_id, sid, target_rm, count = 0;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d;%d;%d;%29[^\n]", &req_id, &sid, &target_rm, status) >= 4) {
            printf("Req #%-5d | Student ID: %-8d | Target Room: %-5d | Status: %s\n",
                   req_id, sid, target_rm, status);
            count++;
        }
    }
    if (count == 0) printf("No transfer requests listed.\n");
    printf("==================================================\n");
    fclose(f);
}

void admin_approve_transfer_request() {
    admin_view_transfer_requests();
    printf("Enter Request ID to Process: ");
    int target_req = get_int();
    printf("1. Approve Transfer\n2. Reject Transfer\nChoice: ");
    int choice = get_int();
    if (choice != 1 && choice != 2) {
        printf("Invalid Choice!\n");
        return;
    }

    FILE *f = fopen("transfers.txt", "r");
    if (!f) {
        printf("No transfer requests file found.\n");
        return;
    }

    char line[256];
    char temp_data[8192] = "";
    int req_id, sid, target_rm, found = 0, req_sid = 0, req_target_rm = 0;
    char status[30];

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d;%d;%d;%29[^\n]", &req_id, &sid, &target_rm, status) >= 4) {
            if (req_id == target_req && strcmp(status, "Pending") == 0) {
                found = 1;
                req_sid = sid;
                req_target_rm = target_rm;
                char new_line[256];
                sprintf(new_line, "%d;%d;%d;%s\n", req_id, sid, target_rm, choice == 1 ? "Approved" : "Rejected");
                strcat(temp_data, new_line);
            } else {
                strcat(temp_data, line);
            }
        } else {
            strcat(temp_data, line);
        }
    }
    fclose(f);

    if (!found) {
        printf("Pending transfer request #%d not found.\n", target_req);
        return;
    }

    FILE *fw = fopen("transfers.txt", "w");
    if (fw) {
        fputs(temp_data, fw);
        fclose(fw);
    }

    if (choice == 1) {
        // Update student's room in students.txt
        FILE *sf = fopen("students.txt", "r");
        if (sf) {
            char st_data[8192] = "";
            char sline[256], name[50], dept[50], phone[20], hash[33];
            int st_id, old_rm;
            int old_room_found = 0;
            while (fgets(sline, sizeof(sline), sf)) {
                if (sscanf(sline, "%d;%49[^;];%49[^;];%19[^;];%32[^;];%d", &st_id, name, dept, phone, hash, &old_rm) == 6) {
                    if (st_id == req_sid) {
                        old_room_found = old_rm;
                        char updated[256];
                        sprintf(updated, "%d;%s;%s;%s;%s;%d\n", st_id, name, dept, phone, hash, req_target_rm);
                        strcat(st_data, updated);
                    } else {
                        strcat(st_data, sline);
                    }
                } else {
                    strcat(st_data, sline);
                }
            }
            fclose(sf);

            FILE *sfw = fopen("students.txt", "w");
            if (sfw) {
                fputs(st_data, sfw);
                fclose(sfw);
            }

            // Update room occupancies in rooms.txt (decrement old_room_found, increment req_target_rm)
            FILE *rf = fopen("rooms.txt", "r");
            if (rf) {
                char rm_data[8192] = "";
                char rline[256];
                int rm, cap, occ;
                double price;
                while (fgets(rline, sizeof(rline), rf)) {
                    price = 0.0;
                    if (sscanf(rline, "%d;%d;%d;%lf", &rm, &cap, &occ, &price) >= 3) {
                        if (rm == old_room_found && old_room_found != 0) {
                            occ = (occ > 0) ? (occ - 1) : 0;
                        }
                        if (rm == req_target_rm) {
                            occ = occ + 1;
                        }
                        char updated_rm[256];
                        sprintf(updated_rm, "%d;%d;%d;%.2f\n", rm, cap, occ, price);
                        strcat(rm_data, updated_rm);
                    } else {
                        strcat(rm_data, rline);
                    }
                }
                fclose(rf);

                FILE *rfw = fopen("rooms.txt", "w");
                if (rfw) {
                    fputs(rm_data, rfw);
                    fclose(rfw);
                }
            }
        }
        printf("Transfer Request #%d APPROVED! Student #%d moved to Room #%d.\n", target_req, req_sid, req_target_rm);
    } else {
        printf("Transfer Request #%d REJECTED.\n", target_req);
    }
}

void public_view_rooms() {
    FILE *f = fopen("rooms.txt", "r");
    if (!f) {
        printf("\nNo room information registered yet.\n");
        return;
    }
    char line[256];
    int rm, cap, occ, count = 0;
    double price;
    printf("\n==================================================\n");
    printf("        HOSTEL ROOM AVAILABILITY & PRICING       \n");
    printf("==================================================\n");
    while (fgets(line, sizeof(line), f)) {
        price = 0.0;
        if (sscanf(line, "%d;%d;%d;%lf", &rm, &cap, &occ, &price) >= 3) {
            int available = cap - occ;
            if (available < 0) available = 0;
            printf("Room #%-5d | Capacity: %d beds | Available: %d | Price: BDT %.2f/mo\n",
                   rm, cap, available, price);
            count++;
        }
    }
    if (count == 0) {
        printf("No rooms listed.\n");
    }
    printf("==================================================\n");
    fclose(f);
}

void show_call_now() {
    printf("\n==================================================\n");
    printf("            CALL NOW / CONTACT INFORMATION        \n");
    printf("==================================================\n");
    printf("  📞 Primary Hotline:   01711111111\n");
    printf("  📞 Secondary Hotline: 01998989898\n");
    printf("  ✉️  Official Email:    info@hostelmanagement.edu.bd\n");
    printf("  🏢 Office Address:    Main Campus Hostel Desk\n");
    printf("==================================================\n");
}

void student_view_booking_history(int sid) {
    printf("\n==================================================\n");
    printf("         STUDENT BOOKING & ALLOCATION HISTORY     \n");
    printf("==================================================\n");

    // 1. Current Room Record
    FILE *sf = fopen("students.txt", "r");
    if (sf) {
        char line[256];
        int id, rm;
        char name[50], dept[50], phone[20], pass[20];
        while (fgets(line, sizeof(line), sf)) {
            if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
                if (id == sid) {
                    printf("Current Active Allocation: Room #%d\n", rm);
                    break;
                }
            }
        }
        fclose(sf);
    }

    // 2. Room Transfer Request History
    FILE *tf = fopen("transfers.txt", "r");
    if (tf) {
        char line[256];
        int req_id, st_id, target_rm, count = 0;
        char status[20];
        printf("\n--- Room Transfer Request Logs ---\n");
        while (fgets(line, sizeof(line), tf)) {
            if (sscanf(line, "%d;%d;%d;%19[^\n]", &req_id, &st_id, &target_rm, status) == 4) {
                if (st_id == sid) {
                    printf("Transfer Request #%d -> Target Room: %d | Status: %s\n", req_id, target_rm, status);
                    count++;
                }
            }
        }
        if (count == 0) {
            printf("No previous transfer history found.\n");
        }
        fclose(tf);
    }
}

void admin_staff_ops() {
    printf("Staff ID: ");
    int id = get_int();
    printf("Staff Name: ");
    char name[50], phone[20], pass[20], data[256];
    get_str(name, sizeof(name));
    printf("Phone Number: ");
    get_str(phone, sizeof(phone));
    printf("Password: ");
    get_str(pass, sizeof(pass));

    sprintf(data, "%d;%s;%s;%s", id, name, phone, pass);
    append_line("staff.txt", data);
    printf("Staff registered successfully!\n");
}

void admin_student_ops() {
    printf("Student ID to Delete: ");
    int target_id = get_int();
    FILE *src = fopen("students.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");
    if (src && tmp) {
        char line[256];
        int id, rm;
        char name[50], dept[50], phone[20], pass[20];
        while (fgets(line, sizeof(line), src)) {
            if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
                if (id != target_id) {
                    fprintf(tmp, "%d;%s;%s;%s;%s;%d\n", id, name, dept, phone, pass, rm);
                }
            }
        }
        fclose(src);
        fclose(tmp);
        remove("students.txt");
        rename("temp.txt", "students.txt");
    }
    printf("Student record removed.\n");
}
