#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/* --- Global Constant Definitions --- */
#define MAX_NAME 50
#define MAX_DEPARTMENT 50
#define MAX_PHONE 20
#define MAX_PASSWORD 20
#define MAX_TRANSACTION 30
#define MAX_DESCRIPTION 101
#define MAX_STATUS 20
#define MAX_DATE 30

#define LINE_BUFFER_SIZE 256
#define SMALL_BUFFER_SIZE 32
#define TRANSFER_FEE_AMOUNT 6000.00


/* --- Data Structure Definitions --- */
// Written by: Prothoma
struct Student {
    int id;
    char name[MAX_NAME];
    char department[MAX_DEPARTMENT];
    char phone[MAX_PHONE];
    char password[MAX_PASSWORD];
    int room_number; /* 0 represents unallocated room */
};

// Written by: Prothoma
struct Room {
    int room_number;
    int capacity;
    int occupied_beds;
};

// Written by: Prothoma
struct Fee {
    int student_id;
    double monthly_fee;
    double extra_fee;
    double due_amount;
    char status[MAX_STATUS]; /* "Paid" or "Unpaid" */
    char payment_date[MAX_DATE];
};

// Written by: Prothoma
struct Complaint {
    int id;
    int student_id;
    char description[MAX_DESCRIPTION];
    char status[MAX_STATUS]; /* "Pending", "In Progress", or "Resolved" */
    char date[MAX_DATE];
};

// Written by: Prothoma
struct RoomTransfer {
    int request_id;
    int student_id;
    int target_room_number;
    char status[MAX_STATUS]; /* "Pending", "Approved", or "Rejected" */
};

// Written by: Prothoma
struct PaymentSlip {
    int slip_id;
    int student_id;
    char transaction_number[MAX_TRANSACTION];
    double amount;
    char status[MAX_STATUS]; /* "Pending", "Approved", or "Rejected" */
};

// Written by: Prothoma
struct LeaveRequest {
    int request_id;
    int student_id;
    char status[MAX_STATUS]; /* "Pending", "Approved", or "Rejected" */
};

// Written by: Prothoma
struct Staff {
    int id;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char password[MAX_PASSWORD];
};


/* ============================================================================
 * STEP 1: ALL FUNCTION PROTOTYPES (ORDER: RAFI -> SADIA -> HIRA -> PROTHOMA)
 * ============================================================================ */

/* --- Rafi's Prototypes --- */

void initialize_files();
void get_input_string(char buffer[], int buffer_size);
int get_input_integer();
double get_input_double();
void get_current_date(char date_buffer[], int buffer_size);
int get_next_complaint_id();
int get_next_transfer_id();
int get_next_slip_id();
int get_next_leave_id();
void student_view_profile(int student_id);
void student_view_fees(int student_id);
int update_room_occupancy(int room_number, int change_amount);
void delete_student_all_records(int student_id);
void admin_fee_operations();
void admin_manage_requests();
void login_portal();
int main();

/* --- Sadia's Prototypes --- */

int student_exists(int student_id);
int get_student_name(int student_id, char name_buffer[]);
struct Room get_room(int room_number);
int staff_exists(int staff_id);
void delete_staff_record(int staff_id);
void admin_search_student();
void admin_room_operations();
void admin_staff_operations();
void admin_student_operations();

/* --- Hira's Prototypes --- */

void admin_register_student();
void admin_executive_summary();
void student_request_transfer(int student_id);
void staff_update_complaint();
void admin_portal();
void student_portal(int student_id);
void staff_portal(int staff_id);

/* --- Prothoma's Prototypes --- */

void clear_screen();
void print_line(int line_length);
void staff_view_complaints();
void student_submit_slip(int student_id);
void student_submit_complaint(int student_id);
void student_request_leave(int student_id);


/* ============================================================================
 * STEP 2: MAIN ENTRY FUNCTION
 * ============================================================================ */

// Written by: Rafi
int main() {
    initialize_files();
    login_portal();
    return 0;
}


/* ============================================================================
 * STEP 3: ACTUAL FUNCTION DEFINITIONS (ORDER: RAFI -> SADIA -> HIRA -> PROTHOMA)
 * ============================================================================ */

/* ============================================================================
 * Section 1: RAFI (46.9% Actual Workload - Core Database Engine, I/O Utilities & Login Controller)
 * ============================================================================ */

// Written by: Rafi
void initialize_files() {
    const char file_names[8][30] = {
        "students.txt", "rooms.txt", "fees.txt", "complaints.txt",
        "transfers.txt", "slips.txt", "leave_requests.txt", "staff.txt"
    };
    for (int index = 0; index < 8; index++) {
        FILE *file_pointer = fopen(file_names[index], "a");
        if (file_pointer != NULL) {
            fclose(file_pointer);
        }
    }
}

// Written by: Rafi
void get_input_string(char buffer[], int buffer_size) {
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        int length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        } else {
            /* Clear any remaining characters left in stdin buffer */
            int character;
            while ((character = getchar()) != '\n' && character != EOF) {
                /* Loop until end of line */
            }
        }
    }
}

// Written by: Rafi
int get_input_integer() {
    char input_buffer[SMALL_BUFFER_SIZE];
    int parsed_value = -1;
    get_input_string(input_buffer, sizeof(input_buffer));
    if (sscanf(input_buffer, "%d", &parsed_value) == 1) {
        return parsed_value;
    }
    return -1;
}

// Written by: Rafi
double get_input_double() {
    char input_buffer[SMALL_BUFFER_SIZE];
    double parsed_value = -1.0;
    get_input_string(input_buffer, sizeof(input_buffer));
    if (sscanf(input_buffer, "%lf", &parsed_value) == 1) {
        return parsed_value;
    }
    return -1.0;
}

// Written by: Rafi
void get_current_date(char date_buffer[], int buffer_size) {
    time_t current_time = time(NULL);
    struct tm time_info = *localtime(&current_time);
    /* Format year (+1900), month (+1), and day into date string */
    snprintf(date_buffer, buffer_size, "%04d-%02d-%02d", 
             time_info.tm_year + 1900, time_info.tm_mon + 1, time_info.tm_mday);
}

// Written by: Rafi
int get_next_complaint_id() {
    FILE *file_pointer = fopen("complaints.txt", "r");
    if (file_pointer == NULL) {
        return 1;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct Complaint complaint;
    int maximum_id = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%100[^;];%19[^;];%29[^\n]",
                   &complaint.id, &complaint.student_id, complaint.description,
                   complaint.status, complaint.date) == 5) {
            if (complaint.id > maximum_id) {
                maximum_id = complaint.id;
            }
        }
    }
    fclose(file_pointer);
    return maximum_id + 1;
}

// Written by: Rafi
int get_next_transfer_id() {
    FILE *file_pointer = fopen("transfers.txt", "r");
    if (file_pointer == NULL) {
        return 1;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct RoomTransfer transfer_request;
    int maximum_id = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%d;%19[^\n]",
                   &transfer_request.request_id, &transfer_request.student_id,
                   &transfer_request.target_room_number, transfer_request.status) == 4) {
            if (transfer_request.request_id > maximum_id) {
                maximum_id = transfer_request.request_id;
            }
        }
    }
    fclose(file_pointer);
    return maximum_id + 1;
}

// Written by: Rafi
int get_next_slip_id() {
    FILE *file_pointer = fopen("slips.txt", "r");
    if (file_pointer == NULL) {
        return 1;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct PaymentSlip payment_slip;
    int maximum_id = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%29[^;];%lf;%19[^\n]",
                   &payment_slip.slip_id, &payment_slip.student_id,
                   payment_slip.transaction_number, &payment_slip.amount, payment_slip.status) == 5) {
            if (payment_slip.slip_id > maximum_id) {
                maximum_id = payment_slip.slip_id;
            }
        }
    }
    fclose(file_pointer);
    return maximum_id + 1;
}

// Written by: Rafi
int get_next_leave_id() {
    FILE *file_pointer = fopen("leave_requests.txt", "r");
    if (file_pointer == NULL) {
        return 1;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct LeaveRequest leave_request;
    int maximum_id = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%19[^\n]",
                   &leave_request.request_id, &leave_request.student_id, leave_request.status) == 3) {
            if (leave_request.request_id > maximum_id) {
                maximum_id = leave_request.request_id;
            }
        }
    }
    fclose(file_pointer);
    return maximum_id + 1;
}

