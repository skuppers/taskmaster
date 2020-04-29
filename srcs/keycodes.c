/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vct_readline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/22 17:22:54 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

uint64_t	assign_keycodes(t_env *env)
{
	env->ak_masks[AK_ARROW_RIGHT] = AK_ARROW_RIGHT_MASK;
	env->ak_masks[AK_ARROW_LEFT] = AK_ARROW_LEFT_MASK;
    env->ak_masks[AK_ARROW_UP] = AK_ARROW_UP_MASK;
	env->ak_masks[AK_ARROW_DOWN] = AK_ARROW_DOWN_MASK;
	env->ak_masks[AK_HOME] = AK_HOME_MASK;
	env->ak_masks[AK_END] = AK_END_MASK;
	env->ak_masks[AK_BACKSPACE] = AK_BACKSPACE_MASK;
	env->ak_masks[AK_DELETE] = AK_DELETE_MASK;
	env->ak_masks[AK_CTRL_A] = AK_CTRL_A_MASK;
	env->ak_masks[AK_CTRL_E] = AK_CTRL_E_MASK;
	env->ak_masks[AK_CTRL_D] = AK_CTRL_D_MASK;
	env->ak_masks[AK_CTRL_L] = AK_CTRL_L_MASK;
	env->ak_masks[AK_CTRL_R] = AK_CTRL_R_MASK;
	env->ak_masks[AK_TABULATION] = AK_TAB_MASK;
	return (SUCCESS);
}

uint64_t	link_keys_functions(t_actionkeys actionkeys[AK_AMOUNT])
{
	actionkeys[AK_ARROW_LEFT] = &ak_arrow_left;
	actionkeys[AK_ARROW_RIGHT] = &ak_arrow_right;
    actionkeys[AK_ARROW_UP] = &ak_arrow_up;
	actionkeys[AK_ARROW_DOWN] = &ak_arrow_down;
	actionkeys[AK_HOME] = &ak_home;
	actionkeys[AK_END] = &ak_end;
	actionkeys[AK_BACKSPACE] = &ak_backspace;
	actionkeys[AK_DELETE] = &ak_delete;
	actionkeys[AK_CTRL_A] = &ak_home;
	actionkeys[AK_CTRL_E] = &ak_end;
	actionkeys[AK_CTRL_D] = &ak_ctrl_d;
	actionkeys[AK_CTRL_L] = &ak_ctrl_l;
	actionkeys[AK_CTRL_R] = &ak_ctrl_r;
	actionkeys[AK_TABULATION] = &ak_hightab;
	return (SUCCESS);
}