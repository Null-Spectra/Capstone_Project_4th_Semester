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

    if (fp == NULL)
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

/*MEAL MANAGEMENT - STUDENT*/

void meal_register(int sid)
{
    FILE *fp;
    int days;
    int delivery;
    int room;
    int deliveryCharge;

    printf("Enter Number of Meal Days: ");
    scanf("%d", &days);

    if (days <= 0)
    {
        printf("Invalid Number of Days!\n");
        return;
    }

    printf("\n1. Dining Hall\n");
    printf("2. Room Delivery\n");
    printf("Choose Meal Delivery: ");
    scanf("%d", &delivery);

    if (delivery == 2)
    {
        printf("Enter Room Number: ");
        scanf("%d", &room);

        if (room <= 0)
        {
            printf("Invalid Room Number!\n");
            return;
        }
    }
    else if (delivery != 1)
    {
        printf("Invalid Choice!\n");
        return;
    }
    else
    {
        room = 0;
    }

    if (delivery == 2)
        deliveryCharge = 50;
    else
        deliveryCharge = 0;

    fp = fopen("meals.txt", "a");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    fprintf(fp, "%d %d %d %d %d Pending\n", sid, days, delivery, room, deliveryCharge);

    fclose(fp);

    printf("Meal Request Submitted Successfully.\n");
}

/*MEAL MANAGEMENT - ADMIN*/

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
    printf("------------------------------------------------------\n");

    while (fscanf(fp, "%d %d %d %d %d %s",
                  &sid, &days, &delivery, &room, &deliveryCharge, status) != EOF)
    {
        printf("%d\t\t%d\t\t", sid, days);

        switch (delivery)
        {
        case 1:
            printf("Dining Hall\t-");
            break;

        case 2:
            printf("Room Delivery\t%d", room);
            break;

        default:
            printf("Unknown\t\t-");
        }

        printf("\t%d\t%s\n", deliveryCharge, status);
    }

    fclose(fp);
}

