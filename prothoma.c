#include "prothoma.h"
#include "rafi.h"

/* ============================================================================
 * Section 4: PROTHOMA (Profile, Fees, Requests & Facilities List)
 * ============================================================================ */

void student_view_profile(int sid) {
    FILE *f = fopen("students.txt", "r");
    if (f) {
        char line[256];
        int id, rm, found = 0;
        char name[50], dept[50], phone[20], pass[20];
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
                if (id == sid) {
                    printf("\n--- Profile ---\nID: %d\nName: %s\nDept: %s\nPhone: %s\nRoom: %d\n", id, name, dept, phone, rm);
                    found = 1;
                    break;
                }
            }
        }
        fclose(f);
        if (!found) {
            printf("Profile not found.\n");
        }
    }
}

void student_view_fees(int sid) {
    FILE *f = fopen("fees.txt", "r");
    if (f) {
        char line[256];
        int id, found = 0;
        double monthly, extra, due;
        char status[20], date[30];
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &monthly, &extra, &due, status, date) == 6) {
                if (id == sid) {
                    printf("\n--- Fee Status ---\nMonthly: BDT %.2f\nExtra: BDT %.2f\nDue: BDT %.2f\nStatus: %s\n", monthly, extra, due, status);
                    found = 1;
                    break;
                }
            }
        }
        fclose(f);
        if (!found) {
            printf("Fee record not found.\n");
        }
    }
}

void student_request_transfer(int sid) {
    printf("Target Room: ");
    int rm = get_int();
    char data[256];
    sprintf(data, "%d;%d;%d;Pending", get_next_id("transfers.txt"), sid, rm);
    append_line("transfers.txt", data);
    printf("Room transfer request logged!\n");
}

void show_available_seats_summary() {
    FILE *rf = fopen("rooms.txt", "r");
    if (!rf) {
        printf("\nNo rooms registered in rooms.txt yet.\n");
        return;
    }

    struct RoomSeatInfo {
        int room_num;
        int capacity;
        int occupied;
        double price;
    } rooms[100];
    int room_count = 0;

    char line[256];
    while (fgets(line, sizeof(line), rf)) {
        int rm, cap, occ;
        double price = 0.0;
        if (sscanf(line, "%d;%d;%d;%lf", &rm, &cap, &occ, &price) >= 3) {
            if (room_count < 100) {
                rooms[room_count].room_num = rm;
                rooms[room_count].capacity = cap;
                rooms[room_count].occupied = 0;
                rooms[room_count].price = price;
                room_count++;
            }
        }
    }
    fclose(rf);

    FILE *sf = fopen("students.txt", "r");
    if (sf) {
        int id, rm;
        char name[50], dept[50], phone[20], pass[33];
        while (fgets(line, sizeof(line), sf)) {
            if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%32[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
                for (int i = 0; i < room_count; i++) {
                    if (rooms[i].room_num == rm) {
                        rooms[i].occupied++;
                        break;
                    }
                }
            }
        }
        fclose(sf);
    }

    int total_capacity = 0, total_occupied = 0, total_free = 0;
    printf("\n==================================================\n");
    printf("        HOSTEL SEAT AVAILABILITY SUMMARY          \n");
    printf("==================================================\n");
    printf("%-10s | %-10s | %-10s | %-12s | %-12s\n",
           "Room No.", "Capacity", "Occupied", "Available", "Price (BDT)");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < room_count; i++) {
        int avail = rooms[i].capacity - rooms[i].occupied;
        if (avail < 0) avail = 0;
        total_capacity += rooms[i].capacity;
        total_occupied += rooms[i].occupied;
        total_free += avail;

        printf("Room #%-5d | %-10d | %-10d | %-12d | BDT %.2f\n",
               rooms[i].room_num, rooms[i].capacity, rooms[i].occupied, avail, rooms[i].price);
    }
    printf("--------------------------------------------------\n");
    printf("TOTALS     | %-10d | %-10d | %-12d |\n",
           total_capacity, total_occupied, total_free);
    printf("==================================================\n");
}

void student_book_for_new(int sid) {
    printf("\n=== Request New Student Room Booking ===\n");
    show_available_seats_summary();

    printf("\nNew Student Full Name: ");
    char name[50], dept[50], phone[20], data[256];
    get_str(name, sizeof(name));
    printf("Department: ");
    get_str(dept, sizeof(dept));
    printf("Phone Number: ");
    get_str(phone, sizeof(phone));
    printf("Preferred Room Number: ");
    int pref_rm = get_int();

    int req_id = get_next_id("new_booking_requests.txt");
    sprintf(data, "%d;%d;%s;%s;%s;%d;Pending", req_id, sid, name, dept, phone, pref_rm);
    append_line("new_booking_requests.txt", data);
    printf("\n[Priority Queue FCFS Order] Booking Request #%d for '%s' submitted successfully!\n", req_id, name);
}

