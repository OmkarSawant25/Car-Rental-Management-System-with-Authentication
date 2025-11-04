#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio_ext.h>

#define MAX_USER 10
#define CREDENTIAL_LENGTH 30

typedef struct
{
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
} User;

User users[MAX_USER];
int user_count = 0;

void register_user();
int login_user();
void fix_gets_input(char *);
void input_credentials(char *username, char *password);

int main()
{
    int option;
    int user_index;
    while (1)
    {
        printf("\nWelcome to User Management System\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Select an option : ");
        scanf("%d", &option);
        getchar(); // Consumes extra \n
        switch (option)
        {
        case 1:
            register_user();
            break;
        case 2:
            user_index = login_user();
            if (user_index >= 0)
            {
                printf("\nLogin Successful ! Welcome %s!.\n", users[user_index].username);
            }
            else
            {
                printf("\nLogin Failed! Incorrect Username or Password.\n");
            }
            break;
        case 3:
            printf("\nExiting the User Management System.\n");
            return 0;
        default:
            printf("\nInvalid choice. Please try again.\n");
            break;
        }
    }
    return 0;
}
void register_user()
{
    if (user_count == MAX_USER)
    {
        printf("\nMaximum %d users are Supported ! No more registrations Allowed !!\n", MAX_USER);
        return;
    }
    int index = user_count;
    printf("\nRegister a New User");
    input_credentials(users[index].username, users[index].password);
    user_count++;
    printf("\n\nRegistration Successful !!\n");
}

void input_credentials(char *username, char *password)
{
    printf("\nEnter username : ");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_gets_input(username);

    printf("Enter password : ");
    fflush(stdout);

    // change termianl properties
    // Store current terminal settings (old_props will be restored later)
    struct termios old_props, new_props;
    tcgetattr(STDIN_FILENO, &old_props); // get current terminal attributes
    new_props = old_props;               // copy them into new_props

    // Modify new_props to disable ECHO (so typed characters are not visible)
    // and disable ICANON (so input is given character-by-character, not line-based)
    new_props.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props); // apply changes immediately

    char ch;
    int i = 0;

    // Read characters one by one until user presses Enter (\n) or buffer fills
    while ((ch = getchar()) != '\n' && ch != EOF && i < CREDENTIAL_LENGTH - 1)
    {
        if (ch == '\b' || ch == 127) // ascii of del is 127
        {
            i--;
            printf("\b \b");
        }
        else
        {
            password[i++] = ch; // store each typed character into password
            printf("*");
        }
    }
    password[i] = '\0'; // terminate password string with null character

    // Restore old terminal settings so echo works again
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
}

void fix_gets_input(char *s)
{
    int position = strcspn(s, "\n"); // get length of a prefix substring
    s[position] = '\0';
}

int login_user()
{
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
    input_credentials(username, password);

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0)
        {
            return i;
        }
    }
    return -1;
}
