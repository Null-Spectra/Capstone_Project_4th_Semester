#ifndef HIRA_H
#define HIRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* Guest Management */
void guest_register(int sid);
void admin_view_guest_requests();
void admin_approve_guest(int sid);
void guest_management(int sid);
void admin_guest_management();

/* Meal Management */
void meal_register(int sid);
void admin_view_meal_requests();
void admin_approve_meal(int sid);
void student_meal_management(int sid);
void admin_meal_management();
void student_view_meal_chart();

/* Time Restriction */
void student_checkin(int sid);
void student_checkout(int sid);
void admin_view_late_entries();
void student_time_entry_management(int sid);
void admin_time_entry_management();

/* Event Management */
void event_request(int sid);
void admin_view_event_requests();
void admin_approve_event(int sid);
void event_management(int sid);
void admin_event_management();

/* Lost & Found */
void report_lost_item(int sid);
void report_found_item(int finderId);
void search_item();
void student_lost_found_management(int sid);
void admin_lost_found_management();
#endif /* HIRA_H */
