/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 09:47:24 by nesdebie          #+#    #+#             */
/*   Updated: 2023/06/08 15:10:58 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	exit_handler(void)
{
	ft_putendl_fd("./pipex file1 cmd1 cmd2 file2", 2);
	exit(EXIT_SUCCESS);
}

static int	no_path(char **av, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 4))
			return (EXIT_SUCCESS);
		i++;
	}
	i = 2;
	while (av[i + 1])
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(av[i], 2);
		ft_putendl_fd(" : No such file or directory", 2);
		i++;
	}
	return (EXIT_FAILURE);
}

int	open_file(char *file, int in_out, char **av, char **envp)
{
	int	ret;

	if (no_path(av, envp))
		exit(127);
	ret = 0;
	if (in_out == 0)
		ret = open(file, O_RDONLY, 0777);
	if (in_out == 1)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (in_out == 2)
		ret = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (ret == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(file, 2);
		ft_putendl_fd(": Permission denied", 2);
		exit(0);
	}
	return (ret);
}

char	*ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (0);
}

char	*get_path(char *cmd, char **envp, int i)
{
	char	**paths;
	char	*path;
	char	*part_path;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	while (!ft_strnstr(envp[i], "PATH", 4))
		i++;
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (0);
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i++], "/");
		path = ft_strjoin(part_path, cmd);
		if (part_path)
			free(part_path);
		if (!access(path, F_OK))
			return (path);
		if (path)
			free(path);
	}
	return (ft_free_tab(paths));
}
