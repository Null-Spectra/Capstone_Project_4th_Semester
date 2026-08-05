#include "sadia.h"
#include "rafi.h"

/* ============================================================================
 * Section 2: SADIA (Room Operations, Pricing, Call Now & Booking History)
 * ============================================================================ */

void admin_room_ops() {
    printf("1. Register Room\n2. View Rooms\n3. Room Wise Search\nChoice: ");
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
