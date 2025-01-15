/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 17:27:43 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/04 20:22:07 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*head;

	head = *lst;
	if (!*lst)
		return ;
	while (*lst)
	{
		head = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = head;
	}
}