// Written by: Rafi
void student_view_profile(int student_id) {
    clear_screen();
    printf("=== Student Personal Profile ===\n");
    FILE *file_pointer = fopen("students.txt", "r");
    if (file_pointer == NULL) {
        return;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct Student student;
    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                   &student.id, student.name, student.department,
                   student.phone, student.password, &student.room_number) == 6) {
            if (student.id == student_id) {
                printf("Student ID:   %d\n", student.id);
                printf("Name:         %s\n", student.name);
                printf("Department:   %s\n", student.department);
                printf("Phone Number: %s\n", student.phone);
                printf("Room Number:  ");
                if (student.room_number > 0) {
                    printf("%d\n", student.room_number);
                } else {
                    printf("Unallocated / None\n");
                }
                break;
            }
        }
    }
    fclose(file_pointer);
    print_line(35);
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Rafi
void student_view_fees(int student_id) {
    clear_screen();
    printf("=== Student Billing & Dues Status ===\n");
    FILE *file_pointer = fopen("fees.txt", "r");
    if (file_pointer == NULL) {
        return;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct Fee fee_record;
    int record_found = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                   &fee_record.student_id, &fee_record.monthly_fee,
                   &fee_record.extra_fee, &fee_record.due_amount,
                   fee_record.status, fee_record.payment_date) == 6) {
            if (fee_record.student_id == student_id) {
                printf("Monthly Baseline: BDT %.2f\n", fee_record.monthly_fee);
                printf("Extra Fees:       BDT %.2f\n", fee_record.extra_fee);
                printf("Total Current:    BDT %.2f\n", fee_record.due_amount);
                printf("Status Indicator: %s\n", fee_record.status);
                printf("Last Payment Log: %s\n", fee_record.payment_date);
                record_found = 1;
                break;
            }
        }
    }
    fclose(file_pointer);
    if (record_found == 0) {
        printf("No fee records established yet for ID %d.\n", student_id);
    }
    print_line(40);
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Rafi
int update_room_occupancy(int room_number, int change_amount) {
    FILE *source_file = fopen("rooms.txt", "r");
    if (source_file == NULL) {
        return 0;
    }
    FILE *temporary_file = fopen("temp_rooms.txt", "w");
    if (temporary_file == NULL) {
        fclose(source_file);
        return 0;
    }

    char line_buffer[LINE_BUFFER_SIZE];
    struct Room room;
    int update_successful = 0;

    while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%d",
                   &room.room_number, &room.capacity, &room.occupied_beds) == 3) {
            if (room.room_number == room_number) {
                room.occupied_beds = room.occupied_beds + change_amount;
                if (room.occupied_beds < 0) {
                    room.occupied_beds = 0;
                }
                if (room.occupied_beds > room.capacity) {
                    room.occupied_beds = room.capacity;
                }
                update_successful = 1;
            }
            fprintf(temporary_file, "%d;%d;%d\n", room.room_number, room.capacity, room.occupied_beds);
        }
    }
    fclose(source_file);
    fclose(temporary_file);

    /* Replace original file with temporary file */
    remove("rooms.txt");
    rename("temp_rooms.txt", "rooms.txt");
    return update_successful;
}

// Written by: Rafi
void delete_student_all_records(int student_id) {
    char line_buffer[LINE_BUFFER_SIZE];

    /* 1. Delete from students.txt */
    FILE *source_file = fopen("students.txt", "r");
    if (source_file != NULL) {
        FILE *temporary_file = fopen("temp_students.txt", "w");
        if (temporary_file != NULL) {
            struct Student student;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                           &student.id, student.name, student.department,
                           student.phone, student.password, &student.room_number) == 6) {
                    if (student.id != student_id) {
                        fprintf(temporary_file, "%d;%s;%s;%s;%s;%d\n",
                                student.id, student.name, student.department,
                                student.phone, student.password, student.room_number);
                    }
                }
            }
            fclose(temporary_file);
        }
        fclose(source_file);
        remove("students.txt");
        rename("temp_students.txt", "students.txt");
    }

    /* 2. Delete from fees.txt */
    source_file = fopen("fees.txt", "r");
    if (source_file != NULL) {
        FILE *temporary_file = fopen("temp_fees.txt", "w");
        if (temporary_file != NULL) {
            struct Fee fee_record;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                           &fee_record.student_id, &fee_record.monthly_fee,
                           &fee_record.extra_fee, &fee_record.due_amount,
                           fee_record.status, fee_record.payment_date) == 6) {
                    if (fee_record.student_id != student_id) {
                        fprintf(temporary_file, "%d;%.2f;%.2f;%.2f;%s;%s\n",
                                fee_record.student_id, fee_record.monthly_fee,
                                fee_record.extra_fee, fee_record.due_amount,
                                fee_record.status, fee_record.payment_date);
                    }
                }
            }
            fclose(temporary_file);
        }
        fclose(source_file);
        remove("fees.txt");
        rename("temp_fees.txt", "fees.txt");
    }

    /* 3. Delete from complaints.txt */
    source_file = fopen("complaints.txt", "r");
    if (source_file != NULL) {
        FILE *temporary_file = fopen("temp_complaints.txt", "w");
        if (temporary_file != NULL) {
            struct Complaint complaint;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%d;%100[^;];%19[^;];%29[^\n]",
                           &complaint.id, &complaint.student_id, complaint.description,
                           complaint.status, complaint.date) == 5) {
                    if (complaint.student_id != student_id) {
                        fprintf(temporary_file, "%d;%d;%s;%s;%s\n",
                                complaint.id, complaint.student_id, complaint.description,
                                complaint.status, complaint.date);
                    }
                }
            }
            fclose(temporary_file);
        }
        fclose(source_file);
        remove("complaints.txt");
        rename("temp_complaints.txt", "complaints.txt");
    }

    /* 4. Delete from transfers.txt */
    source_file = fopen("transfers.txt", "r");
    if (source_file != NULL) {
        FILE *temporary_file = fopen("temp_transfers.txt", "w");
        if (temporary_file != NULL) {
            struct RoomTransfer transfer_request;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%d;%d;%19[^\n]",
                           &transfer_request.request_id, &transfer_request.student_id,
                           &transfer_request.target_room_number, transfer_request.status) == 4) {
                    if (transfer_request.student_id != student_id) {
                        fprintf(temporary_file, "%d;%d;%d;%s\n",
                                transfer_request.request_id, transfer_request.student_id,
                                transfer_request.target_room_number, transfer_request.status);
                    }
                }
            }
            fclose(temporary_file);
        }
        fclose(source_file);
        remove("transfers.txt");
        rename("temp_transfers.txt", "transfers.txt");
    }

    /* 5. Delete from slips.txt */
    source_file = fopen("slips.txt", "r");
    if (source_file != NULL) {
        FILE *temporary_file = fopen("temp_slips.txt", "w");
        if (temporary_file != NULL) {
            struct PaymentSlip payment_slip;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%d;%29[^;];%lf;%19[^\n]",
                           &payment_slip.slip_id, &payment_slip.student_id,
                           payment_slip.transaction_number, &payment_slip.amount, payment_slip.status) == 5) {
                    if (payment_slip.student_id != student_id) {
                        fprintf(temporary_file, "%d;%d;%s;%.2f;%s\n",
                                payment_slip.slip_id, payment_slip.student_id,
                                payment_slip.transaction_number, payment_slip.amount, payment_slip.status);
                    }
                }
            }
            fclose(temporary_file);
        }
        fclose(source_file);
        remove("slips.txt");
        rename("temp_slips.txt", "slips.txt");
    }

    /* 6. Delete from leave_requests.txt */
    source_file = fopen("leave_requests.txt", "r");
    if (source_file != NULL) {
        FILE *temporary_file = fopen("temp_leave.txt", "w");
        if (temporary_file != NULL) {
            struct LeaveRequest leave_request;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%d;%19[^\n]",
                           &leave_request.request_id, &leave_request.student_id, leave_request.status) == 3) {
                    if (leave_request.student_id != student_id) {
                        fprintf(temporary_file, "%d;%d;%s\n",
                                leave_request.request_id, leave_request.student_id, leave_request.status);
                    }
                }
            }
            fclose(temporary_file);
        }
        fclose(source_file);
        remove("leave_requests.txt");
        rename("temp_leave.txt", "leave_requests.txt");
    }
}

