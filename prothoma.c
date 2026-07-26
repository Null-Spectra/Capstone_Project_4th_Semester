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

void student_request_leave(int sid) {
    char data[256];
    sprintf(data, "%d;%d;Pending", get_next_id("leave_requests.txt"), sid);
    append_line("leave_requests.txt", data);
    printf("Leave request logged!\n");
}

void student_book_for_new(int sid) {
    printf("\n=== Request New Student Room Booking ===\n");
    printf("New Student Full Name: ");
    char name[50], dept[50], phone[20], data[256];
    get_str(name, sizeof(name));
    printf("Department: ");
    get_str(dept, sizeof(dept));
    printf("Phone Number: ");
    get_str(phone, sizeof(phone));
    printf("Preferred Room Number: ");
    int pref_rm = get_int();

    sprintf(data, "%d;%d;%s;%s;%s;%d;Pending", get_next_id("new_booking_requests.txt"), sid, name, dept, phone, pref_rm);
    append_line("new_booking_requests.txt", data);
    printf("New student booking request for '%s' submitted successfully!\n", name);
}

void show_facilities_list() {
    printf("\n==================================================\n");
    printf("       PREMIUM HOSTEL FACILITIES & AMENITIES     \n");
    printf("==================================================\n");
    printf(" 📶 1. High-Speed 24/7 Wi-Fi Internet Access\n");
    printf(" ⚡ 2. 24/7 Uninterrupted Electricity & Generator Backup\n");
    printf(" 🛡️ 3. Round-the-Clock CCTV Security & Guarded Entry\n");
    printf(" 💧 4. Pure Mineral Drinking Water Purifiers\n");
    printf(" 🏋️ 5. Modern Gymnasium & Indoor Games Recreation Center\n");
    printf(" 🍲 6. Hygienic Dining Hall with Customized Meal Plans\n");
    printf(" 🧺 7. Weekly Laundry & Daily Housekeeping\n");
    printf(" 📚 8. Quiet Air-Conditioned Study Lounges\n");
    printf("==================================================\n");
}
