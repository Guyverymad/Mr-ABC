#include <stdio.h>
#include <string.h>
#include <assert.h>
//tests
void runUnitTests();
void runIntegrationTests();
void run_all_tests();
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"

int menu();
void add_no_student();      // รับข้อมูล
void display_all_records(); // เเสดงข้อมูลการลงทะเบียนทั้งหมด
void display_menu();
void load_data(const char* filename ); // โหลดข้อมูลจากcsv
void save_data(const char* filename); // เซฟข้อมูล
void delete_by_name();
void update_by_name();
void search_records();
void display_record(int index);
void parse_csv_line(char *line, int index);
int check(char data_to_check[], int check_type); // เช็ค
int record_count = 0;
char all_studentNames[30000][50];
char all_studentIDs[30000][10];
char Registration_data[30000][20];
char all_courseCodes[30000][20];
char all_statuses[30000][20];
const int CHECK_ID = 1;
const int CHECK_NAME = 2;

int main()
{
    load_data("Natthawut.csv");

    while (1)
    {
        int choice = menu();

        switch (choice)
        {
        case 1:
            add_no_student();
            break;
        case 2:
            search_records();
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
            save_data("Natthawut.csv");
            break;
        case 7:
            printf("ออกจากโปรเเกรม \n");
            break;
       case 8: run_all_tests(); 
           break;
        default:
            printf("ไม่ตรงตัวเลือก");
            break;
        }

        if (choice == 7)
        {
            break;
        }
    }
    return 0;
}

int menu()
{
    int choice = 0;
    char buffer[100];

    printf("\n-----ระบบการลงทะเบียนเรียนออนไลน์-----\n");
    printf("[1] เพิ่มข้อมูลการลงทะเบียนเรียน\n");
    printf("[2] ค้นหาข้อมูล \n");
    printf("[3] อัปเดตสถานะ \n");
    printf("[4] ลบข้อมูลผู้เรียน\n");
    printf("[5] แสดงข้อมูลทั้งหมด\n");
    printf("[6] บันทึกข้อมูลลงไฟล์\n");
    printf("[7] ออกจากโปรแกรม\n");
    printf("[8] ** รันโปรแกรมทดสอบ **\n"); 
    printf("-------------------------------------\n");

    while (1)
    {
        printf("เลือก: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &choice) == 1 && choice >= 1 && choice <= 8) { 
                break;
            }
        }
        printf("!! ผิดพลาด: กรุณาป้อนเฉพาะตัวเลข 1-8 เท่านั้น !!\n");
    }
    return choice;
}

