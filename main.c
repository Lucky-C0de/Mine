#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 50
#define RATE_PER_12H 799
#define WEEKEND_EXTRA 100

int running = 1, role;
char option;
int bookingCount = 0;

int bookingIDs[MAX];
char roomIDs[MAX][4];
char lastNames[MAX][30], firstNames[MAX][30];
char middleInitials[MAX];
char contacts[MAX][15];
char bookedDates[MAX][11];
char timeIns[MAX][6], timeOuts[MAX][6];
float totalAmounts[MAX];
char statuses[MAX][3];

const char *roomGrid[6][4] = {
    {"A1", "A2", "A3", "A4"},
    {"A5", "A6", "A7", "A8"},
    {"B1", "B2", "B3", "B4"},
    {"B5", "B6", "B7", "B8"},
    {"C1", "C2", "C3", "C4"},
    {"C5", "C6", "C7", "C8"}
};

char roomStatus[6][4][3];

void saveBookingsToFile() {
    FILE *f = fopen("bookings.txt", "w");
    for (int k = 0; k < bookingCount; k++) {
        fprintf(f, "%d %s %s %s %c . %s %s %s %.2f %s\n",
                bookingIDs[k], roomIDs[k], lastNames[k], firstNames[k],
                middleInitials[k], bookedDates[k], timeIns[k],
                timeOuts[k], totalAmounts[k], statuses[k]);
    }
    fclose(f);
}

void loadBookingsFromFile() {
    FILE *f = fopen("bookings.txt", "r");
    if (f != NULL) {
        while (bookingCount < MAX &&
               fscanf(f, "%d %s %s %s %c . %s %s %s %f %s",
                      &bookingIDs[bookingCount],
                      roomIDs[bookingCount],
                      lastNames[bookingCount],
                      firstNames[bookingCount],
                      &middleInitials[bookingCount],
                      bookedDates[bookingCount],
                      timeIns[bookingCount],
                      timeOuts[bookingCount],
                      &totalAmounts[bookingCount],
                      statuses[bookingCount]) == 10) {
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 4; j++) {
                    if (strcmp(roomGrid[i][j], roomIDs[bookingCount]) == 0) {
                        strcpy(roomStatus[i][j], statuses[bookingCount]);
                    }
                }
            }
            bookingCount++;
        }
        fclose(f);
    }
}

void showMap() {
    while (running) {
        system("cls");
        printf("=== Hotel Room Map ===\n\n");

        for (int x = 0; x < 6; x++) {
            for (int y = 0; y < 4; y++) {
                printf("%s[%s] ", roomGrid[x][y], roomStatus[x][y]);
            }
            printf("\n");
        }

        printf("\n=== Hello ");
        if (role == 0) {
            printf("Admin ===\n");
        } else {
            printf("Dear Customer ===\n");
        }

        printf("\n[1] Back\n");
        printf("\nSelect: ");
        scanf(" %c", &option);

        if (option == '1') {
            if (role == 0) {
                return;
            } else {
                return;
            }
        } else {
            printf("\nInput invalid, try again!\n");
        }
    }
}

void createBooking() {
    if (bookingCount >= MAX) {
        printf("Booking limit reached.\n");
        return;
    }

    char tempRoom[4];
    printf("Enter Room ID (e.g., A1): ");
    scanf("%s", tempRoom);

    int iRoom = -1, jRoom = -1;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(roomGrid[i][j], tempRoom) == 0) {
                iRoom = i;
                jRoom = j;
            }
        }
    }

    if (iRoom == -1) {
        printf("\nInvalid Room ID.\n\nPress any key to continue: ");
        char temp[64];
        scanf("%s", &temp);

        if(temp != NULL){
            return;
        }

    }
    if (strcmp(roomStatus[iRoom][jRoom], "UN") == 0 ||
        strcmp(roomStatus[iRoom][jRoom], "RR") == 0 ||
        strcmp(roomStatus[iRoom][jRoom], "TT") == 0) {
        printf("\nRoom not available.\n\nPress any key to continue: ");
        char temp[64];
        scanf("%s", &temp);

        if(temp != NULL){
            return;
        }
    }

    int i = bookingCount;
    bookingIDs[i] = i + 1;
    strcpy(roomIDs[i], tempRoom);

    printf("Last Name: ");
    scanf("%s", lastNames[i]);
    printf("First Name: ");
    scanf("%s", firstNames[i]);
    printf("Middle Initial: ");
    scanf(" %c", &middleInitials[i]);
    printf("Contact Number: ");
    scanf("%s", contacts[i]);
    printf("Booked Date (MM/DD/YYYY): ");
    scanf("%s", bookedDates[i]);
    printf("Time In (HH:MM): ");
    scanf("%s", timeIns[i]);
    printf("Time Out (HH:MM): ");
    scanf("%s", timeOuts[i]);

    int m, d, y;
    struct tm t = {0};
    sscanf(bookedDates[i], "%d/%d/%d", &m, &d, &y);
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_year = (y >= 1900) ? (y - 1900) : (2000 + y - 1900);
    mktime(&t);

    totalAmounts[i] = RATE_PER_12H;
    if (t.tm_wday == 0 || t.tm_wday == 6) {
        totalAmounts[i] += WEEKEND_EXTRA;
    }

    strcpy(statuses[i], "RR");
    strcpy(roomStatus[iRoom][jRoom], "RR");

    bookingCount++;
    saveBookingsToFile();

    printf("Booking created. ID: %d, Total: PHP %.2f\n", bookingIDs[i], totalAmounts[i]);
    system("pause");
}

