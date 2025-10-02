#include <stdio.h>
#include <string.h>

int menu();
void add_no_student();//รับข้อมูล
void display_all_records();//เเสดงข้อมูลการลงทะเบียนทั้งหมด
void display_menu();
void load_data(); // โหลดข้อมูลจากcsv
void save_data(); // เซฟข้อมูล
void delete_by_name();
void update_by_name();
int check(char data_to_check[], int check_type);//เช็ค
int record_count = 0;
char all_studentNames[100][50];
char all_studentIDs[100][10];
char Registration_data[100][10];
char all_courseCodes[100][20];
char all_statuses[100][20];
//เพิ่มข้อมูลผู้เรียน
//ค้นหาข้อมูลจาก ชื่อหรือรหัสนักศึกษา
//ลบข้อมูลผู้เรียน
//เเก้ไขข้อมูลผู้เรียน
//เเสดงข้อมูลทั้งหมด
const int CHECK_ID = 1;
const int CHECK_NAME = 2;
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
            /* code */
            break;
        case 3:
            update_by_name();
            break;
        case 4:
            delete_by_name();
            break;
        case 5:
            display_all_records();
            break;
        case 6:
            save_data();
            printf("ออกจากโปรเเกรมเเล้วเด่อ\n");
            
            break;

        default:
            printf("ไม่ตรงตัวเลือก");
            break;
        }
        if (choice == 6)
        {
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
    printf("[3] อัปเดตสถานะ \n");
    printf("[4] ลบข้อมูลผู้เรียน\n");
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
    char temp_id[10];//ใช้เพื่อชั่วคราวในการcheck
    char temp_name[50];//ใช้เพื่อชั่วคราวในการcheck
    printf("\n-----เพิ่มข้อมูลการลงทะเบียนใหม่----\n");
    printf("ชื่อผู้เรียน :");
    // scanf(" %[^\n]", all_studentNames[record_count]);
    scanf(" %[^\n]", temp_name);
    if (check(temp_name, CHECK_NAME)) { 
        printf("!! : ชื่อ '%s' นี้มีอยู่แล้วในระบบ !!\n", temp_name);
        return; 
    }
    printf("ป้อนรหัสนักศึกษา:");
    //scanf(" %[^\n]", all_studentIDs[record_count]);
    scanf(" %[^\n]", temp_id);
    if (check(temp_id, CHECK_ID)) { 
        printf("!!: รหัสผู้เรียน '%s' นี้มีอยู่แล้วในระบบ !!\n", temp_id);
        return; 
    }
    strcpy(all_studentIDs[record_count], temp_id);  //  ตรวจรหัสชื่อซ้ำไหม
    strcpy(all_studentNames[record_count], temp_name);
    printf("ป้อนรหัสวิชา:");
    scanf(" %[^\n]", all_courseCodes[record_count]);
    printf("ป้อนวันที่ลงทะเบียน:");
    scanf(" %[^\n]", Registration_data[record_count]);
    printf("ป้อนสถานะการเรียน \"ลงทะเบียนเรียนเเล้ว(กำลังเรียนอยู๋)\",\"เรียนจบเเล้ว เเละผ่านรายวิชานี้\",\"ถอนรายวิชา\",\"อยู่ระหว่างการเรียน(ยังไม่จบเทอม)\",\"ติดf\"");
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
        token = strtok(NULL, ","); if (token != NULL) strcpy(Registration_data[record_count], token);
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
    fprintf(proguy, "StudentName,studentIDs,CourseCode,RegistrationDate,EnrollmentStatus\n");
    for (int i = 0; i < record_count; i++)
    {
        fprintf(proguy, "%s,%s,%s,%s,%s\n", all_studentNames[i], all_studentIDs[i], all_courseCodes[i],Registration_data[i],all_statuses[i]);
    }
    fclose(proguy);
    printf("บันทึกข้อมูลลงไฟล์เรียบร้อย!\n");
}
void display_all_records()//เเสดงข้อมูลทั้งหมด
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
        printf("ชื่อ:%s\n",all_studentNames[i]);
        printf("รหัสประจำตัวผู้เรียน:%s\n",all_studentIDs[i]);
        printf("รหัสวิชา:%s\n",all_courseCodes[i]);
        printf("วันที่ลงทะเบียน YYYY-MM-DD:%s\n",Registration_data[i]);
        printf("สถานะ:%s\n",all_statuses[i]);
        printf("\"==============+==+================\"");
    }
    
}
int check(char data_to_check[], int check_type)
 {
    for (int i = 0; i < record_count; i++) {
        if (check_type == CHECK_ID) {
        
            if (strcmp(all_studentIDs[i], data_to_check) == 0) {
                return 1; 
            }
        }
        else if (check_type == CHECK_NAME) {
            if (strcmp(all_studentNames[i], data_to_check) == 0) {
                return -1; 
            }
        }
    }
    return 0; 
}
int find_record_index_by_name(char search_name[]){
    for (int i = 0; i < record_count; i++){
        if(strcmp(all_studentNames[i],search_name) == 0){
            return i ; //เจอindex
        }
    }
    return -1; //ไม่เจอ
}
void delete_by_name()//ลบข้อมูลโดยชื่อ
{
    char search_name[50];
    int index_to_delete;
    printf("\n-----ลบข้อมูลตามชื่อ-----\n");
    printf("\n-----ป้อนชื่อผู้เรียนที่ต้องการลบ----\n");
    scanf(" %[^\n]", search_name);
    index_to_delete = find_record_index_by_name(search_name);
    if (index_to_delete == -1)
    {
        printf("!!ไม่พบชื่อ%s ในระบบ\n",search_name);
        return ;
    }
    for( int i = index_to_delete; i < record_count - 1; i++ ){
        strcpy(all_studentNames[i],all_studentNames[ i + 1 ]);
        strcpy(all_studentIDs[i],all_studentIDs[ i + 1 ]);
        strcpy(all_courseCodes[i],all_courseCodes[ i + 1]);
        strcpy(all_statuses[i],all_statuses[ i + 1]);
        strcpy(Registration_data[i],Registration_data[ i + 1]);
    }
    record_count -- ;
    printf("ลบข้อมูลของ'%s'สำเร็จเเล้วอ้าย",search_name);
}
int index_by_name(char search_name[]) {
    for (int i = 0; i < record_count; i++) {
        if (strcmp(all_studentNames[i], search_name) == 0) {
            return i; 
        }
    }
    return -1; 
}
void update_by_name()//อัปเดตสถานะ
{   
    char new_id[15], new_course[20], new_status[20];
    char search_name[50];
    printf("\n-----อัปเดทข้อมูลนักศึกษา----\n");
    printf("ป้อนชื่อที่ต้องการเเก้ไข :");
    scanf(" %[^\n]",search_name);
    int index_to_update = index_by_name(search_name);
    if (index_to_update == -1) {
    printf("!! ไม่พบชื่อ '%s' ในระบบ !!\n", search_name);
    return;
}
    printf("\n-- กำลังแก้ไขข้อมูลของ: %s --\n", all_studentNames[index_to_update]);
    printf("สถานะใหม่ (ของเดิม: %s): ", all_statuses[index_to_update]);
    scanf(" %[^\n]", new_status);
    if (strlen(new_status) > 0) {
        strcpy(all_statuses[index_to_update], new_status);
    }

    printf("\n✅ อัปเดตข้อมูลของ '%s' เรียบร้อยแล้ว!\n", search_name);
}
    //printf("(หากไม่ต้องการแก้ไขส่วนไหน ให้กด Enter ข้ามไป)\n");// คืนค่าเป็น0
    //printf("รหัสนักศึกษาใหม่ (ของเดิม: %s): ", all_studentIDs[index_to_update]);
    // scanf(" %[^\n]", new_id);
    // if (strlen(new_id) > 0) {  //นับว่าผู้ใช้ป้อนค่าเข้ามาจริงๆจะได้ไม่เขียนทับ
    //     strcpy(all_studentIDs[index_to_update], new_id);
    // }
    // printf("รหัสวิชาใหม่ (ของเดิม: %s): ", all_courseCodes[index_to_update]);
    // scanf(" %[^\n]", new_course);
    // if (strlen(new_course) > 0) {
    //     strcpy(all_courseCodes[index_to_update], new_course);
    // }