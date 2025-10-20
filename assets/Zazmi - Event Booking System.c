#include <stdio.h>
#include <string.h>

#define ROWS 5
#define COLS 5
#define MAX_TICKETS 5
#define ADMIN_PASSWORD "12345"
#define TICKET_PRICE 100  // Price per ticket in dollars
#define BULK_BOOKING_THRESHOLD 5  // Bulk booking is 5 or more tickets
#define DISCOUNT_PERCENTAGE 10  // 10% discount for bulk bookings

// Event list
const char *events[] = {"Concert", "Sport Games", "Theatre"};

// Structure to store seating and revenue data
typedef struct {
    int seats[ROWS][COLS];  // Seating arrangement
    int ticketsSold;        // Number of tickets sold
} Event;

// Global array to store events
Event eventList[3];  // 3 events (Concert, Sport Games, Theatre)

// Function to display the seating arrangement for a given event
void displaySeats(int seats[ROWS][COLS]) {
    printf("\nUpdated Seating Arrangement:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seats[i][j] == 0)
                printf("[O] ");  // O for available seat
            else
                printf("[X] ");  // X for booked seat
        }
        printf("\n");
    }
}

// Function to check if there are enough consecutive available seats
int checkBulkSeats(int seats[ROWS][COLS], int numTickets, int *startCol, int row) {
    int consecutiveSeats = 0;
    for (int col = 0; col < COLS; col++) {
        if (seats[row][col] == 0) {
            consecutiveSeats++;
            if (consecutiveSeats == numTickets) {
                *startCol = col - numTickets + 1;  // Store the starting column index
                return 1;  // Found enough consecutive seats
            }
        } else {
            consecutiveSeats = 0;
        }
    }
    return 0;  // No enough consecutive seats in this row
}

// Function to book a bulk of seats
void bookBulkSeats(int seats[ROWS][COLS], int numTickets, int memberId, const char *memberName, int eventIndex) {
    int row, startCol;
    int booked = 0;
    int totalCost = numTickets * TICKET_PRICE;  // Calculate the total cost

    // Apply a 10% discount if 5 or more tickets are booked
    if (numTickets >= BULK_BOOKING_THRESHOLD) {
        int discount = (totalCost * DISCOUNT_PERCENTAGE) / 100;
        totalCost -= discount;
        printf("\nYou are eligible for a 10%% discount!\nDiscount Amount: $%d\n", discount);
    }

    // Display total cost before applying discount
    printf("\nTotal Cost for %d tickets before discount: $%d\n", numTickets, numTickets * TICKET_PRICE);

    // Try to book the requested number of tickets
    for (int i = 0; i < numTickets; i++) {
        printf("Enter the row number (1 to %d): ", ROWS);
        scanf("%d", &row);
        row -= 1;  // Adjust for zero-based index

        // Check if the row number is valid
        if (row < 0 || row >= ROWS) {
            printf("Invalid row number. Please try again.\n");
            i--;
            continue;
        }

        // Check if enough consecutive seats are available in this row
        if (checkBulkSeats(seats, numTickets, &startCol, row)) {
            for (int col = startCol; col < startCol + numTickets; col++) {
                seats[row][col] = 1;  // Book the seat
            }
            // Print booking confirmation with member details
            printf("\nBooking Successful!\n");
            printf("Member ID: %d\n", memberId);
            printf("Member Name: %s\n", memberName);
            printf("%d consecutive seats booked successfully at Row %d, from Column %d to %d.\n",
                   numTickets, row + 1, startCol + 1, startCol + numTickets);
            booked = 1;

            // Update the tickets sold for the selected event
            eventList[eventIndex].ticketsSold += numTickets;  // Increment tickets sold for the selected event

            break;
        } else {
            printf("Sorry, there are not enough consecutive seats available in Row %d. Please try another row.\n", row + 1);
            i--;  // Retry this ticket booking
        }
    }

    if (!booked) {
        printf("Could not book all tickets. Please try with fewer tickets.\n");
    }

    // Display the final price after discount (if applicable)
    printf("\nTotal Amount to Pay after Discount: $%d\n", totalCost);

    // Show updated seating arrangement after booking
    displaySeats(seats);
}

