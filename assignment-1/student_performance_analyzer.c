/**
 * Assignment: Student Performance Analyzer
 * File Name: student_performance_analyzer.c
 * Author: Abhishree Gahlot
 * Branch: assignment1/student-performance-analyzer
 *
 * Description:
 *   This program is a console-based Student Performance Analyzer that:
 *     - Accepts student details (roll number, name, marks in 3 subjects)
 *     - Validates input (roll number uniqueness, valid name, marks between 0-100)
 *     - Calculates total marks, average, and assigns grades
 *     - Displays a performance pattern using stars based on the grade
 *     - Includes recursive function to print all student roll numbers
 * 
 * Input Format:
 *   First line: N (number of students)
 *   Next N lines: Roll_Number Name Marks1 Marks2 Marks3
 *
 * Output:
 *   Student performance report showing Roll No, Name, Total, Average, Grade, and Performance Pattern
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


typedef struct StudentInfo
{
    int rollNo; 
    char studentName[50];
    float studentMarks[3];
} StudentInfo;

void getStudentInfo(StudentInfo *student , int numberOfStudents);
void displayStudentInfo(const StudentInfo *student, int numberOfStudents);

bool isRollNumberValid(const StudentInfo *student , int numberOfStudents , int checkRollNo); 
bool isEmptyOrSpaces(char *str);
bool isStudentNameValid(const StudentInfo *student , int numberOfStudents ,  char studentName[50] );
bool isMarksValid(const StudentInfo *student , int numberOfStudents , float marks_1 , float marks_2 ,float marks_3);

float calculateTotalMarks(const StudentInfo *student);
float calculateAverageMarks(const StudentInfo *student);
char calculateStudentGrade(const StudentInfo *student);
void calculatePerformance(const StudentInfo *student);
void printAllStudentsRoll(const StudentInfo *student , int numberOfStudents, int currentIndex  );
void printAllStudentsRollWrapper(const StudentInfo *student, int numberOfStudents);


int main ()
{
    printf("Welcome to Students Performance Analyser\n");
    int numberOfStudents;
    printf("Enter number of students: ");
    scanf("%d", &numberOfStudents);
    getchar();

    if (numberOfStudents > 100)
    {
        printf("Number of students should be less than or equal to 100.\n");
        return 1; //exit
    }
    StudentInfo *students = malloc ( numberOfStudents * sizeof( StudentInfo ));
    if (students == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    getStudentInfo( students , numberOfStudents);

    displayStudentInfo( students , numberOfStudents);

    free(students); 
    return 0;
}


void getStudentInfo(StudentInfo *student, int numberOfStudents)
{
    char studentInput[100];
    printf(" \nEnter the details of the Students");
    printf(" \nEnter in format Roll_Number1 Name Marks1 Marks2 Marks3:\n");

    for ( int studentCount = 0; studentCount < numberOfStudents; studentCount++ )
    {
        while(1)
        {
            printf("\n Student %d : ", studentCount + 1);
            fgets(studentInput , sizeof( studentInput ) , stdin );
            studentInput[strcspn(studentInput, "\n")] = '\0';

            //validation for roll number
            if(isEmptyOrSpaces(studentInput))
            {
                printf("\nStudent Details cannot be empty! Please re-enter.");
                continue; //reprompt
            }

            int rollNo;
            char studentName[50];
            float marks1, marks2, marks3;
            char extra[10];

            int scanned = sscanf(studentInput , "%d %s %f %f %f %s" ,
                                 &rollNo , studentName , &marks1 , &marks2 , &marks3 , extra);

            // check if all 5 values were successfully read
            if (scanned != 5) {
                printf("Invalid input! Please enter RollNo Name Marks1 Marks2 Marks3\n");
                continue; // reprompt the user
            }

            // Validate roll number 
            if (!isRollNumberValid(student, studentCount, rollNo)) {
                printf("Roll number %d already exists or it is not valid value! Please re-enter.\n"
                    , rollNo);
                continue;
            }
            
            //  validate name of student 
            if ( !isStudentNameValid(student , studentCount , studentName))
            {
                printf(" Student Name is not valid ! Please re-enter . \n");
                continue;
            }

            // validate for students marks 
            if( !isMarksValid(student , studentCount , marks1 , marks2 , marks3 ))
            {
                printf(" Student Marks should be between 0 - 100  ! Please re-enter . \n");
                continue;
            }
            //valid input assign values
            student[studentCount].rollNo = rollNo;
            strcpy(student[studentCount].studentName, studentName);
            student[studentCount].studentMarks[0]= marks1;
            student[studentCount].studentMarks[1]= marks2;
            student[studentCount].studentMarks[2]= marks3;

            break;
        }
      
    }
}   


void displayStudentInfo(const StudentInfo *student, int numberOfStudents)
{
    printf("\n===== Students Performance Report =====\n");
    

    for (int studentCount = 0; studentCount < numberOfStudents; studentCount++)
    {
        printf("\nRoll : %d",student[studentCount].rollNo);
        printf("\nName : %s",student[studentCount].studentName);
        float totalMarks = calculateTotalMarks(&student[studentCount]);
        printf("\nTotal : %.2f",totalMarks);
        float averageMarks = calculateAverageMarks(&student[studentCount]);
        printf("\nAverage : %.2f",averageMarks);
        char grade = calculateStudentGrade(&student[studentCount]);
        printf("\nGrade : %c", grade);
        calculatePerformance(&student[studentCount]);
    }
    printf("\nRoll Numbers : ");
    printAllStudentsRollWrapper(student, numberOfStudents);
}


bool isRollNumberValid(const StudentInfo *student , int numberOfStudents , int checkRollNo)
{
    if(checkRollNo <= 0 )
    {
        return false;
    }
    for ( int studentCount =0 ; studentCount < numberOfStudents ; studentCount++)
    {
        if(checkRollNo == student[studentCount].rollNo)
        {
            return false;
        }
    }
    return true;
}


bool isEmptyOrSpaces(char *inputString)
{
    for( int charIndex = 0 ; inputString[charIndex] != '\0' ; charIndex++ )
    {
        if( !isspace(inputString[charIndex]))
        {
            return false ; // found non space characters
        }
    }
    return true; // only spaces or empty
}


bool isStudentNameValid(const StudentInfo *student , int numberOfStudents ,  char studentName[50] )
{
    int size = strlen(studentName);
    for ( int studentIndex =0 ; studentIndex <size ; studentIndex++ )
    {
        if ( !isalpha(studentName[studentIndex]))
        {
            return false;
        }
    }

    return true;
}


bool isMarksValid(const StudentInfo *student , int numberOfStudents ,  float marks_1 , float marks_2 , float marks_3)
{
    if (marks_1 >= 0 && marks_1 <= 100 &&
        marks_2 >= 0 && marks_2 <= 100 &&
        marks_3 >= 0 && marks_3 <= 100)
    {
        return true; // all marks valid
    }
    else
    {
        return false; // at least one mark invalid
    }
}



float calculateTotalMarks(const StudentInfo *student)
{
    return student->studentMarks[0] + student->studentMarks[1] + student->studentMarks[2];
}

float calculateAverageMarks(const StudentInfo *student)
{
    return calculateTotalMarks(student) / 3.0f;
}


char calculateStudentGrade(const StudentInfo *student)
{
    float averageMarks = calculateAverageMarks(student);

    if(averageMarks >= 85)
    {
        return 'A';
    }
    else if( averageMarks >= 70)
    {
        return 'B';
    }
    else if(averageMarks >= 50)
    {
        return 'C';
    }
    else if( averageMarks >= 35)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}


void calculatePerformance(const StudentInfo *student)
{
    int stars=0;
    char grade = calculateStudentGrade(student);
    switch( grade )
    {
        case 'A': 
            stars = 5; 
            break;
        case 'B':
            stars = 4; 
            break;
        case 'C': 
            stars = 3; 
            break;
        case 'D': 
            stars = 2; 
            break;
        case 'F': 
            stars = 0; 
            break;
    }

    if (stars > 0) {
        printf("\nPerformance: ");
        for (int starIndex = 0; starIndex < stars; starIndex++) 
        {
            printf("*");
        }
        printf("\n");
    }
}



void printAllStudentsRoll(const StudentInfo *student , int numberOfStudents, int currentIndex )
{
   // base case
    if(currentIndex>=numberOfStudents)
    {
        return; //exit 
    }

    printf("%d " , student[currentIndex].rollNo);

    printAllStudentsRoll( student , numberOfStudents , currentIndex+1 );
}


void printAllStudentsRollWrapper(const StudentInfo *student, int numberOfStudents)
{
    printAllStudentsRoll(student, numberOfStudents, 0);
}
