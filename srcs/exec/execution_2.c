/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 01:21:32 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/28 02:00:26 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int execute_cmd(char *cmd, t_redirection *redir, t_global *global)
{
	pid_t	pid;
	int		status;
	char	*argv[100];
	int		i;
	char	*cmd_ptr;
	int		j;
	char	**paths;
	char	*path;

	i = 0;
	j = 0;
	cmd_ptr = cmd;
	j = 0;
	while (global->line->token[j])
	{
		if (ft_strncmp(cmd_ptr, global->line->token[j], ft_strlen(global->line->token[j])) == 0)
		{
			argv[i] = global->line->token[j];
			i++;
			cmd_ptr += ft_strlen(global->line->token[j]);
			if (*cmd_ptr == ' ')
				cmd_ptr++;
		}
		j++;
	}
	argv[i] = NULL;
	if (!global->env)
	{
		ft_printf("minishell: %s: No such file or directory\n", argv[0]);
		global->exit_code = 127;
		return (global->exit_code);
	}
	paths = env_to_char(global);
	path = get_path(argv[0], paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", argv[0]);
		global->exit_code = 127;
		return (global->exit_code);
	}
	pid = fork();
	manage_pid(&pid);
	if (!pid)
	{
		if (redir)
		{
			int fd;
			switch (redir->type)
			{
				case HEREDOC_REDIRECTION:
					ft_heredoc(redir->filename, redir->limiter, redir->type_hd);
					exit (EXIT_SUCCESS);
					break;
				case OUTPUT_REDIRECTION:
					fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd == -1)
					{
						ft_printf("minishell: %s: %s\n", redir->filename, strerror(errno));
						global->exit_code = 1;
						exit (EXIT_FAILURE);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
					break;
				case APPEND_REDIRECTION:
					fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fd == -1)
					{
						ft_printf("minishell: %s: %s\n", redir->filename, strerror(errno));
						global->exit_code = 1;
						exit(EXIT_FAILURE);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
					break;
				case INPUT_REDIRECTION:
					fd = open(redir->filename, O_RDONLY);
					if (fd == -1)
					{
						ft_printf("minishell: %s: %s\n", redir->filename, strerror(errno));
						global->exit_code = 1;
						exit(EXIT_FAILURE);
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
					break;
				case NO_REDIRECTION:
					break;
			}
		}
		execve(path, argv, NULL);
		perror("execve");
		global->exit_code = EXIT_FAILURE;
		exit (global->exit_code);
	}
	else if (pid < 0)
	{
		perror("fork");
		return -1;
	}
	else
	{
		waitpid(pid, &status, 0);
		if (manage_exit(NULL) != 130)
			return (WEXITSTATUS(status));
		else
			return (manage_exit(NULL));
	}
}

static void ft_redir(t_global *global, t_cmds *curr_cmd)
{
	execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
}

static void ft_or(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) != 0)
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

