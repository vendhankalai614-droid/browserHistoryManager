#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// STEP 1: Define Constants
// Constants are fixed values that don't change
#define MAX_URL_LENGTH 256
#define MAX_TITLE_LENGTH 256
#define MAX_ENTRIES 1000
#define FILENAME "browser_history.txt"

// STEP 2: Define Structure
// A structure is like a container that holds related data together
typedef struct {
    int id;
    char url[MAX_URL_LENGTH];
    char title[MAX_TITLE_LENGTH];
    char timestamp[50];
    int visit_count;
} HistoryEntry;

// STEP 3: Global Variables
// These variables can be accessed from anywhere in the program
HistoryEntry history[MAX_ENTRIES];
int total_entries = 0;

// STEP 4: Function Declarations
// Telling the compiler what functions exist before we define them
void load_history();
void save_history();
void add_entry();
void view_all_history();
void view_recent_history();
void search_history();
void delete_entry();
void clear_all_history();
void view_statistics();
void get_current_timestamp(char* buffer);
void display_menu();
void clear_screen();

// STEP 5: Main Function
// This is where the program starts executing
int main() {
    int choice;
    
    printf("=======================================================\n");
    printf("           BROWSER HISTORY MANAGER (C)                \n");
    printf("=======================================================\n\n");
    
    // Load existing history from file
    load_history();
    
    // Main program loop
    while (1) {
        display_menu();
        printf("\nEnter your choice (1-9): ");
        scanf("%d", &choice);
        getchar(); // Clear the newline character from buffer
        
        switch (choice) {
            case 1:
                add_entry();
                break;
            case 2:
                view_all_history();
                break;
            case 3:
                view_recent_history();
                break;
            case 4:
                search_history();
                break;
            case 5:
                delete_entry();
                break;
            case 6:
                clear_all_history();
                break;
            case 7:
                view_statistics();
                break;
            case 8:
                save_history();
                break;
            case 9:
                save_history();
                printf("\nThank you for using Browser History Manager!\n");
                return 0;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
    
    return 0;
}

// STEP 6: Function Definitions

void display_menu() {
    printf("\n");
    printf("================== MENU ==================\n");
    printf("1. Add new entry\n");
    printf("2. View all history\n");
    printf("3. View recent history\n");
    printf("4. Search history\n");
    printf("5. Delete entry\n");
    printf("6. Clear all history\n");
    printf("7. View statistics\n");
    printf("8. Save history\n");
    printf("9. Exit\n");
    printf("==========================================\n");
}

void get_current_timestamp(char* buffer) {
    // Get current time and format it as a string
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", t);
}

void load_history() {
    // Open file for reading
    FILE *file = fopen(FILENAME, "r");
    
    if (file == NULL) {
        printf("No existing history file found. Starting fresh!\n");
        return;
    }
    
    // Read entries from file
    total_entries = 0;
    while (fscanf(file, "%d|%255[^|]|%255[^|]|%49[^|]|%d\n",
                  &history[total_entries].id,
                  history[total_entries].url,
                  history[total_entries].title,
                  history[total_entries].timestamp,
                  &history[total_entries].visit_count) == 5) {
        total_entries++;
        if (total_entries >= MAX_ENTRIES) {
            break;
        }
    }
    
    fclose(file);
    printf("Loaded %d history entries.\n", total_entries);
}

void save_history() {
    // Open file for writing
    FILE *file = fopen(FILENAME, "w");
    
    if (file == NULL) {
        printf("Error: Cannot save history!\n");
        return;
    }
    
    // Write all entries to file
    for (int i = 0; i < total_entries; i++) {
        fprintf(file, "%d|%s|%s|%s|%d\n",
                history[i].id,
                history[i].url,
                history[i].title,
                history[i].timestamp,
                history[i].visit_count);
    }
    
    fclose(file);
    printf("\nHistory saved successfully!\n");
}

void add_entry() {
    if (total_entries >= MAX_ENTRIES) {
        printf("\nError: History is full! Cannot add more entries.\n");
        return;
    }
    
    char url[MAX_URL_LENGTH];
    char title[MAX_TITLE_LENGTH];
    
    printf("\n--- Add New Entry ---\n");
    printf("Enter URL: ");
    fgets(url, MAX_URL_LENGTH, stdin);
    url[strcspn(url, "\n")] = 0; // Remove newline
    
    printf("Enter page title: ");
    fgets(title, MAX_TITLE_LENGTH, stdin);
    title[strcspn(title, "\n")] = 0; // Remove newline
    
    // Check if URL already exists
    int found = -1;
    for (int i = 0; i < total_entries; i++) {
        if (strcmp(history[i].url, url) == 0) {
            found = i;
            break;
        }
    }
    
    if (found != -1) {
        // Update existing entry
        history[found].visit_count++;
        get_current_timestamp(history[found].timestamp);
        printf("\nEntry updated! Visit count: %d\n", history[found].visit_count);
    } else {
        // Add new entry
        history[total_entries].id = total_entries + 1;
        strcpy(history[total_entries].url, url);
        strcpy(history[total_entries].title, title);
        get_current_timestamp(history[total_entries].timestamp);
        history[total_entries].visit_count = 1;
        
        total_entries++;
        printf("\nNew entry added successfully!\n");
    }
    
    save_history();
}

void view_all_history() {
    if (total_entries == 0) {
        printf("\nNo browsing history available.\n");
        return;
    }
    
    printf("\n");
    printf("=======================================================================\n");
    printf("                        BROWSING HISTORY                              \n");
    printf("=======================================================================\n");
    
    for (int i = total_entries - 1; i >= 0; i--) {
        printf("\nID: %d\n", history[i].id);
        printf("Title: %s\n", history[i].title);
        printf("URL: %s\n", history[i].url);
        printf("Last Visited: %s\n", history[i].timestamp);
        printf("Visit Count: %d\n", history[i].visit_count);
        printf("-----------------------------------------------------------------------\n");
    }
}

void view_recent_history() {
    if (total_entries == 0) {
        printf("\nNo browsing history available.\n");
        return;
    }
    
    int count;
    printf("\nHow many recent entries to display? ");
    scanf("%d", &count);
    getchar();
    
    if (count > total_entries) {
        count = total_entries;
    }
    
    printf("\n%d Most Recent Entries:\n", count);
    printf("=======================================================================\n");
    
    for (int i = total_entries - 1; i >= total_entries - count; i--) {
        printf("%s | %s | %s\n", 
               history[i].timestamp, 
               history[i].title, 
               history[i].url);
    }
}

void search_history() {
    if (total_entries == 0) {
        printf("\nNo browsing history available.\n");
        return;
    }
    
    char keyword[MAX_TITLE_LENGTH];
    printf("\nEnter search keyword: ");
    fgets(keyword, MAX_TITLE_LENGTH, stdin);
    keyword[strcspn(keyword, "\n")] = 0;
    
    printf("\nSearch Results:\n");
    printf("=======================================================================\n");
    
    int found_count = 0;
    for (int i = 0; i < total_entries; i++) {
        // Search in both URL and title (case-insensitive)
        if (strstr(history[i].url, keyword) != NULL || 
            strstr(history[i].title, keyword) != NULL) {
            printf("\nID: %d\n", history[i].id);
            printf("Title: %s\n", history[i].title);
            printf("URL: %s\n", history[i].url);
            printf("Last Visited: %s\n", history[i].timestamp);
            printf("-----------------------------------------------------------------------\n");
            found_count++;
        }
    }
    
    if (found_count == 0) {
        printf("No results found for '%s'\n", keyword);
    } else {
        printf("\nFound %d result(s)\n", found_count);
    }
}

void delete_entry() {
    if (total_entries == 0) {
        printf("\nNo browsing history available.\n");
        return;
    }
    
    int id;
    printf("\nEnter entry ID to delete: ");
    scanf("%d", &id);
    getchar();
    
    int found = -1;
    for (int i = 0; i < total_entries; i++) {
        if (history[i].id == id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("\nEntry with ID %d not found.\n", id);
        return;
    }
    
    // Shift all entries after the deleted one
    for (int i = found; i < total_entries - 1; i++) {
        history[i] = history[i + 1];
    }
    
    total_entries--;
    printf("\nEntry deleted successfully!\n");
    save_history();
}

void clear_all_history() {
    char confirm[10];
    printf("\nAre you sure you want to clear ALL history? (yes/no): ");
    fgets(confirm, 10, stdin);
    confirm[strcspn(confirm, "\n")] = 0;
    
    if (strcmp(confirm, "yes") == 0) {
        total_entries = 0;
        save_history();
        printf("\nAll history cleared!\n");
    } else {
        printf("\nOperation cancelled.\n");
    }
}

void view_statistics() {
    if (total_entries == 0) {
        printf("\nNo history data available for statistics.\n");
        return;
    }
    
    int total_visits = 0;
    for (int i = 0; i < total_entries; i++) {
        total_visits += history[i].visit_count;
    }
    
    printf("\n");
    printf("=======================================================================\n");
    printf("                      BROWSING STATISTICS                             \n");
    printf("=======================================================================\n");
    printf("Total unique URLs: %d\n", total_entries);
    printf("Total visits: %d\n", total_visits);
    printf("Average visits per URL: %.2f\n", (float)total_visits / total_entries);
    printf("=======================================================================\n");
}