// Written by: Rafi
void admin_fee_operations() {
    clear_screen();
    printf("=== Billings & Fee Operations ===\n");
    printf("1. Record Manual Payment\n");
    printf("2. Reset Billing Cycle (New Month)\n");
    printf("3. View Defaulters List\n");
    printf("4. View Financial Summary\n");
    printf("Choose Action: ");
    int action_choice = get_input_integer();

    if (action_choice == 1) {
        printf("Enter Student ID: ");
        int target_student_id = get_input_integer();

        FILE *fee_file_pointer = fopen("fees.txt", "r");
        if (fee_file_pointer == NULL) {
            printf("No billing data.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        char line_buffer[LINE_BUFFER_SIZE];
        struct Fee target_fee_record;
        int fee_record_found = 0;

        while (fgets(line_buffer, sizeof(line_buffer), fee_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                       &target_fee_record.student_id, &target_fee_record.monthly_fee,
                       &target_fee_record.extra_fee, &target_fee_record.due_amount,
                       target_fee_record.status, target_fee_record.payment_date) == 6) {
                if (target_fee_record.student_id == target_student_id) {
                    fee_record_found = 1;
                    break;
                }
            }
        }
        fclose(fee_file_pointer);

        if (fee_record_found == 0) {
            printf("Record not found.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }

        char student_name[MAX_NAME];
        student_name[0] = '\0';
        get_student_name(target_student_id, student_name);

        printf("\nStudent: %s (ID: %d)\n", student_name, target_student_id);
        printf("Baseline: BDT %.2f\n", target_fee_record.monthly_fee);
        printf("Extra:    BDT %.2f\n", target_fee_record.extra_fee);
        printf("Due:      BDT %.2f\n", target_fee_record.due_amount);
        printf("Status:   %s\n", target_fee_record.status);
        print_line(45);

        printf("Enter Amount Paid: ");
        double paid_amount = get_input_double();
        if (paid_amount <= 0.0) {
            printf("Invalid amount.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }

        char payment_date[MAX_DATE];
        get_current_date(payment_date, sizeof(payment_date));
        printf("Suggested Date [%s]. Custom? (Enter to skip): ", payment_date);
        char custom_date[MAX_DATE];
        get_input_string(custom_date, sizeof(custom_date));
        if (strlen(custom_date) > 0) {
            strcpy(payment_date, custom_date);
        }

        target_fee_record.due_amount = target_fee_record.due_amount - paid_amount;
        if (target_fee_record.due_amount <= 0.0) {
            strcpy(target_fee_record.status, "Paid");
        } else {
            strcpy(target_fee_record.status, "Unpaid");
        }
        strcpy(target_fee_record.payment_date, payment_date);

        FILE *source_file = fopen("fees.txt", "r");
        FILE *temporary_file = fopen("temp_fees.txt", "w");
        if (source_file != NULL && temporary_file != NULL) {
            struct Fee current_fee;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                           &current_fee.student_id, &current_fee.monthly_fee,
                           &current_fee.extra_fee, &current_fee.due_amount,
                           current_fee.status, current_fee.payment_date) == 6) {
                    if (current_fee.student_id == target_student_id) {
                        fprintf(temporary_file, "%d;%.2f;%.2f;%.2f;%s;%s\n",
                                target_fee_record.student_id, target_fee_record.monthly_fee,
                                target_fee_record.extra_fee, target_fee_record.due_amount,
                                target_fee_record.status, target_fee_record.payment_date);
                    } else {
                        fprintf(temporary_file, "%d;%.2f;%.2f;%.2f;%s;%s\n",
                                current_fee.student_id, current_fee.monthly_fee,
                                current_fee.extra_fee, current_fee.due_amount,
                                current_fee.status, current_fee.payment_date);
                    }
                }
            }
            fclose(source_file);
            fclose(temporary_file);
            remove("fees.txt");
            rename("temp_fees.txt", "fees.txt");
        }
        printf("Payment logged. Remaining due: BDT %.2f\n", target_fee_record.due_amount);
    } else if (action_choice == 2) {
        printf("Confirm monthly reset? (Y/N): ");
        char check_confirmation[10];
        get_input_string(check_confirmation, sizeof(check_confirmation));
        if (check_confirmation[0] != 'Y' && check_confirmation[0] != 'y') {
            return;
        }

        FILE *source_file = fopen("fees.txt", "r");
        if (source_file == NULL) {
            return;
        }
        FILE *temporary_file = fopen("temp_fees.txt", "w");
        if (temporary_file == NULL) {
            fclose(source_file);
            return;
        }

        char line_buffer[LINE_BUFFER_SIZE];
        struct Fee current_fee;
        int reset_count = 0;

        while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
            if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                       &current_fee.student_id, &current_fee.monthly_fee,
                       &current_fee.extra_fee, &current_fee.due_amount,
                       current_fee.status, current_fee.payment_date) == 6) {
                current_fee.due_amount = current_fee.due_amount + current_fee.monthly_fee;
                current_fee.extra_fee = 0.0;
                if (current_fee.due_amount > 0.0) {
                    strcpy(current_fee.status, "Unpaid");
                    strcpy(current_fee.payment_date, "N/A");
                } else {
                    strcpy(current_fee.status, "Paid");
                    strcpy(current_fee.payment_date, "Prepaid");
                }
                fprintf(temporary_file, "%d;%.2f;%.2f;%.2f;%s;%s\n",
                        current_fee.student_id, current_fee.monthly_fee,
                        current_fee.extra_fee, current_fee.due_amount,
                        current_fee.status, current_fee.payment_date);
                reset_count = reset_count + 1;
            }
        }
        fclose(source_file);
        fclose(temporary_file);
        remove("fees.txt");
        rename("temp_fees.txt", "fees.txt");
        printf("Billing reset for %d profiles.\n", reset_count);
    } else if (action_choice == 3) {
        FILE *fee_file_pointer = fopen("fees.txt", "r");
        if (fee_file_pointer == NULL) {
            printf("No records found.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        print_line(75);
        printf("%-12s %-25s %-12s %-12s %-12s\n", "Student ID", "Student Name", "Monthly Fee", "Extra Fee", "Balance Due");
        print_line(75);

        char line_buffer[LINE_BUFFER_SIZE];
        struct Fee current_fee;
        int defaulters_count = 0;

        while (fgets(line_buffer, sizeof(line_buffer), fee_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                       &current_fee.student_id, &current_fee.monthly_fee,
                       &current_fee.extra_fee, &current_fee.due_amount,
                       current_fee.status, current_fee.payment_date) == 6) {
                if (strcmp(current_fee.status, "Unpaid") == 0 || current_fee.due_amount > 0.0) {
                    char student_name[MAX_NAME];
                    strcpy(student_name, "Unknown");
                    get_student_name(current_fee.student_id, student_name);
                    printf("%-12d %-25s %-12.2f %-12.2f BDT %-11.2f\n",
                           current_fee.student_id, student_name, current_fee.monthly_fee,
                           current_fee.extra_fee, current_fee.due_amount);
                    defaulters_count = defaulters_count + 1;
                }
            }
        }
        print_line(75);
        printf("Total Defaulters: %d\n", defaulters_count);
        fclose(fee_file_pointer);
    } else if (action_choice == 4) {
        FILE *fee_file_pointer = fopen("fees.txt", "r");
        if (fee_file_pointer == NULL) {
            printf("No billing data.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        double expected_total = 0.0;
        double unpaid_total = 0.0;
        char line_buffer[LINE_BUFFER_SIZE];
        struct Fee current_fee;

        while (fgets(line_buffer, sizeof(line_buffer), fee_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                       &current_fee.student_id, &current_fee.monthly_fee,
                       &current_fee.extra_fee, &current_fee.due_amount,
                       current_fee.status, current_fee.payment_date) == 6) {
                expected_total = expected_total + (current_fee.monthly_fee + current_fee.extra_fee);
                if (current_fee.due_amount > 0.0) {
                    unpaid_total = unpaid_total + current_fee.due_amount;
                }
            }
        }
        fclose(fee_file_pointer);

        double collected_total = expected_total - unpaid_total;
        if (collected_total < 0.0) {
            collected_total = 0.0;
        }
        print_line(45);
        printf("Projected Billings: BDT %.2f\n", expected_total);
        printf("Revenue Collected:  BDT %.2f\n", collected_total);
        printf("Outstanding Dues:   BDT %.2f\n", unpaid_total);
        print_line(45);
    }
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Rafi
void admin_manage_requests() {
    clear_screen();
    printf("=== Process Verification Requests ===\n");
    printf("1. Room Transfer Requests\n");
    printf("2. Payment Slips\n");
    printf("3. Leave Requests\n");
    printf("Choose Verification: ");
    int request_type_choice = get_input_integer();

    char line_buffer[LINE_BUFFER_SIZE];

    if (request_type_choice == 1) {
        FILE *transfer_file_pointer = fopen("transfers.txt", "r");
        if (transfer_file_pointer == NULL) {
            printf("No requests found.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        print_line(65);
        printf("%-10s %-12s %-15s %-12s\n", "Req ID", "Student ID", "Target Room", "Status");
        print_line(65);

        struct RoomTransfer current_transfer;
        int pending_count = 0;
        while (fgets(line_buffer, sizeof(line_buffer), transfer_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%d;%d;%19[^\n]",
                       &current_transfer.request_id, &current_transfer.student_id,
                       &current_transfer.target_room_number, current_transfer.status) == 4) {
                if (strcmp(current_transfer.status, "Pending") == 0) {
                    printf("%-10d %-12d %-15d %-12s\n",
                           current_transfer.request_id, current_transfer.student_id,
                           current_transfer.target_room_number, current_transfer.status);
                    pending_count = pending_count + 1;
                }
            }
        }
        fclose(transfer_file_pointer);
        print_line(65);

        if (pending_count == 0) {
            printf("No pending transfer requests.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }

        printf("Enter Request ID: ");
        int selected_request_id = get_input_integer();

        FILE *source_file = fopen("transfers.txt", "r");
        FILE *temporary_file = fopen("temp_transfers.txt", "w");
        if (source_file != NULL && temporary_file != NULL) {
            struct RoomTransfer transfer_record;
            int found_request = 0;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%d;%d;%19[^\n]",
                           &transfer_record.request_id, &transfer_record.student_id,
                           &transfer_record.target_room_number, transfer_record.status) == 4) {
                    if (transfer_record.request_id == selected_request_id && strcmp(transfer_record.status, "Pending") == 0) {
                        found_request = 1;
                        printf("1. Approve Transfer\n");
                        printf("2. Reject Request\n");
                        printf("Action: ");
                        int action_choice = get_input_integer();

                        if (action_choice == 1) {
                            struct Room target_room_details = get_room(transfer_record.target_room_number);
                            if (target_room_details.room_number != 0 && (target_room_details.capacity - target_room_details.occupied_beds > 0)) {
                                /* Update student's room assignment */
                                FILE *student_source = fopen("students.txt", "r");
                                FILE *student_temp = fopen("temp_students.txt", "w");
                                if (student_source != NULL && student_temp != NULL) {
                                    char s_buffer[LINE_BUFFER_SIZE];
                                    struct Student current_student;
                                    while (fgets(s_buffer, sizeof(s_buffer), student_source) != NULL) {
                                        if (sscanf(s_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                                                   &current_student.id, current_student.name, current_student.department,
                                                   current_student.phone, current_student.password, &current_student.room_number) == 6) {
                                            if (current_student.id == transfer_record.student_id) {
                                                if (current_student.room_number > 0) {
                                                    update_room_occupancy(current_student.room_number, -1);
                                                }
                                                update_room_occupancy(transfer_record.target_room_number, 1);
                                                current_student.room_number = transfer_record.target_room_number;
                                            }
                                            fprintf(student_temp, "%d;%s;%s;%s;%s;%d\n",
                                                    current_student.id, current_student.name, current_student.department,
                                                    current_student.phone, current_student.password, current_student.room_number);
                                        }
                                    }
                                    fclose(student_source);
                                    fclose(student_temp);
                                    remove("students.txt");
                                    rename("temp_students.txt", "students.txt");
                                }

                                /* Apply standard transfer fee */
                                FILE *fee_source = fopen("fees.txt", "r");
                                FILE *fee_temp = fopen("temp_fees.txt", "w");
                                if (fee_source != NULL && fee_temp != NULL) {
                                    char f_buffer[LINE_BUFFER_SIZE];
                                    struct Fee current_fee;
                                    while (fgets(f_buffer, sizeof(f_buffer), fee_source) != NULL) {
                                        if (sscanf(f_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                                                   &current_fee.student_id, &current_fee.monthly_fee,
                                                   &current_fee.extra_fee, &current_fee.due_amount,
                                                   current_fee.status, current_fee.payment_date) == 6) {
                                            if (current_fee.student_id == transfer_record.student_id) {
                                                current_fee.extra_fee = current_fee.extra_fee + TRANSFER_FEE_AMOUNT;
                                                current_fee.due_amount = current_fee.due_amount + TRANSFER_FEE_AMOUNT;
                                                strcpy(current_fee.status, "Unpaid");
                                            }
                                            fprintf(fee_temp, "%d;%.2f;%.2f;%.2f;%s;%s\n",
                                                    current_fee.student_id, current_fee.monthly_fee,
                                                    current_fee.extra_fee, current_fee.due_amount,
                                                    current_fee.status, current_fee.payment_date);
                                        }
                                    }
                                    fclose(fee_source);
                                    fclose(fee_temp);
                                    remove("fees.txt");
                                    rename("temp_fees.txt", "fees.txt");
                                }

                                strcpy(transfer_record.status, "Approved");
                                printf("Transfer approved and BDT %.2f transfer fee applied.\n", TRANSFER_FEE_AMOUNT);
                            } else {
                                printf("Target room space occupied or non-existent.\n");
                            }
                        } else if (action_choice == 2) {
                            strcpy(transfer_record.status, "Rejected");
                            printf("Transfer request rejected.\n");
                        }
                    }
                    fprintf(temporary_file, "%d;%d;%d;%s\n",
                            transfer_record.request_id, transfer_record.student_id,
                            transfer_record.target_room_number, transfer_record.status);
                }
            }
            fclose(source_file);
            fclose(temporary_file);
            remove("transfers.txt");
            rename("temp_transfers.txt", "transfers.txt");

            if (found_request == 0) {
                printf("Request ID not found.\n");
            }
        }
    } else if (request_type_choice == 2) {
        FILE *slip_file_pointer = fopen("slips.txt", "r");
        if (slip_file_pointer == NULL) {
            printf("No payment slips found.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        print_line(70);
        printf("%-10s %-12s %-20s %-15s %-10s\n", "Slip ID", "Student ID", "Tx ID", "Amount", "Status");
        print_line(70);

        struct PaymentSlip current_slip;
        int total_count = 0;
        while (fgets(line_buffer, sizeof(line_buffer), slip_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%d;%29[^;];%lf;%19[^\n]",
                       &current_slip.slip_id, &current_slip.student_id,
                       current_slip.transaction_number, &current_slip.amount, current_slip.status) == 5) {
                printf("%-10d %-12d %-20s BDT %-11.2f %-10s\n",
                       current_slip.slip_id, current_slip.student_id,
                       current_slip.transaction_number, current_slip.amount, current_slip.status);
                total_count = total_count + 1;
            }
        }
        fclose(slip_file_pointer);
        print_line(70);

        if (total_count == 0) {
            printf("No payment slips found.\n");
        } else {
            printf("Note: Payment slips are auto-approved upon student submission.\n");
        }
    } else if (request_type_choice == 3) {
        FILE *leave_file_pointer = fopen("leave_requests.txt", "r");
        if (leave_file_pointer == NULL) {
            printf("No leave requests found.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        print_line(50);
        printf("%-12s %-12s %-12s\n", "Req ID", "Student ID", "Status");
        print_line(50);

        struct LeaveRequest current_leave;
        int pending_count = 0;
        while (fgets(line_buffer, sizeof(line_buffer), leave_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%d;%19[^\n]",
                       &current_leave.request_id, &current_leave.student_id, current_leave.status) == 3) {
                if (strcmp(current_leave.status, "Pending") == 0) {
                    printf("%-12d %-12d %-12s\n",
                           current_leave.request_id, current_leave.student_id, current_leave.status);
                    pending_count = pending_count + 1;
                }
            }
        }
        fclose(leave_file_pointer);
        print_line(50);

        if (pending_count == 0) {
            printf("No pending vacate requests.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }

        printf("Enter Request ID: ");
        int selected_request_id = get_input_integer();

        FILE *source_file = fopen("leave_requests.txt", "r");
        FILE *temporary_file = fopen("temp_leave.txt", "w");
        if (source_file != NULL && temporary_file != NULL) {
            struct LeaveRequest leave_record;
            int found_leave = 0;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%d;%19[^\n]",
                           &leave_record.request_id, &leave_record.student_id, leave_record.status) == 3) {
                    if (leave_record.request_id == selected_request_id && strcmp(leave_record.status, "Pending") == 0) {
                        found_leave = 1;

                        /* Check student dues before approving leave */
                        FILE *fee_file_pointer = fopen("fees.txt", "r");
                        double student_dues = 0.0;
                        if (fee_file_pointer != NULL) {
                            struct Fee fee_record;
                            char f_buffer[LINE_BUFFER_SIZE];
                            while (fgets(f_buffer, sizeof(f_buffer), fee_file_pointer) != NULL) {
                                if (sscanf(f_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                                           &fee_record.student_id, &fee_record.monthly_fee,
                                           &fee_record.extra_fee, &fee_record.due_amount,
                                           fee_record.status, fee_record.payment_date) == 6) {
                                    if (fee_record.student_id == leave_record.student_id) {
                                        student_dues = fee_record.due_amount;
                                        break;
                                    }
                                }
                            }
                            fclose(fee_file_pointer);
                        }

                        if (student_dues > 0.0) {
                            printf("WARNING: Student has BDT %.2f unpaid dues balance.\n", student_dues);
                        }

                        printf("1. Approve Leave (Vacate bed)\n");
                        printf("2. Reject Leave\n");
                        printf("Action: ");
                        int action_choice = get_input_integer();

                        if (action_choice == 1) {
                            FILE *student_source = fopen("students.txt", "r");
                            FILE *student_temp = fopen("temp_students.txt", "w");
                            if (student_source != NULL && student_temp != NULL) {
                                char s_buffer[LINE_BUFFER_SIZE];
                                struct Student current_student;
                                while (fgets(s_buffer, sizeof(s_buffer), student_source) != NULL) {
                                    if (sscanf(s_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                                               &current_student.id, current_student.name, current_student.department,
                                               current_student.phone, current_student.password, &current_student.room_number) == 6) {
                                        if (current_student.id == leave_record.student_id) {
                                            if (current_student.room_number > 0) {
                                                update_room_occupancy(current_student.room_number, -1);
                                                current_student.room_number = 0;
                                            }
                                        }
                                        fprintf(student_temp, "%d;%s;%s;%s;%s;%d\n",
                                                current_student.id, current_student.name, current_student.department,
                                                current_student.phone, current_student.password, current_student.room_number);
                                    }
                                }
                                fclose(student_source);
                                fclose(student_temp);
                                remove("students.txt");
                                rename("temp_students.txt", "students.txt");
                            }
                            strcpy(leave_record.status, "Approved");
                            printf("Leave approved. Bed deallocated successfully!\n");
                        } else if (action_choice == 2) {
                            strcpy(leave_record.status, "Rejected");
                            printf("Leave request rejected.\n");
                        }
                    }
                    fprintf(temporary_file, "%d;%d;%s\n",
                            leave_record.request_id, leave_record.student_id, leave_record.status);
                }
            }
            fclose(source_file);
            fclose(temporary_file);
            remove("leave_requests.txt");
            rename("temp_leave.txt", "leave_requests.txt");

            if (found_leave == 0) {
                printf("Request ID not found.\n");
            }
        }
    }
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Rafi
void login_portal() {
    while (1) {
        clear_screen();
        printf("===========================================\n");
        printf("  UNIVERSITY HOSTEL MANAGEMENT SYSTEM CLI  \n");
        printf("===========================================\n");
        printf("1. Administrator Portal Access\n");
        printf("2. Student Services Login\n");
        printf("3. Technical/Operations Staff Login\n");
        printf("4. Shutdown Terminal\n");
        printf("Choose Entry: ");
        int choice = get_input_integer();

        if (choice == 4) {
            printf("\nShutting down. Goodbye!\n");
            break;
        }

        printf("Enter ID/Username: ");
        char username[32];
        get_input_string(username, sizeof(username));

        printf("Enter Password: ");
        char password[32];
        get_input_string(password, sizeof(password));

        if (choice == 1) {
            /* Admin credentials check */
            if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
                admin_portal();
            } else {
                printf("\nAuthentication Failed!\n");
                printf("Press Enter to continue...");
                getchar();
            }
        } else if (choice == 2 || choice == 3) {
            /* Convert input string ID to integer using standard atoi */
            int parsed_user_id = atoi(username);
            int is_authenticated = 0;

            if (choice == 2) {
                FILE *student_file_pointer = fopen("students.txt", "r");
                if (student_file_pointer != NULL) {
                    char line_buffer[LINE_BUFFER_SIZE];
                    struct Student student;
                    while (fgets(line_buffer, sizeof(line_buffer), student_file_pointer) != NULL) {
                        if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                                   &student.id, student.name, student.department,
                                   student.phone, student.password, &student.room_number) == 6) {
                            if (student.id == parsed_user_id && strcmp(student.password, password) == 0) {
                                is_authenticated = 1;
                                break;
                            }
                        }
                    }
                    fclose(student_file_pointer);
                }
                if (is_authenticated == 1) {
                    student_portal(parsed_user_id);
                }
            } else {
                FILE *staff_file_pointer = fopen("staff.txt", "r");
                if (staff_file_pointer != NULL) {
                    char line_buffer[LINE_BUFFER_SIZE];
                    struct Staff staff_member;
                    while (fgets(line_buffer, sizeof(line_buffer), staff_file_pointer) != NULL) {
                        if (sscanf(line_buffer, "%d;%49[^;];%19[^;];%19[^\n]",
                                   &staff_member.id, staff_member.name, staff_member.phone, staff_member.password) == 4) {
                            if (staff_member.id == parsed_user_id && strcmp(staff_member.password, password) == 0) {
                                is_authenticated = 1;
                                break;
                            }
                        }
                    }
                    fclose(staff_file_pointer);
                }
                if (is_authenticated == 1) {
                    staff_portal(parsed_user_id);
                }
            }

            if (is_authenticated == 0) {
                printf("\nAuthentication Failed: Invalid credentials.\n");
                printf("Press Enter to continue...");
                getchar();
            }
        }
    }
}


/* ============================================================================
 * Section 2: SADIA (22.0% Actual Workload - Entity Lookup & Database Management)
 * ============================================================================ */

// Written by: Sadia
int student_exists(int student_id) {
    FILE *file_pointer = fopen("students.txt", "r");
    if (file_pointer == NULL) {
        return 0;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct Student student;
    int found_student = 0;

    /* Read file line by line */
    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        /* %49[^;] means read text up to 49 characters until semicolon ';' */
        if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                   &student.id, student.name, student.department,
                   student.phone, student.password, &student.room_number) == 6) {
            if (student.id == student_id) {
                found_student = 1;
                break;
            }
        }
    }
    fclose(file_pointer);
    return found_student;
}

