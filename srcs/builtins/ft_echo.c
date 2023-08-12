/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/12 17:43:03 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *str)
{
	for (int i = 1; str[i]; i++)
	{
		if (str[i] != 'n')
			return 0;
	}
	return 1;
}

void echo_print(char *str, t_global *global)
{
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (*str)
    {
        if (*str == '\\' && !in_single_quote)
        {
            str++;
            if (in_double_quote)
            {
                switch (*str)
                {
                    case 'n': ft_printf("\n"); break;
                    case 't': ft_printf("\t"); break;
                    // ... (others)
                    default: ft_printf("\\%c", *str);
                }
            }
            else
            {
                ft_printf("%c", *str);
            }
            str++;
            continue;
        }

        if (*str == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            str++;
            continue;
        }
        else if (*str == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            str++;
            continue;
        }

        if (in_single_quote)
        {
            ft_printf("%c", *str++);
            continue;
        }

        if (*str == '$' && !in_single_quote)
        {
			str++;
			if (*str == '?')
				ft_printf("%d", global->exit_code);
			else if (*str == '$')
				ft_printf("%d", global->pid);
			else if ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || (*str >= '0' && *str <= '9') || *str == '_')
			{
				char *end = str;
				while (*end && ((*end >= 'a' && *end <= 'z') || (*end >= 'A' && *end <= 'Z') || (*end >= '0' && *end <= '9') || *end == '_'))
					end++;
				char temp = *end;
				*end = '\0';
				char *var_value = getenv(str);
				if (var_value)
					ft_printf("%s", var_value);
				else
					ft_printf("");
				*end = temp;
				str = end;
			}
			else
				ft_printf("");
			str++;
        }
        else
            ft_printf("%c", *str++);
    }
}


// void echo_print(char *str, t_global *global)
// {
// 	int in_single_quote = 0;
// 	int in_double_quote = 0;

// 	while (*str)
// 	{
// 		if (*str == '\'' && !in_double_quote)
// 		{
// 			in_single_quote = !in_single_quote;
// 			str++;
// 			continue;
// 		}
// 		else if (*str == '\"' && !in_single_quote)
// 		{
// 			in_double_quote = !in_double_quote;
// 			str++;
// 			continue;
// 		}
// 		if (in_single_quote)
// 		{
// 			ft_printf("%c", *str++);
// 			continue;
// 		}

// 		if (*str == '\\')
// 		{
// 			str++;
// 			if (*str == 'n')
// 				ft_printf("\n");
// 			else if (*str == 't')
// 				ft_printf("\t");
// 			else if (*str == 'v')
// 				ft_printf("\v");
// 			else if (*str == 'b')
// 				ft_printf("\b");
// 			else if (*str == 'r')
// 				ft_printf("\r");
// 			else if (*str == 'f')
// 				ft_printf("\f");
// 			else if (*str == 'a')
// 				ft_printf("\a");
// 			else if (*str == '0')
// 				ft_printf("\0");
// 			else if (*str == '\\')
// 				ft_printf("\\");
// 			else
// 				ft_printf("\\%c", *str);
// 			str++;
// 		}
// 		else if (*str == '$' && !in_single_quote)
// 		{
// 			str++;
// 			if (*str == '?')
// 				ft_printf("%d", global->exit_code);
// 			else if (*str == '$')
// 				ft_printf("%d", global->pid);
// 			else if (*str == '0')
// 				ft_printf("%s", global->cmd->token[0]);
// 			else if (*str == '_')
// 				ft_printf("%s", global->cmd->token[0]);
// 			else
// 				ft_printf("$%c", *str);
// 			str++;
// 		}
// 		else
// 		{
// 			ft_printf("%c", *str++);
// 		}
// 	}
// }

void ft_echo(t_global *global)
{
	int newline = 1; 

	global->cmd->token++;
	global->cmd->type++;
	while (global->cmd->token && *global->cmd->type++ == OPTIONS)
	{
		if (check_option(*global->cmd->token))
		{
			newline = 0;
			global->cmd->token++;
		}
		else
			break;
	}
	while (*global->cmd->token)
	{
		echo_print(*global->cmd->token, global);
		global->cmd->token++;
		if (*global->cmd->token)
			ft_printf(" ");
	}
	if (newline)
		ft_printf("\n");
}
