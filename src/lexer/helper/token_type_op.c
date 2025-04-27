/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_op.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdivan <mdivan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 10:32:18 by mdivan            #+#    #+#             */
/*   Updated: 2025/04/27 10:51:58 by mdivan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/lexer.h"

t_token_type get_token_type(char c, char next_c)
{
    if (c == '|')
        return (next_c == '|' ? TOKEN_OR : TOKEN_PIPE);
    if (c == '<')
        return (next_c == '<' ? TOKEN_HEREDOC : TOKEN_REDIR_IN);
    if (c == '>')
        return (next_c == '>' ? TOKEN_APPEND : TOKEN_REDIR_OUT);
    if (c == '&' && next_c == '&')
        return (TOKEN_AND);
    if (c == ';')
        return (TOKEN_SEMICOLON);
    if (c == '"')
        return (TOKEN_DQUOTE);
    if (c == '\'')
        return (TOKEN_SQUOTE);
    if (c == '$')
        return (TOKEN_DOLLAR);
    
    return (TOKEN_WORD);
}
