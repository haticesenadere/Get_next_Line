/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdere <hdere@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 21:25:44 by hdere             #+#    #+#             */
/*   Updated: 2025/08/02 16:22:36 by hdere            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*update_stash(char *stash)
{
	int		i;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	new_stash = ft_substr(stash, i + 1, ft_strlen(stash) - i - 1);
	free(stash);
	if (!new_stash)
		return (NULL);
	return (new_stash);
}

static char	*get_line(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	if (!stash[i])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		line = ft_substr(stash, 0, i + 1);
	else
		line = ft_strdup(stash);
	return (line);
}

static char	*extract_and_update(char **stash_ptr)
{
	char	*line;

	if (!(*stash_ptr) || !(*stash_ptr)[0])
	{
		free(*stash_ptr);
		*stash_ptr = NULL;
		return (NULL);
	}
	line = get_line(*stash_ptr);
	if (!line)
	{
		free(*stash_ptr);
		*stash_ptr = NULL;
		return (NULL);
	}
	*stash_ptr = update_stash(*stash_ptr);
	return (line);
}

static char	*read_stash(int fd, char *buffer, char *stash)
{
	ssize_t	bytes_read;
	char	*temp;

	while (!ft_strchr(stash, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(stash);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(stash, buffer);
		free(stash);
		stash = temp;
		if (!stash)
			return (NULL);
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	if (!stash)
		stash = ft_strdup("");
	stash = read_stash(fd, buffer, stash);
	free(buffer);
	if (!stash)
		return (NULL);
	line = extract_and_update(&stash);
	return (line);
}