void add_no_student()
{
    char temp_id[100];
    char temp_name[100];

    printf("\n-----เพิ่มข้อมูลการลงทะเบียนใหม่----\n");
    printf("ชื่อผู้เรียน (สูงสุด49 9ตัวอักษร) :");
    fgets(temp_name, sizeof(temp_name), stdin);
    temp_name[strcspn(temp_name, "\n")] = 0;

    if (check(temp_name, CHECK_NAME) == 1)
    {
        printf("!! : ชื่อ-นามสกุล '%s' นี้มีอยู่แล้วในระบบ !!\n", temp_name);
        return;
    }

    printf("ป้อนรหัสประจำตัวผู้เรียน (สูงสุด 9 ตัวอักษร):");
    fgets(temp_id, sizeof(temp_id), stdin);
    temp_id[strcspn(temp_id, "\n")] = 0;

    if (check(temp_id, CHECK_ID))
    {
        printf("!!: รหัสผู้เรียน '%s' นี้มีอยู่แล้วในระบบ !!\n", temp_id);
        return;
    }

    strcpy(all_studentNames[record_count], temp_name);
    strcpy(all_studentIDs[record_count], temp_id);

    printf("ป้อนรหัสวิชา (สูงสุด 19 ตัวอักษร):");
    fgets(all_courseCodes[record_count], sizeof(all_courseCodes[record_count]), stdin);
    all_courseCodes[record_count][strcspn(all_courseCodes[record_count], "\n")] = 0;

    printf("ป้อนวันที่ลงทะเบียน (YYYY-MM-DD)(สูงสุด 19 ตัวอักษร):");
    fgets(Registration_data[record_count], sizeof(Registration_data[record_count]), stdin);
    Registration_data[record_count][strcspn(Registration_data[record_count], "\n")] = 0;

    printf("ป้อนสถานะการเรียน (สูงสุด 49 ตัวอักษร): ");
    printf("เช่น : Progress ,Academic Probation ,Completed,Failed,Dismissed,Resigned\n");
    fgets(all_statuses[record_count], sizeof(all_statuses[record_count]), stdin);
    all_statuses[record_count][strcspn(all_statuses[record_count], "\n")] = 0;

    record_count++;
    printf("\nเพิ่มข้อมูลเสร็จแล้ว\n");
    printf("กด [6] เพื่อบันทึกข้อมูลลงไฟล์ด้วยนะครับ\n");
}
// โหลดข้อมูลจากไฟล์CSV
void load_data(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {

        printf("ไม่พบไฟล์ %s, กำลังสร้างไฟล์ใหม่...\n",filename);
        file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("!! เกิดข้อผิดพลาด: ไม่สามารถสร้างไฟล์ได้ !!\n");
            save_data(filename);
            return;
        }
        fprintf(file, "StudentName,studentIDs,CourseCode,RegistrationDate,EnrollmentStatus\n");
        fclose(file);
        printf("สร้างไฟล์ Natthawut.csv ใหม่เรียบร้อยแล้ว\n");
        return;
    }

    char line[300];
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file) != NULL && record_count < 30000)
    {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) > 1)
        {
            parse_csv_line(line, record_count);
            record_count++;
        }
    }

    fclose(file);
    printf("โหลดข้อมูลสำเร็จ %d รายการ\n", record_count);
}

void save_data(const char* filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("!! ไม่สามารถเปิดไฟล์ได้: %s !!\n", filename);
        return;
    }
    fprintf(file, "StudentName,studentIDs,CourseCode,RegistrationDate,EnrollmentStatus\n");
    for (int i = 0; i < record_count; i++)
    {
        fprintf(file, "%s,%s,%s,%s,%s\n", all_studentNames[i], all_studentIDs[i], all_courseCodes[i], Registration_data[i], all_statuses[i]);
    }
    fclose(file);
    printf("บันทึกข้อมูลลงไฟล์เรียบร้อย!\n");
}

void display_all_records() // เเสดงข้อมูลทั้งหมด
{
    printf("\'------เเสดงข้อมูลทั้งหมดครับ-----\'");
    if (record_count == 0)
    {
        printf("ยังไม่มีข้อมูลเด้ออ้่าย\n");
        return;
    }
    for (int i = 0; i < record_count; i++)
    {
        printf("\nรายการที่%d:\n", i + 1);
        printf("ชื่อ:%s\n", all_studentNames[i]);
        printf("รหัสประจำตัวผู้เรียน:%s\n", all_studentIDs[i]);
        printf("รหัสวิชา:%s\n", all_courseCodes[i]);
        printf("วันที่ลงทะเบียน YYYY-MM-DD:%s\n", Registration_data[i]);
        printf("สถานะ:%s\n", all_statuses[i]);
        printf("\"==============+==+================\"");
    }
}

