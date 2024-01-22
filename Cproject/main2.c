#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define FILE_PATH "/home/details.txt"
#define UPDATE_PRICE_SCRIPT "/home/update_price.awk"
#define UPDATE_QUANTITY_SCRIPT "/home/update_quantity.awk"

struct FileData {
    char name[50];
    int price;
    int quantity;
};

sem_t open_sem, read_sem, write_sem, main_sem, close_sem, update_sem ;
FILE *file;

void* openThread(void* arg);
void* readThread(void* arg);
void* writeThread(void* arg);
void* closeThread(void* arg);
void* updateThread(void* arg);

int main() {
    pthread_t openThreadID, readThreadID, writeThreadID, closeThreadID, updateThreadID;

    
    sem_init(&open_sem, 0, 0);
    sem_init(&read_sem, 0, 0);
    sem_init(&write_sem, 0, 0);
    sem_init(&main_sem, 0, 0);
    sem_init(&close_sem, 0, 0);
    sem_init(&update_sem, 0, 0);

    char userInput;

    while (1) {
        printf("Press 'o' to open the file, 'r' to read the file, 'w' to write to the file, 'u' to update, 'c' to close the file :\n");

        
        userInput = getchar();

       
        while (getchar() != '\n');

        if (userInput == 'o' || userInput == 'O') {
           
            pthread_create(&openThreadID, NULL, openThread, NULL);

            
            sem_wait(&main_sem);

            if (file != NULL) {
                printf("File opened successfully.\n");

                
                sem_post(&read_sem);
                sem_post(&write_sem);
                sem_post(&close_sem);
                sem_post(&update_sem);

               
                pthread_join(openThreadID, NULL);
            } else {
                printf("Error opening the file.\n");
            }
        } else if (userInput == 'r' || userInput == 'R') {
            
            pthread_create(&readThreadID, NULL, readThread, NULL);

            
            pthread_join(readThreadID, NULL);

            
            sem_post(&main_sem);
        } else if (userInput == 'w' || userInput == 'W') {
            
            pthread_create(&writeThreadID, NULL, writeThread, NULL);

            
            pthread_join(writeThreadID, NULL);

            
            sem_post(&main_sem);
        } else if (userInput == 'u' || userInput == 'U') {
            
            pthread_create(&updateThreadID, NULL, updateThread, NULL);

        
            pthread_join(updateThreadID, NULL);

            
            sem_post(&main_sem);
        } else if (userInput == 'c' || userInput == 'C') {
            
            pthread_create(&closeThreadID, NULL, closeThread, NULL);

            
            pthread_join(closeThreadID, NULL);

            
            sem_post(&main_sem);
        } else if (userInput == 'q' || userInput == 'Q') {
            
            sem_post(&open_sem);
            pthread_join(openThreadID, NULL);

            break;  // Exit the loop and end the program
        } else {
            printf("Please press again.\n");
        }
    }


    if (file != NULL) {
        fclose(file);
    }

    return 0;
}

void* openThread(void* arg) {
    printf("Opening the file...\n");

    
    file = fopen(FILE_PATH, "a+");

    if (file == NULL) {
        perror("Error opening the file");
        fprintf(stderr, "File path: %s\n", FILE_PATH);
        exit(EXIT_FAILURE);
    }

    
    sem_post(&main_sem);

    return NULL;
}

void* readThread(void* arg) {
    
    sem_wait(&read_sem);
    file = fopen(FILE_PATH, "a+");

    if (file == NULL) {
        perror("Error opening the file");
        fprintf(stderr, "File path: %s\n", FILE_PATH);
        exit(EXIT_FAILURE);
    }

    printf("Reading the file...\n");

    // Read the file content and print it
    char buffer[300];  
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // Check if the end of file is reached or an error occurred
    if (feof(file)) {
        printf("End of file reached.\n");

        // Move the file pointer back to the beginning of the file
        fseek(file, 0, SEEK_SET);
    } else if (ferror(file)) {
        perror("Error reading file");
    }

    printf("File reading complete.\n");

    // Signal that the read operation is complete
    sem_post(&read_sem);

    // Signal the main thread to continue
    sem_post(&main_sem);

    return NULL;
}

