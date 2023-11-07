#ifndef _SHELL_H_
#define _SHELL_H_

/* Standard header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* Define error codes */
#define END_OF_FILE -2

#define EXIT -3


/* Global environment */

extern
 
char **environ;

/* Global program name */

extern
 
char *name;

/* Global history counter */

extern
 
int hist;

/* Define structure for history list */
typedef struct list_s {
 char *dir;
 struct list_s *next;
} list_t;

/* Define structure for builtin commands */
typedef struct builtin_s {
 char *name;
 int (*f)(char **argv, char **front);
} builtin_t;

/* Define structure for aliases */
typedef struct alias_s {
 char *name;
 char *replacement;
 struct alias_s *next;
} alias_t;

/* Function prototypes */
void print_prompt(void);
void parse_command(char *command, char ***argv, int *argc);
int execute_command(char **argv);
int is_builtin(char *command);
int run_builtin(char **argv);
void handle_signal(int sig);

/* Define the main function */
int main(int argc, char *argv[]) {
 char command[256]; // Buffer to store the command

 // Continuously loop to prompt for and execute commands
 while (1) {
 print_prompt();
 if (fgets(command, sizeof(command), stdin) == NULL) {
  break; // Exit the loop if fgets encounters an error or EOF
 }

 // Parse the command
 char **argv = NULL;
 int argc = 0;
 parse_command(command, &argv, &argc);

 // Execute the command
 execute_command(argv);

 // Free the allocated memory
 free(argv);
 }

 return 0;
}

// Implement the print_prompt function to print the shell prompt
void print_prompt(void) {
 printf("$ ");
}

// Implement the parse_command function to parse the input command
void parse_command(char *command, char ***argv, int *argc) {
 // Split the command into arguments
 char *token = strtok(command, " ");
 while (token != NULL) {
  // Add each token to the argv array
  (*argv) = realloc(*argv, (*argc) + 1);
  (*argv)[(*argc)++] = token;

  // Get the next token
  token = strtok(NULL, " ");
 }
 (*argv)[(*argc)] = NULL; // Terminate the argv array
}

// Implement the execute_command function to execute the parsed command
int execute_command(char **argv) {
 // Check if the command is a builtin command
 if (is_builtin(*argv)) {
  return run_builtin(argv);
 }

 // Execute the external command
 pid_t pid = fork();
 if (pid == 0) {
  // Child process
  execvp(*argv, argv);
  printf("Error executing command: %s\n", *argv);
  exit(1);
 } else if (pid > 0) {
  // Parent process
  int status;
  waitpid(pid, &status, 0);
  return status;
 } else {
  // Fork error
  printf("Error forking process\n");
  return -1;
 }
}

// Implement the is_builtin function to check if the command is a builtin command
int is_builtin(char *command) {
 // TODO: Implement the is_builtin function
 return 0; // Replace with actual implementation
}

// Implement the run_builtin function to execute the builtin command
int run_builtin(char **argv) {
 // TODO: Implement the run_builtin function
 return 0; // Replace with actual implementation
}

#endif
