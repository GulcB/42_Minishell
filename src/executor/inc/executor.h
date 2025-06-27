/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbodur <gbodur@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:06:34 by gbodur            #+#    #+#             */
/*   Updated: 2025/06/27 17:08:58 by gbodur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include	"../../parser/inc/parser.h"

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct 		s_exec_context
{
    t_env   		*env;
    int     		exit_status;
    int     		stdin_backup;
    int     		stdout_backup;
}					t_exec_context;

//execv'ye gelecek olacak olan **command için de değişken lazım


#endif

