#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * STEP 1: ALL FUNCTION PROTOTYPES (ORDER: RAFI -> SADIA -> HIRA -> PROTHOMA)
 * ============================================================================ */

/* --- Rafi's Prototypes --- */
void initialize_files();
void get_str(char *buf, int size);
int get_int();
double get_dbl();
void pause_term();
void append_line(const char *file, const char *data);
int get_next_id(const char *file);
void admin_fee_ops();
void admin_executive_summary();
void login_portal();

/* --- Sadia's Prototypes --- */
void admin_register_student();
void admin_search_student();
void admin_room_ops();
void admin_staff_ops();
void admin_student_ops();
void staff_update_complaint();

/* --- Hira's Prototypes --- */
void student_request_transfer(int sid);
void student_submit_slip(int sid);
void student_request_leave(int sid);
void admin_portal();
void student_portal(int sid);
void staff_portal(int staff_id);

/* --- Prothoma's Prototypes --- */
void student_view_profile(int sid);
void student_view_fees(int sid);
void staff_view_complaints();
void student_submit_complaint(int sid);


/* ============================================================================
 * STEP 2: MAIN ENTRY FUNCTION
 * ============================================================================ */

int main() {
    initialize_files();
    login_portal();
    return 0;
}


/* ============================================================================
 * STEP 3: ACTUAL FUNCTION DEFINITIONS (ORDER: RAFI -> SADIA -> HIRA -> PROTHOMA)
 * ============================================================================ */

/* ============================================================================
 * Section 1: RAFI (Core Storage Engine, Analytics & Login Controller)
 * ============================================================================ */

void initialize_files() {
    const char *files[] = {
        "students.txt", "rooms.txt", "fees.txt", "complaints.txt",
        "transfers.txt", "slips.txt", "leave_requests.txt", "staff.txt"
    };
    for (int i = 0; i < 8; i++) {
        FILE *f = fopen(files[i], "a");
        if (f) {
            fclose(f);
        }
    }
}

void get_str(char *buf, int size) {
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\r\n")] = '\0';
    }
}

int get_int() {
    char buf[32];
    get_str(buf, sizeof(buf));
    return atoi(buf);
}

double get_dbl() {
    char buf[32];
    get_str(buf, sizeof(buf));
    return atof(buf);
}

void pause_term() {
    printf("Press Enter to continue...");
    getchar();
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
    if (!f) {
        return 1;
    }
    char line[256];
    int max_id = 0, id = 0;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d", &id) == 1 && id > max_id) {
            max_id = id;
        }
    }
    fclose(f);
    return max_id + 1;
}

void admin_fee_ops() {
    printf("1. Record Payment\n2. View Defaulters List\nChoice: ");
    int fch = get_int();
    char line[256];

    if (fch == 1) {
        printf("Student ID: ");
        int target_id = get_int();
        printf("Amount Paid: ");
        double amt = get_dbl();
        FILE *src = fopen("fees.txt", "r");
        FILE *tmp = fopen("temp.txt", "w");
        if (src && tmp) {
            int id;
            double monthly, extra, due;
            char status[20], date[30];
            while (fgets(line, sizeof(line), src)) {
                if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &monthly, &extra, &due, status, date) == 6) {
                    if (id == target_id) {
                        due -= amt;
                        if (due <= 0) {
                            strcpy(status, "Paid");
                        }
                    }
                    fprintf(tmp, "%d;%.2f;%.2f;%.2f;%s;%s\n", id, monthly, extra, due, status, date);
                }
            }
            fclose(src);
            fclose(tmp);
            remove("fees.txt");
            rename("temp.txt", "fees.txt");
        }
        printf("Payment recorded successfully!\n");
    } else {
        FILE *f = fopen("fees.txt", "r");
        if (f) {
            int id;
            double monthly, extra, due;
            char status[20], date[30];
            printf("\n--- Fee Defaulters List ---\n");
            while (fgets(line, sizeof(line), f)) {
                if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &monthly, &extra, &due, status, date) == 6) {
                    if (due > 0) {
                        printf("Student #%d: Outstanding Due BDT %.2f (%s)\n", id, due, status);
                    }
                }
            }
            fclose(f);
        }
    }
}

