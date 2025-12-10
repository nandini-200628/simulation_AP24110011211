#include <stdio.h>
#include <string.h>

#define MAX 100
#define FILE_NAME "library.txt"

struct Book {
    int id;
    char title[100];
    int issued;   // 1 = issued, 0 = available
};

struct Book library[MAX];
int countBooks = 0;

// ------------------------ Save Books to File ------------------------
void saveToFile() {
    FILE *fp = fopen(FILE_NAME, "w");
    if(!fp) {
        printf("Error saving file!\n");
        return;
    }

    for(int i = 0; i < countBooks; i++) {
        fprintf(fp, "%d;%s;%d\n",
                library[i].id,
                library[i].title,
                library[i].issued);
    }

    fclose(fp);
}

// ------------------------ Load Books from File ------------------------
void loadFromFile() {
    FILE *fp = fopen(FILE_NAME, "r");
    if(!fp) return;  // file not created yet

    while(fscanf(fp, "%d;%[^;];%d\n",
                 &library[countBooks].id,
                 library[countBooks].title,
                 &library[countBooks].issued) == 3)
    {
        countBooks++;
    }

    fclose(fp);
}

// ------------------------ Add Book ------------------------
void addBook() {
    if(countBooks >= MAX) {
        printf("Library is full!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &library[countBooks].id);

    getchar();
    printf("Enter Book Title: ");
    fgets(library[countBooks].title, 100, stdin);
    library[countBooks].title[strcspn(library[countBooks].title, "\n")] = '\0';

    library[countBooks].issued = 0;
    countBooks++;

    saveToFile();
    printf("Book Added Successfully!\n");
}

// ------------------------ Search Book by ID ------------------------
struct Book* searchBook(int id) {
    for(int i = 0; i < countBooks; i++) {
        if(library[i].id == id)
            return &library[i];
    }
    return NULL;
}

// ------------------------ Delete Book ------------------------
void deleteBook() {
    int id;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    for(int i = 0; i < countBooks; i++) {
        if(library[i].id == id) {

            for(int j = i; j < countBooks - 1; j++)
                library[j] = library[j + 1];

            countBooks--;
            saveToFile();
            printf("Book Deleted Successfully!\n");
            return;
        }
    }

    printf("Book Not Found!\n");
}

// ------------------------ Update Book Title ------------------------
void updateBookTitle() {
    int id;
    printf("Enter Book ID to update title: ");
    scanf("%d", &id);

    struct Book* b = searchBook(id);
    if(!b) {
        printf("Book Not Found!\n");
        return;
    }

    getchar();
    printf("Enter New Title: ");
    fgets(b->title, 100, stdin);
    b->title[strcspn(b->title, "\n")] = '\0';

    saveToFile();
    printf("Book Title Updated!\n");
}

// ------------------------ Issue Book ------------------------
void issueBook() {
    int id;
    printf("Enter Book ID to Issue: ");
    scanf("%d", &id);

    struct Book* b = searchBook(id);
    if(!b) {
        printf("Book Not Found!\n");
        return;
    }

    if(b->issued)
        printf("Book Already Issued!\n");
    else {
        b->issued = 1;
        saveToFile();
        printf("Book Issued Successfully!\n");
    }
}

// ------------------------ Return Book ------------------------
void returnBook() {
    int id;
    printf("Enter Book ID to Return: ");
    scanf("%d", &id);

    struct Book* b = searchBook(id);
    if(!b) {
        printf("Book Not Found!\n");
        return;
    }

    if(!b->issued)
        printf("Book was not issued!\n");
    else {
        b->issued = 0;
        saveToFile();
        printf("Book Returned Successfully!\n");
    }
}

// ------------------------ Count Issued Books ------------------------
void countIssuedBooks() {
    int issuedCount = 0;

    for(int i = 0; i < countBooks; i++)
        if(library[i].issued)
            issuedCount++;

    printf("Total Books Issued: %d\n", issuedCount);
}

// ------------------------ Display All Books ------------------------
void displayBooks() {
    if(countBooks == 0) {
        printf("No Books in Library!\n");
        return;
    }

    printf("\n----- Library Books -----\n");
    for(int i = 0; i < countBooks; i++) {
        printf("ID: %d | Title: %s | Status: %s\n",
               library[i].id,
               library[i].title,
               library[i].issued ? "Issued" : "Available");
    }
}

// ------------------------ Display Available Books Only ------------------------
void displayAvailableBooks() {
    int found = 0;

    printf("\n----- Available Books -----\n");

    for(int i = 0; i < countBooks; i++) {
        if(library[i].issued == 0) {
            printf("ID: %d | Title: %s | Status: Available\n",
                   library[i].id,
                   library[i].title);
            found = 1;
        }
    }

    if(!found)
        printf("No Available Books!\n");
}

// ------------------------ Main Menu ------------------------
int main() {
    loadFromFile();  // Load books when program starts

    int choice;

    while(1) {
        printf("\n===== Library Management System =====\n");
        printf("1. Add Book\n");
        printf("2. Delete Book\n");
        printf("3. Update Book Title\n");
        printf("4. Search Book by ID\n");
        printf("5. Issue Book\n");
        printf("6. Return Book\n");
        printf("7. Count Issued Books\n");
        printf("8. Display All Books\n");
        printf("9. Display Available Books Only\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addBook(); break;
            case 2: deleteBook(); break;
            case 3: updateBookTitle(); break;
            case 4: {
                int id;
                printf("Enter ID: ");
                scanf("%d", &id);
                struct Book* b = searchBook(id);
                if(b)
                    printf("Book Found: %s\n", b->title);
                else
                    printf("Book Not Found!\n");
                break;
            }
            case 5: issueBook(); break;
            case 6: returnBook(); break;
            case 7: countIssuedBooks(); break;
            case 8: displayBooks(); break;
            case 9: displayAvailableBooks(); break;
            case 10: return 0;
            default: printf("Invalid Choice!\n");
        }
    }
}
