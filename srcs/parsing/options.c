/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:43:52 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/25 14:28:21 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*format_options(char *token)
{
	char	*new_token;
	char	*dst;
	char	*scan;

	new_token = ft_gc_malloc(sizeof(char) * (ft_strlen(token) + 1));
	if (!new_token)
		return (NULL);
	dst = new_token;
	while (*token)
	{
		scan = token + 1;
		while (*scan && *scan != *token)
			scan++;
		if (!*scan)
			*dst++ = *token;
		token++;
	}
	*dst = '\0';
	return (new_token);
}
