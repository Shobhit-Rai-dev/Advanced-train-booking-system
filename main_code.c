#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char Name[50];
    int Age;
    char Gender[10];
    int SeatNumber;
    char Phone[50];
    int train_no;
    char train_class[30];
    float Fare;
    int date;
} passengers;

typedef struct
{
    int train_no;
    char train_name[50];
    char route[100];
    char time[30];
} Train;

Train trains[100];
int total_trains = 0;

passengers passengers_details[100];
passengers passenger_waitlist[50];
int waitlist_count = 0;
int booked_seats[50] = {0};
int passenger_count = 0;

void save_trains_data() {
    FILE *fp = fopen("trains.dat", "wb");       
    if (fp == NULL) {
        printf("Error saving the data!\n");
        return;
    }
    fwrite(trains, sizeof(Train), total_trains, fp);
    fclose(fp);
}

void load_trains_data() {
    FILE *fp = fopen("trains.dat", "rb");
    if (fp == NULL) {
        return;
    }
    total_trains = fread(trains, sizeof(Train), 100, fp);
    fclose(fp);
}

void save_passengers_data() {
    FILE *fp = fopen("passengers.dat", "wb");
    if (fp == NULL) {
        printf("Error saving the passenger data!\n");
        return;
    }
    fwrite(passengers_details, sizeof(passengers), passenger_count, fp);
    fclose(fp);
}

void load_passengers_data() {
    FILE *fp = fopen("passengers.dat", "rb");
    if (fp == NULL) {
        return;
    }
    passenger_count = fread(passengers_details, sizeof(passengers), 100, fp);
    fclose(fp);
}

void add_train() {
    printf("Enter number of trains to add: ");
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Enter train number: ");
        scanf("%d", &trains[total_trains].train_no);
        printf("Enter train name: ");
        scanf("%s", trains[total_trains].train_name);
        printf("Enter route (From-To): ");
        scanf("%s", trains[total_trains].route);
        printf("Enter departure time: ");
        scanf("%s", trains[total_trains].time);
        total_trains++;
        printf("Train added successfully!\n");
    }
    save_trains_data();
}

void delete_train() {
    int T_no;
    printf("Enter train number to delete: ");
    scanf("%d", &T_no);
    int found = 0;
    for (int i = 0; i < total_trains; i++) {
        if (trains[i].train_no == T_no) {
            for (int j = i; j < total_trains - 1; j++) {
                trains[j] = trains[j + 1];
            }
            total_trains--;
            found = 1;
            printf("Train deleted successfully!\n");
            break;
        }
    }
    if (!found)
        printf("Train not found!\n");
    save_trains_data();
}

void view_tickets() {
    if (passenger_count == 0) {
        printf("No tickets booked\n");
        return;
    }
    printf("Booked Tickets:\n");
    for (int i = 0; i < passenger_count; i++) {
        printf("Seat Number: %d, Name: %s, Age: %d, Gender: %s, Phone: %s, Train No: %d, Class: %s, Fare: %.2f\n",
               passengers_details[i].SeatNumber,
               passengers_details[i].Name,
               passengers_details[i].Age,
               passengers_details[i].Gender,
               passengers_details[i].Phone,
               passengers_details[i].train_no,
               passengers_details[i].train_class,
               passengers_details[i].Fare);
    }
    return;
}

void revenue_report() {
    float total_revenue = 0;
    for (int i = 0; i < passenger_count; i++) {
        total_revenue += passengers_details[i].Fare;
    }
    printf("Total Revenue Generated: Rs %.2f\n", total_revenue);
    return;
}