int check(char data_to_check[], int check_type)
{
    for (int i = 0; i < record_count; i++)
    {
        if (check_type == CHECK_ID)
        {

            if (strcmp(all_studentIDs[i], data_to_check) == 0)
            {
                return 1;
            }
        }
        else if (check_type == CHECK_NAME)
        {
            if (strcmp(all_studentNames[i], data_to_check) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int find_record_index_by_name(char search_name[])
{
    for (int i = 0; i < record_count; i++)
    {
        if (strcmp(all_studentNames[i], search_name) == 0)
        {
            return i; // เจอindex
        }
    }
    return -1; // ไม่เจอ
}

void delete_by_name()
{ // ลบข้อมูลโดยชื่อ
    char search_name[50];
    int index_to_delete;
    printf("\n-----ลบข้อมูลตามชื่อ-----\n");
    printf("ป้อนชื่อผู้เรียนที่ต้องการลบ: ");

    fgets(search_name, sizeof(search_name), stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    index_to_delete = find_record_index_by_name(search_name);
    if (index_to_delete == -1)
    {
        printf("!!ไม่พบชื่อ '%s' ในระบบ\n", search_name);
        return;
    }
    for (int i = index_to_delete; i < record_count - 1; i++)
    {
        strcpy(all_studentNames[i], all_studentNames[i + 1]);
        strcpy(all_studentIDs[i], all_studentIDs[i + 1]);
        strcpy(all_courseCodes[i], all_courseCodes[i + 1]);
        strcpy(all_statuses[i], all_statuses[i + 1]);
        strcpy(Registration_data[i], Registration_data[i + 1]);
    }
    record_count--;
    printf("ลบข้อมูลของ '%s' สำเร็จแล้ว\n", search_name);
}

int index_by_name(char search_name[])
{
    for (int i = 0; i < record_count; i++)
    {
        if (strcmp(all_studentNames[i], search_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void update_by_name()
{
    char search_name[100];
    printf("\n----- อัปเดตข้อมูลนักศึกษา ----\n");
    printf("ป้อนชื่อที่ต้องการแก้ไข: ");

    fgets(search_name, sizeof(search_name), stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    int index_to_update = index_by_name(search_name);
    if (index_to_update == -1)
    {
        printf("!! ไม่พบชื่อ '%s' ในระบบ !!\n", search_name);
        return;
    }

    int update_choice = 0;
    char new_data[100];
    char buffer[100];

    while (1)
    {
        printf("\n-- กำลังแก้ไขข้อมูลของ: %s --\n", all_studentNames[index_to_update]);
        display_record(index_to_update);
        printf("----------------------------------\n");

        printf("ต้องการแก้ไขส่วนไหน?\n");
        printf("  [1] แก้ไขชื่อ\n");
        printf("  [2] แก้ไขรหัสประจำตัวผู้เรียน\n");
        printf("  [3] แก้ไขรหัสวิชา\n");
        printf("  [4] แก้ไขวันที่ลงทะเบียน\n");
        printf("  [5] แก้ไขสถานะ\n");
        printf("  [6] บันทึกและกลับสู่เมนูหลัก\n");
     
        while (1) {
            printf("เลือก: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%d", &update_choice) == 1 && update_choice >= 1 && update_choice <= 6) 
                {break;}
            }
            printf("!! ผิดพลาด: กรุณาป้อนตัวเลข 1-6 เท่านั้น !!\n");
        }

        if (update_choice == 6) {
            printf("\n✅ อัปเดตข้อมูลเรียบร้อยแล้ว!\n");
            return; 
        }

        printf("(หากไม่ต้องการแก้ไข ให้กด Enter เพื่อข้าม)\n");
        fgets(new_data, sizeof(new_data), stdin);
        if (strlen(new_data) > 1) {
            new_data[strcspn(new_data, "\n")] = 0;

        switch (update_choice)
        {
        case 1:
            printf("ชื่อใหม่ (ของเดิม: %s): ", all_studentNames[index_to_update]);
            if (strlen(new_data) > 0)
            {
                if (check(new_data, CHECK_NAME) == 1)
                {
                    printf("!! ชื่อ '%s' นี้มีอยู่แล้วในระบบ ไม่สามารถใช้ได้ !!\n", new_data);
                }
                else
                {
                    strcpy(all_studentNames[index_to_update], new_data);
                    strcpy(search_name, new_data);
                    printf(" -> อัปเดตชื่อแล้ว\n");
                }
            }
            break;

        case 2:
            printf("รหัสประจำตัวผู้เรียนใหม่ (ของเดิม: %s): ", all_studentIDs[index_to_update]);
            fgets(new_data, sizeof(new_data), stdin);
            new_data[strcspn(new_data, "\n")] = 0;

            if (strlen(new_data) > 0)
            {
                if (check(new_data, CHECK_ID))
                {
                    printf("!! รหัสประจำตัวผู้เรียนใหม่'%s' นี้มีอยู่แล้วในระบบ ไม่สามารถใช้ได้ !!\n", new_data);
                }
                else
                {
                    strcpy(all_studentIDs[index_to_update], new_data);
                    printf(" -> อัปเดตรหัสประจำตัวผู้เรียนใหม่เเล้วเด่อ\n");
                }
            }
            break;

        case 3:
            printf("รหัสวิชาใหม่ (ของเดิม: %s): ", all_courseCodes[index_to_update]);
            fgets(new_data, sizeof(new_data), stdin);
            new_data[strcspn(new_data, "\n")] = 0;

            if (strlen(new_data) > 0)
            {
                strcpy(all_courseCodes[index_to_update], new_data);
                printf(" -> อัปเดตรหัสวิชาแล้ว\n");
            }
            break;

        case 4:
            printf("วันที่ลงทะเบียนใหม่ (ของเดิม: %s): ", Registration_data[index_to_update]);
            fgets(new_data, sizeof(new_data), stdin);
            new_data[strcspn(new_data, "\n")] = 0;

            if (strlen(new_data) > 0)
            {
                strcpy(Registration_data[index_to_update], new_data);
                printf(" -> อัปเดตวันที่ลงทะเบียนแล้ว\n");
            }
            break;

        case 5:
            printf("สถานะใหม่ (ของเดิม: %s): ", all_statuses[index_to_update]);
            fgets(new_data, sizeof(new_data), stdin);
            new_data[strcspn(new_data, "\n")] = 0;

            if (strlen(new_data) > 0)
            {
                strcpy(all_statuses[index_to_update], new_data);
                printf(" -> อัปเดตสถานะแล้ว\n");
            }
            break;
        default:
            printf("!! ตัวเลือกไม่ถูกต้อง กรุณาเลือก 1-6 !!\n");
            break;
        }
    }
}
}

void parse_csv_line(char *line, int index)
{
    char buffer[256];
    int buffer_pos = 0;
    int field_index = 0;

    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == ',')
        {
            buffer[buffer_pos] = '\0';

            if (field_index == 0)
                strcpy(all_studentNames[index], buffer);
            else if (field_index == 1)
                strcpy(all_studentIDs[index], buffer);
            else if (field_index == 2)
                strcpy(all_courseCodes[index], buffer);
            else if (field_index == 3)
                strcpy(Registration_data[index], buffer);

            field_index++;
            buffer_pos = 0;
        }
        else
        {
            buffer[buffer_pos++] = line[i];
        }
    }
    buffer[buffer_pos] = '\0';
    strcpy(all_statuses[index], buffer);
}

void search_records()
{
    int choice = 0;
    char buffer[100], search_term[100];
    int found_count = 0;

    if (record_count == 0)
    {
        printf("\n!! ยังไม่มีข้อมูลในระบบ !!\n");
        return;
    }

    printf("\n----- ค้นหาข้อมูล -----\n[1] ตามชื่อ\n[2] ตามรหัสนักศึกษา\nเลือก: ");

    if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &choice) != 1 || (choice != 1 && choice != 2))
    {
        printf("!! ตัวเลือกไม่ถูกต้อง !!\n");
        return;
    }

    printf("ป้อนคำที่ต้องการค้นหา: ");
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = 0;

    printf("\n...กำลังค้นหา '%s'...\n", search_term);
    for (int i = 0; i < record_count; i++)
    {
        if ((choice == 1 && strcmp(all_studentNames[i], search_term) == 0) ||
            (choice == 2 && strcmp(all_studentIDs[i], search_term) == 0))
        {
            display_record(i);
            found_count++;
        }
    }
    if (found_count == 0)
    {
        printf("\n!! ไม่พบข้อมูลที่ตรงกับ '%s' !!\n", search_term);
    }
}

void display_record(int index) {
    printf("----------------------------------------\n");
    printf("  รายการที่: %d\n", index + 1);
    printf("  ชื่อ: %s\n", all_studentNames[index]);
    printf("  รหัสประจำตัว: %s\n", all_studentIDs[index]);
    printf("  รหัสวิชา: %s\n", all_courseCodes[index]);
    printf("  วันที่ลงทะเบียน: %s\n", Registration_data[index]);
    printf("  สถานะ: %s\n", all_statuses[index]);
}
//
void runUnitTests() {
    printf(YELLOW "\n===== 🔍 เริ่มการทดสอบ Unit Test =====\n" RESET);

      
    printf(BLUE "Scenario:\n" RESET);
    printf("1️⃣  จำลองข้อมูลตั้งต้น (มี 'Alice')\n");
    printf("2️⃣  ทดสอบการ 'เพิ่ม' ข้อมูล 'Bob'\n");
    printf("3️⃣  ทดสอบการ 'ลบ' ข้อมูล 'Alice' ออก\n");
    printf(YELLOW "เริ่มการทดสอบ...\n" RESET);

    // Step 1: จำลองข้อมูลตั้งต้น
    record_count = 0;
    strcpy(all_studentNames[0], "Alice");
    strcpy(all_studentIDs[0], "111");
    strcpy(all_courseCodes[0], "CS101");
    strcpy(Registration_data[0], "2025-01-15");
    strcpy(all_statuses[0], "Completed");
    record_count = 1;
    printf("\n--- Step 1: Prepared initial data ('Alice') ---\n");
    if (record_count == 1) {
        printf(GREEN "✅ เตรียมข้อมูลสำเร็จ\n" RESET);
    } else {
        printf(RED "❌ เตรียมข้อมูลล้มเหลว\n" RESET);
    }

    // Step 2: ทดสอบการเพิ่มข้อมูล
    printf("\n--- Test 1: เพิ่มข้อมูล 'Bob' ---\n");
    strcpy(all_studentNames[record_count], "Bob");
    strcpy(all_studentIDs[record_count], "222");
    strcpy(all_courseCodes[record_count], "ENG102");
    strcpy(Registration_data[record_count], "2025-02-20");
    strcpy(all_statuses[record_count], "In Progress");
    record_count++;
    
    if (record_count == 2 && strcmp(all_studentNames[1], "Bob") == 0) {
        printf(GREEN "✅ Test 1 ผ่าน\n" RESET);
    } else {
        printf(RED "❌ Test 1 ล้มเหลว\n" RESET);
    }
    display_all_records();

    // Step 3: ทดสอบการลบข้อมูล
    printf("\n--- Test 2: ลบข้อมูล 'Alice' ---\n");
    int index_to_delete = find_record_index_by_name("Alice");
    if (index_to_delete != -1) {
        for (int i = index_to_delete; i < record_count - 1; i++) {
            strcpy(all_studentNames[i], all_studentNames[i + 1]);
            strcpy(all_studentIDs[i], all_studentIDs[i + 1]);
            strcpy(all_courseCodes[i], all_courseCodes[i + 1]);
            strcpy(Registration_data[i], Registration_data[i + 1]);
            strcpy(all_statuses[i], all_statuses[i + 1]);
        }
        record_count--;
    }

    if (record_count == 1 && strcmp(all_studentNames[0], "Bob") == 0) {
        printf(GREEN "✅ Test 2 ผ่าน\n" RESET);
    } else {
        printf(RED "❌ Test 2 ล้มเหลว\n" RESET);
    }
    display_all_records();

    printf(GREEN "\n✅ Unit Tests เสร็จสิ้น\n" RESET);
}

// ========== 🧩 ชุดทดสอบที่ 2: Integration Tests 🧩 ==========
void runIntegrationTests() {
    const char *test_file = "test_data_temp.csv";

      // แสดง Scenario คร่าว ๆ ก่อนเริ่ม
    printf(YELLOW "\n===== 🧩 Integration Test Scenario =====\n" RESET);
    printf(BLUE "Scenario:\n" RESET);
    printf("1️⃣  เตรียมข้อมูลนักเรียนใหม่ (Charlie)\n");
    printf("2️⃣  บันทึกข้อมูลลงไฟล์ %s\n", test_file);
    printf("3️⃣  ล้างข้อมูลใน memory และโหลดจากไฟล์\n");
    printf("4️⃣  ตรวจสอบข้อมูลที่โหลดว่าถูกต้องหรือไม่\n");
    printf("5️⃣  แสดงข้อมูลทั้งหมดหลังโหลด\n");
    printf(YELLOW "เริ่มการทดสอบ...\n" RESET);

    // --- Step 0: เตรียมข้อมูลจำลอง ---
    record_count = 0;
    strcpy(all_studentNames[0], "Charlie");
    strcpy(all_studentIDs[0], "333");
    strcpy(all_courseCodes[0], "PHY103");
    strcpy(Registration_data[0], "2025-03-10");
    strcpy(all_statuses[0], "Completed");
    record_count++;
    printf(BLUE "\nStep 1: เตรียมข้อมูลจำลอง\n" RESET);
    printf("Test Data: %s, %s, %s, %s, %s\n", all_studentNames[0], all_studentIDs[0], all_courseCodes[0], Registration_data[0], all_statuses[0]);

    // --- Step 1: Save ---
    printf(BLUE "\nStep 2: Save ข้อมูลลงไฟล์ %s\n" RESET, test_file);
    save_data(test_file);
    printf(GREEN "✅ Save สำเร็จ\n" RESET);

    // --- Step 2: Load ---
    record_count = 0;
    printf(BLUE "\nStep 3: Load ข้อมูลจากไฟล์ %s\n" RESET, test_file);
    load_data(test_file);
    printf(GREEN "✅ Load สำเร็จ\n" RESET);

    // --- Step 3: ตรวจสอบข้อมูล ---
    printf(BLUE "\nStep 4: ตรวจสอบข้อมูลที่โหลด\n" RESET);
    if (record_count == 1 && 
        strcmp(all_studentNames[0], "Charlie") == 0 &&
        strcmp(all_studentIDs[0], "333") == 0 &&
        strcmp(all_courseCodes[0], "PHY103") == 0 &&
        strcmp(Registration_data[0], "2025-03-10") == 0 &&
        strcmp(all_statuses[0], "Completed") == 0)
    {
        printf(GREEN "✅ Integration Test ผ่าน\n" RESET);
    } else {
        printf(RED "❌ Integration Test ล้มเหลว\n" RESET);
    }

    // --- Step 4: แสดงข้อมูลทั้งหมด ---
    printf(BLUE "\nStep 5: แสดงข้อมูลทั้งหมดหลังโหลด\n" RESET);
    display_all_records();
    
    
    remove(test_file);

    printf(GREEN "\n✅ Integration Tests เสร็จสิ้น\n" RESET);
}
// ========== 🚀 ชุดทดสอบที่ 3: End-to-End (E2E) Tests 🚀 ==========
void runE2ETests() {
    printf(YELLOW "\n===== 🚀 เริ่มการทดสอบแบบ End-to-End (E2E) =====\n" RESET);

    const char *test_file = "e2e_test_temp.csv";
    record_count = 0;

    // Step 1: เริ่มระบบ (โหลดไฟล์ - จะสร้างใหม่ถ้าไม่เจอ)
    printf(BLUE "\nStep 1: โหลดไฟล์ทดสอบ %s\n" RESET, test_file);
    load_data(test_file);

    // Step 2: เพิ่มข้อมูลใหม่จำลอง
    printf(BLUE "\nStep 2: เพิ่มข้อมูลนักเรียนใหม่ 'GUY5555'\n" RESET);
    strcpy(all_studentNames[record_count], "GUY5555");
    strcpy(all_studentIDs[record_count], "444");
    strcpy(all_courseCodes[record_count], "MATH104");
    strcpy(Registration_data[record_count], "2025-04-01");
    strcpy(all_statuses[record_count], "In Progress");
    record_count++;
    printf(GREEN "✅ เพิ่มข้อมูล 'GUY5555' สำเร็จ\n" RESET);

    // Step 3: บันทึกข้อมูลลงไฟล์
    printf(BLUE "\nStep 3: บันทึกข้อมูลลงไฟล์ %s\n" RESET, test_file);
    save_data(test_file);

    // Step 4: ตรวจสอบว่าข้อมูลถูกต้องในหน่วยความจำ
    int index = find_record_index_by_name("GUY5555");
    if (index != -1) {
        printf(GREEN "✅ พบข้อมูล 'GUY5555' ในหน่วยความจำ\n" RESET);
    } else {
        printf(RED "❌ ไม่พบข้อมูล 'GUY5555' หลังเพิ่ม\n" RESET);
    }

    // Step 5: อัปเดตสถานะ
    printf(BLUE "\nStep 5: อัปเดตสถานะเป็น 'Completed'\n" RESET);
    strcpy(all_statuses[index], "Completed");
    printf(GREEN "✅ อัปเดตสถานะสำเร็จ\n" RESET);

    // Step 6: บันทึกอีกครั้งหลังอัปเดต
    save_data(test_file);

    // Step 7: ล้างข้อมูลทั้งหมดในหน่วยความจำ
    printf(BLUE "\nStep 6: ล้างข้อมูลใน memory แล้วโหลดกลับจากไฟล์\n" RESET);
    record_count = 0;
    load_data(test_file);

    // Step 8: ตรวจสอบข้อมูลหลังโหลดกลับ
    printf(BLUE "\nStep 7: ตรวจสอบความถูกต้องของข้อมูลที่โหลด\n" RESET);
    if (record_count == 1 &&
        strcmp(all_studentNames[0], "GUY5555") == 0 &&
        strcmp(all_studentIDs[0], "444") == 0 &&
        strcmp(all_courseCodes[0], "MATH104") == 0 &&
        strcmp(Registration_data[0], "2025-04-01") == 0 &&
        strcmp(all_statuses[0], "Completed") == 0)
    {
        printf(GREEN "✅ ข้อมูลที่โหลดกลับมาตรงทั้งหมด\n" RESET);
    } else {
        printf(RED "❌ ข้อมูลไม่ตรงหลังโหลดไฟล์\n" RESET);
    }

    // Step 9: ลบข้อมูลออกจาก memory และไฟล์
    printf(BLUE "\nStep 8: ลบข้อมูล 'GUY5555' ออกจากระบบ\n" RESET);
    int index_to_delete = find_record_index_by_name("GUY5555");
    if (index_to_delete != -1) {
        for (int i = index_to_delete; i < record_count - 1; i++) {
            strcpy(all_studentNames[i], all_studentNames[i + 1]);
            strcpy(all_studentIDs[i], all_studentIDs[i + 1]);
            strcpy(all_courseCodes[i], all_courseCodes[i + 1]);
            strcpy(Registration_data[i], Registration_data[i + 1]);
            strcpy(all_statuses[i], all_statuses[i + 1]);
        }
        record_count--;
        printf(GREEN "✅ ลบข้อมูล 'GUY5555' สำเร็จ\n" RESET);
    } else {
        printf(RED "❌ ไม่พบข้อมูล 'GUY5555' เพื่อลบ\n" RESET);
    }

    // Step 10: เซฟอีกครั้งเพื่อให้ไฟล์ว่าง
    save_data(test_file);

    // Step 11: แสดงผลทั้งหมด
    printf(BLUE "\nStep 9: แสดงข้อมูลที่เหลือหลังลบ\n" RESET);
    display_all_records();

    // Step 12: ลบไฟล์ทดสอบ
    remove(test_file);

    printf(GREEN "\n🎯 E2E Tests เสร็จสิ้น — ระบบผ่านทุกขั้นตอนครบวงจร!\n" RESET);
}

void run_all_tests() {
    // สำรองข้อมูลเดิมไว้ก่อนรันทดสอบ
    int original_record_count = record_count;
    char temp_names[100][50], temp_ids[100][10];
    for(int i=0; i < original_record_count && i < 100; i++) {
        strcpy(temp_names[i], all_studentNames[i]);
        strcpy(temp_ids[i], all_studentIDs[i]);
    }

    printf("\n\n======= 🧪 STARTING ALL TESTS 🧪 =======\n");
    runUnitTests();
    runIntegrationTests();
    runE2ETests(); // ✅ เพิ่มฟังก์ชันทดสอบ End-to-End
    printf("\n\n======= ✅ ALL TESTS FINISHED ✅ =======\n\n");

    // กู้คืนข้อมูลเดิม
    record_count = original_record_count;
    for(int i=0; i < original_record_count && i < 100; i++) {
        strcpy(all_studentNames[i], temp_names[i]);
        strcpy(all_studentIDs[i], temp_ids[i]);
    }
    printf(" -> ข้อมูลเดิมในโปรแกรมถูกกู้คืนเรียบร้อยเเล้ว;\n");
}



