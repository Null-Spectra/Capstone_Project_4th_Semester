#include "hira.h"
#include "rafi.h"
#include "prothoma.h"


/* ============================================================================
 * HIRA
 * ============================================================================ */


/* ============================================================================
 * GUEST MANAGEMENT
 * ============================================================================ */

/* GUEST MANAGEMENT - STUDENT */

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

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %s %s Pending\n", sid, guestName, relation);

    fclose(fp);

    printf("Guest Request Submitted Successfully.\n");
}


/* GUEST MANAGEMENT - ADMIN */

void admin_view_guest_requests()
{
    FILE *fp;
    int sid;
    char guestName[50];
    char relation[30];
    char status[20];
    int priority = 1;

    fp = fopen("guests.txt", "r");

    if (fp == NULL)
    {
        printf("No Guest Requests Found.\n");
        return;
    }

    printf("\n========== GUEST REQUESTS (PRIORITY QUEUE - FCFS ORDER) ==========\n");
    printf("Priority\tStudent ID\tGuest\tRelation\tStatus\n");
    printf("--------------------------------------------------------------------\n");

    while (fscanf(fp, "%d %s %s %s",
                  &sid, guestName, relation, status) != EOF)
    {
        printf("P-%d\t\t%d\t\t%s\t%s\t\t%s\n",
               priority++, sid, guestName, relation, status);
    }

    fclose(fp);
}


/* ADMIN APPROVE GUEST */

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

    if (fp == NULL)
    {
        printf("No Guest Requests Found.\n");
        return;
    }

    while (fscanf(fp, "%d %s %s %s",
                  &studentId, guestName, relation, status) != EOF)
    {
        if (studentId == sid)
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

    if (found)
        printf("Guest Request Approved Successfully.\n");
    else
        printf("Student ID Not Found.\n");
}


/* GUEST MANAGEMENT - STUDENT MENU */

