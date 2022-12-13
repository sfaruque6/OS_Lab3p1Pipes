#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str1[20] = "howard.edu"; 
    char input_str[100]; 
    char concat_str[100];

    pid_t p; 
    int i;
    int k;
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        close(fd2[1]);

        printf("Enter a string to concatenate: ");
        scanf("%s", input_str); 

        printf("PARENT SENDING: %s\n", input_str);
        write(fd1[1], input_str, strlen(input_str)+1);
      

        read(fd2[0], concat_str, 100); 
  
        printf("Concatenated string is %s\n", concat_str);
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 
      
        // Read a string using first pipe 
        read(fd1[0], concat_str, 100); 
      
        printf("CHILD RECV: %s\n", concat_str);

        //close(fd1[0]);
  
        // Concatenate a fixed string with it 
        k = strlen(concat_str); 
        for (i=0; i<strlen(fixed_str1); i++) 
            concat_str[k++] = fixed_str1[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Concatenated string is %s\n", concat_str);
      
        printf("Enter a string to concatenate: ");
        scanf("%s", input_str);

        for (i=0; i<strlen(input_str); i++){ 
            concat_str[k++] = input_str[i]; 
        }
        concat_str[k] = '\0'; 
        write(fd2[1], concat_str, strlen(concat_str)+1);
  
        close(fd2[1]);

    } 
} 