// Written by: Sadia
int get_student_name(int student_id, char name_buffer[]) {
    FILE *file_pointer = fopen("students.txt", "r");
    if (file_pointer == NULL) {
        return 0;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct Student student;
    int found_student = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                   &student.id, student.name, student.department,
                   student.phone, student.password, &student.room_number) == 6) {
            if (student.id == student_id) {
                strcpy(name_buffer, student.name);
                found_student = 1;
                break;
            }
        }
    }
    fclose(file_pointer);
    return found_student;
}

// Written by: Sadia
struct Room get_room(int room_number) {
    struct Room room;
    room.room_number = 0;
    room.capacity = 0;
    room.occupied_beds = 0;

    FILE *file_pointer = fopen("rooms.txt", "r");
    if (file_pointer == NULL) {
        return room;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct Room current_room;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%d",
                   &current_room.room_number, &current_room.capacity, &current_room.occupied_beds) == 3) {
            if (current_room.room_number == room_number) {
                room = current_room;
                break;
            }
        }
    }
    fclose(file_pointer);
    return room;
}

// Written by: Sadia
int staff_exists(int staff_id) {
    FILE *file_pointer = fopen("staff.txt", "r");
    if (file_pointer == NULL) {
        return 0;
    }
    char line_buffer[LINE_BUFFER_SIZE];
    struct Staff staff_member;
    int found_staff = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%49[^;];%19[^;];%19[^\n]",
                   &staff_member.id, staff_member.name, staff_member.phone, staff_member.password) == 4) {
            if (staff_member.id == staff_id) {
                found_staff = 1;
                break;
            }
        }
    }
    fclose(file_pointer);
    return found_staff;
}