void* writeThread(void* arg) {
    // Wait for the open thread to finish opening the file
    sem_wait(&write_sem);

    // Get user input for writing to the file
    struct FileData newData;
    printf("Enter name: ");
    scanf("%s", newData.name);

    printf("Enter price: ");
    scanf("%d", &newData.price);

    printf("Enter quantity: ");
    scanf("%d", &newData.quantity);

    // Clear the input buffer
    while (getchar() != '\n');

    // Move the file pointer to the end of the file
    fseek(file, 0, SEEK_END);

    // Write the data to the file
    fprintf(file, "%s %d %d\n", newData.name, newData.price, newData.quantity);

    // Flush the file buffer to ensure data is written to the file
    fflush(file);

    printf("Data written to the file.\n");

    // Signal that the write operation is complete
    sem_post(&write_sem);

    // Signal the main thread to continue
    sem_post(&main_sem);

    return NULL;
}

void* updateThread(void* arg) {
    // Wait for the open thread to finish opening the file
    sem_wait(&main_sem);

    // Get user input for the update operation
    char itemName[50];
    char updateType;
    int newPrice, newQuantity;

    printf("Enter the name of the item for update: ");
    scanf("%s", itemName);

    printf("Enter 'p' to update price, 'q' to update quantity, 'd' to delete or 'c' to cancel: ");
     scanf(" %c", &updateType);


    if (updateType == 'p' || updateType == 'P') {
        printf("Enter the new price: ");
        scanf("%d", &newPrice);
    } else if (updateType == 'q' || updateType == 'Q') {
        printf("Enter the new quantity: ");
        scanf("%d", &newQuantity);
    } else if (updateType == 'd' || updateType == 'D') {
        
        file = fopen(FILE_PATH, "r+");

        if (file == NULL) {
            perror("Error opening the file");
            fprintf(stderr, "File path: %s\n", FILE_PATH);
            exit(EXIT_FAILURE);
        }

        // Call the appropriate AWK script based on the update operation
        char awkCommand[1000];
        sprintf(awkCommand, "awk -v itemName=%s -f /home/delete.awk %s > temp.txt && mv temp.txt %s",
                itemName, FILE_PATH, FILE_PATH);
        system(awkCommand);

        // Close the file
        fclose(file);

        printf("Delete operation complete.\n");

        // Signal the main thread to continue
        sem_post(&update_sem);
        return NULL;
    } 
    else {
        printf("Update operation canceled.\n");
        sem_post(&update_sem);  // Signal the main thread to continue
        return NULL;
    }

    // Open the file for reading and writing
    file = fopen(FILE_PATH, "r+");

    if (file == NULL) {
        perror("Error opening the file");
        fprintf(stderr, "File path: %s\n", FILE_PATH);
        exit(EXIT_FAILURE);
    }
    // Call the appropriate AWK script based on the update operation
    if (updateType == 'p' || updateType == 'P') {
        char awkCommand[1000];
        sprintf(awkCommand, "awk -v itemName=%s -v newPrice=%d -f /home/update_price.awk %s > temp.txt && mv temp.txt %s",
                itemName, newPrice, FILE_PATH, FILE_PATH);
        system(awkCommand);
    } else if (updateType == 'q' || updateType == 'Q') {
        char awkCommand[1000];
        sprintf(awkCommand, "awk -v itemName=%s -v newQuantity=%d -f /home/update_quantity.awk %s > temp.txt && mv temp.txt %s",
                itemName, newQuantity, FILE_PATH, FILE_PATH);
        system(awkCommand);
    }

    // Close the file
    fclose(file);

    printf("Update operation complete.\n");

    // Signal the main thread to continue
    sem_post(&update_sem);

    return NULL;
}
void* closeThread(void* arg) {
    sem_wait(&close_sem);

    printf("Closing the file...\n");

    // Close the file
    fclose(file);

    printf("File closed successfully.\n");

    sem_post(&close_sem);
    sem_post(&main_sem);

    // Terminate the program
    exit(EXIT_SUCCESS);

    return NULL;
}