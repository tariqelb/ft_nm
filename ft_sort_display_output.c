#include "ft_nm.h"

t_output *ft_new_elem(long int addr, char type, char *name)
{
	t_output *node;

	node = malloc(sizeof(t_output));
	if (!node)
		return (NULL);

	node->addr = addr;
	node->type = type;
	node->name = strdup(name); // VERY IMPORTANT
	node->next = NULL;

	return (node);
}

void ft_add_new_elem(t_output **head, long int addr, char type, char *name)
{
	t_output	*new_node;
	t_output	*tmp;

	new_node = ft_new_elem(addr, type, name);
	if (!new_node)
		return;

	// if list empty
	if (*head == NULL)
	{
		*head = new_node;
		return;
	}

	// go to last node
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;

	tmp->next = new_node;
}

t_output *ft_sort_output(t_output *data)
{
	t_output	*i;
	int		swapped;
	long int	tmp_addr;
	char		*tmp_name;
	char		tmp_type;

	swapped = 1;

	if (!data)
	    return (NULL);

	while (swapped)
	{
		swapped = 0;
		i = data;
		while (i->next)
		{
			if (strcmp(i->name, i->next->name) > 0)
			{
				// swap CONTENT (not nodes)
				tmp_addr = i->addr;
				tmp_name = i->name;
				tmp_type = i->type;

				i->addr = i->next->addr;
				i->name = i->next->name;
				i->type = i->next->type;

				i->next->addr = tmp_addr;
				i->next->name = tmp_name;
				i->next->type = tmp_type;

				swapped = 1;
			}
			i = i->next;
		}
	}
	return (data);
}

void	ft_display_output(t_output *data)
{
	printf("ft_display_output\n");
	while (data != NULL)
	{
		if (data->type == 'U' || data->type == 'w' || data->type == 'v')
			printf("%16s %c %s\n", "", data->type, data->name);
		else
			printf("%016lx %c %s\n", data->addr, data->type, data->name);
		data = data->next;
	}
}
