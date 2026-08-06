#ifndef PROTHOMA_H
#define PROTHOMA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Prothoma's Prototypes --- */
void student_view_profile(int sid);
void student_view_fees(int sid);
void student_book_for_new(int sid);
void show_facilities_list();
void student_request_transfer(int sid);
void show_available_seats_summary();
void admin_view_new_booking_requests_priority_queue();
void admin_approve_new_booking_request_priority_queue();

#endif /* PROTHOMA_H */
