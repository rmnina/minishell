/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:14:00 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/09 00:03:53 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <ctype.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>

# if defined __APPLE__
#  define PTR_NULL "0x0"
# elif __linux__
#  define PTR_NULL "(nil)"
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;

}					t_list;

typedef struct s_alloc {
	void			*adr;
	struct s_alloc	*next;
}	t_alloc;

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_strlen(const char *str);
void		*ft_memset(void *ptr, int value, size_t size);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t size);
void		*ft_memmove(void *dest, const void *src, size_t size);
char		*ft_strcpy(char *dest, const char *src);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strcat(char *dest, const char *src);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *str, int searchedChar);
char		*ft_strrchr(const char *str, int searchedChar);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
long int	ft_atoi(const char *str);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(char *src, t_alloc **garbage);
char		*ft_strndup(char *src, size_t n, t_alloc **garbage);
char		*ft_substr(const char *s, unsigned int start, size_t len, t_alloc **garbage);
char		*ft_strjoin(char *s1, const char *s2, t_alloc **garbage);
char		*ft_strjoin_char(char *s1, const char c, t_alloc **garbage);
char		*ft_strtrim(const char *s1, const char *set, t_alloc **garbage);
char		**ft_split(const char *s, char c);
char		*ft_itoa(int n);
char		*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
int			ft_putunsigned(unsigned int n);
int			ft_put_hex(unsigned int n, char c);
int			ft_putadr(unsigned long n);
int			ft_printf(char *format, ...);
int			choose_format(char *format, va_list args);
int			ft_putstr(char *s);
int			ft_putchar(char c);
int			ft_putnbr(int n);
void		free_garbage(t_alloc **garbage, int i);
void		*garb_malloc(size_t type, size_t size, t_alloc **garbage);

#endif
