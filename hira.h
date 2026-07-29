#ifndef HIRA_H
#define HIRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* Guest Management */
void guest_register(int sid);
void admin_view_guest_requests();
void admin_approve_guest(int sid);

/* Meal Management */
void meal_register(int sid);
void admin_view_meal_requests();
void admin_approve_meal(int sid);

/* Time Restriction */
void student_checkin(int sid);
void student_checkout(int sid);
void admin_view_late_entries();

#endif /* HIRA_H */
