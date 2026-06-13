#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book
{
    int id;
    char title[100];
    char author[100];
    int quantity;
    struct Book *next;
};

struct Book *head = NULL;

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();
void saveBooks();
void loadBooks();
void sortBooks();

int main()
{
    loadBooks();

    int choice;

    while(1)
    {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Sort Books by ID\n");
        printf("8. Exit\n");

        printf("Enter Choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                issueBook();
                break;

            case 5:
                returnBook();
                break;

            case 6:
                deleteBook();
                break;
            case 7:
                sortBooks();
                break;
            case 8:
                exit(0);

            default:
                printf("Invalid Choice\n");
        }
    }

    return 0;
}

void addBook()
{
    struct Book *newBook;
    struct Book *temp;

    newBook = (struct Book *)malloc(sizeof(struct Book));

    printf("Enter Book ID: ");
    scanf("%d", &newBook->id);

    printf("Enter Book Title: ");
    scanf(" %[^\n]", newBook->title);

    printf("Enter Author Name: ");
    scanf(" %[^\n]", newBook->author);

    printf("Enter Quantity: ");
    scanf("%d", &newBook->quantity);

    newBook->next = NULL;

    if(head == NULL)
    {
        head = newBook;
    }
    else
    {
        temp = head;

        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newBook;
    }

    printf("\nBook Added Successfully!\n");
    saveBooks();
}

void displayBooks()
{
    struct Book *temp = head;

    if(head == NULL)
    {
        printf("\nNo Books Available!\n");
        return;
    }

    printf("\n===== BOOK LIST =====\n");

    while(temp != NULL)
    {
        printf("\nBook ID: %d", temp->id);
        printf("\nTitle: %s", temp->title);
        printf("\nAuthor: %s", temp->author);
        printf("\nQuantity: %d\n", temp->quantity);

        temp = temp->next;
    }
}

void searchBook()
{
    int searchId;
    struct Book *temp = head;

    if(head == NULL)
    {
        printf("\nNo Books Available!\n");
        return;
    }

    printf("Enter Book ID to Search: ");
    scanf("%d", &searchId);

    while(temp != NULL)
    {
        if(temp->id == searchId)
        {
            printf("\nBook Found!\n");
            printf("Book ID: %d\n", temp->id);
            printf("Title: %s\n", temp->title);
            printf("Author: %s\n", temp->author);
            printf("Quantity: %d\n", temp->quantity);

            return;
        }

        temp = temp->next;
    }

    printf("\nBook Not Found!\n");
}

void issueBook()
{
    int bookId;
    struct Book *temp = head;

    if(head == NULL)
    {
        printf("\nNo Books Available!\n");
        return;
    }

    printf("Enter Book ID to Issue: ");
    scanf("%d", &bookId);

    while(temp != NULL)
    {
        if(temp->id == bookId)
        {
            if(temp->quantity > 0)
            {
                temp->quantity--;
                saveBooks();
                printf("\nBook Issued Successfully!\n");
            }
            else
            {
                printf("\nBook Out of Stock!\n");
            }

            return;
        }

        temp = temp->next;
    }

    printf("\nBook Not Found!\n");
}

void returnBook()
{
    int bookId;
    struct Book *temp = head;

    if(head == NULL)
    {
        printf("\nNo Books Available!\n");
        return;
    }

    printf("Enter Book ID to Return: ");
    scanf("%d", &bookId);

    while(temp != NULL)
    {
        if(temp->id == bookId)
        {
            temp->quantity++;
            saveBooks();
            printf("\nBook Returned Successfully!\n");
            return;
        }

        temp = temp->next;
    }

    printf("\nBook Not Found!\n");
}

void deleteBook()
{
    int deleteId;

    struct Book *temp = head;
    struct Book *prev = NULL;

    if(head == NULL)
    {
        printf("\nNo Books Available!\n");
        return;
    }

    printf("Enter Book ID to Delete: ");
    scanf("%d", &deleteId);

    if(head->id == deleteId)
    {
        temp = head;
        head = head->next;
        free(temp);

        printf("\nBook Deleted Successfully!\n");
        saveBooks();
        return;
    }

    while(temp != NULL && temp->id != deleteId)
    {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL)
    {
        printf("\nBook Not Found!\n");
        return;
    }

    prev->next = temp->next;
    free(temp);

    printf("\nBook Deleted Successfully!\n");
    saveBooks();
}

void saveBooks()
{
    FILE *fp;
    struct Book *temp = head;

    fp = fopen("books.txt", "w");

    if(fp == NULL)
        return;

    while(temp != NULL)
    {
        fprintf(fp, "%d|%s|%s|%d\n",
                temp->id,
                temp->title,
                temp->author,
                temp->quantity);

        temp = temp->next;
    }

    fclose(fp);
}

void loadBooks()
{
    FILE *fp;

    fp = fopen("books.txt", "r");

    if(fp == NULL)
        return;

    while(1)
    {
        struct Book *newBook =
            (struct Book *)malloc(sizeof(struct Book));

        int result =
        fscanf(fp, "%d|%99[^|]|%99[^|]|%d\n",
               &newBook->id,
               newBook->title,
               newBook->author,
               &newBook->quantity);

        if(result != 4)
        {
            free(newBook);
            break;
        }

        newBook->next = NULL;

        if(head == NULL)
        {
            head = newBook;
        }
        else
        {
            struct Book *temp = head;

            while(temp->next != NULL)
                temp = temp->next;

            temp->next = newBook;
        }
    }

    fclose(fp);
}

void sortBooks()
{
    struct Book *i, *j;

    if(head == NULL)
    {
        printf("\nNo Books Available!\n");
        return;
    }

    for(i = head; i != NULL; i = i->next)
    {
        for(j = i->next; j != NULL; j = j->next)
        {
            if(i->id > j->id)
            {
                int tempId = i->id;
                i->id = j->id;
                j->id = tempId;

                char tempTitle[100];
                strcpy(tempTitle, i->title);
                strcpy(i->title, j->title);
                strcpy(j->title, tempTitle);

                char tempAuthor[100];
                strcpy(tempAuthor, i->author);
                strcpy(i->author, j->author);
                strcpy(j->author, tempAuthor);

                int tempQuantity = i->quantity;
                i->quantity = j->quantity;
                j->quantity = tempQuantity;
            }
        }
    }

    saveBooks();

    printf("\nBooks Sorted Successfully!\n");
}