void viewBookings() {
    if (bookingCount == 0) {
        printf("No bookings available.\n");
    } else {
        for (int i = 0; i < bookingCount; i++) {
            printf("ID: %d | Room: %s | Guest: %s %c. %s | Date: %s | Time: %s-%s | Amount: PHP %.2f\n",
                   bookingIDs[i], roomIDs[i], firstNames[i], middleInitials[i],
                   lastNames[i], bookedDates[i], timeIns[i], timeOuts[i], totalAmounts[i]);
        }
    }
    system("pause");
}

void updateBooking() {
    char searchRoom[4];
    printf("Enter Room ID to update: ");
    scanf("%s", searchRoom);
    int found = 0;
    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(roomIDs[i], searchRoom) == 0) {
            printf("New Last Name: ");
            scanf("%s", lastNames[i]);
            printf("New First Name: ");
            scanf("%s", firstNames[i]);
            printf("New Middle Initial: ");
            scanf(" %c", &middleInitials[i]);
            printf("New Contact Number: ");
            scanf("%s", contacts[i]);
            printf("New Booked Date (MM/DD/YYYY): ");
            scanf("%s", bookedDates[i]);
            printf("New Time In: ");
            scanf("%s", timeIns[i]);
            printf("New Time Out: ");
            scanf("%s", timeOuts[i]);
            found = 1;
            break;
        }
    }

    if (found) {
        saveBookingsToFile();
        printf("Booking updated.\n");
    } else {
        printf("Room ID not found.\n");
    }
    system("pause");
}

void manageRooms() {
    char input[4], newStat[3];
    printf("Enter Room ID to change status: ");
    scanf("%s", input);
    int found = 0;

    for (int i = 0; i < 6 && !found; i++) {
        for (int j = 0; j < 4 && !found; j++) {
            if (strcmp(roomGrid[i][j], input) == 0) {
                printf("Enter new status (RR, TT, UN, --): ");
                scanf("%s", newStat);
                strcpy(roomStatus[i][j], newStat);

                for (int k = 0; k < bookingCount; k++) {
                    if (strcmp(roomIDs[k], input) == 0) {
                        for (int l = k; l < bookingCount - 1; l++) {
                            bookingIDs[l] = bookingIDs[l + 1];
                            strcpy(roomIDs[l], roomIDs[l + 1]);
                            strcpy(lastNames[l], lastNames[l + 1]);
                            strcpy(firstNames[l], firstNames[l + 1]);
                            middleInitials[l] = middleInitials[l + 1];
                            strcpy(contacts[l], contacts[l + 1]);
                            strcpy(bookedDates[l], bookedDates[l + 1]);
                            strcpy(timeIns[l], timeIns[l + 1]);
                            strcpy(timeOuts[l], timeOuts[l + 1]);
                            totalAmounts[l] = totalAmounts[l + 1];
                            strcpy(statuses[l], statuses[l + 1]);
                        }
                        bookingCount--;
                        break;
                    }
                }
                found = 1;
            }
        }
    }

    if (found) {
        saveBookingsToFile();
        printf("Room status updated and associated bookings removed.\n");
    } else {
        printf("Room ID not found.\n");
    }
    system("pause");
}

void userMenu() {
    while (running) {
        system("cls");
        printf("Hello, our valued customer!\n");
        printf("\n[1] Create bookings\n");
        printf("[2] View Room Map\n");
        printf("[3] View Bookings\n");
        printf("[0] Back\n");
        printf("\nSelect: ");
        scanf(" %c", &option);

        if (option == '1') {
            createBooking();
        } else if (option == '2') {
            showMap();
        } else if (option == '3') {
            viewBookings();
        } else if (option == '0') {
            return;
        } else {
            printf("Invalid input, try again.\n");
            system("pause");
        }
    }
}

void adminMenu() {
    while (running) {
        system("cls");
        printf("Welcome, Admin!\n");
        printf("\n[1] Create bookings\n");
        printf("[2] View Room Map\n");
        printf("[3] Update Bookings\n");
        printf("[4] Delete Bookings and Manage Rooms\n");
        printf("[5] View Bookings\n");
        printf("[0] Back\n");
        printf("\nSelect: ");
        scanf(" %c", &option);

        if (option == '1') {
            createBooking();
        } else if (option == '2') {
            showMap();
        } else if (option == '3') {
            updateBooking();
        } else if (option == '4') {
            manageRooms();
        } else if (option == '5') {
            viewBookings();
        } else if (option == '0'){
            return;
        } else {
            printf("Invalid input, try again.\n");
            system("pause");
        }
    }
}

void start() {
    while (running) {
        system("cls");
        printf("Welcome to the hotel reservation management system!\n");
        printf("\nLog in as?\n");
        printf("[1] User\n");
        printf("[2] Administrator\n");
        printf("[3] Close\n");
        printf("\nInput: ");
        scanf(" %c", &option);

        if (option == '1') {
            role = 1;
            userMenu();
        } else if (option == '2') {
            char password[10];
            printf("\nEnter password: ");
            scanf(" %s", &password);

            if (strcmp( password, "admin123" ) == 0) {
                role = 0;
                adminMenu();
            } else {
                printf("Password is wrong, please try again!\n");
                system("pause");
            }

        } else if (option == '3') {
            system("cls");
            printf("Closing application, thank you for your patronage!\n");
            running = 0;
        } else {
            printf("Input invalid, please try again!\n");
            system("pause");
        }
    }
}

int main() {
    for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 4; y++) {
            strcpy(roomStatus[x][y], "--");
        }
    }
    loadBookingsFromFile();
    start();
    return 0;
}
