#ifndef RAFI_H
#define RAFI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

/* --- Rafi's Prototypes --- */
void initialize_files();
void get_str(char *buf, int size);
int get_int();
double get_dbl();
void pause_term();
void append_line(const char *file, const char *data);
int get_next_id(const char *file);
void get_password_md5(const char *password, char hash_out[33]);
void admin_register_student();
void admin_search_student();
void admin_fee_ops();
void admin_executive_summary();
void student_submit_slip(int sid);
void staff_view_complaints();
void student_submit_complaint(int sid);
void staff_update_complaint();
void admin_portal();
void student_portal(int sid);
void staff_portal(int staff_id);
void login_portal();

#endif /* RAFI_H */
