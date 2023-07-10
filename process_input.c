/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarks <jmarks@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 11:10:49 by jchurch           #+#    #+#             */
/*   Updated: 2023/06/20 18:27:24 by jmarks           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern	t_program	g_program;

/*Input: string to return an integer the number of "tokens" that can be 
created. A token is seperated by a space or tab, unless the space or tab 
is enclosed within matching double or single quotation marks. 

Any word that has no space or no tab and is next to the "outside" of a 
quotation mark: One token will be created to encompas the outside word 
and the whole quotation mark.*/
int	find_token_number(t_token_list **root)
{
	int			i;
	t_token_list *curr;

	i = 0;
	curr = *root;

	while (curr != NULL)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

// int	find_token_number(char *str)
// {
// 	int			i;
// 	char		type;
// 	int			count;

// 	i = 0;
// 	count = 0;
// 	if (ft_isprint(str[i]) == 1)
// 		count++;
// 	while (str[i] != '\0')
// 	{
// 		if (ft_is_quote(str[i]) && str[i + 1] != '\0') //IF index i is double or single quote and next exists. 
// 		{
// 			type = str[i]; // The type of quote. 
// 			i++; // One beyond the quote. 
// 			while (str[i] != type && str[i] != '\0') // WHILE index i exists and is not (matching/closing) double quote. 
// 				i++;
// 			// if (str[i] == type) // Found the matching close quote. 
// 			// 	i++;
// 		}
// 		else if (ft_is_white_space(str[i] == 1) && str[i + 1] != '\0') // ELSE IF is space or tab AND next is not end of string but an actual char. Then it will be a new word. 
// 		{
// 			count++;
// 			while (ft_is_white_space(str[i]) == 1) && ft_is_white_space(str[i + 1]) == 1 && str[i + 1] != '\0') // While spaces!
// 				i++;
// 		}
// 		i++;
// 	}
// 	// printf("count is: %d\n", count);
// 	return (count);
// }

/*Returns the index number of a string, the return value is the first space or 
tab to occur. Spaces within matching double or single quotation marks are 
ignored. The quotation is treated as one word. 

Any word that has no space or no tab and is next to the "outside" of a 
quotation mark: One token will be created to encompas the outside word 
and the whole quotation mark.*/
int	find_end(char *str)
{
	int			i;
	char		type;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_is_white_space(str[i]) == 1) // IF whitespace, stop. 
			break ;
		// IF quote, ignore white space
		if (ft_is_quote(str[i]) == 1) //IF index i is double or single quote and next exists. 
		{
			type = str[i]; // Set type to quote type. 
			i++;
			while (str[i] != type && str[i] != '\0') // WHILE index i exists and is not (matching/closing) double quote. 
				i++;
		}
		i++;
	}
	return (i);
}

/*Create a "token" from an input string. 
A token is defined by any word that has a space or tab either side of it. 
Unless it is within matching double or single quotaiton marks. 

Spaces within matching double or single quotation marks are ignored. 
The quotation is treated as one word. 

Any word that has no space or no tab and is next to the "outside" of a 
quotation mark: One token will be created to encompas the outside word 
and the whole quotation mark.*/
char	*make_token(char *str)
{
	debugFunctionName("MAKE_TOKEN");
	int		i;
	char	*return_token;

	i = 0;
	int length = find_end(str);
	// printf("length %d\n", length);
	return_token = malloc(sizeof(char) * length + 1); //JC MALLOC
	while (i < length)
	{
		return_token[i] = str[i];
		i++;
	}
	return_token[i] = '\0';
	return(return_token);
}


/*Creates a double pointer of type char into **split 
based on the input *str. 

A token is defined by any word that has a space or tab either side of it. 
Unless it is within matching double or single quotaiton marks. 

Spaces within matching double or single quotation marks are ignored. 
The quotation is treated as one word. 

Any word that has no space or no tab and is next to the "outside" of a 
quotation mark: One token will be created to encompas the outside word 
and the whole quotation mark.*/
void	make_tokens(char *str, t_token_list **root)
{
	debugFunctionName("MAKE_TOKENS");
	int				input_index;
	t_token_list	*new_node;

	input_index = 0;
	while (str[input_index] != '\0')
	{
		if (ft_is_not_white_space(str[input_index]) == 1)
		{
			new_node = make_new_node(make_token(&str[input_index]));
			ll_insert_end(root, new_node);
			input_index = ft_strlen(new_node->data) - 1 + input_index;
		}
		input_index++; // if whitespace
	}
}


/*
copy_int_array
Moves linked list t_token_list->data* into program->token** array. 
*/
void	copy_into_array(t_program *program, t_token_list **root)
{
	t_token_list	*curr;
	int		token_number;
	int		i;

	token_number = find_token_number(root);
	// printf("Token Number is: %d\n", token_number);
	program->token = (char**)malloc(sizeof(char*) * token_number + 1);
	i = 0;
	curr = *root;
	while (i < token_number)
	{
		program->token[i] = ft_strdup(curr->data);
		ll_print_token(root);
		curr = curr->next;
		i++;
	}
	program->token[i] = 0;
	ll_deallocate(root);

	printf("Printing tokens in Jo's array\n");
	int j = 0;
	while (j < token_number)
	{
		printf("token[%d]: %s\n", j, program->token[j]);
		j++;
	}
}

/*
process_input(char *str: raw user input, char **token: is empty pointer to string that will become tokenised string).
Return value is always 0? Why
*/
void	process_input(char *str, t_program *program)
{
	t_token_list	*root;

	root = NULL;
	debugFunctionName("PROCESS_INPUT");
	make_tokens(str, &root);
	ll_print_token(&root);
	expand_tokens(&root);
	copy_into_array(program, &root);
}
