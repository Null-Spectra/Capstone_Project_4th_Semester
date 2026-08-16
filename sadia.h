#ifndef SADIA_H
#define SADIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Sadia's Prototypes --- */
void admin_room_ops();
void admin_staff_ops();
void admin_student_ops();
void public_view_rooms();
void show_call_now();
void student_view_booking_history(int sid);
void admin_view_transfer_requests();
void admin_approve_transfer_request();

#endif /* SADIA_H */
