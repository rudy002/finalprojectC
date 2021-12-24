#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#define fileEmployee "employee.csv"
#define fileEmployer "employer.csv"
#define fileJobs "jobs.csv"
#define fileJobRequest "jobrequest.csv"
#define fileSecurityQuestion "Security_Questions.csv"
#define Question_one "What is your favorite color"
#define Question_two "What is your childhood name"
#define Question_tree "What is your mother's name"
//Structs
typedef struct Employee {
    char mail[50], password[13], first_name[20], last_name[20], ID[10], gender[7], phone[11], location[10], cv_file_name[30], pud_file_name[30];
    int age;
} Employee;
typedef struct Employer {
    char mail[50], password[13], ID[10];
} Employer;
typedef struct {
    char mail_Employer[30], company[30], scope[20], time[20], area[15], phone[11], requirements[30], experience_required[30];
    unsigned int no_job;

} Job;
typedef struct Job_Request {
    char employer_mail[30];
    char employee_mail[30];
    char company_name[20];
    char job_scope[20];
    char status[10];

} Req;
//prototypes
void save_employer(Employer* to_save);
void save_employee(Employee* to_save);
void save_job_request_data(Req* j);
void save_job_data(const char* mail, const char* scope);
Req make_request_from_line(const char* line);
Employer* sign_in_employer();
Employer* make_employer_from_file(const char* mail);
Employee* sign_in_employee();
Employee* make_employee_from_file(const char* mail);
char* get_pass_by_mail(const char* mail, const char* fileName);
bool sign_up_employer(Employer* emp);
bool sign_up_employee(Employee* emp);
void add_job(Employer *emp);
void apply_for_job(const char* employee_mail, Job** job_list);
bool print_job(Job job);
Job** search_job(char* research);
char * request_search_job();
void delete_job(Employer *emp);
bool edit_account(Employee* user);
void print_employee(Employee* emp);
bool upload_cv(Employee* emp);
void forget_password(const char* mail);
void init_question(const char* mail);
bool check_question(const char* mail);
void conversations(char* mail, char* filename);






