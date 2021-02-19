/*
Peter-Newman Messan
Operating Systems
Sheet 7
quiz.c

Code references: Stack Overflow with some help from the TAs.

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <json-c/json.h>
#define BUF_SIZE 1024
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

//global vars to enable perse json update multiple values
static char problem_str[BUF_SIZE];  
static long prob_ans;
char *readall(int fd)
{
    //read all the data from fd (file descriptor) until eof
    //then store in dynamically allocated string.
    char buffer[BUF_SIZE];
    size_t contentSize = 1; // includes NULL
    // Preallocate space.
    char *content = malloc(sizeof(char) * BUF_SIZE);
    if (content == NULL)
    {
        perror("Failed to allocate content");
        exit(1);
    }
    content[0] = '\0'; // make null-terminated
    while (read(fd, buffer, BUF_SIZE))
    {
        char *old = content;
        contentSize += strlen(buffer);
        content = realloc(content, contentSize);
        if (content == NULL)
        {
            perror("Failed to reallocate content");
            free(old);
            exit(2);
        }
        strcat(content, buffer);
    }

    if (ferror(stdin))
    {
        free(content);
        perror("Error reading from stdin.");
        exit(3);
    }

    return content;
}

void parseJson(char *buffer)
{
    struct json_object *parsed_json;
    struct json_object *text;
    struct json_object *number;

    parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(parsed_json, "text", &text);
    json_object_object_get_ex(parsed_json, "number", &number);

    strcpy(problem_str, json_object_get_string(text));

    int tmp = json_object_get_int(number);
    prob_ans = (long int)tmp;
}

char *fetch()
{
    int pid, pip[2], status;
    // char instring[20];
    char* curl_res = malloc(sizeof(char)*BUF_SIZE);

    //create the pipe
    status = pipe(pip);
    if (status < 0)
    {
        perror("pipe");
        return (char*) EXIT_FAILURE;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (char*) EXIT_FAILURE;
    }
    if (pid == 0)
    { //child process

        status = dup2(pip[1], STDOUT_FILENO);
        if (status == -1)
        {
            perror("dup2");
            return (char*) EXIT_FAILURE;
        }
        close(pip[0]);
        close(pip[1]);
        execlp("curl", "curl", "-s",
               "http://numbersapi.com/random/math?min=1&max=100&fragment&json",
               NULL); //execute the command
        // die("execlp")
        perror("execlp");
        _exit(EXIT_FAILURE);
    }
    else
    { //parent process

        // char buffer[BUF_SIZE] = {0};
        
        close(pip[1]);

        curl_res = readall(pip[0]);
        
        
        do
        {
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        
        // close(1);
    }
    
    
    return curl_res;
}

unsigned play(unsigned n, unsigned score, char* text, long answer)
{
    //post the question, iterate through attempts (4) then return updated score
    int n_attempts = 0, i, n_flag = 0, q_score = 0;
    char* ans_str;
    long ans_atmpt=0;

    ans_str = malloc(sizeof(char)*BUF_SIZE);

    n++;    //increase question number

    printf("%s\n", text);
    while (n_attempts < 4)
    {
        
        
        scanf("%s", ans_str); 
        
        //now we check if input is valid
        for (i = 0; i < strlen(ans_str); i++)
        {
            // printf("here!\n");
            if (!isdigit(ans_str[i]))
            {
                printf("Entered input is not a number\n");
                n_flag = 1;
                break;
            }
        }
        if (n_flag == 0)
            // Given input is a number
            ans_atmpt = atol(ans_str);
        else
        {
            do
            {
                printf("Please enter a number:");
                scanf("%s", ans_str);
                n_flag = 0;
                for (i = 0; i < strlen(ans_str); i++)
                {
                    if (!isdigit(ans_str[i]))
                    {
                        printf("Entered input is not a number\n");
                        n_flag = 1;
                        break;
                    }
                }
            } while (n_flag == 1);
            ans_atmpt = atol(ans_str);
        }

        //check if ans_str is correct
        if (ans_atmpt == answer)
        {
            printf("Congratulations! Your answer %li is correct\n", ans_atmpt);
            switch (n_attempts)
            {
            case 1:
                q_score = 8; //full points
                break;
            case 2:
                q_score = 4; //4 points
                break;
            case 3:
                q_score = 2; //2 points
                break;
            case 4:
                q_score = 1; //one point
                break;
            default:
                break;
            }
            score += q_score;
        }
        else
        {
            if (ans_atmpt < answer)
                printf("Too small, try again.\n");
            else //too large
                printf("Too large, try again.\n");
        }
        n_attempts++;
    }

    //get here when n_attempts=4
    printf("The correct answer is: %li\n", answer);
    printf("Current score: %u / %d\n", score, n*8);
    return score;
}

int main()
{
    unsigned score = 0, q_number = 0;
    char c;
    char buffer[BUF_SIZE];
    
    
    // loop runs until read EOF
    printf("\nPress Enter to start!\n\n");
    while((c = read(0, buffer, BUF_SIZE)) > 0){
        parseJson(fetch());

        score = play(q_number, score, problem_str, prob_ans);
        
        //ask for next round
        printf("Next Question? y/n\n");
        scanf(" %c", &c);

        getchar();
        printf("%c", c);
        switch (c)
        {
        case 'y':
            printf("\nOk, here we go!\nPress Enter to continue!\n");
            break;
        default:
            printf("Bye!\n");
            exit(1);
            break;

        }
    }


    return 0;
}