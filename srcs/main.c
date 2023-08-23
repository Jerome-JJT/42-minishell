/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:32:20 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/23 11:41:18 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ✅: display prompt when minishell is ready to receive input
// ✅: read input
// ✅: parse input
// ✅: Search and launch the right executable (based on the PATH variable
//			or by using relative or absolute path)
// ✅: display output
// ✅: loop
// ✅: Have a working history
// ✅: Do not use more than one global variable, think about it and be ready
//			to explain why you do it.
// ✅: Do not interpret unclosed quotes or unspecified special characters
//			like \ (eg with $’\n’)
// ✅: Handle ' and " (quotes) correctly
// ❌: Handle redirections > >> < <<
// ❌: Handle pipes | correctly
// ✅: Handle environment variables ($ followed by characters)
// ✅: Handle $? (exit code of the previous program)
// ✅: Handle ctrl-C ctrl-D ctrl-\ correctly
// ✅: Implement echo with option ’-n’
// ✅: Implement cd with only a relative or absolute path
// ✅: Implement pwd without any options
// ✅: Implement export without any options
// ✅: Implement unset without any options
// ✅: Implement env without any options and any arguments
// ✅: Implement exit without any options
// ✅: exit
// BONUS
// ❌: Detect Wilcard * (globbing)
// ❌: Detect && and ||

static int	line_is_wspaces(char *line)
{
	while (*line)
	{
		if (!ft_isspace(*line++))
			return (0);
	}
	return (1);
}

static char	*rm_newline(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
		{
			line[i] = '\0';
			break ;
		}
		i++;
	}
	return (line);
}

void	add_to_history_list(t_history **head, char *line)
{
	t_history	*new_entry;
	t_history	*current;

	new_entry = ft_gc_malloc(sizeof(t_history));
	new_entry->line = ft_strdup(line);
	new_entry->next = NULL;
	if (!(*head))
		*head = new_entry;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_entry;
	}
}

// Linear Feedback Shift Register
int	get_random(void)
{
	static unsigned int	lfsr;
	unsigned int		bit;

	if (!lfsr)
		lfsr = 0xACE1u;
	bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
	lfsr = (lfsr >> 1) | (bit << 15);
	return (lfsr % 4);
}

static int	lsh_loop(t_global *global)
{
	char		*line;
	char		*rdm_prompt_clr;
	int			history_fd;
	t_history	*history_head;
	t_history	*last_entry;

	history_head = NULL;
	history_fd = open(".minishell_history", O_CREAT | O_RDWR, 0644);
	if (history_fd == -1)
		ft_printf("minishell: can't open history file\n");
	else
	{
		line = get_next_line(history_fd);
		while (line)
		{
			line = rm_newline(line);
			add_history(line);
			add_to_history_list(&history_head, line);
			line = get_next_line(history_fd);
		}
	}
	while (1)
	{
		rdm_prompt_clr = ft_strjoin(ft_strjoin(ft_strjoin("\033[", ft_itoa(get_random() % 7 + 31)), "m"), PROMPT);
		line = readline(rdm_prompt_clr);
		if (!check_token(line))
			break ;
		if (line_is_wspaces(line))
			continue ;
		if (line && ft_strcmp(line, ""))
		{
			last_entry = history_head;
			while (last_entry && last_entry->next)
				last_entry = last_entry->next;
			if (!last_entry || ft_strcmp(line, last_entry->line))
			{
				add_history(line);
				ft_putendl_fd(line, history_fd);
				add_to_history_list(&history_head, line);
			}
		}
		if (!ft_strcmp(line, ""))
			continue ;
		global->line = init_line(line, global);
		if (!global->line)
		{
			global->exit_code = 258;
			continue ;
		}
		free(line);
		parse_cmd(global, global->line);
		run_cmd(global);
	}
	return (global->exit_code);
}

int	main(int ac, char **av, char **envp)
{
	t_global	*global;
	int			err_code;

	(void)ac;
	(void)av;
	set_termios();
	ft_signal();
	global = init_global(envp);
	if (!global)
		return (1);
	err_code = lsh_loop(global);
	exit(err_code);
}
