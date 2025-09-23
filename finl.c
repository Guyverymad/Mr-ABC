#include <stdio.h>
#include <string.h>

int menu();
void add_no_student();
void display_all_records();
void display_menu();
void load_data(); // โหลดข้อมูลจากcsv
void save_data(); // 
int record_count = 0;

char all_studentNames[100][50];
char all_studentIDs[100][10];
char all_courseCodes[100][20];
char all_statuses[100][20];

int main()
{
    load_data();

    while (1)
    {
        int choice = menu();
        
        switch (choice)
        {
        case 1:
            add_no_student();
            break;
        case 2:
            // display_all_records();
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            save_data();
            break;

        default:
            printf("a");
            break;
        }
        if (choice == 6)
        {
            // save_data();
            printf("ออกจากโปรเเกรม\n");
            break;
        }
    }
    return 0;
}
int menu()
{
    int result = 0;
    int choice;
    printf("-----ระบบการลงทะเบียนเรียนออนไลน์-----\n");
    printf("1.เพิ่มข้อมูลการลงทะเบียนเรียน\n");
    printf("2.ค้นหาข้อมูลจากการลงทะเบียนเรียน\n");
    printf("3.การอัพเดดสถาณะการเรียน\n");
    printf("4.ลบข้อมูลการลงทะเบียนเรียน\n");
    printf("5.เมนูทางเลือก\n");
    result = scanf("%d", &choice);
    getchar();
    return choice;
}
void add_no_student()
{

    printf("\n-----เพิ่มข้อมูลการลงทะเบียนใหม่----\n");
    printf("ชื่อผู้ใช้_");
    scanf(" %[^\n]", all_studentNames[add]);
    printf("ป้อนรหัสนักศึกษา:");
    scanf(" %[^\n]", all_studentIDs[add]);
    printf("ป้อนรหัสวิชา:");
    scanf(" %[^\n]", all_courseCodes[add]);
    printf("ป้อนสถานะโสด or มีเเฟน");
    scanf(" %[^\n]", all_statuses[add]);
    printf("\nเพิ่มข้อมูลเสร็จเเล่วเด้อ\n");

    record_count++;
}
void load_data()
{
    FILE *file = fopen("Natthawut.csv", "r");
    if (file == NULL)
    {
        printf("ไม่สามารถเปิดไฟล์ได้\n");
    }
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("อ่านบรรทัด%s\n", line);
    }
}


void save_data()
{
    FILE *proguy = fopen("Natthawut.csv", "w");
    if (proguy == NULL)
    {
        printf("ไม่สามารถเปิดไฟล์ได้");
        return;
    }
    fprintf(proguy, "StudentName,CourseCode,RegistrationDate,EnrollmentStatus\n");
    for (int i = 0; i < record_count; i++)
    {
        fprintf(proguy, "%s,%s,%s,%s\n", all_studentNames[i], all_studentIDs[i], all_courseCodes[i], all_statuses[i]);
    }
    fclose(proguy);
    printf("บันทึกข้อมูลลงไฟล์เรียบร้อย!\n");
}