void feedback() {
    FILE *fp = fopen("feedback.txt", "a");
    if (fp == NULL) {
        printf("Error opening feedback file!\n");
        return;
    }
    char name[50];
    char message[200];
    printf("\nEnter your name: ");
    scanf("%s", name);
    getchar();
    printf("Enter your feedback (max 200 characters): ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';
    fprintf(fp, "Name: %s\nFeedback: %s\n------------------\n", name, message);
    fclose(fp);
    printf("Thank you for your feedback!\n");
}

void view_feedback() {
    FILE *fp = fopen("feedback.txt", "r");
    if (fp == NULL) {
        printf("No feedback found.\n");
        return;
    }
    char line[256];
    printf("\n------ Passenger Feedback ------\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void view_trains() {
    if (total_trains == 0) {
        printf("No trains available.\n");
        return;
    }
    printf("\nList of Trains:\n");
    for (int i = 0; i < total_trains; i++) {
        printf("%d. %s - %s - %s\n", trains[i].train_no, trains[i].train_name, trains[i].route, trains[i].time);
    }
}

void admin_menu() {
    int admin_choice;
    int password;
    printf("Enter admin password: ");
    scanf("%d", &password);
    if (password != 1234) {
        printf("Incorrect password! Access denied.\n");
        return;
    } else if (password == 1234) {
        printf("Access granted. Welcome Admin!\n");

        while (1) {
            printf("\n--- Admin Menu ---\n");
            printf("1. Add Train\n");
            printf("2. Delete Train\n");
            printf("3. View All Trains\n");
            printf("4. View All Booked Tickets\n");
            printf("5. View Revenue\n");
            printf("6. View Feedback\n");
            printf("7. Return to main menu\n");
            printf("Enter your choice: ");
            scanf("%d", &admin_choice);
            switch (admin_choice) {
                case 1:
                    add_train();
                    break;

                case 2:
                    delete_train();
                    break;

                case 3:
                    view_trains();
                    break;

                case 4:
                    view_tickets();
                    break;

                case 5:
                    revenue_report();
                    break;

                case 6:
                    view_feedback();
                    break;
                case 7:
                    return;

                default:
                    printf("Invalid Choice!\n");
                    break;

            }
        }
    }
}

void view_waitlist() {
    if (waitlist_count == 0) {
        printf("No passengers in waitlist.\n");
        return;
    }
    printf("\n--- Current Waitlist ---\n");
    for (int i = 0; i < waitlist_count; i++) {
        printf("%d. %s (Train %d, Class: %s)\n",
               i + 1, passenger_waitlist[i].Name,
               passenger_waitlist[i].train_no,
               passenger_waitlist[i].train_class);
    }
}

void Booked_Tickets() {
    if (passenger_count >= 50) {
        printf("All seats are currently booked.\n");
        if (waitlist_count < 50) {
            passengers Details;

            printf("You are being added to the waitlist.\n");
            printf("Enter Name: ");
            scanf("%s", Details.Name);
            printf("Enter Age: ");
            scanf("%d", &Details.Age);
            printf("Enter Gender: ");
            scanf("%s", Details.Gender);
            printf("Enter Phone Number: ");
            scanf("%s", Details.Phone);
            printf("Enter Train Number: ");
            scanf("%d", &Details.train_no);
            printf("Enter Date of Journey (DDMMYYYY): ");
            scanf("%d", &Details.date);
            printf("Enter Class: ");
            scanf("%s", Details.train_class);

            Details.Fare = 100;
            Details.SeatNumber = -1;
            passenger_waitlist[waitlist_count++] = Details;
            printf("You have been added to the waitlist. Position: %d\n", waitlist_count);
        } else {
            printf("Waitlist is also full. Please try later.\n");
        }
        return;
    }

    passengers Details;
    printf("Enter Name: ");
    scanf("%s", Details.Name);
    printf("Enter Age: ");
    scanf("%d", &Details.Age);
    printf("Enter Gender: ");
    scanf("%s", Details.Gender);
    printf("Enter Phone Number: ");
    scanf("%s", Details.Phone);
    printf("Enter Train Number: ");
    scanf("%d", &Details.train_no);
    printf("Enter Date of Journey (DDMMYYYY): ");
    scanf("%d", &Details.date);

    printf("\nSelect Class\n");
    printf("1.General\n2.Sleeper\n3.Ac 3-Tier\n4.Ac 2-Tier\n5.Ac First Class\n");

    int classchoice;
    printf("Enter your choice: ");
    scanf("%d", &classchoice);

    switch (classchoice) {
        case 1:
            strcpy(Details.train_class, "General");
            Details.Fare = 100;
            break;

        case 2:
            strcpy(Details.train_class, "Sleeper");
            Details.Fare = 600;
            break;

        case 3:
            strcpy(Details.train_class, "Ac 3-tier");
            Details.Fare = 1200;
            break;

        case 4:
            strcpy(Details.train_class, "Ac 2-tier");
            Details.Fare = 2500;
            break;

        case 5:
            strcpy(Details.train_class, "Ac First class");
            Details.Fare = 3800;
            break;

        default:
            strcpy(Details.train_class, "General");
            Details.Fare = 100;

    }

    for (int i = 0; i < 50; i++) {
        if (booked_seats[i] == 0) {
            booked_seats[i] = 1;
            Details.SeatNumber = i + 1;
            passengers_details[passenger_count] = Details;
            passenger_count++;
            printf("\nTicket booked successfully! Seat Number: %d\n", Details.SeatNumber);
            break;
        }
    }
    save_passengers_data();
    return;
}

void cancel_ticket() {
    int seat_no;
    printf("Enter Seat Number to cancel: ");
    scanf("%d", &seat_no);

    if (seat_no <= 0 || seat_no > 50 || booked_seats[seat_no - 1] == 0) {
        printf("Invalid Seat Number or Seat not booked\n");
        return;
    }

    booked_seats[seat_no - 1] = 0;

    for (int i = 0; i < passenger_count; i++) {
        if (passengers_details[i].SeatNumber == seat_no) {
            for (int j = i; j < passenger_count - 1; j++) {
                passengers_details[j] = passengers_details[j + 1];
            }
            passenger_count--;
            printf("Ticket cancelled successfully for Seat Number: %d\n", seat_no);
            break;
        }
    }

    if (waitlist_count > 0) {
        passengers next = passenger_waitlist[0];

        for (int i = 0; i < waitlist_count - 1; i++) {
            passenger_waitlist[i] = passenger_waitlist[i + 1];
        }
        waitlist_count--;

        next.SeatNumber = seat_no;
        booked_seats[seat_no - 1] = 1;
        passengers_details[passenger_count++] = next;

        printf("\nSeat reassigned to waitlisted passenger: %s (Seat %d)\n", next.Name, seat_no);
    }

    save_passengers_data();
}

void Trains() {
    if (total_trains == 0) {
        printf("No trains added yet! Admin can add new trains.\n");
        return;
    }
    printf("Available Trains:\n");
    for (int i = 0; i < total_trains; i++) {
        printf("%d. %s - %s - %s\n", trains[i].train_no, trains[i].train_name, trains[i].route, trains[i].time);
    }
    return;
}

void train_details() {
    int choice;
    printf("Enter 1 to Show All Trains\n");
    printf("Enter 2 to find Train by Number\n");
    scanf("%d", &choice);
    if (choice == 1) {
        Trains();
    } else if (choice == 2) {
        int train_no;
        printf("Enter Train Number: ");
        scanf("%d", &train_no);
        int found = 0;
        for (int i = 0; i < total_trains; i++) {
            if (trains[i].train_no == train_no) {
                printf("Train %d - %s - %s - %s\n", trains[i].train_no, trains[i].train_name, trains[i].route, trains[i].time);
                found = 1;
                break;
            }
        }
        if (!found) printf("Train not found!\n");
    }
    return;
}

void view_seats_avilable() {
    printf("Seats Available:\n");
    for (int i = 0; i < 50; i++) {
        if (booked_seats[i] == 0)
            printf("[ %02d ] ", i + 1);
        else
            printf("[XX] ");
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    printf("\n");
    if (passenger_count == 50) {
        printf("No Seats Available\n");
    }
    return;
}

void showFareChart() {
    printf("==== Fare Chart ====\n");
    printf("General   : Rs. 100\n");
    printf("Sleeper   : Rs. 600\n");
    printf("AC 3-Tier : Rs. 1200\n");
    printf("AC 2-Tier : Rs. 2500\n");
    printf("First AC  : Rs. 3800\n");
    return;
}

void search_passenger() {
    char name[50];
    printf("Enter a passenger's name:");
    scanf("%s", name);
    int found = 0;
    for (int i = 0; i < passenger_count; i++) {
        if (strcmp(passengers_details[i].Name, name) == 0) {
            printf("Ticket Found! Seat: %d, Train: %d, Class: %s, Fare: %.2f\n",
                   passengers_details[i].SeatNumber,
                   passengers_details[i].train_no,
                   passengers_details[i].train_class,
                   passengers_details[i].Fare);
            found = 1;
        }
    }
    if (!found) printf("No ticket found for this name!\n");
}

void print_ticket(passengers Details) {
    printf("\n========== TICKET SUMMARY ==========\n");
    printf("Name        : %s\n", Details.Name);
    printf("Age         : %d\n", Details.Age);
    printf("Gender      : %s\n", Details.Gender);
    printf("Train No    : %d\n", Details.train_no);
    printf("Class       : %s\n", Details.train_class);
    printf("Seat Number : %d\n", Details.SeatNumber);
    printf("Fare        : Rs %.2f\n", Details.Fare);
    printf("Date        : %d\n", Details.date);
    printf("====================================\n");
}

void meal_plan() {
    int meal;
    printf("Select Meal Plan:\n");
    printf("1. Vegetarian - Rs. 150\n");
    printf("2. Non-Vegetarian - Rs. 200\n");
    printf("3. Choose Items\n");
    printf("Enter your choice: ");
    scanf("%d", &meal);
    switch (meal) {
        case 1:
            printf("Vegetarian meal selected. Pay 150 Rs to the Food Vendor at time of Delivery\n");
            break;

        case 2:
            printf("Non-Vegetarian meal selected. Pay 200 Rs to the Food Vendor at time of Delivery\n");
            break;
        case 3:
            {
                int items, quantity;
                float total = 0;
                printf("Menu:\n");
                printf("1. Sandwich - Rs. 100\n");
                printf("2. Salad - Rs. 80\n");
                printf("3. Juice - Rs. 50\n");
                printf("4. Coffee - Rs. 40\n");
                printf("5. Dessert - Rs. 60\n");
                printf("6. Snacks - Rs. 70\n");
                printf("7. Water Bottle - Rs. 30\n");
                printf("8. Tea - Rs. 20\n");
                printf("9. Soft Drink - Rs. 90\n");
                printf("10. Fruit Bowl - Rs. 110\n");
                printf("Enter number of items you want to order: ");
                scanf("%d", &items);
                for (int i = 0; i < items; i++) {
                    int choice;
                    printf("Enter item number (1-10): ");
                    scanf("%d", &choice);
                    printf("Enter quantity: ");
                    scanf("%d", &quantity);
                    switch (choice) {
                        case 1:
                            total += 100 * quantity;
                            break;

                        case 2:
                            total += 80 * quantity;
                            break;

                        case 3:
                            total += 50 * quantity;
                            break;

                        case 4:
                            total += 40 * quantity;
                            break;

                        case 5:
                            total += 60 * quantity;
                            break;

                        case 6:
                            total += 70 * quantity;
                            break;

                        case 7:
                            total += 30 * quantity;
                            break;

                        case 8:
                            total += 20 * quantity;
                            break;

                        case 9:
                            total += 90 * quantity;
                            break;

                        case 10:
                            total += 110 * quantity;
                            break;

                        default:
                            printf("Invalid item choice.\n");

                    }
                }
                printf("Total meal cost: Rs %.2f. Pay to the Food Vendor at time of Delivery\n", total);
            }
            break;


        default:
            printf("Invalid choice. No meal selected.\n");

    }
  return;
}


void e_wallet_functions(){
    int choice;
    float balance = 0.0;
    printf("E-Wallet Functions:\n");
    printf("1. Add Money\n");
    printf("2. View Balance\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            {
                char name[50];
                float amount,account_no,ifsc_code,irctc_id;

                printf("Enter Name: ");
                scanf("%s", name);
                printf("Enter amount to add: ");
                scanf("%f", &amount);
                printf("Enter IRCTC ID: ");
                scanf("%f", &irctc_id);
                printf("Enter Account Number: ");
                scanf("%f", &account_no);
                printf("Enter IFSC Code: ");
                scanf("%f", &ifsc_code);
                balance += amount;
                printf("Rs %.2f added to your wallet. Current Balance: Rs %.2f\n", amount, balance);
            }
            break;

        case 2:
            printf("Current Wallet Balance: Rs %.2f\n", balance);
            break;
        default:
            printf("Invalid choice.\n");
    }
      return;
}

   void emergency_contact(){
    printf("In case of emergency, please contact the following numbers:\n");
    printf("Railway Helpline: 139\n");
    printf("Women's Helpline: 181\n");
    printf("Police: 100\n");
    printf("Ambulance: 102\n");
    printf("Fire Brigade: 101\n");
    return;
   }


int main() {
    load_trains_data();
    load_passengers_data();
    int choose;
    while (1) {
        printf("\nAdvanced Train Ticket Booking System\n");
        printf("What will you be doing today?\n");
        printf("Function you can perform:-\n");
        printf("1.Ticket Booking\n");
        printf("2.Ticket cancelling\n");
        printf("3.View Passengers list\n");
        printf("4.Trains Details\n");
        printf("5.View available seats\n");
        printf("6.Show fare chart\n");
        printf("7.Admin Login\n");
        printf("8.Search Passenger\n");
        printf("9.Print Ticket\n");
        printf("10.View Waitlist\n");
        printf("11.Feedback/Complaint\n");
        printf("12.Meal Plan\n");
        printf("13.E-Wallet Functions\n");
        printf("14.Emergency Contact Numbers\n");
        printf("15.Exit\n");
        printf("Choose the function you wish to perform: ");
        scanf("%d", &choose);

        switch (choose) {
            case 1:
                Booked_Tickets();
                break;

            case 2:
                cancel_ticket();
                break;

            case 3:
                view_tickets();
                break;

            case 4:
                train_details();
                break;

            case 5:
                view_seats_avilable();
                break;

            case 6:
                showFareChart();
                break;

            case 7:
                admin_menu();
                break;

            case 8:
                search_passenger();
                break;

            case 9:
                if (passenger_count > 0)
                    print_ticket(passengers_details[passenger_count - 1]);
                else
                    printf("No ticket to print!\n");
                break;

            case 10:
                view_waitlist();
                break;

            case 11:
                feedback();
                break;

            case 12:
                meal_plan();
                break;

            case 13:
                e_wallet_functions();
                break;

            case 14:
                emergency_contact();
                break;    
                
            case 15:
                printf("Exiting system... Data saved successfully.\n");
                save_trains_data();
                save_passengers_data();
                exit(0);

            default:
                printf("Invalid Choice! Please Choose again\n");
        }
    }
    return 0;
}