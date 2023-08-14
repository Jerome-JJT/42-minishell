/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/12 16:50:51 by mvillarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ✅: display prompt when minishell is ready to receive input
// ✅: read input
// ❌: parse input
// ❌: Search and launch the right executable (based on the PATH variable
//		 or by using relative or absolute path)
// ❌: display output
// ❌: loop
// ❌: Have a working history
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
// BONUS: Implement && and ||
// BONUS: Implement Wilcard * (globbing)

t_cmd	*init_cmds(char **cmds)
{
  t_cmd	*cmd;

  cmd = ft_gc_malloc(sizeof(t_cmd));
  if (!cmd)
    return (NULL);
  cmd->args = cmds;
  cmd->cmd = NULL;
  cmd->redir_in = NULL;
  cmd->redir_out = NULL;
  cmd->redir_append = NULL;
  cmd->pipe = NULL;
  cmd->next = NULL;
  return (cmd);
}

t_env	*init_env(char **envp)
{
  t_env	*env;
  int i;

  i = -1;
  env = ft_gc_malloc(sizeof(t_env));
  if (!env)
    return (NULL);  while (envp[++i])
  {
    if (!ft_strncmp(envp[i], "PATH=", 5))
      env->path = ft_split(envp[i] + 5, ':');
    else if (!ft_strncmp(envp[i], "HOME=", 5))
      env->home = ft_strdup(envp[i] + 5);
    else if (!ft_strncmp(envp[i], "PWD=", 4))
      env->pwd = ft_strdup(envp[i] + 4);
    else if (!ft_strncmp(envp[i], "USER=", 5))
      env->user = ft_strdup(envp[i] + 5);
    else if (!ft_strncmp(envp[i], "SHELL=", 6))
      env->shell = ft_strdup(envp[i] + 6);
    else if (!ft_strncmp(envp[i], "OLDPWD=", 7))
      env->oldpwd = ft_strdup(envp[i] + 7);
  }
  return (env);
}

bool	check_args(char *line)
{
	if (!line)
	{
		free(line);
		ft_printf("Error while loading argument, exiting...\n");
		return false;
	}
	return true;
}

bool	check_exit_args(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", arg);
			return (false);
		}
	}
	return (true);
}

void	execute(t_cmd *cmd)
{
	(void) cmd;
}

int	lsh_loop(void)
{
	char	*line;
	t_cmd	*cmd;
	int		err_code;

	err_code = 0;
	while (1)
	{
		line = readline(PROMPT);
		if (!check_args(line))
			break ;
		cmd = init_cmds(ft_split(line, ' '));
		free(line);
		if (!ft_strcmp(cmd->args[0], "exit"))
		{
			ft_printf("exit\n");
			if (!cmd->args[1])
				break ;
			if (!check_exit_args(cmd->args[1]))
				err_code = 255;
			else if (cmd->args[2])
			{
				ft_printf("minishell: exit: too many arguments\n");
				continue ;
			}
			else if (cmd->args[1])
			{
				err_code = ft_atoi(cmd->args[1]);
			 	if (err_code < 0 || err_code > 255)
					err_code %= 256;
			}
			break ;
		}
		if (cmd->args[0])
			add_history(*cmd->args);
	}
	return (err_code);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;

	(void)ac;
	(void)av;
	env = init_env(envp);
        set_termios();
        ft_signal();
	if (!env)
		return (1);
<<<<<<< HEAD
	int err_code = lsh_loop();
	exit (err_code);
}
=======
	lsh_loop();
	return (0);
}

>>>>>>> origin/maria
