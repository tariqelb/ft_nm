/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_display_output.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 18:34:16 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/05 19:23:44 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	ft_clear_output(t_output **head)
{
	t_output	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		free((*head)->name);
		free(*head);
		*head = tmp;
	}
}

t_output	*ft_new_elem(long int addr, char type, char *name)
{
	t_output	*node;

	node = malloc(sizeof(t_output));
	if (!node)
		return (NULL);
	node->addr = addr;
	node->type = type;
	node->name = strdup(name);
	node->next = NULL;
	return (node);
}

void	ft_add_new_elem(t_output **head, long int addr, char type, char *name)
{
	t_output	*new_node;
	t_output	*tmp;

	new_node = ft_new_elem(addr, type, name);
	if (!new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

int	ft_compare_nm(char *s1, char *s2)
{
	char	*p1;
	char	*p2;
	int		res;

	p1 = s1;
	p2 = s2;
	while (*p1 && *p2)
	{
		while (*p1 && !isalnum(*p1))
			p1++;
		while (*p2 && !isalnum(*p2))
			p2++;
		if (!*p1 || !*p2)
			break ;
		if (tolower(*p1) != tolower(*p2))
			return (tolower(*p1) - tolower(*p2));
		p1++;
		p2++;
	}
	res = strcmp(p1, p2);
	if (res == 0)
		return (strcmp(s1, s2));
	else
		return (res);
}

void	ft_display_output(t_output *data)
{
	while (data != NULL)
	{
		if (data->type == 'U' || data->type == 'w' || data->type == 'v')
			printf("%16s %c %s\n", "", data->type, data->name);
		else
			printf("%016lx %c %s\n", data->addr, data->type, data->name);
		data = data->next;
	}
}