void guest_management(int sid)
{
    int choice;

    while (1)
    {
        printf("\n===== Guest Management =====\n");
        printf("1. Register Guest\n");
        printf("2. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                guest_register(sid);
                break;

            case 2:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* GUEST MANAGEMENT - ADMIN MENU */

void admin_guest_management()
{
    int choice;
    int sid;

    while (1)
    {
        printf("\n===== Guest Management =====\n");
        printf("1. View Guest Requests\n");
        printf("2. Approve Guest Request\n");
        printf("3. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                admin_view_guest_requests();
                break;

            case 2:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                admin_approve_guest(sid);
                break;

            case 3:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ============================================================================
 * MEAL MANAGEMENT
 * ============================================================================ */

/* MEAL MANAGEMENT - STUDENT */

void meal_register(int sid)
{
    FILE *fp;
    int days;
    int delivery;
    int room = 0;
    int deliveryCharge = 0;

    printf("\n=== Meal Registration ===\n");

    printf("Enter Number of Meal Days: ");
    scanf("%d", &days);

    if (days <= 0)
    {
        printf("Invalid Number of Days!\n");
        return;
    }

    printf("\n1. Dining Hall\n");
    printf("2. Room Delivery\n");
    printf("Choose Meal Delivery Type: ");
    scanf("%d", &delivery);

    if (delivery == 2)
    {
        printf("Enter Room Number: ");
        scanf("%d", &room);

        deliveryCharge = 50;
    }
    else if (delivery != 1)
    {
        printf("Invalid Choice!\n");
        return;
    }

    fp = fopen("meals.txt", "a");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %d %d %d %d Pending\n",
            sid, days, delivery, room, deliveryCharge);

    fclose(fp);

    printf("Meal Request Submitted Successfully.\n");
}


/* MEAL MANAGEMENT - ADMIN */

void admin_view_meal_requests()
{
    FILE *fp;
    int sid, days, delivery, room, deliveryCharge;
    char status[20];

    fp = fopen("meals.txt", "r");

    if (fp == NULL)
    {
        printf("No Meal Requests Found.\n");
        return;
    }

    printf("\n========== MEAL REQUESTS ==========\n");
    printf("Student ID\tMeal Days\tDelivery\tRoom\tCharge\tStatus\n");
    printf("----------------------------------------------------------------\n");

    while (fscanf(fp, "%d %d %d %d %d %s",
                  &sid, &days, &delivery, &room,
                  &deliveryCharge, status) != EOF)
    {
        printf("%d\t\t%d\t\t%d\t\t%d\t%d\t%s\n",
               sid, days, delivery, room, deliveryCharge, status);
    }

    fclose(fp);
}


/* ADMIN APPROVE MEAL */

void admin_approve_meal(int sid)
{
    FILE *fp, *temp;
    int studentId, days, delivery, room, deliveryCharge;
    char status[20];
    int found = 0;

    fp = fopen("meals.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL)
    {
        printf("No Meal Requests Found.\n");
        return;
    }

    while (fscanf(fp, "%d %d %d %d %d %s",
                  &studentId, &days, &delivery,
                  &room, &deliveryCharge, status) != EOF)
    {
        if (studentId == sid)
        {
            fprintf(temp, "%d %d %d %d %d Approved\n",
                    studentId, days, delivery,
                    room, deliveryCharge);

            found = 1;
        }
        else
        {
            fprintf(temp, "%d %d %d %d %d %s\n",
                    studentId, days, delivery,
                    room, deliveryCharge, status);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("meals.txt");
    rename("temp.txt", "meals.txt");

    if (found)
        printf("Meal Request Approved Successfully.\n");
    else
        printf("Student ID Not Found.\n");
}


/* ROOM DELIVERY REQUESTS */

void admin_view_room_delivery_requests()
{
    FILE *fp;
    int sid, days, delivery, room, deliveryCharge;
    char status[20];
    int found = 0;

    fp = fopen("meals.txt", "r");

    if (fp == NULL)
    {
        printf("No Room Delivery Requests Found.\n");
        return;
    }

    printf("\n========== ROOM DELIVERY REQUESTS ==========\n");

    while (fscanf(fp, "%d %d %d %d %d %s",
                  &sid, &days, &delivery,
                  &room, &deliveryCharge, status) != EOF)
    {
        if (delivery == 2)
        {
            printf("Student ID: %d | Days: %d | Room: %d | Delivery Charge: %d | Status: %s\n",
                   sid, days, room, deliveryCharge, status);

            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
        printf("No Room Delivery Requests Found.\n");
}

/* ============================================================================ 
 * WEEKLY MEAL CHART
 * ============================================================================ */

/* ADMIN MEAL CHART */

/* ============================================================================ 
 * WEEKLY MEAL CHART
 * ============================================================================ */

/* ADMIN MEAL CHART */

void admin_meal_chart()
{
    FILE *fp;
    FILE *temp;
    int choice;

    char day[20];
    char breakfast[100];
    char lunch[100];
    char dinner[100];

    while (1)
    {
        printf("\n===== Weekly Meal Chart =====\n");
        printf("1. Create / Add Meal Chart\n");
        printf("2. Update Meal Chart\n");
        printf("3. Delete Meal Chart\n");
        printf("4. Back\n");
        printf("Choice: ");
        scanf(" %d", &choice);

        /* ============================================================
         * CREATE / ADD MEAL CHART
         * ============================================================ */

        if (choice == 1)
        {
            fp = fopen("meal_chart.txt", "a");

            if (fp == NULL)
            {
                printf("File Error!\n");
                continue;
            }

            while (1)
            {
                printf("\n========== Add Meal Chart ==========\n");

                printf("Enter Day (or type Back to return): ");
                scanf(" %19s", day);

                if (strcmp(day, "Back") == 0 ||
                    strcmp(day, "back") == 0)
                {
                    break;
                }

                printf("Enter Breakfast: ");
                scanf(" %[^\n]", breakfast);

                printf("Enter Lunch: ");
                scanf(" %[^\n]", lunch);

                printf("Enter Dinner: ");
                scanf(" %[^\n]", dinner);

                fprintf(fp, "%s|%s|%s|%s\n",
                        day,
                        breakfast,
                        lunch,
                        dinner);

                printf("Meal Chart Added Successfully.\n");
            }

            fclose(fp);
        }

        /* ============================================================
         * UPDATE MEAL CHART
         * ============================================================ */

        else if (choice == 2)
        {
            fp = fopen("meal_chart.txt", "r");
            temp = fopen("temp.txt", "w");

            if (fp == NULL)
            {
                printf("No Meal Chart Found.\n");

                if (temp != NULL)
                    fclose(temp);

                continue;
            }

            if (temp == NULL)
            {
                printf("File Error!\n");
                fclose(fp);
                continue;
            }

            printf("\nEnter Day to Update: ");
            scanf(" %19s", day);

            printf("Enter New Breakfast: ");
            scanf(" %[^\n]", breakfast);

            printf("Enter New Lunch: ");
            scanf(" %[^\n]", lunch);

            printf("Enter New Dinner: ");
            scanf(" %[^\n]", dinner);

            char oldDay[20];
            char oldBreakfast[100];
            char oldLunch[100];
            char oldDinner[100];

            int found = 0;

            while (fscanf(fp, " %19[^|]|%99[^|]|%99[^|]|%99[^\n]",
                          oldDay,
                          oldBreakfast,
                          oldLunch,
                          oldDinner) == 4)
            {
                if (strcmp(oldDay, day) == 0)
                {
                    fprintf(temp, "%s|%s|%s|%s\n",
                            day,
                            breakfast,
                            lunch,
                            dinner);

                    found = 1;
                }
                else
                {
                    fprintf(temp, "%s|%s|%s|%s\n",
                            oldDay,
                            oldBreakfast,
                            oldLunch,
                            oldDinner);
                }
            }

            fclose(fp);
            fclose(temp);

            remove("meal_chart.txt");
            rename("temp.txt", "meal_chart.txt");

            if (found)
                printf("Meal Chart Updated Successfully.\n");
            else
                printf("Day Not Found.\n");
        }

        /* ============================================================
         * DELETE MEAL CHART
         * ============================================================ */

        else if (choice == 3)
        {
            fp = fopen("meal_chart.txt", "r");
            temp = fopen("temp.txt", "w");

            if (fp == NULL)
            {
                printf("No Meal Chart Found.\n");

                if (temp != NULL)
                    fclose(temp);

                continue;
            }

            if (temp == NULL)
            {
                printf("File Error!\n");
                fclose(fp);
                continue;
            }

            printf("\nEnter Day to Delete: ");
            scanf(" %19s", day);

            char oldDay[20];
            char oldBreakfast[100];
            char oldLunch[100];
            char oldDinner[100];

            int found = 0;

            while (fscanf(fp, " %19[^|]|%99[^|]|%99[^|]|%99[^\n]",
                          oldDay,
                          oldBreakfast,
                          oldLunch,
                          oldDinner) == 4)
            {
                if (strcmp(oldDay, day) != 0)
                {
                    fprintf(temp, "%s|%s|%s|%s\n",
                            oldDay,
                            oldBreakfast,
                            oldLunch,
                            oldDinner);
                }
                else
                {
                    found = 1;
                }
            }

            fclose(fp);
            fclose(temp);

            remove("meal_chart.txt");
            rename("temp.txt", "meal_chart.txt");

            if (found)
                printf("Meal Chart Deleted Successfully.\n");
            else
                printf("Day Not Found.\n");
        }

        /* ============================================================
         * BACK
         * ============================================================ */

        else if (choice == 4)
        {
            return;
        }

        else
        {
            printf("Invalid Choice!\n");
        }
    }
}
/* STUDENT VIEW MEAL CHART */

/* STUDENT VIEW MEAL CHART */

void student_view_meal_chart()
{
    FILE *fp;

    char day[20];
    char breakfast[100];
    char lunch[100];
    char dinner[100];

    int found = 0;

    fp = fopen("meal_chart.txt", "r");

    if (fp == NULL)
    {
        printf("No Meal Chart Found.\n");
        return;
    }

    printf("\n========== WEEKLY MEAL CHART ==========\n");
    printf("Day\t\tBreakfast\t\tLunch\t\t\tDinner\n");
    printf("--------------------------------------------------------------------------\n");

    while (fscanf(fp, " %19[^|]|%99[^|]|%99[^|]|%99[^\n]",
                  day,
                  breakfast,
                  lunch,
                  dinner) == 4)
    {
        printf("%-10s\t%-20s\t%-20s\t%-20s\n",
               day,
               breakfast,
               lunch,
               dinner);

        found = 1;
    }

    fclose(fp);

    if (found == 0)
        printf("No Meal Chart Available.\n");
}

/* ============================================================================
 * DAILY MEAL PAYMENT
 * ============================================================================ */

void meal_daily_payment(int sid)
{
    FILE *fp;
    int studentId;

    char date[20];
    char mealType[20];
    char amount[20];
    char status[20];
    char paymentTime[20];

    char oldDate[20];
    char oldMealType[20];
    char oldAmount[20];
    char oldStatus[20];
    char oldTime[20];

    int duplicate = 0;

    printf("\n=== Daily Meal Payment ===\n");

    printf("Enter Date (dd-mm-yyyy): ");
    scanf("%s", date);

    printf("Enter Meal Type (Breakfast/Lunch/Dinner): ");
    scanf("%s", mealType);

    printf("Enter Amount: ");
    scanf("%s", amount);

    printf("Enter Payment Time: ");
    scanf("%s", paymentTime);

    fp = fopen("daily_payments.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s %s %s %s",
                      &studentId,
                      oldDate,
                      oldMealType,
                      oldAmount,
                      oldStatus,
                      oldTime) != EOF)
        {
            if (studentId == sid &&
                strcmp(oldDate, date) == 0 &&
                strcmp(oldMealType, mealType) == 0)
            {
                duplicate = 1;
            }
        }

        fclose(fp);
    }

    if (duplicate)
    {
        printf("Payment Already Exists for This Student, Date and Meal Type.\n");
        return;
    }

    fp = fopen("daily_payments.txt", "a");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    strcpy(status, "Paid");

    fprintf(fp, "%d %s %s %s %s %s\n",
            sid,
            date,
            mealType,
            amount,
            status,
            paymentTime);

    fclose(fp);

    printf("Payment Successful. Status: Paid.\n");
}


/* STUDENT RECEIVE MEAL */

void student_receive_meal(int sid)
{
    FILE *fp;

    int studentId;

    char date[20];
    char mealType[20];
    char amount[20];
    char status[20];
    char paymentTime[20];

    char inputDate[20];
    char inputMeal[20];

    int paid = 0;

    printf("\n=== Receive Meal ===\n");

    printf("Enter Date (dd-mm-yyyy): ");
    scanf("%s", inputDate);

    printf("Enter Meal Type (Breakfast/Lunch/Dinner): ");
    scanf("%s", inputMeal);

    fp = fopen("daily_payments.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s %s %s %s",
                      &studentId,
                      date,
                      mealType,
                      amount,
                      status,
                      paymentTime) != EOF)
        {
            if (studentId == sid &&
                strcmp(date, inputDate) == 0 &&
                strcmp(mealType, inputMeal) == 0 &&
                strcmp(status, "Paid") == 0)
            {
                paid = 1;
            }
        }

        fclose(fp);
    }

    if (paid)
        printf("Payment Verified. You can receive today's meal.\n");
    else
        printf("Payment Not Found. You cannot receive this meal today.\n");
}


/* ADMIN VIEW DAILY PAYMENTS */

void admin_view_daily_payments()
{
    FILE *fp;

    int sid;

    char date[20];
    char mealType[20];
    char amount[20];
    char status[20];
    char paymentTime[20];

    fp = fopen("daily_payments.txt", "r");

    if (fp == NULL)
    {
        printf("No Daily Payment Records Found.\n");
        return;
    }

    printf("\n========== DAILY PAYMENT RECORDS ==========\n");

    while (fscanf(fp, "%d %s %s %s %s %s",
                  &sid,
                  date,
                  mealType,
                  amount,
                  status,
                  paymentTime) != EOF)
    {
        printf("Student ID: %d | Date: %s | Meal: %s | Amount: %s | Status: %s | Time: %s\n",
               sid,
               date,
               mealType,
               amount,
               status,
               paymentTime);
    }

    fclose(fp);
}


/* ADMIN VIEW PAYMENT HISTORY */

void admin_view_meal_payment_history(int sid)
{
    FILE *fp;

    int studentId;

    char date[20];
    char mealType[20];
    char amount[20];
    char status[20];
    char paymentTime[20];

    int found = 0;

    fp = fopen("daily_payments.txt", "r");

    if (fp == NULL)
    {
        printf("No Payment Records Found.\n");
        return;
    }

    printf("\n========== PAYMENT HISTORY ==========\n");

    while (fscanf(fp, "%d %s %s %s %s %s",
                  &studentId,
                  date,
                  mealType,
                  amount,
                  status,
                  paymentTime) != EOF)
    {
        if (studentId == sid)
        {
            printf("Date: %s | Meal: %s | Amount: %s | Status: %s | Time: %s\n",
                   date,
                   mealType,
                   amount,
                   status,
                   paymentTime);

            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
        printf("No Payment History Found for Student ID %d.\n", sid);
}


/* STUDENT MEAL MANAGEMENT */

void student_meal_management(int sid)
{
    int choice;

    while (1)
    {
        printf("\n===== Student Meal Management =====\n");
        printf("1. Register Meal\n");
        printf("2. View Meal Chart\n");
        printf("3. Make Daily Meal Payment\n");
        printf("4. Receive Meal\n");
        printf("5. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                meal_register(sid);
                break;

            case 2:
                student_view_meal_chart();
                break;

            case 3:
                meal_daily_payment(sid);
                break;

            case 4:
                student_receive_meal(sid);
                break;

            case 5:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ADMIN MEAL MANAGEMENT */

void admin_meal_management()
{
    int choice;
    int sid;

    while (1)
    {
        printf("\n===== Admin Meal Management =====\n");
        printf("1. View Meal Requests\n");
        printf("2. Approve Meal Request\n");
        printf("3. Meal Chart\n");
        printf("4. View Room Delivery Requests\n");
        printf("5. View Daily Payments\n");
        printf("6. View Student Payment History\n");
        printf("7. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                admin_view_meal_requests();
                break;

            case 2:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                admin_approve_meal(sid);
                break;

            case 3:
                admin_meal_chart();
                break;

            case 4:
                admin_view_room_delivery_requests();
                break;

            case 5:
                admin_view_daily_payments();
                break;

            case 6:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                admin_view_meal_payment_history(sid);
                break;

            case 7:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ============================================================================
 * TIME & ENTRY MANAGEMENT
 * ============================================================================ */

/* STUDENT CHECK IN */

void student_checkin(int sid)
{
    FILE *fp;

    int hour;

    char studentName[50];
    char gender[20];
    char hallName[50];
    char status[20];

    printf("Enter Student Name: ");
    scanf("%s", studentName);

    printf("Enter Gender (Male/Female): ");
    scanf("%s", gender);

    printf("Enter Hall Name: ");
    scanf("%s", hallName);

    printf("Enter Current Hour (0-23): ");
    scanf("%d", &hour);

    if (hour < 0 || hour > 23)
    {
        printf("Invalid Time! Hour must be between 0 and 23.\n");
        return;
    }

    if (strcmp(gender, "Female") == 0 ||
        strcmp(gender, "female") == 0)
    {
        if (hour >= 19)
            strcpy(status, "Late");
        else
            strcpy(status, "OnTime");
    }
    else if (strcmp(gender, "Male") == 0 ||
             strcmp(gender, "male") == 0)
    {
        if (hour >= 22)
            strcpy(status, "Late");
        else
            strcpy(status, "OnTime");
    }
    else
    {
        printf("Invalid Gender!\n");
        return;
    }

    fp = fopen("checkin.txt", "a");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %s %s %s %d %s\n",
            sid,
            studentName,
            gender,
            hallName,
            hour,
            status);

    fclose(fp);

    if (strcmp(status, "Late") == 0)
        printf("Late Entry! Warden Notification Required.\n");
    else
        printf("Check-in Successful.\n");
}


/* STUDENT CHECK OUT */

void student_checkout(int sid)
{
    FILE *fp;
    int hour;

    printf("Enter Exit Hour (0-23): ");
    scanf("%d", &hour);

    if (hour < 0 || hour > 23)
    {
        printf("Invalid Time! Hour must be between 0 and 23.\n");
        return;
    }

    fp = fopen("checkout.txt", "a");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %d\n", sid, hour);

    fclose(fp);

    printf("Check-out Successful.\n");
}


/* ADMIN VIEW LATE ENTRIES */

void admin_view_late_entries()
{
    FILE *fp;

    int sid;
    int hour;

    char studentName[50];
    char gender[20];
    char hallName[50];
    char status[20];

    int found = 0;

    fp = fopen("checkin.txt", "r");

    if (fp == NULL)
    {
        printf("No Check-in Records Found.\n");
        return;
    }

    printf("\n========== LATE ENTRIES ==========\n");

    while (fscanf(fp, "%d %s %s %s %d %s",
                  &sid,
                  studentName,
                  gender,
                  hallName,
                  &hour,
                  status) != EOF)
    {
        if (strcmp(status, "Late") == 0)
        {
            printf("Student ID: %d | Name: %s | Gender: %s | Hall: %s | Time: %d:00 | Status: Late\n",
                   sid,
                   studentName,
                   gender,
                   hallName,
                   hour);

            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
        printf("No Late Entries Found.\n");
}


/* STUDENT TIME & ENTRY MANAGEMENT */

void student_time_entry_management(int sid)
{
    int choice;

    while (1)
    {
        printf("\n===== Time & Entry Management =====\n");
        printf("1. Check In\n");
        printf("2. Check Out\n");
        printf("3. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                student_checkin(sid);
                break;

            case 2:
                student_checkout(sid);
                break;

            case 3:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ADMIN TIME & ENTRY MANAGEMENT */

void admin_time_entry_management()
{
    int choice;

    while (1)
    {
        printf("\n===== Time & Entry Management =====\n");
        printf("1. View Late Entries\n");
        printf("2. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                admin_view_late_entries();
                break;

            case 2:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ============================================================================
 * EVENT MANAGEMENT
 * ============================================================================ */

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

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %s %d %d Pending\n",
            sid,
            eventName,
            type,
            participants);

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

    if (fp == NULL)
    {
        printf("No Event Requests Found.\n");
        return;
    }

    printf("\n========== EVENT REQUESTS ==========\n");

    while (fscanf(fp, "%d %s %d %d %s",
                  &sid,
                  eventName,
                  &type,
                  &participants,
                  status) != EOF)
    {
        printf("\nStudent ID   : %d", sid);
        printf("\nEvent Name   : %s", eventName);

        switch (type)
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

    if (fp == NULL)
    {
        printf("No Event Requests Found.\n");
        return;
    }

    printf("\n1. Approve\n");
    printf("2. Reject\n");
    printf("Choose: ");
    scanf("%d", &choice);

    while (fscanf(fp, "%d %s %d %d %s",
                  &studentId,
                  eventName,
                  &type,
                  &participants,
                  status) != EOF)
    {
        if (studentId == sid &&
            strcmp(status, "Pending") == 0)
        {
            switch (choice)
            {
                case 1:
                    fprintf(temp, "%d %s %d %d Approved\n",
                            studentId,
                            eventName,
                            type,
                            participants);

                    printf("Event Approved Successfully.\n");

                    found = 1;
                    break;

                case 2:
                    fprintf(temp, "%d %s %d %d Rejected\n",
                            studentId,
                            eventName,
                            type,
                            participants);

                    printf("Event Rejected.\n");

                    found = 1;
                    break;

                default:
                    fprintf(temp, "%d %s %d %d %s\n",
                            studentId,
                            eventName,
                            type,
                            participants,
                            status);

                    printf("Invalid Choice!\n");
                    break;
            }
        }
        else
        {
            fprintf(temp, "%d %s %d %d %s\n",
                    studentId,
                    eventName,
                    type,
                    participants,
                    status);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("events.txt");
    rename("temp.txt", "events.txt");

    if (found == 0)
    {
        printf("No Pending Request Found for Student ID %d.\n", sid);
    }
}


/* STUDENT EVENT MANAGEMENT */

void event_management(int sid)
{
    int choice;

    while (1)
    {
        printf("\n===== Event Management =====\n");
        printf("1. Request Event\n");
        printf("2. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                event_request(sid);
                break;

            case 2:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ADMIN EVENT MANAGEMENT */

void admin_event_management()
{
    int choice;
    int sid;

    while (1)
    {
        printf("\n===== Event Management =====\n");
        printf("1. View Event Requests\n");
        printf("2. Approve / Reject Event\n");
        printf("3. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                admin_view_event_requests();
                break;

            case 2:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                admin_approve_event(sid);
                break;

            case 3:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ============================================================================
 * LOST & FOUND
 * ============================================================================ */

/* REPORT LOST ITEM */

void report_lost_item(int sid)
{
    FILE *fp;

    char studentName[50];
    char itemName[50];
    char description[100];
    char date[20];
    char location[50];

    printf("\n===== Report Lost Item =====\n");

    printf("Enter Student Name: ");
    scanf("%s", studentName);

    printf("Enter Item Name: ");
    scanf("%s", itemName);

    printf("Enter Item Description: ");
    scanf("%s", description);

    printf("Enter Date: ");
    scanf("%s", date);

    printf("Enter Lost Location: ");
    scanf("%s", location);

    fp = fopen("lost_items.txt", "a");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %s %s %s %s %s Lost\n",
            sid,
            studentName,
            itemName,
            description,
            date,
            location);

    fclose(fp);

    printf("Lost Item Reported Successfully.\n");
}


/* REPORT FOUND ITEM */

void report_found_item(int finderId)
{
    FILE *fp;

    char finderName[50];
    char itemName[50];
    char description[100];
    char date[20];
    char location[50];

    printf("\n===== Report Found Item =====\n");

    printf("Enter Finder/Staff Name: ");
    scanf("%s", finderName);

    printf("Enter Item Name: ");
    scanf("%s", itemName);

    printf("Enter Item Description: ");
    scanf("%s", description);

    printf("Enter Date: ");
    scanf("%s", date);

    printf("Enter Found Location: ");
    scanf("%s", location);

    fp = fopen("found_items.txt", "a");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %s %s %s %s %s Found\n",
            finderId,
            finderName,
            itemName,
            description,
            date,
            location);

    fclose(fp);

    printf("Found Item Reported Successfully.\n");
}


/* SEARCH ITEM */

void search_item()
{
    FILE *fp;

    char itemName[50];

    int id;

    char name[50];
    char item[50];
    char description[100];
    char date[20];
    char location[50];
    char status[20];

    int found = 0;

    printf("\n===== Search Item =====\n");

    printf("Enter Item Name: ");
    scanf("%s", itemName);

    printf("\n--- Lost Items ---\n");

    fp = fopen("lost_items.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s %s %s %s %s",
                      &id,
                      name,
                      item,
                      description,
                      date,
                      location,
                      status) != EOF)
        {
            if (strcmp(item, itemName) == 0)
            {
                printf("Student ID: %d | Name: %s | Item: %s | Description: %s | Date: %s | Location: %s | Status: %s\n",
                       id,
                       name,
                       item,
                       description,
                       date,
                       location,
                       status);

                found = 1;
            }
        }

        fclose(fp);
    }

    printf("\n--- Found Items ---\n");

    fp = fopen("found_items.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s %s %s %s %s",
                      &id,
                      name,
                      item,
                      description,
                      date,
                      location,
                      status) != EOF)
        {
            if (strcmp(item, itemName) == 0)
            {
                printf("Finder ID: %d | Name: %s | Item: %s | Description: %s | Date: %s | Location: %s | Status: %s\n",
                       id,
                       name,
                       item,
                       description,
                       date,
                       location,
                       status);

                found = 1;
            }
        }

        fclose(fp);
    }

    if (found == 0)
        printf("No Matching Item Found.\n");
}


/* CHECK ITEM MATCH */

void check_item_match()
{
    FILE *lost;
    FILE *found;

    int lostId;
    int foundId;

    char lostStudent[50];
    char foundPerson[50];

    char lostItem[50];
    char foundItem[50];

    char lostDescription[100];
    char foundDescription[100];

    char lostDate[20];
    char foundDate[20];

    char lostLocation[50];
    char foundLocation[50];

    char lostStatus[20];
    char foundStatus[20];

    int match = 0;

    lost = fopen("lost_items.txt", "r");

    if (lost == NULL)
    {
        printf("No Lost Item Records Found.\n");
        return;
    }

    found = fopen("found_items.txt", "r");

    if (found == NULL)
    {
        fclose(lost);
        printf("No Found Item Records Found.\n");
        return;
    }

    while (fscanf(lost, "%d %s %s %s %s %s %s",
                  &lostId,
                  lostStudent,
                  lostItem,
                  lostDescription,
                  lostDate,
                  lostLocation,
                  lostStatus) != EOF)
    {
        while (fscanf(found, "%d %s %s %s %s %s %s",
                      &foundId,
                      foundPerson,
                      foundItem,
                      foundDescription,
                      foundDate,
                      foundLocation,
                      foundStatus) != EOF)
        {
            if (strcmp(lostItem, foundItem) == 0 &&
                strcmp(lostDescription, foundDescription) == 0)
            {
                printf("\nPossible Match Found.\n");
                printf("Lost Item: %s\n", lostItem);
                printf("Lost Description: %s\n", lostDescription);
                printf("Found Item: %s\n", foundItem);
                printf("Found Description: %s\n", foundDescription);

                match = 1;
            }
        }

        rewind(found);
    }

    fclose(lost);
    fclose(found);

    if (match == 0)
        printf("No Possible Match Found.\n");
}


/* ADMIN VIEW LOST ITEMS */

void admin_view_lost_items()
{
    FILE *fp;

    int sid;

    char studentName[50];
    char itemName[50];
    char description[100];
    char date[20];
    char location[50];
    char status[20];

    fp = fopen("lost_items.txt", "r");

    if (fp == NULL)
    {
        printf("No Lost Items Found.\n");
        return;
    }

    printf("\n========== LOST ITEMS ==========\n");

    while (fscanf(fp, "%d %s %s %s %s %s %s",
                  &sid,
                  studentName,
                  itemName,
                  description,
                  date,
                  location,
                  status) != EOF)
    {
        printf("Student ID: %d | Name: %s | Item: %s | Description: %s | Date: %s | Location: %s | Status: %s\n",
               sid,
               studentName,
               itemName,
               description,
               date,
               location,
               status);
    }

    fclose(fp);
}


/* ADMIN VIEW FOUND ITEMS */

void admin_view_found_items()
{
    FILE *fp;

    int finderId;

    char finderName[50];
    char itemName[50];
    char description[100];
    char date[20];
    char location[50];
    char status[20];

    fp = fopen("found_items.txt", "r");

    if (fp == NULL)
    {
        printf("No Found Items Found.\n");
        return;
    }

    printf("\n========== FOUND ITEMS ==========\n");

    while (fscanf(fp, "%d %s %s %s %s %s %s",
                  &finderId,
                  finderName,
                  itemName,
                  description,
                  date,
                  location,
                  status) != EOF)
    {
        printf("Finder ID: %d | Name: %s | Item: %s | Description: %s | Date: %s | Location: %s | Status: %s\n",
               finderId,
               finderName,
               itemName,
               description,
               date,
               location,
               status);
    }

    fclose(fp);
}


/* ADMIN RETURN LOST ITEM */

void admin_return_lost_item(int sid)
{
    FILE *fp;
    FILE *temp;

    int studentId;

    char studentName[50];
    char itemName[50];
    char description[100];
    char date[20];
    char location[50];
    char status[20];

    int found = 0;

    fp = fopen("lost_items.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL)
    {
        printf("No Lost Items Found.\n");
        return;
    }

    while (fscanf(fp, "%d %s %s %s %s %s %s",
                  &studentId,
                  studentName,
                  itemName,
                  description,
                  date,
                  location,
                  status) != EOF)
    {
        if (studentId == sid &&
            strcmp(status, "Lost") == 0)
        {
            fprintf(temp, "%d %s %s %s %s %s Returned\n",
                    studentId,
                    studentName,
                    itemName,
                    description,
                    date,
                    location);

            found = 1;
        }
        else
        {
            fprintf(temp, "%d %s %s %s %s %s %s\n",
                    studentId,
                    studentName,
                    itemName,
                    description,
                    date,
                    location,
                    status);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("lost_items.txt");
    rename("temp.txt", "lost_items.txt");

    if (found)
        printf("Lost Item Marked as Returned.\n");
    else
        printf("Lost Item Not Found for Student ID %d.\n", sid);
}


/* STUDENT LOST & FOUND MANAGEMENT */

void student_lost_found_management(int sid)
{
    int choice;

    while (1)
    {
        printf("\n===== Lost & Found Management =====\n");
        printf("1. Report Lost Item\n");
        printf("2. Report Found Item\n");
        printf("3. Search Item\n");
        printf("4. Check Item Match\n");
        printf("5. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                report_lost_item(sid);
                break;

            case 2:
                report_found_item(sid);
                break;

            case 3:
                search_item();
                break;

            case 4:
                check_item_match();
                break;

            case 5:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}


/* ADMIN LOST & FOUND MANAGEMENT */

void admin_lost_found_management()
{
    int choice;
    int sid;

    while (1)
    {
        printf("\n===== Lost & Found Management =====\n");
        printf("1. View Lost Items\n");
        printf("2. View Found Items\n");
        printf("3. Search Item\n");
        printf("4. Check Item Match\n");
        printf("5. Mark Lost Item as Returned\n");
        printf("6. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                admin_view_lost_items();
                break;

            case 2:
                admin_view_found_items();
                break;

            case 3:
                search_item();
                break;

            case 4:
                check_item_match();
                break;

            case 5:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                admin_return_lost_item(sid);
                break;

            case 6:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}