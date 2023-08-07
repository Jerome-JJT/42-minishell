/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:32:20 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/07 17:39:06 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ✅: display prompt when minishell is ready to receive input
// ✅: read input
// ✅: parse input
// ❌: Search and launch the right executable (based on the PATH variable
//		 or by using relative or absolute path)
// ❌: display output
// ✅: loop
// ✅: Have a working history
// ❌: Do not use more than one global variable, think about it and be ready
//		 to explain why you do it.
// ❌: Do not interpret unclosed quotes or unspecified special characters
//		 like \ (eg with $’\n’)
// ❌: Handle ' and " (quotes) correctly
// ❌: Handle redirections > >> < <<
// ❌: Handle pipes | correctly
// ❌: Handle environment variables ($ followed by characters)
// ❌: Handle $? (exit code of the previous program)
// ❌: Handle ctrl-C ctrl-D ctrl-\ correctly
// ❌: Implement echo with option ’-n’
// ❌: Implement cd with only a relative or absolute path
// ❌: Implement pwd without any options
// ❌: Implement export without any options
// ❌: Implement unset without any options
// ❌: Implement env without any options and any arguments
// ✅: Implement exit without any options
// ✅: exit
// BONUS
// ✅: Detect Wilcard * (globbing)
// ✅: Detect && and ||
// BONUS: Implement && and ||
// BONUS: Implement Wilcard * (globbing)

void	execute(t_cmd *cmd)
{
	(void) cmd;
}

void	run_cmd(t_global *global)
{
	if (!ft_strcmp(global->cmd->token[0], "echo"))
		ft_echo(global->cmd);
	else if (!ft_strcmp(global->cmd->token[0], "cd"))
		ft_cd(global->cmd);
	else if (!ft_strcmp(global->cmd->token[0], "pwd"))
		ft_pwd(global->cmd);
	else if (!ft_strcmp(global->cmd->token[0], "export"))
		ft_export(global->cmd);
	else if (!ft_strcmp(global->cmd->token[0], "unset"))
		ft_unset(global->cmd);
	else if (!ft_strcmp(global->cmd->token[0], "env"))
		ft_env(global);
	else if (!ft_strcmp(global->cmd->token[0], "exit"))
		ft_exit(global);
	else
		execute(global->cmd);
}

int	lsh_loop(t_global *global)
{
	char	*line;

	while (1)
	{
		line = readline(PROMPT);
		if (!check_token(line))
			break ;
		if (line && ft_strcmp(line, ""))
			add_history(line);
		line = epur_str(line);
		if (!ft_strcmp(line, ""))
			continue ;
		global->cmd = init_cmds(ft_split(line, ' '));
		if (!global->cmd)
		{
			global->exit_code = 258;
			continue ;
		}
		print_infos(global->cmd); // delete this line when done
		free(line);
		parse_cmd(global, global->cmd);
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
	global = init_global(envp);
	if (!global)
		return (1);
	err_code = lsh_loop(global);
	return (err_code);
}
