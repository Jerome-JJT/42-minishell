/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:19:43 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/25 17:09:36 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_remove_char(char *str, char c)
{
	int		len;
	char	*new_str;

	int i, j;
	len = ft_strlen(str);
	new_str = (char *)ft_gc_malloc(len + 1);
	if (!new_str)
		return (NULL);
	j = 0;
	for (i = 0; i < len; i++)
	{
		if (str[i] != c)
		{
			new_str[j] = str[i];
			j++;
		}
	}
	new_str[j] = '\0';
	return (new_str);
}

// 9223372036854775807 is the max value of a long long int (64 bits)
// -9223372036854775807 is the min value of a long long int (64 bits)
// static bool	check_exit_token(char *arg)
// {
// 	char *tmp;
// 	char *max_ll_str;
// 	char *min_ll_str;
// 	int i = 0;

// 	tmp = arg;
// 	while (tmp[i])
// 	{
// 		char c = *tmp;
// 		if (tmp[i] == '\"' || tmp[i] == '\'' || tmp[i] == ' ')
// 			tmp = ft_remove_char(tmp, c);
// 		i++;
// 	}
// 	arg = tmp;
// 	max_ll_str = "9223372036854775807";
// 	min_ll_str = "-9223372036854775807";
// 	if (!*tmp)
// 		return (false);
// 	while (*tmp)
// 	{
// 		if ((*tmp != ' ' && !ft_isdigit(*tmp)) || ((*tmp == '-' || *tmp == '+')
//				&& *(tmp + 1) == *tmp))
// 		{
// 			return (false);
// 		}
// 		tmp++;
// 	}
// 	if (ft_strlen(arg) > ft_strlen(min_ll_str))
// 		return (false);
// 	else if (ft_strlen(arg) == ft_strlen(max_ll_str))
// 	{
// 		int i = 1;
// 		while (arg[i])
// 		{
// 			if (arg[i] > max_ll_str[i])
// 				return (false);
// 			i++;
// 		}
// 	}
// 	else if (ft_strlen(arg) == ft_strlen(min_ll_str))
// 	{
// 		int i = 1;
// 		while (arg[i])
// 		{
// 			if (arg[i] > min_ll_str[i])
// 				return (false);
// 			i++;
// 		}
// 	}
// 	if (!*tmp)
// 		return (false);
// 	while (*tmp)
// 	{
// 		if (!ft_isdigit(*tmp) && *tmp != '-' && *tmp != '+')
// 			return (false);
// 		tmp++;
// 	}
// 	return (true);
// }

static bool	check_exit_token(char *arg)
{
	char	*tmp;
	char	*max_ll_str;
	char	*min_ll_str;
	int		i;
	bool	is_negative;

	i = 0;
	is_negative = false;
	tmp = arg;
	if (*tmp == '-')
	{
		is_negative = true;
		tmp++;
	}
	else if (*tmp == '+')
		tmp++;
	while (*tmp && (ft_isdigit(*tmp) || *tmp == ' ' || *tmp == '\"'
			|| *tmp == '\''))
		tmp++;
	if (*tmp)
		return (false);
	max_ll_str = "9223372036854775807";
	min_ll_str = "9223372036854775808";
	if (is_negative && ft_strlen(arg) - 1 > ft_strlen(min_ll_str))
		return (false);
	else if (!is_negative && ft_strlen(arg) > ft_strlen(max_ll_str))
		return (false);
	else if (is_negative && ft_strlen(arg) - 1 == ft_strlen(min_ll_str))
	{
		i = 1;
		while (arg[i])
		{
			if (arg[i] > min_ll_str[i - 1])
				return (false);
			i++;
		}
	}
	else if (!is_negative && ft_strlen(arg) == ft_strlen(max_ll_str))
	{
		i = 0;
		while (arg[i])
		{
			if (arg[i] > max_ll_str[i])
				return (false);
			i++;
		}
	}
	return (true);
}

void	ft_exit(t_global *global)
{
	int		i;
	char	c;

	i = 1;
	global->exit_code = manage_exit(NULL);
	ft_printf("exit\n");
	if (global->line->token[i + 1] && global->line->type[i] == IGNORE)
		i++;
	if (!global->line->token[i] || (!ft_strcmp(global->line->token[1], "--")
			&& !global->line->token[2]))
		exit(global->exit_code);
	else if (!check_exit_token(global->line->token[i]))
	{
		c = *global->line->token[i];
		if (*global->line->token[i] == '\"' || *global->line->token[i] == '\'')
			global->line->token[i] = ft_remove_char(global->line->token[i], c);
		ft_printf("minishell: exit: %s: numeric argument required\n",
			global->line->token[i]);
		global->exit_code = 255;
	}
	else if (global->line->token[i + 1])
	{
		ft_printf("minishell: exit: too many arguments\n");
		return ;
	}
	else
	{
		c = *global->line->token[i];
		if (*global->line->token[i] == '\"' || *global->line->token[i] == '\'')
			global->line->token[i] = ft_remove_char(global->line->token[i], c);
		global->exit_code = ft_atoi(global->line->token[i]);
		if (global->exit_code < 0 || global->exit_code > 255)
			global->exit_code %= 256;
	}
	exit(manage_exit(&global->exit_code));
}
