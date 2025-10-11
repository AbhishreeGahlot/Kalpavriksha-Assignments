/**
 * Assignment: Student Performance Analyzer
 * File Name: student_performance_analyzer.c
 * Author: Abhishree Gahlor
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


/**
 * @struct StudentInfo
 * @brief Represents a student's basic information.
 * @details This  structure contains the information of a 
 *          Student and data required to generate the performance report.
 */
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
    getchar(); // consume the leftover '\n'
    
    StudentInfo *students = malloc ( numberOfStudents * sizeof( StudentInfo ));
    if (students == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    getStudentInfo( students , numberOfStudents);

    displayStudentInfo( students , numberOfStudents);

    free(students); // freeing the memory at the end of program
    return 0;
}


/**
 * @function getStudentInfo()
 * @brief This function represents taking input of all the information about the students
 * @remark this function takes input data from user while checking whether the data 
 *          given by user is valid or not
 */
void getStudentInfo(StudentInfo *student, int numberOfStudents)
{
    char buffer[100];
    printf(" \nEnter the details of the Students");
    printf(" \nEnter in format Roll_Number1 Name Marks1 Marks2 Marks3:\n");

    for ( int i = 0; i < numberOfStudents; i++ )
    {
        while(1)
        {
            printf("\n Student %d : ", i + 1);
            fgets(buffer , sizeof( buffer ) , stdin );
            buffer[strcspn(buffer, "\n")] = '\0';

            //validation for roll number
            if(isEmptyOrSpaces(buffer))
            {
                printf("\nStudent Details cannot be empty! Please re-enter.");
                continue; //reprompt
            }

            int rollNo;
            char studentName[50];
            float marks1, marks2, marks3;
            char extra[10];

            int scanned = sscanf(buffer , "%d %s %f %f %f %s" ,
                                 &rollNo , studentName , &marks1 , &marks2 , &marks3 , extra);

            // check if all 5 values were successfully read
            if (scanned != 5) {
                printf("Invalid input! Please enter RollNo Name Marks1 Marks2 Marks3\n");
                continue; // reprompt the user
            }

            // Validate roll number 
            if (!isRollNumberValid(student, i, rollNo)) {
                printf("Roll number %d already exists or it is not valid value! Please re-enter.\n"
                    , rollNo);
                continue;
            }
            
            //  validate name of student 
            if ( !isStudentNameValid(student , i , studentName))
            {
                printf(" Student Name is not valid ! Please re-enter . \n");
                continue;
            }

            // validate for students marks 
            if( !isMarksValid(student , i , marks1 , marks2 , marks3 ))
            {
                printf(" Student Marks should be between 0 - 100  ! Please re-enter . \n");
                continue;
            }
            //valid input assign values
            student[i].rollNo = rollNo;
            strcpy(student[i].studentName, studentName);
            student[i].studentMarks[0]= marks1;
            student[i].studentMarks[1]= marks2;
            student[i].studentMarks[2]= marks3;

            break;
        }
      
    }
}   

/**
 * @function displayStudentInfo()
 * @brief Displays all student details
 */
void displayStudentInfo(const StudentInfo *student, int numberOfStudents)
{
    printf("\n===== Students Performance Report =====\n");
    

    for (int i = 0; i < numberOfStudents; i++)
    {
        printf("\nRoll : %d",student[i].rollNo);
        printf("\nName : %s",student[i].studentName);
        float totalMarks = calculateTotalMarks(&student[i]);
        printf("\nTotal : %.2f",totalMarks);
        float averageMarks = calculateAverageMarks(&student[i]);
        printf("\nAverage : %.2f",averageMarks);
        char grade = calculateStudentGrade(&student[i]);
        printf("\nGrade : %c", grade);
        calculatePerformance(&student[i]);
    }
    printf("\nRoll Numbers : ");
    printAllStudentsRollWrapper(student, numberOfStudents);
}


/**
 * @function isRollNumberValid()
 * @brief This function checks whether the roll number is valid or not 
 * @return true if roll number is valid else false
 */
bool isRollNumberValid(const StudentInfo *student , int numberOfStudents , int checkRollNo)
{
    if(checkRollNo <= 0 )
    {
        return false;
    }
    for ( int i=0 ; i< numberOfStudents ; i++)
    {
        if(checkRollNo == student[i].rollNo)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief : Helper function to check if a string is empty or just spaces
 */
bool isEmptyOrSpaces(char *str)
{
    for( int i = 0 ; str[i] != '\0' ; i++ )
    {
        if( !isspace(str[i]))
        {
            return false ; // found non space characters
        }
    }
    return true; // only spaces or empty
}

/**
 * @brief : This function checks whether the name typed is valid or not 
 * @return true if student's name is valid , else returns false
 */
bool isStudentNameValid(const StudentInfo *student , int numberOfStudents ,  char studentName[50] )
{
    int size = strlen(studentName);
    for ( int i=0 ; i<size ; i++ )
    {
        if ( !isalpha(studentName[i]))
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief : This function checks whether the student's marks are valid or not
 */
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


/**
 * @brief This function returns the total marks gained by a student 
 */
float calculateTotalMarks(const StudentInfo *student)
{
    return student->studentMarks[0] + student->studentMarks[1] + student->studentMarks[2];
}

/**
 * @brief This function returns the average score by a student 
 */
float calculateAverageMarks(const StudentInfo *student)
{
    return calculateTotalMarks(student) / 3.0f;
}

/**
 * @brief : This function assigns the grade for the score of student
 */
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

/**
 * @brief: prints THE Stars according to students performance on average marks
 */
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
        for (int j = 0; j < stars; j++) 
        {
            printf("*");
        }
        printf("\n");
    }
}


/**
 * @brief prints the roll number of all students
 * @description  : recursive helper function
 */
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

/**
 * @brief : Wrapper function for the recursive function 
 */
void printAllStudentsRollWrapper(const StudentInfo *student, int numberOfStudents)
{
    printAllStudentsRoll(student, numberOfStudents, 0);
}
