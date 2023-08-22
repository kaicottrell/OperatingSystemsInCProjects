#include <stdio.h>
#include <unistd.h>

int main(){
    int x = 0; 
    int y = 0;
    int z = 0;

    pid_t pid; 
    printf("Hello From C World\n");

    x = 5;
    y += 2;

    pid = fork(); // create a child process - refer to man (manual) pages
    //When we hit fork we have two processes running (parent process and child process (from fork) )
    //PID takes two different values because of the fork
    if(pid == 0){ // Child process - Has own seperate variables from the other process
        x += 3;
        y += 3;
        z += 3;
        printf("Hello From Child\n");
        printf("Child Forked PID Value = %i\n" , pid); //expect to be 0 
        printf("Child PID: %i\n", getpid());
        printf("Child Values X: %i Y: %i Z: %i\n", x, y, z);
    }else if (pid < 0){ //error forking
        
    } 
    else{ //Parent process
        x += 2;
        y += 2;
        z += 2;
        printf("Hello From Parent\n");
        printf("Parent Forked PID Value = %i\n" , pid); // Should Match the PID of the child
        printf("Parent PID: %i\n", getpid()); //PID of running parent process
        printf("Parent Values X: %i Y: %i Z: %i\n", x, y, z);
    }
    return 0;
}