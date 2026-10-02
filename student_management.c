#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"
#define TEMP_FILE "temp.dat"
#define MAX_BATCH 100

struct Student
{
    int rollNo;
    char name[50];
    float marks;
};

/* Check whether a roll number already exists in the file */
int rollNumberExists(int rollNo)
{
    struct Student student;

    FILE *file = fopen(FILE_NAME, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&student, sizeof(struct Student), 1, file) == 1)
    {
        if (student.rollNo == rollNo)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

/* Check whether a roll number already exists in the current batch */
int rollNumberInBatch(int rollNumbers[], int count, int rollNo)
{
    for (int i = 0; i < count; i++)
    {
        if (rollNumbers[i] == rollNo)
        {
            return 1;
        }
    }

    return 0;
}

/* Add multiple students */
void addStudents()
{
    struct Student student;
    int count;
    int addedCount = 0;
    int rollNumbers[MAX_BATCH];

    printf("\nEnter number of students to add: ");
    scanf("%d", &count);

    if (count <= 0 || count > MAX_BATCH)
    {
        printf("\nInvalid number of students.\n");
        return;
    }

    FILE *file = fopen(FILE_NAME, "ab");

    if (file == NULL)
    {
        printf("\nUnable to open file.\n");
        return;
    }

    for (int i = 1; i <= count; i++)
    {
        printf("\n--- Student %d ---\n", i);

        while (1)
        {
            printf("Enter roll number: ");
            scanf("%d", &student.rollNo);

            if (rollNumberExists(student.rollNo))
            {
                printf("Roll number already exists. Please enter a different roll number.\n");
            }
            else if (rollNumberInBatch(rollNumbers, addedCount, student.rollNo))
            {
                printf("This roll number was already entered in this batch. Please enter a different roll number.\n");
            }
            else
            {
                break;
            }
        }

        printf("Enter name: ");
        scanf(" %49[^\n]", student.name);

        printf("Enter marks: ");
        scanf("%f", &student.marks);

        if (fwrite(&student, sizeof(struct Student), 1, file) == 1)
        {
            rollNumbers[addedCount] = student.rollNo;
            addedCount++;
        }
        else
        {
            printf("Error adding this student.\n");
        }
    }

    fclose(file);

    printf("\n%d student(s) added successfully.\n", addedCount);
}

/* Display all students */
void displayStudents()
{
    struct Student student;
    int count = 0;

    FILE *file = fopen(FILE_NAME, "rb");

    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\n========== STUDENT RECORDS ==========\n");

    while (fread(&student, sizeof(struct Student), 1, file) == 1)
    {
        printf("\nRoll Number : %d\n", student.rollNo);
        printf("Name        : %s\n", student.name);
        printf("Marks       : %.2f\n", student.marks);

        count++;
    }

    fclose(file);

    if (count == 0)
    {
        printf("\nNo student records found.\n");
    }
    else
    {
        printf("\nTotal students: %d\n", count);
    }
}

/* Search for one student */
void searchStudent()
{
    struct Student student;
    int rollNo;
    int found = 0;

    FILE *file = fopen(FILE_NAME, "rb");

    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\nEnter roll number to search: ");
    scanf("%d", &rollNo);

    while (fread(&student, sizeof(struct Student), 1, file) == 1)
    {
        if (student.rollNo == rollNo)
        {
            printf("\n========== STUDENT FOUND ==========\n");
            printf("Roll Number : %d\n", student.rollNo);
            printf("Name        : %s\n", student.name);
            printf("Marks       : %.2f\n", student.marks);

            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nStudent with roll number %d not found.\n", rollNo);
    }
}

/* Update multiple students */
void updateStudents()
{
    struct Student student;
    int count;
    int rollNo;
    int found;

    FILE *file = fopen(FILE_NAME, "rb+");

    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\nEnter number of students to update: ");
    scanf("%d", &count);

    if (count <= 0 || count > MAX_BATCH)
    {
        printf("\nInvalid number of students.\n");
        fclose(file);
        return;
    }

    for (int i = 1; i <= count; i++)
    {
        printf("\n--- Update Student %d ---\n", i);

        printf("Enter roll number to update: ");
        scanf("%d", &rollNo);

        found = 0;

        rewind(file);

        while (fread(&student, sizeof(struct Student), 1, file) == 1)
        {
            if (student.rollNo == rollNo)
            {
                printf("\nCurrent details:\n");
                printf("Name  : %s\n", student.name);
                printf("Marks : %.2f\n", student.marks);

                printf("\nEnter new name: ");
                scanf(" %49[^\n]", student.name);

                printf("Enter new marks: ");
                scanf("%f", &student.marks);

                fseek(file, -(long)sizeof(struct Student), SEEK_CUR);

                if (fwrite(&student, sizeof(struct Student), 1, file) == 1)
                {
                    printf("\nStudent updated successfully.\n");
                }
                else
                {
                    printf("\nError updating student.\n");
                }

                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("\nStudent with roll number %d not found.\n", rollNo);
        }
    }

    fclose(file);
}

/* Delete multiple students */
void deleteStudents()
{
    struct Student student;
    int count;
    int rollNo;
    int deletedCount = 0;
    int rollNumbers[MAX_BATCH];

    FILE *file = fopen(FILE_NAME, "rb");

    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\nEnter number of students to delete: ");
    scanf("%d", &count);

    if (count <= 0 || count > MAX_BATCH)
    {
        printf("\nInvalid number of students.\n");
        fclose(file);
        return;
    }

    /* Get unique roll numbers */
    for (int i = 0; i < count; i++)
    {
        while (1)
        {
            printf("Enter roll number %d: ", i + 1);
            scanf("%d", &rollNo);

            if (rollNumberInBatch(rollNumbers, i, rollNo))
            {
                printf("This roll number was already entered. Please enter a different roll number.\n");
            }
            else
            {
                rollNumbers[i] = rollNo;
                break;
            }
        }
    }

    FILE *tempFile = fopen(TEMP_FILE, "wb");

    if (tempFile == NULL)
    {
        printf("\nUnable to create temporary file.\n");
        fclose(file);
        return;
    }

    while (fread(&student, sizeof(struct Student), 1, file) == 1)
    {
        int deleteThisStudent = 0;

        for (int i = 0; i < count; i++)
        {
            if (student.rollNo == rollNumbers[i])
            {
                deleteThisStudent = 1;
                break;
            }
        }

        if (deleteThisStudent)
        {
            deletedCount++;
        }
        else
        {
            fwrite(&student, sizeof(struct Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove(FILE_NAME) != 0)
    {
        printf("\nError removing old student file.\n");
        remove(TEMP_FILE);
        return;
    }

    if (rename(TEMP_FILE, FILE_NAME) != 0)
    {
        printf("\nError updating student file.\n");
        return;
    }

    if (deletedCount == 0)
    {
        printf("\nNo matching students were found.\n");
    }
    else
    {
        printf("\n%d student(s) deleted successfully.\n", deletedCount);
    }
}

/* Main menu */
int main(void)
{
    int choice;

    do
    {
        printf("\n====================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Add Students\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Students\n");
        printf("5. Delete Students\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addStudents();
                break;

            case 2:
                displayStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudents();
                break;

            case 5:
                deleteStudents();
                break;

            case 6:
                printf("\nExiting program...\n");
                break;

            default:
                printf("\nInvalid choice. Please select 1 to 6.\n");
        }

    } while (choice != 6);

    return 0;
}
