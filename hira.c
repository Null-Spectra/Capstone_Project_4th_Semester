#include "hira.h"
#include "rafi.h"

/* ============================================================================
 * Section 3: HIRA (Request Services)
 * ============================================================================ */

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
