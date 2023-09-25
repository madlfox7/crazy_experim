#include "../includes/shelly.h"
#include <limits.h>
#include <string.h>

#define MAX_TOKENS 100000

typedef enum {
    S_WORD,  //some chars that are not metachars 
    t_SPACE, //' '
    PIPE,    // |
    AND,    // &
    SEMICON, //;
    O_C_BRACE, // )
    C_C_BRACE, // (
    LESS_OP, //<
    GREATER_OP, //>
    DOLLAR, //$
    S_QUOTE, //'
    D_QUOTE
} token_type;

typedef struct {
    char *value;
    token_type type;
} t_token;

typedef struct {
    char *str;
    t_token tokens[MAX_TOKENS];
    int count;
}
tokenize;

typedef struct s_shelly
{
    tokenize tokenizer;
} t_shelly;


int     unclosed_quotes(char *input)
{
        int             i;
        int             S_quote;
        int             D_quote;

        i = 0;
        S_quote = 0;
        D_quote = 0;
        while (input[i])
        {
                if (input[i] == '\'' && !D_quote)
                        S_quote = !S_quote;
                else if (input[i] == '\"' && !S_quote)
                        D_quote = !D_quote;
                i++;
        }
        if (S_quote || D_quote)
                return 1;
        return 0;
}


int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

int whitespaces(char *input)
{
    if (input == NULL)
        return 0;

    while (*input != '\0')
    {
        if (!ft_isspace(*input))
            return 0;
        input++;
    }
    return 1;
}

int init_shell(t_shelly *shell)
{
    
    shell = (t_shelly *)malloc(sizeof(t_shelly));

    if (shell == NULL)
    {
        printf("Error: Failed to allocate memory for shell.\n");
        return 1; 
    }

    shell->tokenizer.count = 0;
    shell->tokenizer.str = NULL;

    return 0; 
}


void space(t_shelly *shell, t_token *token)
{
	(void)shell;

    token->type = t_SPACE;
    token->value = strdup(" ");
}

/*void tokenize_func(t_shelly *shell, char *input)
{
    int i = 0;

	(void)shell;
    while (*input)
    {
        if (*input == ' ')
        {
            tokenize_space(shell, &shell->tokenizer.tokens[shell->tokenizer.count]);
            shell->tokenizer.count++;
            i++;
        }
      else if (*input == '\'' || *input == '\"')
        {
            tokenize_brackets(shell, &shell->tokenizer.tokens[shell->tokenizer.count]);
            shell->tokenizer.count++;
            i++;
        }
		else if (*input == '<' || *input == '>')
        {
            tokenize_less_bigger(shell, &shell->tokenizer.tokens[shell->tokenizer.count]);
            shell->tokenizer.count++;
            i++;
        }
        else if (*input == '|' || *input == '$')
        {
            tokenize_pipe_and_dollar(shell, &shell->tokenizer.tokens[shell->tokenizer.count]);
            shell->tokenizer.count++;
            i++;
        }
        else if (ft_isascii(*input) && !is_breaking_character(*input))
        {
            tokenize_bare_word(shell, &shell->tokenizer.tokens[shell->tokenizer.count], input);
            shell->tokenizer.count++;
            i++;
        }
        else
        {
            break;
        }
        input++;
    }
}
*/
/*
void less_or_greater(t_shelly *shell, t_token *token, char *input)
{
    (void)shell;

    if (*input == '>')
    {

    }
    else
    {

    }
}*/

void space_token(t_shelly *shell, t_token *token)
{
	(void)shell;

    token->type = t_SPACE;
    token->value = strdup(" ");
}

void tokenize_func(t_shelly *shell, char *input)
{
    //int i = 0;

    shell->tokenizer.count = 0; // Initialize count to 0
    //check ascii symbols;
    while (*input)
    {
        if (*input == ' ')
        {
            space_token(shell, &shell->tokenizer.tokens[shell->tokenizer.count]);
            shell->tokenizer.count++;
        }

    input++;
    }
}


void echo(char *input)
{
    int i = 0;


    while (input[i])
    {
        //vars can be like ${ARG} too  
        if ((input[i] == '$' && input[i + 1] == '{') || input[i] == '$')
        {

            char *closing_brace = strchr(&input[i + 2], '}');
            if (closing_brace)
            {
               
                size_t var_name_length = closing_brace - (&input[i + 2]);
                char var_name[var_name_length + 1];
                strncpy(var_name, &input[i + 2], var_name_length);
                var_name[var_name_length] = '\0';

                char *env_value = getenv(var_name);
                if (env_value)
                {
                    write(1, env_value, strlen(env_value));
                    i += var_name_length + 3;
                }
                else
                {
                    write(1, &input[i], 1);
                    i++;
                }
            }
            else
            {
                write(1, &input[i], 1);
                i++;
            }
        }
        else
        {
            write(1, &input[i], 1);
            i++;
        }
      
    }
      write(1, "\n", 1);
}


void echo_2(char *input)
{
    int i = 0;
    int suppress_newline = 0; 

    if (strncmp(input, "-n", 2) == 0)
    {
        suppress_newline = 1; 
        i += 2; 
    }

    while (input[i])
    {
        // Vars can be like ${ARG} too
         while (input[i] == 'n' && suppress_newline)
            i++;
        if (input[i] == '\'' || input[i] == '\"')
            i++;
            
        if (input[i] == '$')
        {
            char *delimiter = strchr(&input[i + 1], ' ');
            size_t var_name_length;

        if (delimiter) 
                 var_name_length = (size_t)(delimiter - (&input[i + 1]));
     else 
            var_name_length = strlen(&input[i + 1]);
            char var_name[var_name_length + 1];
            strncpy(var_name, &input[i + 1], var_name_length);
            var_name[var_name_length] = '\0';
            char *env_value = getenv(var_name);
            if (env_value)
            {
                write(1, env_value, strlen(env_value));
                i += var_name_length + 2; 
            }
            else
            {
                write(1, &input[i], 1);
                i++;
            }
        }
        else 
        {
            write(1, &input[i], 1);
            i++;
        }
    }
    if (!suppress_newline)
    {
        write(1, "\n", 1);
    }
}



int main(int argc, char **argv)
{
    (void)argv;
    char *input = NULL;
    t_shelly shell;

    if (argc != 1)
    {
        printf("Usage: ./minishell\n");
        exit(EXIT_SUCCESS);
    }
    logo();
    init_shell(&shell);
    while (1)
    {
		printf("\033[36m");
        input = readline("Minishell$\t\033[33m");
        if (input == NULL)
        {
           printf("Error reading input. Exiting.\n");
            break;
        }
        if (strcmp(input, "exit") == 0)
            break;
        else if (strncmp(input, "echo ", 5) == 0)
        {
            printf("s ================ %s\n", input);
            echo_2(&input[5]); 
        }
           if (unclosed_quotes(input))
           {
                   printf("Error, unclosed brace\n");
                   //it must be added to history? even if not valid input
           }
        if (strlen(input) > 0 && input[0] != '\0' && !whitespaces(input))
        {
            add_history(input);
			
        }
        free(input);
        }
 return 0;
}