static void	ft_and(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	if (execute_cmd(curr_cmd->cmd, curr_cmd->redir, global) == 0)
		execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

static void execute_pipeline(t_global *global, t_cmds *cmds)
{
	int num_cmds = 0;
	t_cmds *tmp = cmds;
	while (tmp)
	{
		num_cmds++;
		tmp = tmp->next;
	}
	int fds[num_cmds - 1][2];
	int i = -1;
	while (++i < num_cmds - 1)
	{
		if (pipe(fds[i]) < 0)
		{
			perror("pipe");
			global->exit_code = EXIT_FAILURE;
			exit (global->exit_code);
		}
	}
	i = -1;
	while (++i < num_cmds)
	{
		pid_t pid = fork();
		if (!pid)
		{
			if (i != 0)
			{
				dup2(fds[i - 1][0], STDIN_FILENO);
				close(fds[i - 1][0]);
			}
			if (i != num_cmds - 1)
			{
				dup2(fds[i][1], STDOUT_FILENO);
				close(fds[i][1]);
			}
			int j = -1;
			while (++j < num_cmds - 1)
			{
				if (j != i - 1) close(fds[j][0]);
				if (j != i) close(fds[j][1]);
			}
			execute_cmd(cmds->cmd, cmds->redir, global);
			global->exit_code = EXIT_SUCCESS;
			exit (global->exit_code);
		}
		else if (pid < 0)
		{
			perror("fork");
			global->exit_code = EXIT_FAILURE;
			exit (global->exit_code);
		}
		cmds = cmds->next;
	}
	i = -1;
	while (++i < num_cmds - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
	}
	i = -1;
	while (++i < num_cmds)
		wait(NULL);
}

static void	ft_pipe(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	int fds[2];
	if (pipe(fds) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_t pid = fork();
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

	pid_t pid2 = fork();
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
	close(fds[0]);
	close(fds[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}


static void	ft_semicolon(t_global *global, t_cmds *curr_cmd, t_cmds *next_cmd)
{
	execute_cmd(curr_cmd->cmd, curr_cmd->redir, global);
	execute_cmd(next_cmd->cmd, next_cmd->redir, global);
}

static void	execute_specials(t_global *global)
{
	t_count	*count_tmp;
	t_cmds 	*curr_cmd;
	t_token	*type_tmp;

	count_tmp = global->line->count;
	curr_cmd = global->line->cmds;
	type_tmp = global->line->type;
	while (count_tmp->special_cases)
	{
		while (!(*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp == APPEND || *type_tmp == HEREDOC || *type_tmp == AND || *type_tmp == OR || *type_tmp == SEMICOLON || *type_tmp == PIPE) && *type_tmp != END)
			type_tmp++;
		if (*type_tmp == INPUT || *type_tmp == HEREDOC || *type_tmp == APPEND || *type_tmp == OUTPUT)
		{
			if (count_tmp->nbr_inputs > 0 && *type_tmp == INPUT)
				count_tmp->nbr_inputs--;
			else if (count_tmp->nbr_heredocs > 0 && *type_tmp == HEREDOC)
				count_tmp->nbr_heredocs--;
			else if (count_tmp->nbr_appends > 0 && *type_tmp == APPEND)
				count_tmp->nbr_appends--;
			else if (count_tmp->nbr_outputs > 0 && *type_tmp == OUTPUT)
				count_tmp->nbr_outputs--;
			ft_redir(global, curr_cmd);
			return ;
		}
		else if (count_tmp->nbr_ors > 0 && *type_tmp == OR)
		{
			count_tmp->nbr_ors--;
			ft_or(global, curr_cmd, curr_cmd->next);
			if (global->exit_code == 0)
				return;
			if (curr_cmd->next)
			{
				curr_cmd = curr_cmd->next;
				type_tmp++;
				while (*type_tmp != OR && *type_tmp != END)
					type_tmp++;
				if (*type_tmp == END)
					return;
			}
		}
		else if (count_tmp->nbr_ands > 0 && *type_tmp == AND)
		{
			count_tmp->nbr_ands--;
			ft_and(global, curr_cmd, curr_cmd->next);
			if (curr_cmd->next)
			{
				curr_cmd = curr_cmd->next;
				type_tmp++;
				while (*type_tmp != AND && *type_tmp != END)
					type_tmp++;
				if (*type_tmp == END)
					return;
			}
		}
		else if (count_tmp->nbr_semicolons > 0 && *type_tmp == SEMICOLON)
		{
			count_tmp->nbr_semicolons--;
			ft_semicolon(global, curr_cmd, curr_cmd->next);
		}
		else if (count_tmp->nbr_pipes > 0 && *type_tmp == PIPE)
		{
			if (count_tmp->nbr_pipes > 1)
				execute_pipeline(global, curr_cmd);
			else
				ft_pipe(global, curr_cmd, curr_cmd->next);
			count_tmp->nbr_pipes = 0;
		}
		if (curr_cmd->next)
			curr_cmd = curr_cmd->next;
		type_tmp++;
		if (!count_tmp->nbr_inputs && !count_tmp->nbr_outputs
			&& !count_tmp->nbr_appends && !count_tmp->nbr_heredocs
			&& !count_tmp->nbr_ors && !count_tmp->nbr_ands
			&& !count_tmp->nbr_semicolons && !count_tmp->nbr_pipes)
			count_tmp->special_cases = false;
	}
}

void	run_cmd(t_global *global)
{
	int primaries;

	global->exit_code = 0;
	manage_exit(&global->exit_code);
	if (global->line->count->special_cases == true)
	{
		execute_specials(global);
		return ;
	}
	primaries = cmd_is_primaries(global->line->cmds->cmd);
	if (primaries)
	{
		execute_primaries(global->line->cmds->cmd, global);
		return ;
	}
	if (!get_env_value("PATH", global->env))
	{
		ft_printf("minishell: %s: No such file or directory\n", global->line->cmds->cmd);
		global->exit_code = 127;
		manage_exit(&global->exit_code);
		return ;
	}
	global->exit_code = execute_cmd(global->line->cmds->cmd, global->line->cmds->redir, global);
	manage_exit(&global->exit_code);
}
