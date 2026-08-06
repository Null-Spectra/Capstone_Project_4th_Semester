#include "hira.h"
#include "rafi.h"
#include "prothoma.h"

/* ============================================================================
 * Section 3: HIRA
 * ============================================================================ */


/*GUEST MANAGEMENT - STUDENT*/

void guest_register(int sid)
{
    FILE *fp;
    char guestName[50];
    char relation[30];

    printf("\n=== Guest Registration ===\n");
    show_available_seats_summary();

    printf("\nEnter Guest Name: ");
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
    int priority = 1;

    fp = fopen("guests.txt", "r");

    if(fp == NULL)
    {
        printf("No Guest Requests Found.\n");
        return;
    }

    printf("\n========== GUEST REQUESTS (PRIORITY QUEUE - FCFS ORDER) ==========\n");
    printf("Priority\tStudent ID\tGuest\tRelation\tStatus\n");
    printf("--------------------------------------------------------------------\n");

    while(fscanf(fp, "%d %s %s %s",
                 &sid, guestName, relation, status) != EOF)
    {
        printf("P-%d\t\t%d\t\t%s\t%s\t\t%s\n",
               priority++, sid, guestName, relation, status);
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

void event_request(int sid)
{
    FILE *fp;
    char eventName[100];
    int type;
    int participants;

    printf("\n===== Event Request =====\n");

    printf("Enter Event Name: ");
    scanf(" %[^\n]", eventName);

    printf("\n1. Cultural\n");
    printf("2. Sports\n");
    printf("3. Seminar\n");

    printf("Choose Event Type: ");
    scanf("%d", &type);

    printf("Number of Participants: ");
    scanf("%d", &participants);

    fp = fopen("events.txt", "a");

    if(fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %s %d %d Pending\n",
            sid, eventName, type, participants);

    fclose(fp);

    printf("\nEvent Request Sent Successfully.\n");
}

void admin_view_event_requests()
{
    FILE *fp;

    int sid;
    char eventName[100];
    int type;
    int participants;
    char status[20];

    fp = fopen("events.txt", "r");

    if(fp == NULL)
    {
        printf("No Event Requests Found.\n");
        return;
    }

    printf("\n========== EVENT REQUESTS ==========\n");

    while(fscanf(fp, "%d %s %d %d %s",
                 &sid,
                 eventName,
                 &type,
                 &participants,
                 status) != EOF)
    {
        printf("\nStudent ID   : %d", sid);
        printf("\nEvent Name   : %s", eventName);

        switch(type)
        {
            case 1:
                printf("\nEvent Type   : Cultural");
                break;

            case 2:
                printf("\nEvent Type   : Sports");
                break;

            case 3:
                printf("\nEvent Type   : Seminar");
                break;

            default:
                printf("\nEvent Type   : Unknown");
        }

        printf("\nParticipants : %d", participants);
        printf("\nStatus       : %s\n", status);
    }

    fclose(fp);
}

void admin_approve_event(int sid)
{
    FILE *fp;
    FILE *temp;

    int studentId;
    char eventName[100];
    int type;
    int participants;
    char status[20];

    int choice;
    int found = 0;

    fp = fopen("events.txt", "r");
    temp = fopen("temp.txt", "w");

    if(fp == NULL)
    {
        printf("No Event Requests Found.\n");
        return;
    }

    printf("\n1. Approve\n");
    printf("2. Reject\n");
    printf("Choose: ");
    scanf("%d", &choice);

    while(fscanf(fp, "%d %s %d %d %s",
                 &studentId,
                 eventName,
                 &type,
                 &participants,
                 status) != EOF)
    {
        if(studentId == sid && strcmp(status, "Pending") == 0)
        {
            switch(choice)
            {
                case 1:
                    fprintf(temp, "%d %s %d %d Approved\n",
                            studentId, eventName, type, participants);

                    printf("Event Approved Successfully.\n");
                    found = 1;
                    break;

                case 2:
                    fprintf(temp, "%d %s %d %d Rejected\n",
                            studentId, eventName, type, participants);

                    printf("Event Rejected.\n");
                    found = 1;
                    break;

                default:
                    fprintf(temp, "%d %s %d %d %s\n",
                            studentId, eventName, type,
                            participants, status);

                    printf("Invalid Choice!\n");
                    break;
            }
        }
        else
        {
            fprintf(temp, "%d %s %d %d %s\n",
                    studentId, eventName, type,
                    participants, status);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("events.txt");
    rename("temp.txt", "events.txt");

    if(found == 0)
    {
        printf("No Pending Request Found for Student ID %d.\n", sid);
    }
}