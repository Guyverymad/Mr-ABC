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
//เพิ่มข้อมูลผู้เรียน
//ค้นหาข้อมูลจาก ชื่อหรือรหัสนักศึกษา
//ลบข้อมูลผู้เรียน
//เเก้ไขข้อมูลผู้เรียน
//เเสดงข้อมูลทั้งหมด

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
            display_all_records();
            break;
        case 6:
            save_data();
            printf("ออกจากโปรเเกรมเเล้วเด่อ\n");
            break;

        default:
            printf("a");
            break;
        }
       
    }
    return 0;
}
int menu()
{
    //int result = 0;
   int choice;
    printf("\n-----ระบบการลงทะเบียนเรียนออนไลน์-----\n");
    printf("[1] เพิ่มข้อมูลการลงทะเบียนเรียน\n");
    printf("[2] ค้นหาข้อมูล (จะเกิดขึ้นเร็วๆนี้)\n");
    printf("[3] อัปเดตสถานะ (จะเกิดเร็วๆนี้)\n");
    printf("[4] ลบข้อมูล (จะเกิดขึ้นเร็วๆนี้)\n");
    printf("[5] แสดงข้อมูลทั้งหมด\n");   
    printf("[6] บันทึกและออกจากโปรแกรม\n"); 
    printf("-------------------------------------\n");
    printf("เลือก: ");
    scanf("%d", &choice);
    getchar(); 
    return choice;
}
void add_no_student()
{

    printf("\n-----เพิ่มข้อมูลการลงทะเบียนใหม่----\n");
    printf("ชื่อผู้ใช้_");
    scanf(" %[^\n]", all_studentNames[record_count]);
    printf("ป้อนรหัสนักศึกษา:");
    scanf(" %[^\n]", all_studentIDs[record_count]);
    printf("ป้อนรหัสวิชา:");
    scanf(" %[^\n]", all_courseCodes[record_count]);
    printf("ป้อนสถานะโสด or มีเเฟน");
    scanf(" %[^\n]", all_statuses[record_count]);
    record_count ++ ;
    printf("\nเพิ่มข้อมูลเสร็จเเล่วเด้อ\n");
    
}
//โหลดข้อมูลจากไฟล์CSV
void load_data()
{
    FILE *file = fopen("Natthawut.csv", "r");
    if (file == NULL)
    {
        printf("ไม่สามารถเปิดไฟล์ได้\n");
    }
    char line[256];
    fgets(line, sizeof(line), file); 
    while (fgets(line, sizeof(line), file) != NULL && record_count < 257) {
        line[strcspn(line, "\n")] = 0;

        char *token;
        token = strtok(line, ","); if (token != NULL) strcpy(all_studentNames[record_count], token);

        token = strtok(NULL, ","); if (token != NULL) strcpy(all_studentIDs[record_count], token);

        token = strtok(NULL, ","); if (token != NULL) strcpy(all_courseCodes[record_count], token);

        token = strtok(NULL, ","); if (token != NULL) strcpy(all_statuses[record_count], token);

        record_count++; 
    }

    fclose(file);
    printf("โหลดข้อมูลสำเร็จ %d รายการ\n", record_count);
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
void display_all_records()
{
    printf("\'------เเสดงข้อมูลทั้งหมดครับ-----\'");
    if (record_count == 0)
    {
        printf("ยังไม่มีข้อมูลเด้ออ้่าย\n");
        return;
    }
   for (int i = 0; i < record_count; i++)
    {
        printf("รายการที่%d:\n",i + 1 );
        printf("ชื่อ%s:\n",all_studentNames[i]);
        printf("รหัสนักศึกษา%s:\n",all_studentIDs[i]);
        printf("รหัสวิชา%s:\n",all_courseCodes[i]);
        printf("สถานะ%s:\n",all_statuses[i]);
        printf("\"==============+==+================\"");
    }
    
}