/*ADMIN APPROVE MEAL*/

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
                  &studentId, &days, &delivery, &room, &deliveryCharge, status) != EOF)
    {
        if (studentId == sid && strcmp(status, "Pending") == 0)
        {
            fprintf(temp, "%d %d %d %d %d Approved\n",
                    studentId, days, delivery, room, deliveryCharge);

            found = 1;
        }
        else
        {
            fprintf(temp, "%d %d %d %d %d %s\n",
                    studentId, days, delivery, room, deliveryCharge, status);
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

/*MEAL MANAGEMENT - EXTRA FEATURES*/

void admin_meal_chart()
{
    FILE *fp, *temp;
    int choice;
    char day[20];
    char breakfast[50];
    char lunch[50];
    char dinner[50];
    char oldDay[20];
    char oldBreakfast[50];
    char oldLunch[50];
    char oldDinner[50];
    int found;

    while (1)
    {
        printf("\n===== Weekly Meal Chart =====\n");
        printf("1. Create / Update Chart\n");
        printf("2. Delete Chart\n");
        printf("3. View Chart\n");
        printf("4. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter Day: ");
            scanf("%s", day);
            printf("Breakfast: ");
            scanf("%s", breakfast);
            printf("Lunch: ");
            scanf("%s", lunch);
            printf("Dinner: ");
            scanf("%s", dinner);

            fp = fopen("meal_chart.txt", "r");
            temp = fopen("temp.txt", "w");
            found = 0;

            if (temp == NULL)
            {
                if (fp != NULL)
                    fclose(fp);
                printf("File Error!\n");
                break;
            }

            if (fp != NULL)
            {
                while (fscanf(fp, "%s %s %s %s", oldDay, oldBreakfast,
                              oldLunch, oldDinner) != EOF)
                {
                    if (strcmp(oldDay, day) == 0)
                    {
                        fprintf(temp, "%s %s %s %s\n", day, breakfast,
                                lunch, dinner);
                        found = 1;
                    }
                    else
                    {
                        fprintf(temp, "%s %s %s %s\n", oldDay,
                                oldBreakfast, oldLunch, oldDinner);
                    }
                }
                fclose(fp);
            }

            if (found == 0)
                fprintf(temp, "%s %s %s %s\n", day, breakfast,
                        lunch, dinner);

            fclose(temp);
            remove("meal_chart.txt");
            rename("temp.txt", "meal_chart.txt");
            printf("Meal Chart Saved Successfully.\n");
            break;

        case 2:
            fp = fopen("meal_chart.txt", "w");
            if (fp != NULL)
            {
                fclose(fp);
                printf("Meal Chart Deleted.\n");
            }
            else
                printf("File Error!\n");
            break;

        case 3:
            student_view_meal_chart();
            break;

        case 4:
            return;

        default:
            printf("Invalid Choice!\n");
        }
    }
}

void student_view_meal_chart()
{
    FILE *fp;
    char day[20];
    char breakfast[50];
    char lunch[50];
    char dinner[50];

    fp = fopen("meal_chart.txt", "r");

    if (fp == NULL)
    {
        printf("No Meal Chart Found.\n");
        return;
    }

    printf("\n========== WEEKLY MEAL CHART ==========\n");

    while (fscanf(fp, "%s %s %s %s", day, breakfast, lunch, dinner) != EOF)
    {
        printf("\nDay       : %s", day);
        printf("\nBreakfast : %s", breakfast);
        printf("\nLunch     : %s", lunch);
        printf("\nDinner    : %s\n", dinner);
    }

    fclose(fp);
}

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

    printf("Enter Date: ");
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
        while (fscanf(fp, "%d %s %s %s %s %s", &studentId, oldDate,
                      oldMealType, oldAmount, oldStatus, oldTime) != EOF)
        {
            if (studentId == sid && strcmp(oldDate, date) == 0 &&
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
    fprintf(fp, "%d %s %s %s %s %s\n", sid, date, mealType, amount,
            status, paymentTime);
    fclose(fp);

    printf("Payment Successful. Status: Paid.\n");
}

void student_receive_meal(int sid)
{
    FILE *fp;
    int studentId;
    char date[20], mealType[20], amount[20], status[20], paymentTime[20];
    char inputDate[20], inputMeal[20];
    int paid = 0;

    printf("Enter Date: ");
    scanf("%s", inputDate);
    printf("Enter Meal Type (Breakfast/Lunch/Dinner): ");
    scanf("%s", inputMeal);

    fp = fopen("daily_payments.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s %s %s %s", &studentId, date, mealType,
                      amount, status, paymentTime) != EOF)
        {
            if (studentId == sid && strcmp(date, inputDate) == 0 &&
                strcmp(mealType, inputMeal) == 0 && strcmp(status, "Paid") == 0)
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

void admin_view_daily_payments()
{
    FILE *fp;
    int sid;
    char date[20], mealType[20], amount[20], status[20], paymentTime[20];

    fp = fopen("daily_payments.txt", "r");

    if (fp == NULL)
    {
        printf("No Daily Payment Records Found.\n");
        return;
    }

    printf("\n========== DAILY MEAL PAYMENTS ==========\n");

    while (fscanf(fp, "%d %s %s %s %s %s", &sid, date, mealType,
                  amount, status, paymentTime) != EOF)
    {
        printf("Student ID: %d | Date: %s | Meal: %s | Amount: %s | Status: %s | Payment Time: %s\n",
               sid, date, mealType, amount, status, paymentTime);
    }

    fclose(fp);
}

void admin_view_meal_payment_history(int sid)
{
    FILE *fp;
    int studentId;
    char date[20], mealType[20], amount[20], status[20], paymentTime[20];
    int found = 0;

    fp = fopen("daily_payments.txt", "r");

    if (fp == NULL)
    {
        printf("No Daily Payment Records Found.\n");
        return;
    }

    printf("\n========== PAYMENT HISTORY ==========\n");

    while (fscanf(fp, "%d %s %s %s %s %s", &studentId, date, mealType,
                  amount, status, paymentTime) != EOF)
    {
        if (studentId == sid)
        {
            printf("Date: %s | Meal: %s | Amount: %s | Status: %s | Payment Time: %s\n",
                   date, mealType, amount, status, paymentTime);
            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
        printf("No Payment History Found.\n");
}

void admin_view_room_delivery_requests()
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

    printf("\n========== ROOM DELIVERY REQUESTS ==========\n");

    while (fscanf(fp, "%d %d %d %d %d %s", &sid, &days, &delivery,
                  &room, &deliveryCharge, status) != EOF)
    {
        if (delivery == 2)
        {
            printf("Student ID: %d | Days: %d | Room: %d | Delivery Charge: %d | Status: %s\n",
                   sid, days, room, deliveryCharge, status);
        }
    }

    fclose(fp);
}

/*TIME & ENTRY MANAGEMENT - STUDENT CHECK IN*/

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

    if (strcmp(gender, "Female") == 0 || strcmp(gender, "female") == 0)
    {
        if (hour >= 19)
            strcpy(status, "Late");
        else
            strcpy(status, "OnTime");
    }
    else if (strcmp(gender, "Male") == 0 || strcmp(gender, "male") == 0)
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

    fprintf(fp, "%d %s %s %s %d %s\n", sid, studentName, gender,
            hallName, hour, status);
    fclose(fp);

    if (strcmp(status, "Late") == 0)
        printf("Late Entry! Warden Notification Required.\n");
    else
        printf("Check-in Successful.\n");
}

/*TIME & ENTRY MANAGEMENT - STUDENT CHECK OUT*/

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

/*TIME & ENTRY MANAGEMENT - ADMIN*/

void admin_view_late_entries()
{
    FILE *fp;
    int sid, hour;
    char studentName[50], gender[20], hallName[50], status[20];

    fp = fopen("checkin.txt", "r");
    if (fp == NULL)
    {
        printf("No Check-in Records Found.\n");
        return;
    }

    printf("\n========== LATE ENTRIES ==========\n");

    while (fscanf(fp, "%d %s %s %s %d %s", &sid, studentName, gender,
                  hallName, &hour, status) != EOF)
    {
        if (strcmp(status, "Late") == 0)
        {
            printf("Student ID: %d | Name: %s | Gender: %s | Hall: %s | Time: %d:00 | Status: Late\n",
                   sid, studentName, gender, hallName, hour);
        }
    }
    fclose(fp);
}

/*LOST & FOUND*/

void report_lost_item(int sid)
{
    FILE *fp;
    char studentName[50], itemName[50], description[100], date[20], location[50];

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

    fprintf(fp, "%d %s %s %s %s %s Lost\n", sid, studentName, itemName,
            description, date, location);
    fclose(fp);
    printf("Lost Item Reported Successfully.\n");
}

void report_found_item(int finderId)
{
    FILE *fp;
    char itemName[50], description[100], date[20], location[50];

    printf("\n===== Report Found Item =====\n");
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

    fprintf(fp, "%d %s %s %s %s Found\n", finderId, itemName,
            description, date, location);
    fclose(fp);
    printf("Found Item Reported Successfully.\n");
}

void check_item_match()
{
    FILE *lost, *found;
    int lostId, foundId;
    char lostItem[50], lostDescription[100], lostDate[20], lostLocation[50], lostStatus[20];
    char foundItem[50], foundDescription[100], foundDate[20], foundLocation[50], foundStatus[20];
    int match = 0;

    lost = fopen("lost_items.txt", "r");
    found = fopen("found_items.txt", "r");

    if (lost == NULL || found == NULL)
    {
        if (lost != NULL)
            fclose(lost);
        if (found != NULL)
            fclose(found);
        return;
    }

    while (fscanf(lost, "%d %s %s %s %s %s %s", &lostId, lostItem,
                  lostDescription, lostDate, lostLocation, lostStatus) != EOF)
    {
        while (fscanf(found, "%d %s %s %s %s %s", &foundId, foundItem,
                      foundDescription, foundDate, foundLocation, foundStatus) != EOF)
        {
            if (strcmp(lostItem, foundItem) == 0 &&
                strcmp(lostDescription, foundDescription) == 0)
            {
                printf("Possible Match Found.\n");
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

void search_item()
{
    FILE *fp;
    int id;
    char itemName[50], description[100], date[20], location[50], status[20];
    char searchName[50];
    int found = 0;

    printf("Enter Item Name to Search: ");
    scanf("%s", searchName);

    fp = fopen("lost_items.txt", "r");
    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s %s %s %s", &id, itemName, description,
                      date, location, status) != EOF)
        {
            if (strcmp(itemName, searchName) == 0)
            {
                printf("Lost Item: ID %d | Item %s | Description %s | Date %s | Location %s | Status %s\n",
                       id, itemName, description, date, location, status);
                found = 1;
            }
        }
        fclose(fp);
    }

    fp = fopen("found_items.txt", "r");
    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s %s %s %s", &id, itemName, description,
                      date, location, status) != EOF)
        {
            if (strcmp(itemName, searchName) == 0)
            {
                printf("Found Item: ID %d | Item %s | Description %s | Date %s | Location %s | Status %s\n",
                       id, itemName, description, date, location, status);
                found = 1;
            }
        }
        fclose(fp);
    }

    if (found == 0)
        printf("No Item Found.\n");

    check_item_match();
}

void admin_view_lost_items()
{
    FILE *fp;
    int id;
    char studentName[50], itemName[50], description[100], date[20], location[50], status[20];

    fp = fopen("lost_items.txt", "r");
    if (fp == NULL)
    {
        printf("No Lost Items Found.\n");
        return;
    }

    printf("\n========== LOST ITEMS ==========\n");
    while (fscanf(fp, "%d %s %s %s %s %s %s", &id, studentName, itemName,
                  description, date, location, status) != EOF)
    {
        printf("Student ID: %d | Name: %s | Item: %s | Description: %s | Date: %s | Location: %s | Status: %s\n",
               id, studentName, itemName, description, date, location, status);
    }
    fclose(fp);
}

void admin_view_found_items()
{
    FILE *fp;
    int id;
    char itemName[50], description[100], date[20], location[50], status[20];

    fp = fopen("found_items.txt", "r");
    if (fp == NULL)
    {
        printf("No Found Items Found.\n");
        return;
    }

    printf("\n========== FOUND ITEMS ==========\n");
    while (fscanf(fp, "%d %s %s %s %s %s", &id, itemName, description,
                  date, location, status) != EOF)
    {
        printf("Finder ID: %d | Item: %s | Description: %s | Date: %s | Location: %s | Status: %s\n",
               id, itemName, description, date, location, status);
    }
    fclose(fp);
}

void admin_return_lost_item(int sid)
{
    FILE *fp, *temp;
    int studentId;
    char studentName[50], itemName[50], description[100], date[20], location[50], status[20];
    int found = 0;

    fp = fopen("lost_items.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL)
    {
        if (temp != NULL)
            fclose(temp);
        printf("No Lost Items Found.\n");
        return;
    }

    while (fscanf(fp, "%d %s %s %s %s %s %s", &studentId, studentName,
                  itemName, description, date, location, status) != EOF)
    {
        if (studentId == sid && strcmp(status, "Lost") == 0)
        {
            fprintf(temp, "%d %s %s %s %s %s Returned\n", studentId,
                    studentName, itemName, description, date, location);
            found = 1;
        }
        else
        {
            fprintf(temp, "%d %s %s %s %s %s %s\n", studentId, studentName,
                    itemName, description, date, location, status);
        }
    }

    fclose(fp);
    fclose(temp);
    remove("lost_items.txt");
    rename("temp.txt", "lost_items.txt");

    if (found)
        printf("Item Marked as Returned.\n");
    else
        printf("Lost Item Not Found.\n");
}

void student_meal_management(int sid)
{
    int ch;

    while (1)
    {
        printf("\n===== Meal Management =====\n");
        printf("1. Register Meal\n");
        printf("2. View Meal Chart\n");
        printf("3. Daily Meal Payment\n");
        printf("4. Receive Meal\n");
        printf("5. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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

void admin_meal_management()
{
    int ch, sid;

    while (1)
    {

        printf("\n===== Meal Management =====\n");
        printf("1. View Meal Requests\n");
        printf("2. Approve Meal Request\n");
        printf("3. View Room Delivery Requests\n");
        printf("4. Weekly Meal Chart\n");
        printf("5. View Daily Payments\n");
        printf("6. View Student Payment History\n");
        printf("7. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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
            admin_view_room_delivery_requests();
            break;
        case 4:
            admin_meal_chart();
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

void student_time_entry_management(int sid)
{
    int ch;

    while (1)
    {

        printf("\n===== Time & Entry Management =====\n");
        printf("1. Check In\n");
        printf("2. Check Out\n");
        printf("3. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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

void student_lost_found_management(int sid)
{
    int ch;

    while (1)
    {

        printf("\n===== Lost & Found =====\n");
        printf("1. Report Lost Item\n");
        printf("2. Report Found Item\n");
        printf("3. Search Item\n");
        printf("4. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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
            return;
        default:
            printf("Invalid Choice!\n");
        }
    }
}

void admin_lost_found_management()
{
    int ch, sid;

    while (1)
    {

        printf("\n===== Lost & Found =====\n");
        printf("1. View Lost Items\n");
        printf("2. View Found Items\n");
        printf("3. Search Item\n");
        printf("4. Mark Lost Item as Returned\n");
        printf("5. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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
            printf("Enter Student ID: ");
            scanf("%d", &sid);
            admin_return_lost_item(sid);
            break;
        case 5:
            return;
        default:
            printf("Invalid Choice!\n");
        }
    }
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

    if (fp == NULL)
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
        if (studentId == sid && strcmp(status, "Pending") == 0)
        {
            switch (choice)
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

    if (found == 0)
    {
        printf("No Pending Request Found for Student ID %d.\n", sid);
    }
}

/* MODULE MANAGEMENT */

void guest_management(int sid)
{
    int ch;

    while (1)
    {

        printf("\n===== Guest Management =====\n");
        printf("1. Guest Registration\n");
        printf("2. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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

void admin_guest_management()
{
    int ch, sid;

    while (1)
    {

        printf("\n===== Guest Management =====\n");
        printf("1. View Guest Requests\n");
        printf("2. Approve Guest Request\n");
        printf("3. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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

void admin_time_entry_management()
{
    int ch;

    while (1)
    {

        printf("\n===== Time & Entry Management =====\n");
        printf("1. View Late Entries\n");
        printf("2. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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

void event_management(int sid)
{
    int ch;

    while (1)
    {

        printf("\n===== Event Management =====\n");
        printf("1. Event Request\n");
        printf("2. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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

void admin_event_management()
{
    int ch, sid;

    while (1)
    {

        printf("\n===== Event Management =====\n");
        printf("1. View Event Requests\n");
        printf("2. Approve/Reject Event\n");
        printf("3. Back\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch)
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