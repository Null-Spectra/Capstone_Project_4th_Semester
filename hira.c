#include "hira.h"
#include "rafi.h"

/* ============================================================================
 * Section 3: HIRA
 * ============================================================================ */


/*GUEST MANAGEMENT - STUDENT*/

void guest_register(int sid)
{
    FILE *fp;
    char guestName[50];
    char relation[30];

    printf("Enter Guest Name: ");
    scanf("%s", guestName);

    printf("Enter Relation: ");
    scanf("%s", relation);

    fp = fopen("guests.txt", "a");

    if(fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %s %s Pending\n", sid, guestName, relation);

    fclose(fp);

    printf("Guest Request Submitted Successfully.\n");
}


/*GUEST MANAGEMENT - ADMIN*/

void admin_view_guest_requests()
{
    FILE *fp;
    int sid;
    char guestName[50];
    char relation[30];
    char status[20];

    fp = fopen("guests.txt", "r");

    if(fp == NULL)
    {
        printf("No Guest Requests Found.\n");
        return;
    }

    printf("\n========== GUEST REQUESTS ==========\n");
    printf("Student ID\tGuest\tRelation\tStatus\n");
    printf("------------------------------------\n");

    while(fscanf(fp, "%d %s %s %s",
                 &sid, guestName, relation, status) != EOF)
    {
        printf("%d\t\t%s\t%s\t\t%s\n",
               sid, guestName, relation, status);
    }

    fclose(fp);
}


/*ADMIN APPROVE GUEST
*/

void admin_approve_guest(int sid)
{
    FILE *fp, *temp;
    int studentId;
    char guestName[50];
    char relation[30];
    char status[20];
    int found = 0;

    fp = fopen("guests.txt", "r");
    temp = fopen("temp.txt", "w");

    if(fp == NULL)
    {
        printf("No Guest Requests Found.\n");
        return;
    }

    while(fscanf(fp, "%d %s %s %s",
                 &studentId, guestName, relation, status) != EOF)
    {
        if(studentId == sid)
        {
            fprintf(temp, "%d %s %s Approved\n",
                    studentId, guestName, relation);

            found = 1;
        }
        else
        {
            fprintf(temp, "%d %s %s %s\n",
                    studentId, guestName, relation, status);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("guests.txt");
    rename("temp.txt", "guests.txt");

    if(found)
        printf("Guest Request Approved Successfully.\n");
    else
        printf("Student ID Not Found.\n");
}



/*MEAL MANAGEMENT - STUDENT*/

void meal_register(int sid)
{
    FILE *fp;
    int days;

    printf("Enter Number of Meal Days: ");
    scanf("%d", &days);

    fp = fopen("meals.txt", "a");

    if(fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %d Pending\n", sid, days);

    fclose(fp);

    printf("Meal Request Submitted Successfully.\n");
}


/*MEAL MANAGEMENT - ADMIN*/

void admin_view_meal_requests()
{
    FILE *fp;
    int sid, days;
    char status[20];

    fp = fopen("meals.txt", "r");

    if(fp == NULL)
    {
        printf("No Meal Requests Found.\n");
        return;
    }

    printf("\n========== MEAL REQUESTS ==========\n");
    printf("Student ID\tMeal Days\tStatus\n");
    printf("-----------------------------------\n");

    while(fscanf(fp, "%d %d %s",
                 &sid, &days, status) != EOF)
    {
        printf("%d\t\t%d\t\t%s\n",
               sid, days, status);
    }

    fclose(fp);
}


/*ADMIN APPROVE MEAL*/

void admin_approve_meal(int sid)
{
    FILE *fp, *temp;
    int studentId, days;
    char status[20];
    int found = 0;

    fp = fopen("meals.txt", "r");
    temp = fopen("temp.txt", "w");

    if(fp == NULL)
    {
        printf("No Meal Requests Found.\n");
        return;
    }

    while(fscanf(fp, "%d %d %s",
                 &studentId, &days, status) != EOF)
    {
        if(studentId == sid)
        {
            fprintf(temp, "%d %d Approved\n",
                    studentId, days);

            found = 1;
        }
        else
        {
            fprintf(temp, "%d %d %s\n",
                    studentId, days, status);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("meals.txt");
    rename("temp.txt", "meals.txt");

    if(found)
        printf("Meal Request Approved Successfully.\n");
    else
        printf("Student ID Not Found.\n");
}



/*TIME RESTRICTION - STUDENT CHECK IN*/

void student_checkin(int sid)
{
    FILE *fp;
    int hour;

    printf("Enter Current Hour (0-23): ");
    scanf("%d", &hour);

    if(hour < 0 || hour > 23)
    {
        printf("Invalid Time! Hour must be between 0 and 23.\n");
        return;
    }

    fp = fopen("checkin.txt", "a");

    if(fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %d\n", sid, hour);

    fclose(fp);

    if(hour > 22)
        printf("Late Entry! Warden Notification Required.\n");
    else
        printf("Check-in Successful.\n");
}


/*TIME RESTRICTION - STUDENT CHECK OUT*/

void student_checkout(int sid)
{
    FILE *fp;
    int hour;

    printf("Enter Exit Hour (0-23): ");
    scanf("%d", &hour);

    if(hour < 0 || hour > 23)
    {
        printf("Invalid Time! Hour must be between 0 and 23.\n");
        return;
    }

    fp = fopen("checkout.txt", "a");

    if(fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %d\n", sid, hour);

    fclose(fp);

    printf("Check-out Successful.\n");
}

/*TIME RESTRICTION - ADMIN*/

void admin_view_late_entries()
{
    FILE *fp;
    int sid, hour;

    fp = fopen("checkin.txt", "r");

    if(fp == NULL)
    {
        printf("No Check-in Records Found.\n");
        return;
    }

    printf("\n========== LATE ENTRIES ==========\n");

    while(fscanf(fp, "%d %d", &sid, &hour) != EOF)
    {
        if(hour > 22)
        {
            printf("Student ID: %d\tTime: %d:00\n",
                   sid, hour);
        }
    }

    fclose(fp);
}