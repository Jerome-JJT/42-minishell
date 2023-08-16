/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/16 10:31:45 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**env_to_char(t_global *global)
{
	char	**paths;

	while (ft_strcmp(global->env->name, "PATH"))
	{
		global->env = global->env->next;
		if (!global->env)
			return (NULL);
	}
	paths = ft_gc_malloc(sizeof(char *) * (ft_strlen(global->env->value) + 1));
	if (!paths)
		return (NULL);
	paths = ft_split(global->env->value, ':');
	return (paths);
}

static char	*get_path(char *command, char **paths)
{
	char	*path;

	while (*paths)
	{
		path = ft_strjoin(*paths, "/");
		path = ft_strjoin(path, command);
		if (access(path, F_OK) == 0)
			return (path);
		paths++;
	}
	return (NULL);
}

static void	execute_pipe(t_global *global, char **paths)
{
	(void) paths;
	(void) global;
	ft_printf("execute_pipe\n");
}

static void	pid_working(char *path, char **paths, t_global *global)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		if (execve(path, global->line->token, paths) == -1)
		{
			ft_printf("minishell: %s: %s\n", *global->line->token,
				strerror(errno));
			global->exit_code = 126;
		}
	}
	else if (pid == -1)
	{
		ft_printf("minishell: %s: %s\n", *global->line->token, strerror(errno));
		global->exit_code = 126;
	}
	else
		waitpid(pid, &global->exit_code, 0);
}

static void	execute(t_global *global)
{
	char	*path;
	char	**paths;

	paths = env_to_char(global);
	if (!paths)
	{
		ft_printf("minishell: %s: command not found\n", *global->line->token);
		global->exit_code = 127;
		return ;
	}
	path = get_path(*global->line->token, paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", *global->line->token);
		global->exit_code = 127;
	}
	if (global->line->nbr_cmd > 1)
	{
		execute_pipe(global, paths);
		return ;
	}
	pid_working(path, paths, global);
}

void	run_cmd(t_global *global)
{
	int	i;

	i = 0;
	// while (global->line->cmds->cmd[i])
	// {
	// 	if (global->line->cmds->cmd[i] == '|')
	// 	{
	// 		global->line->nbr_cmd++;
	// 		global->line->cmds->cmd[i] = '\0';
	// 	}
	// 	i++;
	// }
	if (!ft_strcmp(global->line->token[0], "echo"))
		ft_echo(global);
	else if (!ft_strcmp(global->line->token[0], "cd"))
		ft_cd(global);
	else if (!ft_strcmp(global->line->token[0], "pwd"))
		ft_pwd(global->line);
	else if (!ft_strcmp(global->line->token[0], "export"))
		ft_export(global, global->line);
	else if (!ft_strcmp(global->line->token[0], "unset"))
		ft_unset(global, global->line);
	else if (!ft_strcmp(global->line->token[0], "env"))
		ft_env(global);
	else if (!ft_strcmp(global->line->token[0], "exit"))
		ft_exit(global);
	else
		execute(global);
}