//general function
bool check_phone_number(char* phone){
    if (strlen(phone) != 10)
        return false;
    else if (phone[0] == '0' && phone[1] == '5') { return true; }
    else
        return false;

} //function check if phone number is correct or not
bool check_mail_in_system(char* mail, char* theFile){
    char wrd[256], *token;
    FILE *file;
    file = fopen(theFile, "r");
    if (!file){
        printf("cant open the file\n");
        return false;
    }
    while(fgets(wrd, 256, file) != NULL) {
        token = strtok(wrd, ",");
        if (strcmp(token, mail) == 0 ){
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}//function check in mail is inside system that mean if is inside file
bool check_password(const char* password){

    if(strlen(password) < 6 || strlen(password) > 12 ){
        return false;
    }

    int lower = 0, upper = 0, digit = 0;
    for (int i = 0; i < (strlen(password)); ++i) {
        if (isupper(password[i])) {
            upper = 1;
        } else if (isdigit(password[i])) {
            digit = 1;
        } else if (islower(password[i])) {
            lower = 1;
        }
    }

    if(lower && upper && digit){
        return true;
    }
    else
        return false;

} //check if password is good or no
bool check_email (const char* mail) {
    char* token, * user_name, * site_mail, tmp[30], special_charcters[] = "!#$%^&*()+=-`~\'\",\\/";
    strcpy(tmp, mail);
    if (!strchr(tmp, '@'))return false;
    user_name = strtok(tmp, "@");
    token = strtok(NULL, "@");
    if (strchr(token, '@'))return false;// check if there's another '@'.
    int i = 0;
    while (user_name[i]) { if (strchr(special_charcters, user_name[i++]))return false; }
    if (strstr(token, ".com")) {
        site_mail = strtok(token, ".com");
        token = strtok(NULL, ".com");
    }
    else if (strstr(token, ".il")) {
        site_mail = strtok(token, ".il");
        token = strtok(NULL, ".il");
    }
    else return false;
    while (site_mail[i]) { if (strchr(special_charcters, site_mail[i++]))return false; }
    return true;

} //check if mail is good or no
bool check_id_in_system(char* id, char* f)
{

    char buffer[512];
    FILE* file;
    file = fopen(f, "r");
    if (!file)
    {
        printf("can't open the file\n");
        return false;
    }
    while(!feof(file)){
        fgets(buffer, 512, file);
        if (strstr(buffer, id) != NULL)
        {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}
bool check_comma(char* str){
    if(strchr(str, ',') != NULL)
    {
        return false;
    }
    else
        return true;
}
bool check_ID(const char* id) {
    if (strlen(id) != 9)
        return false;
    else {
        for (int i = 0; i < 9; ++i) {
            if (id[i] < '0' || id[i] > '9')
                return false;
        }
        return true;
    }
}

void clearBuff(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

}
void save_employer(Employer* to_save) {
    char buffer[256];
    FILE *f1 = fopen(fileEmployer, "r");
    FILE *f2 = fopen("tmp.csv", "w");
    while (fgets(buffer, 256, f1)) {
        if (strstr(buffer, to_save->mail)) { continue; }
        fputs(buffer, f2);
    }
    char line[256];
    strcpy(line, to_save->mail);
    strcat(line, ",");
    strcat(line, to_save->password);
    strcat(line, ",");
    strcat(line, to_save->ID);
    strcat(line, "\n");
    fprintf(f2, line, "%s");

    fclose(f1);
    fclose(f2);
    remove(fileEmployer);
    rename("tmp.csv", fileEmployer);


}//log out






//function users
bool upload_cv(Employee* emp) {
    char cvFileName[50];
    do {
        printf("Enter your cv file name: ");
        scanf("%s", cvFileName);
    }while(!check_comma(cvFileName));
    FILE* tmp = fopen(fileEmployee, "r");
    if (tmp) { strcpy(emp->cv_file_name, cvFileName); fclose(tmp); return true; }
    else { puts("can't open your file, make sure you've wrote the right path to file."); return false; }
}
void print_employee(Employee* emp) {
    char buffer[256];
    printf("Full Name: %s %s.\n", emp->first_name, emp->last_name);
    printf("Mail: %s.\n", emp->mail);
    printf("ID: %s.\nPhone Number: %s.\nGender: %s.\nAddress: %s.\nAge: %d.\n", emp->ID, emp->phone, emp->gender, emp->location, emp->age);
    if (strcmp(emp->cv_file_name, "None")) {
        FILE* tmp = fopen(emp->cv_file_name, "r");
        if (tmp) {
            printf("CV: \n");
            while (fgets(buffer, 256, tmp)) {
                puts(buffer);
            }
            fclose(tmp);
        }
    }
    if (strcmp(emp->pud_file_name, "None")) {

        FILE* tmp = fopen(emp->pud_file_name, "r");
        if (tmp) {
            printf("personal user details: \n");
            while (fgets(buffer, 256, tmp)) {
                puts(buffer);
            }
            fclose(tmp);
        }
    }
}               //open file and print specific employee
bool edit_account(Employee* user) {
    int ans;
    char name[20], name1[11], answer[2], age[4];
    printf("hello. you here for edit your account\n");
    print_employee(user);

    do {
        printf("choose what you want edit :\n1.first name\n2.last name\n3.phone\n4.gender\n5.location\n6.age\n7.CV file\n8.EXIT\n");
        printf("================================================================================\n");
        scanf("%s", answer);
        while (strcmp(answer,"1") != 0 && strcmp(answer,"2") != 0 &&strcmp(answer,"3") != 0 &&strcmp(answer,"4") != 0 &&strcmp(answer,"5") != 0 &&strcmp(answer,"6") != 0 &&strcmp(answer,"7") != 0 && strcmp(answer,"8") != 0 ) {
            scanf("%s", answer); }
        ans = atoi(answer);

        switch (ans) {
            case 1:             //first name

                do {
                    printf("enter new name please :\t");
                    scanf("%s", name);
                }while(!check_comma(name));
                strcpy(user->first_name, name);
                printf("you change information successfully\n");
                break;

            case 2:             //last name
                do {
                    printf("enter new name please :\t");
                    scanf("%s", name);
                }while(!check_comma(name));
                strcpy(user->last_name, name);
                printf("you change information successfully\n");
                break;
            case 3:              //phone
                do {
                    printf("enter new phone please :\t");
                    scanf("%s", name1);
                }while(!check_phone_number(name1));
                strcpy(user->phone, name1);
                printf("you change information successfully\n");
                break;
            case 4:              //gender
                do {
                    printf("enter new gender please :\n1.male\n2.female\n");
                    scanf("%s", name1);
                }while(strcmp("1", name1) != 0 && strcmp("2", name1) != 0);
                if(strcmp(name1, "1") == 0)
                    strcpy(user->gender, "male");
                else
                    strcpy(user->gender, "female");
                printf("you change information successfully\n");
                break;
            case 5:              //location
                do {
                    printf("choose your new area :\n1.south\n2.jerusalem\n3.center\n4.north\n5.negev\n");
                    scanf("%s", name1);
                }while(strcmp("1", name1) != 0 && strcmp("2", name1) != 0 && strcmp("3", name1) != 0 && strcmp("4", name1) != 0 && strcmp("5", name1) != 0);
                if (strcmp(name1, "1") == 0 ) {
                    strcpy(user->location, "south");
                }
                else if (strcmp(name1, "2") == 0 ) {
                    strcpy(user->location, "jerusalem");
                }
                else if (strcmp(name1, "3") == 0 ) {
                    strcpy(user->location, "center");
                }
                else if (strcmp(name1, "4") == 0 ) {
                    strcpy(user->location, "north");
                }
                else strcpy(user->location, "negev");
                printf("you change information successfully\n");
                break;
            case 6:              //age
                do{
                    printf("enter your new age :");
                    scanf("%s", age);
                    ans = atoi(age);
                }while((ans < 18 || ans > 120));
                user->age = ans;
                printf("you change information successfully\n");
                break;
            case 7:             //CV file
                //je dois demander a shoam de menvoyer la fonction qui concerne upload cv IMPORTANT
                break;
            case 8:             //EXIT
                break;
        }
    }while (ans != 8);

    print_employee(user);
    return true;


}//ok
char * request_search_job(){
    char answers[2];
    int ans;
    static char search[30];
    do {
        printf("You choose option Search Job. what do you want to Search ?\n1.time of work\n2.scope Job\n3.area you want to work\n-----> ");
        scanf("%s", answers);
    }while(strcmp(answers,"1") != 0  && strcmp(answers,"2") != 0 && strcmp(answers,"3") != 0);
    ans = atoi(answers);
    switch (ans) {
        case 1:
            do {
                printf("you choose : time to work. choose possibility you want to search.\n1.half-time\n2.full-time\n-----> ");
                scanf("%s", answers);
            }while(strcmp(answers,"1") != 0  && strcmp(answers,"2") != 0);
            if (strcmp(answers,"1") != 0)
                return "half-time";
            else
                return "full-time";

        case 2:
            printf("you choose : scope Job. enter which kind of work you want to search (example : security-----> ");
            scanf("%s", search);
            return search;

        case 3:
            do {
                printf("you choose : Area. choose which area you want to Work.\n1.south\n2.jerusalem\n3.center\n4.north\n5.negev\n-----> ");
                scanf("%s", answers);
            }while(strcmp(answers,"1") != 0  && strcmp(answers,"2") != 0 && strcmp(answers,"3") != 0 && strcmp(answers,"4") != 0 && strcmp(answers,"5") != 0);

            if (strcmp(answers,"1") == 0){
                return "south";
            }
            else if (strcmp(answers,"2") == 0){
                return "jerusalem";
            }
            else if (strcmp(answers,"3") == 0){
                return "center";
            }
            else if (strcmp(answers,"4") == 0){
                return "north";
            }
            else
                return "negev";

            break;
    }
}//this function ask the user what he wants to send to the function SearchJob;
Job** search_job(char* research) {
    Job** jobs = malloc(sizeof(Job*) * 25);
    char wrd[256], buffer[256], * token = NULL;
    int find_result = 0;
    FILE* file;
    file = fopen(fileJobs, "r");
    if (!file) {
        printf("cant open the file\n");
    }
    else {

        while (fgets(wrd, 256, file) != NULL) {
            strcpy(buffer, wrd);
            if ((strstr(wrd, research))) {
                jobs[find_result] = (Job*) malloc(sizeof (Job));
                jobs[find_result]->no_job = find_result + 1;
                token = strtok(buffer, ",");
                strcpy(jobs[find_result]->mail_Employer, token);
                token = strtok(NULL, ",");
                strcpy(jobs[find_result]->company, token);
                token = strtok(NULL, ",");
                strcpy(jobs[find_result]->scope, token);
                token = strtok(NULL, ",");
                strcpy(jobs[find_result]->time, token);
                token = strtok(NULL, ",");
                strcpy(jobs[find_result]->area, token);
                token = strtok(NULL, ",");
                strcpy(jobs[find_result]->phone, token);
                token = strtok(NULL, ",");
                strcpy(jobs[find_result]->requirements, token);
                token = strtok(NULL, "\n");
                strcpy(jobs[find_result]->experience_required, token);

                // printf("A match found on line: %d\n", line);
                // printf("\n%s\n", wrd);
                find_result++;

            }
            jobs[find_result] = (Job*)malloc(sizeof(Job));
            jobs[find_result]->no_job = 0;
        }
    }
    fclose(file);
    if (find_result == 0)return NULL;
    return jobs;
} //ok its work
bool print_job(Job job) {
    if (job.no_job == 0) { puts("______________"); return false; }
    puts("______________");
    printf("Job #%d: \n", job.no_job);
    printf("Company Name: "); puts(job.company);
    printf("Job Scope: "); puts(job.scope);
    printf("Job Time: "); puts(job.time);
    printf("Location: "); puts(job.area);
    printf("Phone : "); puts(job.phone);
    printf("Requirements : "); puts(job.requirements);
    printf("Experience Required: "); puts(job.experience_required);
    return true;
}


void apply_for_job(const char* employee_mail, Job** job_list) {
    if(!job_list) {
        printf("ERROR! no job found\n");
    }


    else {

        int i = 0, choise;
        char choice[3];
        while (print_job(*job_list[i++])) {}
        do {
            printf("Enter Your Choice: ");
            scanf("%s", choice);
            choise = atoi(choice);
        } while (choise == 0);
        if (choise >= 1 && choise <= i + 1) {
            char request[256];
            strcpy(request, job_list[choise - 1]->mail_Employer);
            strcat(request, ",");
            strcat(request, employee_mail);
            strcat(request, ",");
            strcat(request, job_list[choise - 1]->company);
            strcat(request, ",");
            strcat(request, job_list[choise - 1]->scope);
            strcat(request, ",");
            strcat(request, "waiting\n");
            FILE *f = fopen(fileJobRequest, "a+");
            if (f) {
                fputs(request, f);
                printf("your request added to the system.\n");
            }
            fclose(f);

        } else { printf("Wrong Index!\n"); }
    }

}
void add_job(Employer *emp){
    char choice[2];
    char tmp[256];
    FILE *f1 = fopen(fileJobs, "a+");
    char  input_string[100];
    char line[256] = "";
    strcat(line, emp->mail);
    strcat(line,",");
    do {
        printf("what is the company name of the job you want to add ?\n----->");
        scanf(" %[^\n]s", input_string);
    }while(!check_comma(input_string));
    strcat(line, input_string);
    strcat(line,",");
    do {
        printf("what is the scope of the job you want to add ?\n----->");
        scanf(" %[^\n]s", input_string);
    }while(!check_comma(input_string));
    strcat(line, input_string);
    strcat(line,",");
    do{
        printf("choose kind time of work please\n1.half time\n2.full time\n");
        scanf("%s", choice);
    }while (strcmp(choice, "1") != 0 && (strcmp(choice, "2") != 0));
    if (strcmp(choice, "1") != 0)
        strcat(line, "half-time");
    else
        strcat(line, "full-time");

    strcat(line, ",");
    do {
        printf("choose your new area :\n1.south\n2.jerusalem\n3.center\n4.north\n5.negev\n");
        scanf("%s", input_string);
    }while(strcmp("1", input_string) != 0 && strcmp("2", input_string) != 0 && strcmp("3", input_string) != 0 && strcmp("4", input_string) != 0 && strcmp("5", input_string) != 0);
    if (strcmp(input_string, "1") != 0 ) {
        strcat(line,"south");
    }
    else if (strcmp(input_string, "2") != 0 ) {
        strcat(line, "jerusalem");
    }
    else if (strcmp(input_string, "3") != 0 ) {
        strcat(line, "center");
    }
    else if (strcmp(input_string, "4") != 0 ) {
        strcat(line, "north");
    }

    else strcat(line, "negev");
    strcat(line, ",");
    do {
        printf("what is the  phone number of your company  ?\n----->");
        scanf("%s",input_string);
    }while(!check_phone_number(input_string));
    strcat(line, input_string);
    strcat(line,",");
    do {
        printf("what is the requirement needed for the job ? ");
        scanf(" %[^\n]s", input_string);
    }while(!check_comma(input_string));
    strcat(line, input_string);
    strcat(line,",");
    do {
        printf("what is experience required needed for the job ? ");
        scanf(" %[^\n]s", input_string);
    }while(!check_comma(input_string));
    strcat(line, input_string);
    strcat(line,"\n");
    fputs(line, f1);
    fclose(f1);

} // ok. this function work correctly


bool sign_up_employee(Employee* emp) {

    char input[100];
    int age;
    printf("SIGN UP:\n");
    do {
        printf("Enter your first name:");
        scanf("%s", input);
    }while(!check_comma(input));
    strcpy(emp->first_name, input);
    do {
        printf("Enter your last name:");
        scanf("%s", input);
    }while(!check_comma(input));
    strcpy(emp->last_name, input);

    do {
        printf("Enter your phone number:");
        scanf("%s", input);
    }while(!check_phone_number(input));
    strcpy(emp->phone, input);

    do {
        printf("Enter your mail:");
        scanf("%s", input);
    }while(!check_email(input) || check_mail_in_system(input, fileEmployee) || !check_comma(input));
    strcpy(emp->mail, input);

    do {
        printf("Enter your Password:");
        scanf("%s", input);
    } while (!check_password(input));
    strcpy(emp->password, input);


    do {
        printf("Reenter your password:");
        scanf("%s", input);
    } while (strcmp(emp->password, input) != 0);

    do {
        printf("enter new gender please :\n1.male\n2.female\n");
        scanf("%s", input);
    }while(strcmp("1", input) != 0 && strcmp("2", input) != 0);
    strcpy(emp->gender, input);

    do {
        printf("choose your new area :\n1.south\n2.jerusalem\n3.center\n4.north\n5.negev\n");
        scanf("%s", input);
    }while(strcmp("1", input) != 0 && strcmp("2", input) != 0 && strcmp("3", input) != 0 && strcmp("4", input) != 0 && strcmp("5", input) != 0);
    if (strcmp(input, "1") != 0 ) {
        strcpy(emp->location, "south");
    }
    else if (strcmp(input, "2") != 0 ) {
        strcpy(emp->location, "jerusalem");
    }
    else if (strcmp(input, "3") != 0 ) {
        strcpy(emp->location, "center");
    }
    else if (strcmp(input, "4") != 0 ) {
        strcpy(emp->location, "north");
    }
    else strcpy(emp->location, "negev");

    strcpy(emp->cv_file_name, "None");

    do{
        printf("enter your new age :");
        scanf("%d", &age);
    }while(!(age >= 18 || age <= 120));
    emp->age = age;

    init_question(emp->mail);

    return true; // if all the allocates memory, and the user verificated his password so return true.
}
bool sign_up_employer(Employer* emp){
    char input[49];
    printf("SIGN UP:\n");

    do {
        printf("Enter your mail:");
        scanf("%s", input);
    }while(!check_comma(input) &&  !check_email(input) && check_mail_in_system(input, fileEmployee));
    strcpy(emp->mail, input);

    do {
        printf("Enter your Password:");
        scanf(" %s", input);
    } while (!check_password(input));
    strcpy(emp->password, input);


    do {
        printf("Reenter your password:");
        scanf(" %s", input);
    } while (strcmp(emp->password, input) != 0);

    clearBuff();
    do{
        printf("Enter your ID:");
        scanf(" %s", input);
    }while(check_id_in_system(input, fileEmployer) || !check_ID(input));

    init_question(emp->mail);

    return true;
}

char* get_pass_by_mail(const char* mail, const char* fileName) {
    FILE* tmp = fopen(fileName, "r");
    char line[255], c, *token;
    //while the file not in the end of file (eof).
    while (!feof(tmp)){
        fgets(line, 255, tmp);
        token = strtok(line, ",");//'strtok' function split a line(string) to some tokens by the sign we choose.

        while (token != NULL)
        {
            // if the token is the same mail(that's mean the same user) so return the password of this account.
            if (strcmp(token, mail) == 0) { return strtok(NULL, ","); }
            token = strtok(NULL, ",");// promote token to the next token.
        }
    }

    fclose(tmp);// close and save the file.
    return "not found";
}
Employee* make_employee_from_file(const char* mail) {
    Employee* res = NULL;
    FILE* f = fopen(fileEmployee, "r");
    char buffer[256], *token = NULL;
    while (fgets(buffer, 256, f)) {
        token = strtok(buffer, ",");
        if (!strcmp(token, mail)) {
            res = (Employee*)malloc(sizeof(Employee));
            strcpy(res->mail, token);
            token = strtok(NULL, ",");
            strcpy(res->password, token);
            token = strtok(NULL, ",");
            strcpy(res->first_name, token);
            token = strtok(NULL, ",");
            strcpy(res->last_name, token);
            token = strtok(NULL, ",");
            strcpy(res->ID, token);
            token = strtok(NULL, ",");
            strcpy(res->gender, token);
            token = strtok(NULL, ",");
            res->age = atoi(token);
            token = strtok(NULL, ",");
            strcpy(res->phone, token);
            token = strtok(NULL, ",");
            strcpy(res->location, token);
            token = strtok(NULL, ",");
            strcpy(res->cv_file_name, token);
            token = strtok(NULL, ",");
            strcpy(res->pud_file_name, token);
        }
    }
    return res;
}
Employee* sign_in_employee() {
    //boolean signIn = f;
    Employee* emp = NULL;
    int tries = 0;
    char input1[50] = "", input2[50];// to store the user inputs.
    char password[13] = "";// to store the password.
    do {
        printf("Mail: ");//ask user for mail.
        scanf("%s", input1);
        if (check_comma(input1) && check_mail_in_system(input1, fileEmployee))
            emp = make_employee_from_file(input1);
            break;
    }while(true);
    //strcpy(emp->mail, input1);
    strcpy(password, get_pass_by_mail(input1, fileEmployee));// 'get_pass_by_mail()' return the password of the account if exist in the system.
    do {
        printf("enter your password. if your forgot password, enter '1'\nPassword: ");//ask user for password.
        scanf("%s", input2);
        if (check_comma(input2) && strcmp(input2, password) == 0) {
            //emp = make_employee_from_file(input1);
            break;
        }
        else if (strlen(input2) == 1 && strcmp(input2, "1") == 0) {
            forget_password(input1);
        }

    }while(true);
    return emp;

}
//ok. 3 function work correctly

Employer* make_employer_from_file(const char* mail) {
    Employer* res = NULL;
    FILE* f = fopen(fileEmployer, "r");
    char buffer[256], * token = NULL;
    while (fgets(buffer, 256, f)) {
        token = strtok(buffer, ",");
        if (!strcmp(token, mail)) {
            res = (Employer*)malloc(sizeof(Employer));
            strcpy(res->mail, token);
            token = strtok(NULL, ",");
            strcpy(res->password, token);
            token = strtok(NULL, ",");
            strcpy(res->ID, token);
            token = strtok(NULL, ",");
        }
    }
    return res;
}
Employer* sign_in_employer() {
    //boolean signIn = f;
    Employer* emp = NULL;
    char input1[50] = "", input2[50];// to store the user inputs.
    char password[13] = "";// to store the password.
    do {
        printf("Mail: ");//ask user for mail.
        scanf("%s", input1);
        if (check_comma(input1) && check_mail_in_system(input1, fileEmployer))
            break;

    } while (true);
    strcpy(password, get_pass_by_mail(input1, fileEmployer));// 'get_pass_by_mail()' return the password of the account if exist in the system.
    do {
        printf("Password: ");//ask user for password.
        scanf("%s", input2);
        if (check_comma(input2) && strcmp(input2, password) == 0) {
            emp = make_employer_from_file(input1);
            break;
        }
    }while(true);
    return emp;

}

Req make_request_from_line(const char* line) {
    char* token = NULL;
    Req req;
    token = strtok(line, ",");
    strcpy(req.employer_mail, token);
    token = strtok(NULL, ",");
    strcpy(req.employee_mail, token);
    token = strtok(NULL, ",");
    strcpy(req.company_name, token);
    token = strtok(NULL, ",");
    strcpy(req.job_scope, token);
    token = strtok(NULL, "\n");
    strcpy(req.status, token);
    return req;
}

void save_job_data(const char* mail, const char* scope){
    FILE* fp = fopen(fileJobs, "r");
    FILE* tmp = fopen("temp.csv", "w");
    if (!(fp && tmp)) {/* failed to open file */ }
    char buffer[256], line[256];
    while (fgets(buffer, 256, fp)) {
        if (strstr(buffer, mail)) {
            if (strstr(buffer, scope)) {
                continue;
            }

        }// if we get here we are in the end of file.
        fputs(buffer, tmp);
    }
    // here we convert the object to csv line.
    // the first word we need to copy and then we can use strcat() function.
    // print the line on the file (the last one)
    // close the files.
    fclose(fp);
    fclose(tmp);
    // free function to free the data of the object(need to create)
    remove(fileJobs);
    rename("temp.csv", fileJobs);
}//exit job
void save_job_request_data(Req* j)
{
    FILE* fp = fopen(fileJobRequest, "r");
    FILE* tmp = fopen("temp.csv", "w");
    if (!(fp && tmp)) {/* failed to open file */ }
    char buffer[256], line[256];
    while (fgets(buffer, 256, fp)) {
        if (strstr(buffer, j->employer_mail)) { continue; }
        fprintf(tmp, buffer, "%s");
    }// if we get here we are in the end of file.
    // here we convert the object to csv line.
    // the first word we need to copy and then we can use strcat() function.
    strcpy(line, j->employer_mail);
    strcat(line, ",");
    strcat(line, j->employee_mail);
    strcat(line, ",");
    strcat(line, j->company_name);
    strcat(line, ",");
    strcat(line, j->job_scope);
    strcat(line, ",");
    strcat(line, j->status);


    strcat(line, "\n");

    // print the line on the file (the last one)
    fprintf(tmp,line, "%s");

    // close the files.
    fclose(fp);
    fclose(tmp);

    free(j);
    // free function to free the data of the object(need to create)
    remove(fileJobRequest);
    rename("temp.csv", fileJobRequest);
}//exit job
void save_employee(Employee* to_save) {
    char temp[3];
    char buffer[256];
    char tmp[256], *token = NULL;
    FILE *f1 = fopen(fileEmployee, "r");
    FILE *f2 = fopen("tmp.csv", "w");
    while (fgets(buffer, 256, f1)) {
        if (strstr(buffer, to_save->mail)) { continue; }
        fprintf(f2, buffer, "%s");
    }
    char line[256];
    strcpy(line, to_save->mail);
    strcat(line, ",");
    strcat(line, to_save->password);
    strcat(line, ",");
    strcat(line, to_save->first_name);
    strcat(line, ",");
    strcat(line, to_save->last_name);
    strcat(line, ",");
    strcat(line, to_save->ID);
    strcat(line, ",");
    strcat(line, to_save->gender);
    strcat(line, ",");
    sprintf(temp, "%d", to_save->age);
    strcat(line, temp);
    strcat(line, ",");
    strcat(line, to_save->phone);
    strcat(line, ",");
    strcat(line, to_save->location);
    strcat(line, ",");
    strcat(line, to_save->cv_file_name);
    strcat(line, ",");
    strcat(line, to_save->pud_file_name);
    strcat(line, "\n");
    fprintf(f2, line ,"%s");
    fclose(f1);
    fclose(f2);
    remove(fileEmployee);
    rename("tmp.csv", fileEmployee);


}//log out

void delete_job(Employer *emp) {
    char scope[20];
    puts("Enter job scope to delete.");
    fgets(scope, 20, stdin);
    save_job_data(emp->mail, scope);
    FILE* f1 = fopen(fileJobRequest, "r");
    FILE* f2 = fopen("temp.csv", "w");
    char buff[255];
    while (!feof(f1)) {
        fgets(buff, 255, f1);
        if (strstr(buff, emp->mail))
            if (strstr(buff, scope))
                continue;
        fprintf(f2, buff, "%s\n");
    }
    fclose(f1);
    fclose(f2);
    remove(fileJobRequest);
    rename("temp.csv", fileJobRequest);
    clearBuff();
}

void update_candidates(const char* employer_mail) {
    char line[256], buffer[256], * token = NULL; // line and token for strtok and buffer to store the buffs from the file
    bool loop_condition = true; // to keep changing the statuses.
    Employee** tmp = malloc(sizeof(Employee*) * 25); // array of 25 employees (MAXIMUM)
    Req* reqs = malloc(sizeof(Req) * 25); // array of 25 requests (MAXIMUM)
    int index = 0, choise, userIndex;// index for the first loop(when we creating), choise to the menu, and userindex is to get to the index in the same place.
    FILE* f = fopen(fileJobRequest, "r");
    // in this loop we run over the Job Request file.
    // we store all the data of the the requests and employees we need.
    // and by the way, print the menu of the candidates.
    while (fgets(buffer, 256, f)) {
        strcpy(line, buffer);
        if (strstr(buffer, employer_mail)) {
            puts("_______");
            printf("Candidate #%d:\n", index + 1);
            token = strtok(line, ","); token = strtok(NULL, ",");// now token keep the employee's mail.
            tmp[index] = (Employee*) malloc(sizeof(Employee));
            tmp[index] = make_employee_from_file(token);
            //memcpy(&tmp[index], make_employee_from_file(token), sizeof(Employee)); // store the employee's data.
            //tmp[index] = make_employee_from_file(token);
            reqs[index] = make_request_from_line(buffer);// store the request data.
            printf("Job Details:\nCompany Name: %s\tScope: %s.\n\nEmployee Details:\n", reqs[index].company_name, reqs[index].job_scope);
            print_employee(tmp[index++]);
        }
    }
    fclose(f);
    puts("_______");/// end of employees menu.
    // starting a do while loop to keeps the user change how many he like.
    do {
        printf("Which candidate you wanna call: press wrong index to finish");
        scanf("%d", &choise);
        if (choise > index + 1 || choise < 1) { loop_condition = false; }// the way to get out of the loop.
        else {
            userIndex = choise - 1;
            printf("enter 1 for accept, 2 for refuse, 3 to skip\n");
            scanf("%d", &choise);
            switch (choise) {
                case 1:
                    strcpy(reqs[userIndex].status, "Accepted\n");
                    break;

                case 2:
                    strcpy(reqs[userIndex].status, "Refused\n");
                    break;

                case 3:
                    break;

                default:
                    printf("you enter wrong input, youre changes doesn't saves!\n");
                    break;
            }// end of swithcase.
        }// end of else statement.
    } while (loop_condition);

    for (int i = 0; i < index; ++i) {
        free(tmp[i]);
    }
    free(tmp);
    //return reqs; // the changed requests (NOT ALL THE REQUEST!!(only of the same Employer!!!!)).
    for (int i = 0; i < index; i++) {
        save_job_request_data(&reqs[i]);
    }
}
//get statuses of requested jobs.
void get_status(const char* employee_mail) {
    char line[256], buffer[256], print_line[256], *token = NULL;
    FILE* f = fopen(fileJobRequest, "r");
    puts("_________________________________");
    puts("Company Name\tScope\tStatus\n ");
    puts("_________________________________");
    while (fgets(buffer, 256, f)) {
        strcpy(line, buffer);
        if (strstr(line, employee_mail)) {
            token = strtok(buffer, ",");  token = strtok(NULL, ",");  token = strtok(NULL, ",");
            strcpy(print_line, token);// company name.
            strcat(print_line, "\t");
            token = strtok(NULL, ",");
            strcat(print_line, token);// job scope
            strcat(print_line, "\t");
            token = strtok(NULL, ",");
            strcat(print_line, token);// status
            strcat(print_line, "\n");
            puts(print_line);
        }
    }
    puts("_________________________________");
    fclose(f);
}


//forget password (3 functions)
bool check_question(const char* mail)
{
    char answer[30];
    FILE* f = fopen(fileSecurityQuestion, "r");
    char buff[256], *token = NULL, tmp_tok[30];
    do {
        fgets(buff, 256, f);
    } while (!strstr(buff, mail));
    token = strtok(buff, ","); token = strtok(NULL, ",");
    printf("%s? ", Question_one);
    scanf(" %[^\n]", answer);

    if (strcmp(token, answer)) { fclose(f); return false; }
    token = strtok(NULL, ",");
    printf("%s? ", Question_two);
    //fgets(answer, 30, stdin);
    scanf(" %[^\n]", answer);

    if (strcmp(token, answer)) { fclose(f); return false; }
    token = strtok(NULL, "\n");
    printf("%s? ", Question_tree);
    scanf(" %[^\n]", answer);

    if (strcmp(token, answer)) { fclose(f); return false; }
    fclose(f);
    return true;
}
void init_question(const char* mail)
{
    FILE* f = fopen(fileSecurityQuestion, "a+");
    char line[256], answer[30],*token = NULL;
    strcpy(line, mail);
    strcat(line, ",");
    printf("%s? ", Question_one);
    //fgets(answer, 30, stdin);
    //gets(answer);
    scanf(" %[^\n]", answer);
    token = strtok(answer, "\n");
    strcat(line, token);
//	clean_buffer();
    strcat(line, ",");
    printf("%s? ", Question_two);
    //gets(answer);
    //fgets(answer, 30, stdin);
    scanf(" %[^\n]", answer);
    token = strtok(answer, "\n");
    strcat(line, token);
//	clean_buffer();
    strcat(line, ",");
    printf("%s? ", Question_tree);
    //gets(answer);
    //fgets(answer, 30, stdin);
    scanf(" %[^\n]", answer);
    token = strtok(answer, "\n");
    strcat(line, token);
//	clean_buffer();
    strcat(line, "\n");
    fprintf(f, line, "%s");
    fclose(f);
}
void forget_password(const char* mail){
    if (check_question(mail)) {
        char res1[13];
        strcpy(res1, get_pass_by_mail(mail, fileEmployee));
        char res2[13];
        strcpy(res2, get_pass_by_mail(mail, fileEmployer));
        if (strcmp(res1, "not found")) {
            printf("Employee \n ");
            printf("%s", res1);
        }
        if (strcmp(res2, "not found")) {
            printf("Employer\n");
            printf("%s", res2);
        }
    }
}



//conversation : this function open file and the user can send message
void conversations(char* mail, char* filename)
{
    char cnversationinput[150];
    FILE* cnversation = fopen(filename, "a+");
    if (cnversation)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("\n\n");
        while (!feof(cnversation))
        {
            printf("%c", fgetc(cnversation));

        }
        printf("Enter your message: ");
        fgets(cnversationinput, 150, stdin);
        fprintf(cnversation, "Email:%s; date:%d-%02d-%02d;  time:%02d:%02d:%02d;\nmessage:%s;\n\n", mail, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, cnversationinput);
        fclose(cnversation);
        return;
    }
    printf("\nError can't open your file.\n );");
}






void employee_Menu(Employee* emp){
    char choice[3];
    int ans;
    do{
        printf("hi %s\n Please choose an option:\n1 - Search for available job\n2 - Edit account\n3 - Check requests status\n4 - Upload CV\n5 - Log out\n------>", emp->first_name);
        do{
            scanf("%s", choice);
        } while (strcmp(choice,"1") != 0 && strcmp(choice, "2") != 0 &&strcmp(choice,"3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice,"5") != 0);
        ans = atoi(choice);

        switch (ans)
        {
            case 1://check and test OK
                apply_for_job(emp->mail, search_job(request_search_job()));
                break;
            case 2:
                edit_account(emp);
                break;
            case 3:
                get_status(emp->mail);
                break;
            case 4:
                printf(upload_cv(emp) ? "CV uploaded successfully\n" : "failed to upload your file\n");
                break;
            case 5:

                save_employee(emp);
                free(emp);
                return;

        }
    } while (ans != 5);
}
void employerMenu(Employer *emp)
{
    int choice;
    do {
        do {
            printf("Please choose an option:\n 1. Post new job\n2. Delete existing job \n3. Check For Candidates\n4. Log out\n");
            scanf("%d", &choice);
        } while (choice < 1 || choice > 5);
        switch (choice)
        {
            case 1:
                add_job(emp);
                break;
            case 2:
                delete_job(emp);
                break;
            case 3:
                update_candidates(emp->mail);
                break;
            case 4:
                save_employer(emp);
                free(emp);
                break;
        }
    } while (choice != 4);
}


int main(){
    char choice[3];
    bool sign_up;

    Employer* emp1 = malloc(sizeof(Employer));
    Employee* emp2 = malloc(sizeof(Employee));

    printf("Welcome.Its job portal system.\n who are you ?\n1 - Employee\n2 - Employer\n");
    do{
        printf("Enter your answer : ");
        scanf("%s", choice);
    }while(strcmp(choice,"1") != 0 && strcmp(choice, "2") != 0);
    int ans = atoi(choice);

    switch (ans) {
        case 1:
            printf("welcome Employee. what do you want to do ?\n1 - sign up\n2 -sign in\nif you're forget password, enter in sign in and tap '1'\n");
            do{
                printf("\nEnter your answer : ");
                scanf("%s", choice);
            }while(strcmp(choice,"1") != 0 && strcmp(choice, "2") != 0);
            ans = atoi(choice);
            switch (ans) {

                case 1:
                    sign_up_employee(emp2);
                    save_employee(emp2);
                case 2:
                    emp2 = sign_in_employee();
                    if (emp2){employee_Menu(emp2);
                    }
                    return 0 ;

            }

            break;
        case 2:
            printf("welcome Employer. what do you want to do ?\n1 - sign up\n2 -sign in\n");
            do{
                printf("Enter your answer : ");
                scanf("%s", choice);
            }while(strcmp(choice,"1") != 0 && strcmp(choice, "2") != 0);
            ans = atoi(choice);
            switch (ans) {
                case 1:
                    sign_up = sign_up_employer(emp1);
                    save_employer(emp1);
                case 2:
                    emp1 = sign_in_employer();
                    if(emp1){employerMenu(emp1);}

                    return 0;
            }
            break;

    }
    return 0;
}