// Written by: Sadia
void delete_staff_record(int staff_id) {
    FILE *source_file = fopen("staff.txt", "r");
    if (source_file != NULL) {
        FILE *temporary_file = fopen("temp_staff.txt", "w");
        if (temporary_file != NULL) {
            char line_buffer[LINE_BUFFER_SIZE];
            struct Staff staff_member;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%49[^;];%19[^;];%19[^\n]",
                           &staff_member.id, staff_member.name, staff_member.phone, staff_member.password) == 4) {
                    if (staff_member.id != staff_id) {
                        fprintf(temporary_file, "%d;%s;%s;%s\n",
                                staff_member.id, staff_member.name, staff_member.phone, staff_member.password);
                    }
                }
            }
            fclose(temporary_file);
        }
        fclose(source_file);
        remove("staff.txt");
        rename("temp_staff.txt", "staff.txt");
    }
}

// Written by: Sadia
void admin_search_student() {
    clear_screen();
    printf("=== Search Student Database ===\n");
    printf("Enter partial name to search: ");
    char query_string[50];
    char query_uppercase[50];
    get_input_string(query_string, sizeof(query_string));

    for (int index = 0; query_string[index] != '\0'; index++) {
        query_uppercase[index] = toupper(query_string[index]);
    }
    query_uppercase[strlen(query_string)] = '\0';

    FILE *student_file_pointer = fopen("students.txt", "r");
    if (student_file_pointer == NULL) {
        printf("No records found.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    print_line(75);
    printf("%-10s %-25s %-15s %-15s %-8s\n", "ID", "Name", "Dept", "Phone", "Room");
    print_line(75);

    char line_buffer[LINE_BUFFER_SIZE];
    struct Student current_student;
    int total_match_count = 0;

    while (fgets(line_buffer, sizeof(line_buffer), student_file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                   &current_student.id, current_student.name, current_student.department,
                   current_student.phone, current_student.password, &current_student.room_number) == 6) {
            char name_uppercase[50];
            for (int index = 0; current_student.name[index] != '\0'; index++) {
                name_uppercase[index] = toupper(current_student.name[index]);
            }
            name_uppercase[strlen(current_student.name)] = '\0';

            if (strstr(name_uppercase, query_uppercase) != NULL) {
                printf("%-10d %-25s %-15s %-15s %-8d\n",
                       current_student.id, current_student.name, current_student.department,
                       current_student.phone, current_student.room_number);
                total_match_count = total_match_count + 1;
            }
        }
    }
    print_line(75);
    printf("Total matches: %d\n", total_match_count);
    fclose(student_file_pointer);
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Sadia
void admin_room_operations() {
    clear_screen();
    printf("=== Room Operations ===\n");
    printf("1. Configure New Room\n");
    printf("2. View Vacancies List\n");
    printf("Choose Action: ");
    int action_choice = get_input_integer();

    if (action_choice == 1) {
        printf("Enter Room Number: ");
        int new_room_number = get_input_integer();
        struct Room existing_room = get_room(new_room_number);
        if (new_room_number <= 0 || existing_room.room_number != 0) {
            printf("Invalid or duplicate Room Number.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        struct Room new_room;
        new_room.room_number = new_room_number;
        new_room.capacity = 0;
        new_room.occupied_beds = 0;

        printf("Enter Bed Capacity: ");
        new_room.capacity = get_input_integer();
        if (new_room.capacity <= 0) {
            printf("Capacity must be positive.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        FILE *room_file_pointer = fopen("rooms.txt", "a");
        if (room_file_pointer != NULL) {
            fprintf(room_file_pointer, "%d;%d;%d\n",
                    new_room.room_number, new_room.capacity, new_room.occupied_beds);
            fclose(room_file_pointer);
            printf("Room registered!\n");
        }
    } else if (action_choice == 2) {
        FILE *room_file_pointer = fopen("rooms.txt", "r");
        if (room_file_pointer == NULL) {
            printf("No rooms registered.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        print_line(55);
        printf("%-15s %-12s %-12s %-10s\n", "Room Number", "Capacity", "Occupied", "Vacant");
        print_line(55);

        char line_buffer[LINE_BUFFER_SIZE];
        struct Room current_room;
        while (fgets(line_buffer, sizeof(line_buffer), room_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%d;%d",
                       &current_room.room_number, &current_room.capacity, &current_room.occupied_beds) == 3) {
                int vacant_beds = current_room.capacity - current_room.occupied_beds;
                printf("%-15d %-12d %-12d %-10d\n",
                       current_room.room_number, current_room.capacity, current_room.occupied_beds, vacant_beds);
            }
        }
        print_line(55);
        fclose(room_file_pointer);
    }
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Sadia
void admin_staff_operations() {
    clear_screen();
    printf("=== Staff Operations ===\n");
    printf("1. Register Technical Staff\n");
    printf("2. Delete Staff Account\n");
    printf("Choose Action: ");
    int action_choice = get_input_integer();

    if (action_choice == 1) {
        printf("Enter unique Staff ID: ");
        int new_staff_id = get_input_integer();
        if (new_staff_id <= 0 || staff_exists(new_staff_id)) {
            printf("Invalid or duplicate Staff ID.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        struct Staff new_staff_member;
        new_staff_member.id = new_staff_id;
        new_staff_member.name[0] = '\0';
        new_staff_member.phone[0] = '\0';
        new_staff_member.password[0] = '\0';

        printf("Enter Staff Name: ");
        get_input_string(new_staff_member.name, sizeof(new_staff_member.name));

        printf("Enter Phone: ");
        get_input_string(new_staff_member.phone, sizeof(new_staff_member.phone));

        printf("Enter Password: ");
        get_input_string(new_staff_member.password, sizeof(new_staff_member.password));

        if (strlen(new_staff_member.name) == 0 || strlen(new_staff_member.password) == 0) {
            printf("Fields cannot be empty.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        FILE *staff_file_pointer = fopen("staff.txt", "a");
        if (staff_file_pointer != NULL) {
            fprintf(staff_file_pointer, "%d;%s;%s;%s\n",
                    new_staff_member.id, new_staff_member.name,
                    new_staff_member.phone, new_staff_member.password);
            fclose(staff_file_pointer);
            printf("Staff registered successfully!\n");
        }
    } else if (action_choice == 2) {
        printf("Enter Staff ID: ");
        int target_staff_id = get_input_integer();
        if (staff_exists(target_staff_id) == 0) {
            printf("Staff Account not found.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        printf("Confirm deleting Staff ID %d? (Y/N): ", target_staff_id);
        char check_confirmation[10];
        get_input_string(check_confirmation, sizeof(check_confirmation));

        if (check_confirmation[0] == 'Y' || check_confirmation[0] == 'y') {
            delete_staff_record(target_staff_id);
            printf("Staff Account removed.\n");
        }
    }
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Sadia
void admin_student_operations() {
    clear_screen();
    printf("=== Student Database Operations ===\n");
    printf("Enter Student ID: ");
    int target_student_id = get_input_integer();

    FILE *student_file_pointer = fopen("students.txt", "r");
    if (student_file_pointer == NULL) {
        printf("No records found.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    char line_buffer[LINE_BUFFER_SIZE];
    struct Student found_student_data;
    int student_is_found = 0;

    while (fgets(line_buffer, sizeof(line_buffer), student_file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                   &found_student_data.id, found_student_data.name, found_student_data.department,
                   found_student_data.phone, found_student_data.password, &found_student_data.room_number) == 6) {
            if (found_student_data.id == target_student_id) {
                student_is_found = 1;
                break;
            }
        }
    }
    fclose(student_file_pointer);

    if (student_is_found == 0) {
        printf("Student not found.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    printf("\nFound student: %s (Dept: %s, Phone: %s, Room: %d)\n",
           found_student_data.name, found_student_data.department,
           found_student_data.phone, found_student_data.room_number);
    printf("1. Edit Profile Details\n");
    printf("2. Delete Student Profile\n");
    printf("3. Assign/Reassign Room\n");
    printf("4. Cancel\n");
    printf("Choose Action: ");
    int action_choice = get_input_integer();

    if (action_choice == 1) {
        char new_value[50];

        printf("New Name (Enter to keep): ");
        get_input_string(new_value, sizeof(new_value));
        if (strlen(new_value) > 0) {
            strcpy(found_student_data.name, new_value);
        }

        printf("New Department (Enter to keep): ");
        get_input_string(new_value, sizeof(new_value));
        if (strlen(new_value) > 0) {
            strcpy(found_student_data.department, new_value);
        }

        printf("New Phone (Enter to keep): ");
        get_input_string(new_value, sizeof(new_value));
        if (strlen(new_value) > 0) {
            strcpy(found_student_data.phone, new_value);
        }

        printf("New Password (Enter to keep): ");
        get_input_string(new_value, sizeof(new_value));
        if (strlen(new_value) > 0) {
            strcpy(found_student_data.password, new_value);
        }

        FILE *source_file = fopen("students.txt", "r");
        FILE *temporary_file = fopen("temp_students.txt", "w");
        if (source_file != NULL && temporary_file != NULL) {
            struct Student current_student;
            while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                           &current_student.id, current_student.name, current_student.department,
                           current_student.phone, current_student.password, &current_student.room_number) == 6) {
                    if (current_student.id == target_student_id) {
                        fprintf(temporary_file, "%d;%s;%s;%s;%s;%d\n",
                                found_student_data.id, found_student_data.name, found_student_data.department,
                                found_student_data.phone, found_student_data.password, found_student_data.room_number);
                    } else {
                        fprintf(temporary_file, "%d;%s;%s;%s;%s;%d\n",
                                current_student.id, current_student.name, current_student.department,
                                current_student.phone, current_student.password, current_student.room_number);
                    }
                }
            }
            fclose(source_file);
            fclose(temporary_file);
            remove("students.txt");
            rename("temp_students.txt", "students.txt");
        }
        printf("Student profile updated!\n");
    } else if (action_choice == 2) {
        printf("Confirm deletion of '%s' (Y/N): ", found_student_data.name);
        char check_confirmation[10];
        get_input_string(check_confirmation, sizeof(check_confirmation));

        /* Accept both upper 'Y' and lower 'y' for confirmation */
        if (check_confirmation[0] == 'Y' || check_confirmation[0] == 'y') {
            if (found_student_data.room_number > 0) {
                update_room_occupancy(found_student_data.room_number, -1);
            }
            delete_student_all_records(target_student_id);
            printf("Student purged system-wide.\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
    } else if (action_choice == 3) {
        printf("Enter Target Room (0 to unallocate): ");
        int new_target_room = get_input_integer();
        if (new_target_room == found_student_data.room_number) {
            printf("Already in Room %d.\n", new_target_room);
        } else {
            int room_change_successful = 0;
            if (new_target_room > 0) {
                struct Room target_room_details = get_room(new_target_room);
                if (target_room_details.room_number == 0 || (target_room_details.capacity - target_room_details.occupied_beds <= 0)) {
                    printf("Room full or does not exist.\n");
                } else {
                    if (found_student_data.room_number > 0) {
                        update_room_occupancy(found_student_data.room_number, -1);
                    }
                    update_room_occupancy(new_target_room, 1);
                    found_student_data.room_number = new_target_room;
                    room_change_successful = 1;
                    printf("Room space reassigned!\n");
                }
            } else {
                if (found_student_data.room_number > 0) {
                    update_room_occupancy(found_student_data.room_number, -1);
                }
                found_student_data.room_number = 0;
                room_change_successful = 1;
                printf("Room space unallocated.\n");
            }

            if (room_change_successful == 1) {
                FILE *source_file = fopen("students.txt", "r");
                FILE *temporary_file = fopen("temp_students.txt", "w");
                if (source_file != NULL && temporary_file != NULL) {
                    struct Student current_student;
                    while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
                        if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                                   &current_student.id, current_student.name, current_student.department,
                                   current_student.phone, current_student.password, &current_student.room_number) == 6) {
                            if (current_student.id == target_student_id) {
                                fprintf(temporary_file, "%d;%s;%s;%s;%s;%d\n",
                                        found_student_data.id, found_student_data.name, found_student_data.department,
                                        found_student_data.phone, found_student_data.password, found_student_data.room_number);
                            } else {
                                fprintf(temporary_file, "%d;%s;%s;%s;%s;%d\n",
                                        current_student.id, current_student.name, current_student.department,
                                        current_student.phone, current_student.password, current_student.room_number);
                            }
                        }
                    }
                    fclose(source_file);
                    fclose(temporary_file);
                    remove("students.txt");
                    rename("temp_students.txt", "students.txt");
                }
            }
        }
    }
    printf("Press Enter to continue...");
    getchar();
}


/* ============================================================================
 * Section 3: HIRA (18.3% Actual Workload - Student Registration & Sub-Menu Portals)
 * ============================================================================ */

// Written by: Hira
void admin_register_student() {
    clear_screen();
    printf("=== Register New Student ===\n");
    printf("Enter Student ID: ");
    int student_id = get_input_integer();

    if (student_id <= 0 || student_exists(student_id)) {
        printf("Error: Invalid or duplicate Student ID.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct Student new_student;
    new_student.id = student_id;
    new_student.name[0] = '\0';
    new_student.department[0] = '\0';
    new_student.phone[0] = '\0';
    new_student.password[0] = '\0';
    new_student.room_number = 0;

    printf("Enter Full Name: ");
    get_input_string(new_student.name, sizeof(new_student.name));

    printf("Enter Department: ");
    get_input_string(new_student.department, sizeof(new_student.department));

    printf("Enter Phone Number: ");
    get_input_string(new_student.phone, sizeof(new_student.phone));

    printf("Enter Password: ");
    get_input_string(new_student.password, sizeof(new_student.password));

    if (strlen(new_student.name) == 0 || strlen(new_student.password) == 0) {
        printf("Error: Name and Password fields cannot be empty.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    printf("Enter Room Number to Allocate (0 for Unallocated): ");
    int chosen_room_number = get_input_integer();
    if (chosen_room_number > 0) {
        struct Room room_details = get_room(chosen_room_number);
        if (room_details.room_number != 0 && (room_details.capacity - room_details.occupied_beds > 0)) {
            new_student.room_number = chosen_room_number;
            update_room_occupancy(chosen_room_number, 1);
            printf("Bed allocated successfully in room %d.\n", chosen_room_number);
        } else {
            printf("Warning: Room space unavailable. Registered as Unallocated.\n");
        }
    }

    printf("Enter Baseline Monthly Fee Amount: ");
    double baseline_monthly_fee = get_input_double();
    if (baseline_monthly_fee < 0.0) {
        baseline_monthly_fee = 0.0;
    }

    FILE *student_file_pointer = fopen("students.txt", "a");
    if (student_file_pointer != NULL) {
        fprintf(student_file_pointer, "%d;%s;%s;%s;%s;%d\n",
                new_student.id, new_student.name, new_student.department,
                new_student.phone, new_student.password, new_student.room_number);
        fclose(student_file_pointer);
    }

    struct Fee new_fee;
    new_fee.student_id = new_student.id;
    new_fee.monthly_fee = baseline_monthly_fee;
    new_fee.extra_fee = 0.0;
    new_fee.due_amount = baseline_monthly_fee;
    strcpy(new_fee.status, "Unpaid");
    strcpy(new_fee.payment_date, "N/A");

    FILE *fee_file_pointer = fopen("fees.txt", "a");
    if (fee_file_pointer != NULL) {
        fprintf(fee_file_pointer, "%d;%.2f;%.2f;%.2f;%s;%s\n",
                new_fee.student_id, new_fee.monthly_fee, new_fee.extra_fee,
                new_fee.due_amount, new_fee.status, new_fee.payment_date);
        fclose(fee_file_pointer);
    }

    printf("\nStudent registered successfully!\n");
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Hira
void admin_executive_summary() {
    clear_screen();
    printf("=============================================\n");
    printf("      EXECUTIVE SUMMARY REPORT DASHBOARD     \n");
    printf("=============================================\n");

    int total_students = 0;
    int total_beds = 0;
    int total_occupied = 0;
    int active_complaints = 0;
    int total_defaulters = 0;

    char line_buffer[LINE_BUFFER_SIZE];

    FILE *file_pointer = fopen("students.txt", "r");
    if (file_pointer != NULL) {
        struct Student student;
        while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%d",
                       &student.id, student.name, student.department,
                       student.phone, student.password, &student.room_number) == 6) {
                total_students = total_students + 1;
            }
        }
        fclose(file_pointer);
    }

    file_pointer = fopen("rooms.txt", "r");
    if (file_pointer != NULL) {
        struct Room room;
        while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%d;%d",
                       &room.room_number, &room.capacity, &room.occupied_beds) == 3) {
                total_beds = total_beds + room.capacity;
                total_occupied = total_occupied + room.occupied_beds;
            }
        }
        fclose(file_pointer);
    }

    file_pointer = fopen("complaints.txt", "r");
    if (file_pointer != NULL) {
        struct Complaint complaint;
        while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%d;%100[^;];%19[^;];%29[^\n]",
                       &complaint.id, &complaint.student_id, complaint.description,
                       complaint.status, complaint.date) == 5) {
                if (strcmp(complaint.status, "Resolved") != 0) {
                    active_complaints = active_complaints + 1;
                }
            }
        }
        fclose(file_pointer);
    }

    file_pointer = fopen("fees.txt", "r");
    if (file_pointer != NULL) {
        struct Fee fee_record;
        while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                       &fee_record.student_id, &fee_record.monthly_fee,
                       &fee_record.extra_fee, &fee_record.due_amount,
                       fee_record.status, fee_record.payment_date) == 6) {
                if (strcmp(fee_record.status, "Unpaid") == 0 || fee_record.due_amount > 0.0) {
                    total_defaulters = total_defaulters + 1;
                }
            }
        }
        fclose(file_pointer);
    }

    printf("Total Registered Students:  %d\n", total_students);
    printf("Configured Room Bed Spaces: %d\n", total_beds);
    printf("Allocated Bed Occupancies:  %d\n", total_occupied);
    printf("Hostel Vacancies Remaining: %d\n", total_beds - total_occupied);
    printf("Active Logged Complaints:   %d\n", active_complaints);
    printf("Defaulter Dues Balances:    %d\n", total_defaulters);
    print_line(45);
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Hira
void student_request_transfer(int student_id) {
    clear_screen();
    printf("=== Request Room Transfer ===\n");
    printf("Enter Desired Target Room Number: ");
    int target_room_number = get_input_integer();
    struct Room target_room_details = get_room(target_room_number);

    if (target_room_details.room_number == 0 || (target_room_details.capacity - target_room_details.occupied_beds <= 0)) {
        printf("Error: Room is full or non-existent.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct RoomTransfer transfer_request;
    transfer_request.request_id = get_next_transfer_id();
    transfer_request.student_id = student_id;
    transfer_request.target_room_number = target_room_number;
    strcpy(transfer_request.status, "Pending");

    FILE *file_pointer = fopen("transfers.txt", "a");
    if (file_pointer != NULL) {
        fprintf(file_pointer, "%d;%d;%d;%s\n",
                transfer_request.request_id, transfer_request.student_id,
                transfer_request.target_room_number, transfer_request.status);
        fclose(file_pointer);
        printf("Room transfer request logged (ID: %d).\n", transfer_request.request_id);
        printf("Note: Approval adds BDT %.2f fee.\n", TRANSFER_FEE_AMOUNT);
    }
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Hira
void staff_update_complaint() {
    clear_screen();
    printf("=== Update Complaint Status ===\n");
    printf("Enter Complaint Ref ID: ");
    int reference_id = get_input_integer();

    FILE *complaint_file_pointer = fopen("complaints.txt", "r");
    if (complaint_file_pointer == NULL) {
        printf("No records found.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    char line_buffer[LINE_BUFFER_SIZE];
    struct Complaint target_complaint;
    int complaint_found = 0;

    while (fgets(line_buffer, sizeof(line_buffer), complaint_file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%100[^;];%19[^;];%29[^\n]",
                   &target_complaint.id, &target_complaint.student_id, target_complaint.description,
                   target_complaint.status, target_complaint.date) == 5) {
            if (target_complaint.id == reference_id) {
                complaint_found = 1;
                break;
            }
        }
    }
    fclose(complaint_file_pointer);

    if (complaint_found == 0) {
        printf("Complaint not found.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    printf("\nDetails: %s\n", target_complaint.description);
    printf("Current: %s\n", target_complaint.status);
    print_line(30);
    printf("Select Status:\n");
    printf("1. Pending\n");
    printf("2. In Progress\n");
    printf("3. Resolved\n");
    printf("Choose Option: ");
    int option_choice = get_input_integer();

    if (option_choice == 1) {
        strcpy(target_complaint.status, "Pending");
    } else if (option_choice == 2) {
        strcpy(target_complaint.status, "In Progress");
    } else if (option_choice == 3) {
        strcpy(target_complaint.status, "Resolved");
    } else {
        return;
    }

    FILE *source_file = fopen("complaints.txt", "r");
    FILE *temporary_file = fopen("temp_complaints.txt", "w");
    if (source_file != NULL && temporary_file != NULL) {
        struct Complaint current_complaint;
        while (fgets(line_buffer, sizeof(line_buffer), source_file) != NULL) {
            if (sscanf(line_buffer, "%d;%d;%100[^;];%19[^;];%29[^\n]",
                       &current_complaint.id, &current_complaint.student_id, current_complaint.description,
                       current_complaint.status, current_complaint.date) == 5) {
                if (current_complaint.id == reference_id) {
                    fprintf(temporary_file, "%d;%d;%s;%s;%s\n",
                            target_complaint.id, target_complaint.student_id, target_complaint.description,
                            target_complaint.status, target_complaint.date);
                } else {
                    fprintf(temporary_file, "%d;%d;%s;%s;%s\n",
                            current_complaint.id, current_complaint.student_id, current_complaint.description,
                            current_complaint.status, current_complaint.date);
                }
            }
        }
        fclose(source_file);
        fclose(temporary_file);
        remove("complaints.txt");
        rename("temp_complaints.txt", "complaints.txt");
    }

    printf("Complaint status updated to: %s!\n", target_complaint.status);
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Hira
void admin_portal() {
    while (1) {
        clear_screen();
        printf("=== Administrator Operations Portal ===\n");
        printf("1. Register Student Profile\n");
        printf("2. Student Database Operations (Edit/Delete/Room Allocation)\n");
        printf("3. Search Student database\n");
        printf("4. Room Configuration & Vacancies\n");
        printf("5. Billings & Fees Operations\n");
        printf("6. Staff Accounts Management\n");
        printf("7. Process Verification Requests\n");
        printf("8. Generate Executive Summary Dashboard\n");
        printf("9. Log out of Portal\n");
        printf("Choose Option: ");
        int choice = get_input_integer();

        if (choice == 1) {
            admin_register_student();
        } else if (choice == 2) {
            admin_student_operations();
        } else if (choice == 3) {
            admin_search_student();
        } else if (choice == 4) {
            admin_room_operations();
        } else if (choice == 5) {
            admin_fee_operations();
        } else if (choice == 6) {
            admin_staff_operations();
        } else if (choice == 7) {
            admin_manage_requests();
        } else if (choice == 8) {
            admin_executive_summary();
        } else if (choice == 9) {
            return;
        }
    }
}

// Written by: Hira
void student_portal(int student_id) {
    char student_name[MAX_NAME];
    strcpy(student_name, "Student");
    get_student_name(student_id, student_name);

    while (1) {
        clear_screen();
        printf("=== Welcome to the Student Portal, %s ===\n", student_name);
        printf("1. View Profile and Room\n");
        printf("2. Track Monthly Fee status\n");
        printf("3. Submit Payment Slip\n");
        printf("4. Request Room Transfer\n");
        printf("5. Log Maintenance Complaint\n");
        printf("6. Request Vacating room\n");
        printf("7. Sign Out\n");
        printf("Choose Option: ");
        int choice = get_input_integer();

        if (choice == 1) {
            student_view_profile(student_id);
        } else if (choice == 2) {
            student_view_fees(student_id);
        } else if (choice == 3) {
            student_submit_slip(student_id);
        } else if (choice == 4) {
            student_request_transfer(student_id);
        } else if (choice == 5) {
            student_submit_complaint(student_id);
        } else if (choice == 6) {
            student_request_leave(student_id);
        } else if (choice == 7) {
            return;
        }
    }
}

// Written by: Hira
void staff_portal(int staff_id) {
    char staff_name[MAX_NAME];
    strcpy(staff_name, "Technician");

    FILE *staff_file_pointer = fopen("staff.txt", "r");
    if (staff_file_pointer != NULL) {
        char line_buffer[LINE_BUFFER_SIZE];
        struct Staff staff_member;
        while (fgets(line_buffer, sizeof(line_buffer), staff_file_pointer) != NULL) {
            if (sscanf(line_buffer, "%d;%49[^;];%19[^;];%19[^\n]",
                       &staff_member.id, staff_member.name, staff_member.phone, staff_member.password) == 4) {
                if (staff_member.id == staff_id) {
                    strcpy(staff_name, staff_member.name);
                    break;
                }
            }
        }
        fclose(staff_file_pointer);
    }

    while (1) {
        clear_screen();
        printf("=== Welcome to the Maintenance Portal, %s ===\n", staff_name);
        printf("1. View Hostel Complaints list\n");
        printf("2. Resolve/Update Complaint Status\n");
        printf("3. Sign Out\n");
        printf("Choose Option: ");
        int choice = get_input_integer();

        if (choice == 1) {
            staff_view_complaints();
        } else if (choice == 2) {
            staff_update_complaint();
        } else if (choice == 3) {
            return;
        }
    }
}


/* ============================================================================
 * Section 4: PROTHOMA (11.9% Actual Workload - Screen Utilities & Simple Entry Forms)
 * ============================================================================ */

// Written by: Prothoma
void clear_screen() {
#ifdef _WIN32
    system("cls"); /* Clears command prompt in Windows */
#else
    system("clear"); /* Clears terminal in Linux/macOS */
#endif
}

// Written by: Prothoma
void print_line(int line_length) {
    for (int index = 0; index < line_length; index++) {
        printf("-");
    }
    printf("\n");
}

// Written by: Prothoma
void staff_view_complaints() {
    clear_screen();
    printf("=== Global Hostel Facilities Complaints ===\n");
    FILE *file_pointer = fopen("complaints.txt", "r");
    if (file_pointer == NULL) {
        printf("No complaints.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    print_line(80);
    printf("%-8s %-12s %-12s %-30s %-12s\n", "Ref ID", "Student ID", "Date", "Description", "Status");
    print_line(80);

    char line_buffer[LINE_BUFFER_SIZE];
    struct Complaint complaint;
    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        if (sscanf(line_buffer, "%d;%d;%100[^;];%19[^;];%29[^\n]",
                   &complaint.id, &complaint.student_id, complaint.description,
                   complaint.status, complaint.date) == 5) {
            printf("%-8d %-12d %-12s %-30.30s %-12s\n",
                   complaint.id, complaint.student_id, complaint.date,
                   complaint.description, complaint.status);
        }
    }
    print_line(80);
    fclose(file_pointer);
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Prothoma
void student_submit_slip(int student_id) {
    clear_screen();
    printf("=== Submit Payment Slip ===\n");
    printf("Enter Bank Transaction reference ID: ");
    char transaction_number[MAX_TRANSACTION];
    get_input_string(transaction_number, sizeof(transaction_number));
    if (strlen(transaction_number) == 0) {
        printf("Invalid transaction.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    printf("Enter Amount Paid: ");
    double paid_amount = get_input_double();
    if (paid_amount <= 0.0) {
        printf("Error: Amount must be positive.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct PaymentSlip new_slip;
    new_slip.slip_id = get_next_slip_id();
    new_slip.student_id = student_id;
    strcpy(new_slip.transaction_number, transaction_number);
    new_slip.amount = paid_amount;
    strcpy(new_slip.status, "Approved"); /* Auto-approved immediately without admin verification */

    FILE *file_pointer = fopen("slips.txt", "a");
    if (file_pointer != NULL) {
        fprintf(file_pointer, "%d;%d;%s;%.2f;%s\n",
                new_slip.slip_id, new_slip.student_id,
                new_slip.transaction_number, new_slip.amount, new_slip.status);
        fclose(file_pointer);
    }

    /* Auto-update student fee balance in fees.txt */
    double remaining_due = 0.0;
    FILE *fee_source = fopen("fees.txt", "r");
    FILE *fee_temp = fopen("temp_fees.txt", "w");
    if (fee_source != NULL && fee_temp != NULL) {
        char line_buffer[LINE_BUFFER_SIZE];
        struct Fee current_fee;
        while (fgets(line_buffer, sizeof(line_buffer), fee_source) != NULL) {
            if (sscanf(line_buffer, "%d;%lf;%lf;%lf;%19[^;];%29[^\n]",
                       &current_fee.student_id, &current_fee.monthly_fee,
                       &current_fee.extra_fee, &current_fee.due_amount,
                       current_fee.status, current_fee.payment_date) == 6) {
                if (current_fee.student_id == student_id) {
                    current_fee.due_amount = current_fee.due_amount - paid_amount;
                    if (current_fee.due_amount <= 0.0) {
                        strcpy(current_fee.status, "Paid");
                    } else {
                        strcpy(current_fee.status, "Unpaid");
                    }
                    get_current_date(current_fee.payment_date, sizeof(current_fee.payment_date));
                    remaining_due = current_fee.due_amount;
                }
                fprintf(fee_temp, "%d;%.2f;%.2f;%.2f;%s;%s\n",
                        current_fee.student_id, current_fee.monthly_fee,
                        current_fee.extra_fee, current_fee.due_amount,
                        current_fee.status, current_fee.payment_date);
            }
        }
        fclose(fee_source);
        fclose(fee_temp);
        remove("fees.txt");
        rename("temp_fees.txt", "fees.txt");
    }

    printf("\nPayment slip submitted and automatically approved!\n");
    printf("Amount BDT %.2f credited successfully.\n", paid_amount);
    printf("Remaining Balance Due: BDT %.2f\n", remaining_due);
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Prothoma
void student_submit_complaint(int student_id) {
    clear_screen();
    printf("=== Lodge Complaint ===\n");
    printf("Enter description (max 100 characters):\n");
    char complaint_description[MAX_DESCRIPTION];
    get_input_string(complaint_description, sizeof(complaint_description));

    if (strlen(complaint_description) == 0) {
        printf("Error: Description empty.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    struct Complaint new_complaint;
    new_complaint.id = get_next_complaint_id();
    new_complaint.student_id = student_id;
    strcpy(new_complaint.description, complaint_description);
    strcpy(new_complaint.status, "Pending");
    get_current_date(new_complaint.date, sizeof(new_complaint.date));

    FILE *file_pointer = fopen("complaints.txt", "a");
    if (file_pointer != NULL) {
        fprintf(file_pointer, "%d;%d;%s;%s;%s\n",
                new_complaint.id, new_complaint.student_id, new_complaint.description,
                new_complaint.status, new_complaint.date);
        fclose(file_pointer);
        printf("Complaint logged (ID: %d).\n", new_complaint.id);
    }
    printf("Press Enter to continue...");
    getchar();
}

// Written by: Prothoma
void student_request_leave(int student_id) {
    clear_screen();
    printf("Confirm checkout vacation for next month? (Y/N): ");
    char check_confirmation[10];
    get_input_string(check_confirmation, sizeof(check_confirmation));

    if (check_confirmation[0] != 'Y' && check_confirmation[0] != 'y') {
        return;
    }

    struct LeaveRequest leave_request;
    leave_request.request_id = get_next_leave_id();
    leave_request.student_id = student_id;
    strcpy(leave_request.status, "Pending");

    FILE *file_pointer = fopen("leave_requests.txt", "a");
    if (file_pointer != NULL) {
        fprintf(file_pointer, "%d;%d;%s\n",
                leave_request.request_id, leave_request.student_id, leave_request.status);
        fclose(file_pointer);
        printf("Vacate request logged (ID: %d).\n", leave_request.request_id);
    }
    printf("Press Enter to continue...");
    getchar();
}
