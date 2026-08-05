#ifndef RAFI_H
#define RAFI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

#define DEFAULT_DATE "2026-07-28"

/* --- Rafi's Simplified Helper & Module Prototypes --- */
int get_int();
double get_dbl();
void get_str(char *buf, int size);
void initialize_files();
void pause_term();
void clear_term();
void clear_screen();
void append_line(const char *file, const char *data);
int get_next_id(const char *file);
void get_password_md5(const char *password, char hash_out[33]);
void admin_register_student();
void admin_search_student();
void admin_search_room();
void admin_fee_ops();
void admin_executive_summary();
void admin_view_complaints();
void admin_assign_complaint();
void staff_view_complaints(int staff_id);
void student_request_leave(int sid);
void student_submit_complaint(int sid);
void staff_update_complaint();
void provost_portal();
void provost_view_all_students();
void provost_view_all_staff();
void provost_view_fee_records();
void admin_portal();
void student_portal(int sid);
void staff_portal(int staff_id);
void login_portal();

#endif /* RAFI_H */