void admin_view_new_booking_requests_priority_queue() {
    FILE *f = fopen("new_booking_requests.txt", "r");
    if (!f) {
        printf("\nNo new student booking requests found.\n");
        return;
    }

    printf("\n======================================================================\n");
    printf("    NEW STUDENT BOOKING REQUESTS (PRIORITY QUEUE - FCFS ORDER)      \n");
    printf("======================================================================\n");
    printf("%-10s | %-8s | %-10s | %-18s | %-8s | %-10s | %-10s\n",
           "Priority", "Req ID", "By Student", "Name", "Dept", "Pref Room", "Status");
    printf("----------------------------------------------------------------------\n");

    char line[256], name[50], dept[50], phone[20], status[20];
    int req_id, by_sid, pref_rm, count = 0, priority = 1;

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d;%d;%49[^;];%49[^;];%19[^;];%d;%19[^\n]",
                   &req_id, &by_sid, name, dept, phone, &pref_rm, status) == 7) {
            printf("P-%-8d | #%-7d | %-10d | %-18s | %-8s | Room #%-4d | %-10s\n",
                   priority++, req_id, by_sid, name, dept, pref_rm, status);
            count++;
        }
    }

    if (count == 0) {
        printf("No booking requests found.\n");
    }
    printf("======================================================================\n");
    fclose(f);
}

void admin_approve_new_booking_request_priority_queue() {
    admin_view_new_booking_requests_priority_queue();

    FILE *f = fopen("new_booking_requests.txt", "r");
    if (!f) return;
    fclose(f);

    printf("\nEnter Request ID to Process (in Priority Queue FCFS Order): ");
    int target_req;
    if (scanf("%d", &target_req) != 1) return;

    printf("Action (1. Approve & Assign Room, 2. Reject): ");
    int act;
    if (scanf("%d", &act) != 1) return;

    FILE *src = fopen("new_booking_requests.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");
    int found = 0;

    if (src && tmp) {
        char line[256], name[50], dept[50], phone[20], status[20];
        int req_id, by_sid, pref_rm;

        while (fgets(line, sizeof(line), src)) {
            if (sscanf(line, "%d;%d;%49[^;];%49[^;];%19[^;];%d;%19[^\n]",
                       &req_id, &by_sid, name, dept, phone, &pref_rm, status) == 7) {
                if (req_id == target_req && strcmp(status, "Pending") == 0) {
                    found = 1;
                    if (act == 1) {
                        strcpy(status, "Approved");

                        int new_sid = get_next_id("students.txt");
                        char hash[33], student_data[256], fee_data[256];
                        md5_hash("1234", hash);

                        sprintf(student_data, "%d;%s;%s;%s;%s;%d", new_sid, name, dept, phone, hash, pref_rm);
                        append_line("students.txt", student_data);

                        sprintf(fee_data, "%d;14000.00;0.00;14000.00;Unpaid;%s", new_sid, DEFAULT_DATE);
                        append_line("fees.txt", fee_data);

                        printf("\n[SUCCESS] Request #%d Approved! New Student Registered with ID #%d in Room %d.\n",
                               req_id, new_sid, pref_rm);
                    } else {
                        strcpy(status, "Rejected");
                        printf("\nRequest #%d Rejected.\n", req_id);
                    }
                }
                fprintf(tmp, "%d;%d;%s;%s;%s;%d;%s\n", req_id, by_sid, name, dept, phone, pref_rm, status);
            }
        }
        fclose(src);
        fclose(tmp);
        remove("new_booking_requests.txt");
        rename("temp.txt", "new_booking_requests.txt");

        if (!found) {
            printf("Pending Request ID #%d not found.\n", target_req);
        }
    }
}

void show_facilities_list() {
    printf("\n==================================================\n");
    printf("       PREMIUM HOSTEL FACILITIES & AMENITIES     \n");
    printf("==================================================\n");
    printf(" 1. High-Speed 24/7 Wi-Fi Internet Access\n");
    printf(" 2. 24/7 Uninterrupted Electricity & Generator Backup\n");
    printf(" 3. Round-the-Clock CCTV Security & Guarded Entry\n");
    printf(" 4. Pure Mineral Drinking Water Purifiers\n");
    printf(" 5. Modern Gymnasium & Indoor Games Recreation Center\n");
    printf(" 6. Hygienic Dining Hall with Customized Meal Plans\n");
    printf(" 7. Weekly Laundry & Daily Housekeeping\n");
    printf(" 8. Quiet Air-Conditioned Study Lounges\n");
    printf("==================================================\n");
}
