/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:15:52 by maambuhl          #+#    #+#             */
/*   Updated: 2024/10/03 12:42:36 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*occ;

	i = 0;
	occ = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			occ = (char *)s + i;
		i++;
	}
	if (s[i] == (char)c)
		occ = (char *)s + i;
	return (occ);
}
