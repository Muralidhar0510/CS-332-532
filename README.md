
* Run the program in a compiler 
          
          gcc -pthread -o mchapala_HW04 mchapala_HW04.c
          
                           OR
          
* We can use the make file to compile the code by using the argument

         make // Make sure you have make file 

* Now run the program by giving the required arguments.

          ./mchapala_HW04

* Then we can get desired output.

                                                Approach 
                                                
                * Initially i have a created inter process communication.
                
                * Then i have created signal mechanism.
                
                * Then written a function "generate_numbers(void *arg)" for number generation.
                
                * Then created child process for number processing.
                
                * Then calculated the average.
                
                AI Tool Usage :
                
                * While i am done with first four steps of homework i was stuck in the signal handling mechanism.
                
                * So, I just take reference from UAB Approved AI tool Microsoft Copilot.
                
                * My prompt is "How to implement a signal from parent process to child process".
                
                * It shows some references and Examples like.
                
                Ex : To implement a signal from a parent process to a child process, you can use the kill function to
                     send signals in Unix-like operating systems. Here's a basic example in C:
                     
                     #include <stdio.h>
                     #include <stdlib.h>
           	     #include <unistd.h>
                     #include <signal.h>
                     void signal_handler(int signum) {
                             printf("Received signal %d\n", signum);
                            exit(signum);
                      }

                     int main() {
                     pid_t pid = fork();

                     if (pid < 0) {
                         perror("Fork failed");
                         exit(1);
                     } else if (pid == 0) {
                     // Child process
                     signal(SIGUSR1, signal_handler);
                     while (1) {
                      // Child process waiting for signal
                     }
                     } else {
                     // Parent process
                     sleep(1);
                     kill(pid, SIGUSR1);
                    }

                   return 0;
                   }
                   
                In this example:

                              The parent process creates a child process using fork().
 
                              The child process sets up a signal handler for SIGUSR1 using signal().

                              The parent process sends the SIGUSR1 signal to the child using kill().   
                     
                     
                     
                * Then i execute some examples to understand it.

                * Then i return to the homework and i implemented the signal mechanism on my own. 
                
                