void admin_executive_summary() {
    int st_count = 0, room_count = 0, comp_count = 0, def_count = 0;
    char line[256];

    FILE *f = fopen("students.txt", "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            st_count++;
        }
        fclose(f);
    }
    f = fopen("rooms.txt", "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            room_count++;
        }
        fclose(f);
    }
    f = fopen("complaints.txt", "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            comp_count++;
        }
        fclose(f);
    }
    f = fopen("fees.txt", "r");
    if (f) {
        int id;
        double monthly, extra, due;
        char status[20], date[30];
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]", &id, &monthly, &extra, &due, status, date) == 6) {
                if (due > 0) {
                    def_count++;
                }
            }
        }
        fclose(f);
    }

    printf("\n=== Executive Summary Dashboard ===\n");
    printf("Total Registered Students: %d\n", st_count);
    printf("Total Registered Rooms:    %d\n", room_count);
    printf("Logged Complaints:         %d\n", comp_count);
    printf("Defaulter Fee Accounts:    %d\n", def_count);
}

void login_portal() {
    while (1) {
        printf("\n===========================================\n");
        printf("  UNIVERSITY HOSTEL MANAGEMENT SYSTEM CLI  \n");
        printf("===========================================\n");
        printf("1. Admin Portal\n2. Student Login\n3. Staff Login\n4. Exit\nChoice: ");
        int ch = get_int();
        if (ch == 4) {
            break;
        }

        printf("ID / Username: ");
        char u[32], p[32];
        get_str(u, sizeof(u));
        printf("Password: ");
        get_str(p, sizeof(p));

        if (ch == 1) {
            if (strcmp(u, "admin") == 0 && strcmp(p, "admin") == 0) {
                admin_portal();
            } else {
                printf("Auth Failed!\n");
                pause_term();
            }
        } else if (ch == 2) {
            student_portal(atoi(u));
        } else if (ch == 3) {
            staff_portal(atoi(u));
        }
    }
}


/* ============================================================================
 * Section 2: SADIA (Registration, Search, Room/Staff Management & Complaints)
 * ============================================================================ */

void admin_register_student() {
    printf("Student ID: ");
    int id = get_int();
    printf("Full Name: ");
    char name[50], dept[50], phone[20], pass[20], data[256];
    get_str(name, sizeof(name));
    printf("Department: ");
    get_str(dept, sizeof(dept));
    printf("Phone Number: ");
    get_str(phone, sizeof(phone));
    printf("Password: ");
    get_str(pass, sizeof(pass));
    printf("Room Number: ");
    int rm = get_int();
    printf("Monthly Fee: ");
    double fee = get_dbl();

    sprintf(data, "%d;%s;%s;%s;%s;%d", id, name, dept, phone, pass, rm);
    append_line("students.txt", data);
    sprintf(data, "%d;%.2f;0.00;%.2f;Unpaid;2026-07-22", id, fee, fee);
    append_line("fees.txt", data);
    printf("Student registered successfully!\n");
}

void admin_search_student() {
    printf("Enter Name Search Query: ");
    char q[50];
    get_str(q, sizeof(q));
    FILE *f = fopen("students.txt", "r");
    if (f) {
        char line[256];
        int id, rm;
        char name[50], dept[50], phone[20], pass[20];
        printf("\n--- Search Results ---\n");
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d", &id, name, dept, phone, pass, &rm) == 6) {
                if (strstr(name, q)) {
                    printf("ID: %d | Name: %s | Dept: %s | Phone: %s | Room: %d\n", id, name, dept, phone, rm);
                }
            }
        }
        fclose(f);
    }
}

