/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_pid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 17:47:31 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/30 19:42:21 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pid_creation(t_global *global, char **paths, char *argv[],
		t_redirection *redir)
{
	pid_t	pid;
	int		status;
	char	**env_array;

	g_current_state = STATE_BLOCKING_CMD;
	pid = fork();
	manage_pid(&pid);
	if (!pid)
	{
		if (handle_redirections(redir, global))
			return (manage_exit(NULL));
		env_array = env_list_to_array(global->env);
		execve(found_command(global, paths, argv), argv, env_array);
		if (manage_exit(NULL) == 127)
			exit(127);
		global->exit_code = EXIT_FAILURE;
		exit(global->exit_code);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	waitpid(pid, &status, 0);
	g_current_state = STATE_NORMAL;
	if (manage_exit(NULL) != 130)
		return (WEXITSTATUS(status));
	return (manage_exit(NULL));
}

pid_t	manage_pid(pid_t *new_pid)
{
	static pid_t	child_pid;

	if (new_pid)
		child_pid = *new_pid;
	return (child_pid);
}

void	check_first_pid(pid_t pid, int fds[2], t_cmds *curr_cmd,
	t_global *global)
{
	pid = fork();
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	check_second_pid(pid_t pid2, int fds[2], t_cmds *next_cmd,
	t_global *global)
{
	pid2 = fork();
	if (pid2 == 0)
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
		exit(EXIT_SUCCESS);
	}
	else if (pid2 < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}
