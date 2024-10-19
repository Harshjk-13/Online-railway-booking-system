#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRAINS 5
#define MAX_QUEUE_SIZE 100

// Structure for a train booking request
struct Booking {
    char username[50];
    int train_id;
    int seats_requested;
};

// Structure for a train
struct Train {
    char name[50];
    char source[50];
    char destination[50];
    int seats_available;
};

// Queue data structure
struct BookingQueue {
    struct Booking bookings[MAX_QUEUE_SIZE];
    int front;
    int rear;
};

// Global variables
struct Train trains[MAX_TRAINS] = {
    {"Express 101", "CityA", "CityB", 50},
    {"Express 202", "CityB", "CityC", 30},
    {"Express 303", "CityA", "CityC", 20},
    {"Express 404", "CityC", "CityD", 40},
    {"Express 505", "CityD", "CityA", 25}
};

struct BookingQueue bookingQueue;

// Function prototypes
void initializeQueue();
int isQueueFull();
int isQueueEmpty();
void enqueue(struct Booking booking);
struct Booking dequeue();
void searchTrains();
void bookTicket();
void processBookings();

// Main function
int main() {
    int choice;

    initializeQueue();

    while (1) {
        printf("\n===== Online Railway Booking System =====\n");
        printf("1. Search Trains\n");
        printf("2. Book Ticket\n");
        printf("3. Process Bookings\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                searchTrains();
                break;
            case 2:
                bookTicket();
                break;
            case 3:
                processBookings();
                break;
            case 4:
                printf("Thank you for using the Railway Booking System!\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Initialize the booking queue
void initializeQueue() {
    bookingQueue.front = -1;
    bookingQueue.rear = -1;
}

// Check if the queue is full
int isQueueFull() {
    return (bookingQueue.rear + 1) % MAX_QUEUE_SIZE == bookingQueue.front;
}

// Check if the queue is empty
int isQueueEmpty() {
    return bookingQueue.front == -1;
}

// Add a booking to the queue (enqueue)
void enqueue(struct Booking booking) {
    if (isQueueFull()) {
        printf("Booking queue is full! Cannot accept more bookings.\n");
        return;
    }

    if (isQueueEmpty()) {
        bookingQueue.front = 0;
    }

    bookingQueue.rear = (bookingQueue.rear + 1) % MAX_QUEUE_SIZE;
    bookingQueue.bookings[bookingQueue.rear] = booking;
    printf("Booking request for %s has been added to the queue.\n", booking.username);
}

// Remove and return the next booking from the queue (dequeue)
struct Booking dequeue() {
    struct Booking booking;

    if (isQueueEmpty()) {
        printf("No bookings in the queue.\n");
        strcpy(booking.username, ""); // Return an empty booking
        return booking;
    }

    booking = bookingQueue.bookings[bookingQueue.front];
    if (bookingQueue.front == bookingQueue.rear) {
        bookingQueue.front = -1;  // Queue becomes empty
        bookingQueue.rear = -1;
    } else {
        bookingQueue.front = (bookingQueue.front + 1) % MAX_QUEUE_SIZE;
    }

    return booking;
}

// Function to search available trains
void searchTrains() {
    char source[50], destination[50];
    printf("\n===== Search Trains =====\n");
    printf("Enter source station: ");
    scanf("%s", source);
    printf("Enter destination station: ");
    scanf("%s", destination);

    printf("\nAvailable trains:\n");
    int found = 0;
    for (int i = 0; i < MAX_TRAINS; i++) {
        if (strcmp(trains[i].source, source) == 0 && strcmp(trains[i].destination, destination) == 0) {
            printf("%d. %s (Seats available: %d)\n", i + 1, trains[i].name, trains[i].seats_available);
            found = 1;
        }
    }

    if (!found) {
        printf("No trains available for the specified route.\n");
    }
}

// Function to book a ticket (enqueue booking request)
void bookTicket() {
    struct Booking booking;
    int trainChoice, seats;

    printf("\n===== Book Ticket =====\n");
    printf("Enter username: ");
    scanf("%s", booking.username);

    searchTrains();

    printf("\nEnter the train number to book: ");
    scanf("%d", &trainChoice);

    if (trainChoice < 1 || trainChoice > MAX_TRAINS) {
        printf("Invalid train selection!\n");
        return;
    }

    printf("Enter the number of seats to book: ");
    scanf("%d", &seats);

    if (seats <= 0) {
        printf("Invalid number of seats!\n");
        return;
    }

    booking.train_id = trainChoice - 1;
    booking.seats_requested = seats;

    enqueue(booking);  // Add booking request to the queue
}

// Function to process all bookings in the queue
void processBookings() {
    printf("\n===== Processing Bookings =====\n");

    while (!isQueueEmpty()) {
        struct Booking booking = dequeue();

        // Check if booking is valid (non-empty)
        if (strcmp(booking.username, "") == 0) {
            continue;
        }

        int train_id = booking.train_id;
        int seats_requested = booking.seats_requested;

        if (trains[train_id].seats_available >= seats_requested) {
            trains[train_id].seats_available -= seats_requested;
            printf("Booking successful! %s has booked %d seat(s) on train %s.\n", 
                    booking.username, seats_requested, trains[train_id].name);
        } else {
            printf("Booking failed for %s! Not enough seats available on train %s.\n", 
                    booking.username, trains[train_id].name);
        }
    }

    if (isQueueEmpty()) {
        printf("All bookings have been processed.\n");
    }
}
