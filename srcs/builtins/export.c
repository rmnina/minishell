/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/03 18:27:27 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int envp_length(char **envp)
{
	int length;

	length = 0;
	while (envp[length] != NULL)
	{
		length++;
	}
	return (length);
}

char **copy_envp(char **envp, int new_size, t_alloc *garbage)
{
	int i;
	char **new_envp;

	i = 0;
	new_envp = malloc(sizeof(char *) * (new_size + 1));
	if (new_envp == NULL)
	{
		return (NULL);
	}
	while (i < new_size)
	{
		if (envp[i] != NULL)
		{
			new_envp[i] = ft_strdup(envp[i], garbage);
			if (new_envp[i] == NULL)
			{
				perror("Failed to duplicate string");
				while (i > 0)
				{
					free(new_envp[--i]);
				}
				free(new_envp);
				return (NULL);
			}
		}
		else
		{
			new_envp[i] = NULL;
		}
		i++;
	}
	new_envp[new_size] = NULL;
	return (new_envp);
}

void add_or_update_env_var(char ***envp, char *var, t_alloc *garbage)
{
	int len;
	int envp_len;
	char **new_envp;
	int i;
	bool found;

	i = 0;
	len = 0;
	envp_len = 0;
	new_envp = NULL;
	found = false;
	while (var[len] != '=' && var[len] != '\0')
		len++;
	envp_len = envp_length(*envp);
	new_envp = copy_envp(*envp, envp_len + 1, garbage);
	if (new_envp == NULL)
		return;
	while (i < envp_len)
	{
		if (ft_strncmp(new_envp[i], var, len) == 0)
		{
			free(new_envp[i]);
			new_envp[i] = ft_strdup(var, garbage);
			if (new_envp[i] == NULL)
			{
				free(new_envp);
				return;
			}
			found = true;
			break;
		}
		i++;
	}
	if (!found)
	{
		new_envp[envp_len] = ft_strdup(var, garbage);
		new_envp[envp_len + 1] = NULL;
	}
	*envp = new_envp;
}


bool	is_valid_identifier(const char *str)
{
	const char	*ptr;
	bool		equals_encountered;
	bool		non_space_after_equals;

	equals_encountered = false;
	non_space_after_equals = false;
	if (!str)
		return (false);
	if (*str == ' ' || *str == '=' || !(*str == '_' || ft_isalpha(*str)))
	{
		printf("export: `%s': not a valid identifier\n", str);
		return (false);
	}
	ptr = str + 1;
	while (*ptr)
	{
		if (*ptr == '=')
		{
			equals_encountered = true;
		}
		else if (equals_encountered)
		{
			if (!non_space_after_equals && *ptr != ' ')
				non_space_after_equals = true;
		}
		else if (non_space_after_equals && *ptr == ' ')
		{
			return (false);
		}
		else if (!(*ptr == '_' || ft_isalnum(*ptr)))
		{
			return (false);
		}
		ptr++;
	}
	return (true);
}

// void handle_value_case(char ***envp, char *var_name, char *value, t_alloc *garbage)
// {
// 	char *new_var;
// 	char *formatted_value;

// 	if (!value || *value == '\0')
// 	{
// 		new_var = ft_strjoin(var_name, "=\"\"", garbage);
// 		add_or_update_env_var(envp, new_var, garbage);
// 	}
// 	else if (*value == '$')
// 	{
// 		printf("avant formatage\n");
// 		formatted_value = ft_strjoin("\\", value, garbage);
// 		new_var = ft_strjoin(var_name, "=", garbage);
// 		new_var = ft_strjoin(new_var, formatted_value, garbage);
// 		printf("apres formatage, new var : %s\n", new_var);
// 	}
// 	else if (*value == '"' || *value == '\'')
// 	{
// 		formatted_value = ft_strdup(value, garbage);
// 		new_var = ft_strjoin(var_name, "=", garbage);
// 		new_var = ft_strjoin(new_var, formatted_value, garbage);
// 	}
// 	else
// 	{
// 		formatted_value = ft_strjoin("\"", value, garbage);
// 		formatted_value = ft_strjoin(formatted_value, "\"", garbage);
// 		new_var = ft_strjoin(var_name, "=", garbage);
// 		new_var = ft_strjoin(new_var, formatted_value, garbage);
// 	}
// 	add_or_update_env_var(envp, new_var, garbage);
// }

// char *escape_special_chars(const char *value)
// {
//     char *escaped_value;
//     int i, j, len;

//     len = strlen(value);
//     escaped_value = malloc(sizeof(char) * (2 * len + 1)); // Allouer suffisamment d'espace pour les caractères échappés
//     if (!escaped_value) return NULL;

//     for (i = 0, j = 0; i < len; ++i, ++j)
//     {
//         if (value[i] == '\\' || value[i] == '$')
//         {
//             escaped_value[j++] = '\\'; // Ajouter un backslash supplémentaire
//         }
//         escaped_value[j] = value[i];
//     }
//     escaped_value[j] = '\0';