// Function to input member details and process the booking
void processBooking(int seats[ROWS][COLS], int memberId, const char *memberName, int eventIndex) {
    int numTickets;
    
    // Get the number of tickets
    printf("Enter the number of tickets you want to book: ");
    scanf("%d", &numTickets);

    // Validate the number of tickets
    if (numTickets > MAX_TICKETS) {
        printf("Sorry, you can book a maximum of %d tickets at once.\n", MAX_TICKETS);
        return;
    }

    // Validate the number of tickets against available seats
    int availableSeats = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seats[i][j] == 0) availableSeats++;
        }
    }

    if (availableSeats < numTickets) {
        printf("Not enough seats available for your request. Please reduce the number of tickets.\n");
        return;
    }

    // If tickets are available, proceed with bulk booking
    bookBulkSeats(seats, numTickets, memberId, memberName, eventIndex);
}

// Function to select event and display its seating arrangement
void selectEventAndBook(int memberId, const char *memberName) {
    int eventChoice;

    // Display event options
    printf("\nSelect an event to book tickets:\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, events[i]);
    }
    printf("Enter your choice: ");
    scanf("%d", &eventChoice);

    // Validate event choice
    if (eventChoice < 1 || eventChoice > 3) {
        printf("Invalid event choice. Please try again.\n");
        return;
    }

    // Define separate seating arrangements for each event
    int (*seats)[ROWS][COLS] = NULL;
    int eventIndex = eventChoice - 1;  // Adjust index for array

    switch (eventChoice) {
        case 1:
            printf("\nYou selected the Concert event.\n");
            seats = &eventList[0].seats;
            break;
        case 2:
            printf("\nYou selected the Sport Games event.\n");
            seats = &eventList[1].seats;
            break;
        case 3:
            printf("\nYou selected the Theatre event.\n");
            seats = &eventList[2].seats;
            break;
        default:
            printf("Invalid choice, please try again.\n");
            return;
    }

    // Display ticket price after selecting the event
    printf("\nTicket Price for %s: $%d\n", events[eventIndex], TICKET_PRICE);

    // Display available seats for the selected event
    displaySeats(*seats);

    // Proceed with booking tickets for the selected event
    processBooking(*seats, memberId, memberName, eventIndex);  // Pass actual member details
}

// Function to handle member login
void memberLogin() {
    int memberId;
    char memberName[50];

    // Get member details
    printf("\nEnter your member ID: ");
    scanf("%d", &memberId);
    printf("Enter your name: ");
    scanf(" %[^\n]%*c", memberName);  // To read the full name with spaces

    // Proceed to event booking system with correct member details
    selectEventAndBook(memberId, memberName);
}

// Function to handle admin login
void adminLogin() {
    char password[10];

    printf("\nEnter admin password: ");
    scanf("%s", password);

    if (strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("\nAdmin login successful!\n");
        // Show the admin menu
        int choice;
        while (1) {
            printf("\nAdmin Menu:\n");
            printf("1. Check Seats Status\n");
            printf("2. Check Revenue and Sales\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    for (int i = 0; i < 3; i++) {
                        printf("\nEvent: %s\n", events[i]);
                        displaySeats(eventList[i].seats);
                    }
                    break;
                case 2:
                    for (int i = 0; i < 3; i++) {
                        int totalRevenue = eventList[i].ticketsSold * TICKET_PRICE;
                        // Apply discount if more than 5 tickets are sold
                        if (eventList[i].ticketsSold >= BULK_BOOKING_THRESHOLD) {
                            totalRevenue -= (totalRevenue * DISCOUNT_PERCENTAGE) / 100;
                        }
                        printf("\nEvent: %s\n", events[i]);
                        printf("Tickets Sold: %d\n", eventList[i].ticketsSold);
                        printf("Revenue: $%d\n", totalRevenue);
                    }
                    break;
                case 3:
                    return;  // Exit admin menu
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } else {
        printf("Invalid password. Returning to main menu.\n");
    }
}

// Main menu for login selection
void mainMenu() {
    int choice;

    while (1) {
        printf("\nWelcome to Event Ticket Booking System\n");
        printf("1. Login as Member\n");
        printf("2. Login as Admin\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                memberLogin();
                break;
            case 2:
                adminLogin();
                break;
            case 3:
                printf("Exiting the system. Thank you!\n");
                return;
            default:
                printf("Invalid choice, please try again.\n");
        }
    }
}

int main() {
    // Initialize all events' seat arrangements
    for (int i = 0; i < 3; i++) {
        memset(eventList[i].seats, 0, sizeof(eventList[i].seats));  // Clear seats
        eventList[i].ticketsSold = 0;  // Reset ticket sales
    }

    mainMenu();  // Start the main menu
    return 0;
}