void admin_room_ops() {
    printf("1. Register Room\n2. View Rooms\nChoice: ");
    int rch = get_int();
    char data[256], line[256];

    if (rch == 1) {
        printf("Room Number: ");
        int rm = get_int();
        printf("Bed Capacity: ");
        int cap = get_int();
        sprintf(data, "%d;%d;0", rm, cap);
        append_line("rooms.txt", data);
        printf("Room registered!\n");
    } else {
        FILE *f = fopen("rooms.txt", "r");
        if (f) {
            int rm, cap, occ;
            printf("\n--- Rooms Registry ---\n");
            while (fgets(line, sizeof(line), f)) {
                if (sscanf(line, "%d;%d;%d", &rm, &cap, &occ) == 3) {
                    printf("Room %d: Occupied %d / %d beds\n", rm, occ, cap);
                }
            }
            fclose(f);
        }
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

void staff_update_complaint() {
    printf("Enter Complaint Ref ID: ");
    int ref_id = get_int();
    FILE *src = fopen("complaints.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");
    if (src && tmp) {
        char line[256];
        int cid, sid, found = 0;
        char desc[100], status[20], date[30];
        while (fgets(line, sizeof(line), src)) {
            if (sscanf(line, "%d;%d;%99[^;];%19[^;];%29[^\n]", &cid, &sid, desc, status, date) == 5) {
                if (cid == ref_id) {
                    strcpy(status, "Resolved");
                    found = 1;
                }
                fprintf(tmp, "%d;%d;%s;%s;%s\n", cid, sid, desc, status, date);
            }
        }
        fclose(src);
        fclose(tmp);
        remove("complaints.txt");
        rename("temp.txt", "complaints.txt");
        if (found) {
            printf("Complaint marked as Resolved!\n");
        } else {
            printf("Complaint ID not found.\n");
        }
    }
}


/* ============================================================================
 * Section 3: HIRA (Portal Menu Controllers & Request Services)
 * ============================================================================ */

void student_request_transfer(int sid) {
    printf("Target Room: ");
    int rm = get_int();
    char data[256];
    sprintf(data, "%d;%d;%d;Pending", get_next_id("transfers.txt"), sid, rm);
    append_line("transfers.txt", data);
    printf("Room transfer request logged!\n");
}

void student_submit_slip(int sid) {
    printf("Transaction ID: ");
    char tx[32], data[256];
    get_str(tx, sizeof(tx));
    printf("Amount: ");
    double amt = get_dbl();
    sprintf(data, "%d;%d;%s;%.2f;Approved", get_next_id("slips.txt"), sid, tx, amt);
    append_line("slips.txt", data);
    printf("Payment slip submitted & approved!\n");
}

void student_request_leave(int sid) {
    char data[256];
    sprintf(data, "%d;%d;Pending", get_next_id("leave_requests.txt"), sid);
    append_line("leave_requests.txt", data);
    printf("Leave request logged!\n");
}

void admin_portal() {
    while (1) {
        printf("\n=== Administrator Portal ===\n");
        printf("1. Register Student\n2. Delete Student\n3. Search Student\n4. Room Operations (Add / List)\n5. Fee Operations (Payment / Defaulters)\n6. Register Staff\n7. Executive Summary\n8. Logout\nChoice: ");
        int ch = get_int();
        if (ch == 8) {
            break;
        }

        if (ch == 1) {
            admin_register_student();
        } else if (ch == 2) {
            admin_student_ops();
        } else if (ch == 3) {
            admin_search_student();
        } else if (ch == 4) {
            admin_room_ops();
        } else if (ch == 5) {
            admin_fee_ops();
        } else if (ch == 6) {
            admin_staff_ops();
        } else if (ch == 7) {
            admin_executive_summary();
        }
        pause_term();
    }
}

void student_portal(int sid) {
    while (1) {
        printf("\n=== Student Portal (ID: %d) ===\n", sid);
        printf("1. View Profile\n2. Fee Status\n3. Submit Payment Slip\n4. Request Room Transfer\n5. Lodge Complaint\n6. Request Leave\n7. Logout\nChoice: ");
        int ch = get_int();
        if (ch == 7) {
            break;
        }

        if (ch == 1) {
            student_view_profile(sid);
        } else if (ch == 2) {
            student_view_fees(sid);
        } else if (ch == 3) {
            student_submit_slip(sid);
        } else if (ch == 4) {
            student_request_transfer(sid);
        } else if (ch == 5) {
            student_submit_complaint(sid);
        } else if (ch == 6) {
            student_request_leave(sid);
        }
        pause_term();
    }
}

void staff_portal(int staff_id) {
    while (1) {
        printf("\n=== Staff Portal (ID: %d) ===\n", staff_id);
        printf("1. View Complaints\n2. Resolve Complaint\n3. Logout\nChoice: ");
        int ch = get_int();
        if (ch == 3) {
            break;
        }

        if (ch == 1) {
            staff_view_complaints();
        } else if (ch == 2) {
            staff_update_complaint();
        }
        pause_term();
    }
}


/* ============================================================================
 * Section 4: PROTHOMA (Screen Utilities & Data Inspection Views)
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

void staff_view_complaints() {
    FILE *f = fopen("complaints.txt", "r");
    if (f) {
        printf("\n--- Complaints Log ---\n");
        char line[256];
        int cid, sid;
        char desc[100], status[20], date[30];
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d;%d;%99[^;];%19[^;];%29[^\n]", &cid, &sid, desc, status, date) == 5) {
                printf("Ref #%d | Student #%d | %s | Status: %s\n", cid, sid, desc, status);
            }
        }
        fclose(f);
    }
}

void student_submit_complaint(int sid) {
    printf("Complaint Description: ");
    char desc[100], data[256];
    get_str(desc, sizeof(desc));
    sprintf(data, "%d;%d;%s;Pending;2026-07-22", get_next_id("complaints.txt"), sid, desc);
    append_line("complaints.txt", data);
    printf("Complaint logged!\n");
}