//     return escaped_value;
// }
void	handle_value_case(char ***envp, char *arg, t_alloc *garbage)
{
	char	*equal;
	char	*var_name;
	char	*value;
	char	*new_var;
	char	*formatted_value;

	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		if (is_valid_identifier(arg))
		{
			new_var = ft_strjoin(arg, "=\"\"", garbage);
			add_or_update_env_var(envp, new_var, garbage);
		}
		return ;
	}
	var_name = strndup(arg, equal - arg);
	value = equal + 1;
	if (!value || *value == '\0')
		new_var = ft_strjoin(var_name, "=\"\"", garbage);
	else if (*value == '$')
	{
		formatted_value = ft_strjoin("\\", value, garbage);
		new_var = ft_strjoin(var_name, "=", garbage);
		new_var = ft_strjoin(new_var, formatted_value, garbage);
	}
	else if (*value == '"' || *value == '\'')
	{
		formatted_value = ft_strdup(value, garbage);
		new_var = ft_strjoin(var_name, "=", garbage);
		new_var = ft_strjoin(new_var, formatted_value, garbage);
	}
	else
	{
		formatted_value = ft_strjoin("\"", value, garbage);
		formatted_value = ft_strjoin(formatted_value, "\"", garbage);
		new_var = ft_strjoin(var_name, "=", garbage);
		new_var = ft_strjoin(new_var, formatted_value, garbage);
	}
	add_or_update_env_var(envp, new_var, garbage);
	free(var_name);
}


// void handle_value_case(char ***envp, char *arg, t_alloc *garbage)
// {
//     char *equal = ft_strchr(arg, '=');
//     char *var_name;
//     char *value;
//     char *new_var;
//     char *formatted_value;

//     if (!equal)
//     {
//         // Gestion des cas où il n'y a pas de valeur (juste un nom de variable)
//         if (is_valid_identifier(arg))
//         {
//             new_var = ft_strjoin(arg, "=\"\"", garbage);
//             add_or_update_env_var(envp, new_var, garbage);
//         }
//         return;
//     }

//     // Séparation du nom de la variable et de la valeur
//     var_name = strndup(arg, equal - arg);
//     value = equal + 1;

//     // Traiter et échapper la valeur
//     if (!value || *value == '\0')
//     {
//         new_var = ft_strjoin(var_name, "=\"\"", garbage);
//     }
//     else
//     {
//         // Créer une nouvelle chaîne pour la valeur formatée en échappant les caractères spéciaux
//         formatted_value = escape_special_chars(value);
//         new_var = ft_strjoin(var_name, "=", garbage);
//         new_var = ft_strjoin(new_var, formatted_value, garbage);
// 		new_var = ft_strjoin(new_var, "\"", garbage);
//     }
//     add_or_update_env_var(envp, new_var, garbage);

//     free(var_name); // Libérer la mémoire allouée pour var_name
// }


int ft_export(char ***envp, char **argv, t_code *code, t_alloc *garbage)
{
	int		i;
	char	*equal;
	char	*space;

	equal = NULL;
	space = NULL;
	if (argv[1] == NULL)
	{
		i = 0;
		while ((*envp)[i] != NULL)
		{
			equal = ft_strchr((*envp)[i], '=');
			if (equal && *(equal + 1) == '\0')
				printf("export %.*s=\"\"\n", (int)(equal - (*envp)[i]), (*envp)[i]);
			else
				printf("export %s\n", (*envp)[i]);
			i++;
		}
		code->code_status = 0;
		return (code->code_status);
	}
	else
	{
		i = 1;
		while (argv[i] != NULL)
		{
			equal = ft_strchr(argv[i], '=');
			space = ft_strchr(argv[i], ' ');
			if (!space && !equal)
			{
				while (argv[i] != NULL)
				{
					if (is_valid_identifier(argv[i]))
						add_or_update_env_var(envp, argv[i], garbage);
					i++;
				}
			}
			if (equal)
			{
				//*equal = '\0';
				if (is_valid_identifier(argv[i]) || ft_strchr(argv[i], '='))
					handle_value_case(envp, argv[i], garbage);
				//*equal = '=';
			}
			else
			{
				if (is_valid_identifier(argv[i]))
					add_or_update_env_var(envp, argv[i], garbage);
			}
			i++;
		}
	}
	code->code_status = 0;
	return (code->code_status);
}

// int	ft_export(char ***envp, char **argv, t_code *code, t_alloc *garbage)
// {
// 	int		i;
// 	char	*equal;
// 	char	*identifier;

// 	equal = NULL;
// 	identifier = NULL;
// 	if (argv[1] == NULL)
// 	{

// 		i = 0;
// 		while ((*envp)[i] != NULL)
// 		{
// 			equal = ft_strchr((*envp)[i], '=');
// 			if (equal && *(equal + 1) == '\0')
// 				printf("export %.*s=\"\"\n", (int)(equal - (*envp)[i]), (*envp)[i]);
// 			else
// 				printf("export %s\n", (*envp)[i]);
// 			i++;
// 		}
// 		code->code_status = 0;
// 		return (code->code_status);
// 	}
// 	else
// 	{
// 		i = 1;
// 		while (argv[i] != NULL)
// 		{
// 			identifier = argv[i];
// 			if (!is_valid_identifier(identifier))
// 			{
// 				printf("export: '%s': not a valid identifier\n", argv[i]);
// 			}
// 			if (is_valid_identifier(identifier))
// 				add_or_update_env_var(envp, argv[i], garbage);
// 			i++;
// 		}
// 	}
// 	code->code_status = 0;
// 	return (code->code_status);
// }
