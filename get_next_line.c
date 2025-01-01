/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxoph <maxoph@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 15:48:34 by mdsiurds          #+#    #+#             */
/*   Updated: 2024/12/31 16:52:11 by maxoph           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr(const char *s, int c)
{
	size_t i;

	i = 0;
	while (i < ft_strlen(s))
	{
		if (s[i] != (char)c)
			i++;
		else
			return (i);
	}
	if (s[i] == (char)c)
		return (i);
	return (-1);
}
char *get_next_line(int fd)
{
    static char *to_save = NULL;
    char *temp_read;
    char *temp_save;
    int bytes_read;
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);

    temp_read = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!temp_read)
        return (NULL);
    bytes_read = 1;
	int newline_pos;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, temp_read, BUFFER_SIZE);
		if (bytes_read < 0)
    	{
			free(temp_read);
			free(to_save);
			to_save = NULL;
			return (NULL);
    	}
		if (bytes_read == 0)
			break;
			
		temp_read[bytes_read] = '\0';
		temp_save = ft_strjoin(to_save, temp_read);
		if (!temp_save)
    	{
			free(temp_read);
			free(to_save);
			return (NULL);
    	}
		free(to_save);
		to_save = temp_save;
		newline_pos = ft_strchr(temp_read, '\n');
		if (newline_pos != -1)
			break;
	}
    free(temp_read);
    if (!to_save || !*to_save)
        return (NULL);
    // Chercher le '\n'
    int newline_index = ft_strchr(to_save, '\n');
    if (newline_index == -1)
    {
        // Pas de '\n', on renvoie tout
        line = to_save;
        to_save = NULL;
        return (line);
    }

    // Si on trouve un '\n'
    line = ft_substr(to_save, 0, newline_index + 1);
    if (!line)
    {
        free(to_save);
        return (NULL);
    }

    // Préparer to_save pour le prochain appel
     char *new_to_save = NULL;
	if (to_save[newline_index + 1] != '\0')
	{
		char *start = &to_save[newline_index + 1];
		char *temp = start;
		size_t len = 0;
		
		while (*temp++)
			len++;
		
		new_to_save = malloc(len + 1);
		if (!new_to_save)
		{
			free(line);
			free(to_save);
			return (NULL);
		}
		
		char *dest = new_to_save;
		while (*start)
			*dest++ = *start++;
		*dest = '\0';
	}

	free_all(to_save);
	to_save = new_to_save;
	return (line);
}
