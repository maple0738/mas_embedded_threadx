/*
 * @Author: laladuduqq 2807523947@qq.com
 * @Date: 2026-05-14 20:56:35
 * @LastEditors: maple0738 1573764313@qq.com
 * @LastEditTime: 2026-07-18 10:39:29
 * @FilePath: /mas_embedded_threadx/apps/infantry3/single_board/gimbal_func/gimbal_func.h
 * @Description:
 */
#ifndef _GIMBAL_FUNC_H_
#define _GIMBAL_FUNC_H_

#include "mailun_def.h"

void gimbal_init(void);

void gimbal_func(Gimbal_Ctrl_Cmd_t *gimbal_cmd, uint16_t *yaw_ecd);

#endif // _GIMBAL_FUNC_H_