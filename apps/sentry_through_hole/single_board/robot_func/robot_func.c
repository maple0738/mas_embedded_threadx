/*
 * @Author: maple0738 1573764313@qq.com
 * @Date: 2026-08-29 16:27:28
 * @LastEditors: maple0738 1573764313@qq.com
 * @LastEditTime: 2026-09-07 17:24:16
 * @FilePath: \mas_embedded_threadx\apps\mailun\single_board\robot_func\robot_func.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "robot_func.h"
#include <stdint.h>
#include "module_remote.h"
#include <string.h>


int16_t CalcOffsetAngle(float getyawangle)
{
    float offset_ecd;

    const float ECD_MAX  = 8191.0f;
    const float ECD_HALF = 4095.5f;

    offset_ecd = getyawangle - YAW_CHASSIS_ALIGN_ECD;

    // 归一化到最小旋转角度对应的编码器差值 ([-ECD_HALF, ECD_HALF])
    while (offset_ecd > ECD_HALF) offset_ecd -= ECD_MAX;
    while (offset_ecd < -ECD_HALF) offset_ecd += ECD_MAX;

    return (int16_t)offset_ecd;
}

void RemoteControlSet(Chassis_Ctrl_Cmd_t *Chassis_Ctrl, Shoot_Ctrl_Cmd_t *Shoot_Ctrl, Gimbal_Ctrl_Cmd_t *Gimbal_Ctrl)
{
       if (!Chassis_Ctrl || !Shoot_Ctrl || !Gimbal_Ctrl) return;

    uint8_t state = Module_Remote_get_offline_status();
        if (state & 0x01)
    {
        /* 摇杆 → 速度比例 (-1.0 ~ +1.0) */

        Chassis_Ctrl->vx = (float)Module_Remote_get_channel(2) / (float)(SBUS_CHX_DOWN - SBUS_CHX_BIAS);
        Chassis_Ctrl->vy = -(float)Module_Remote_get_channel(1) / (float)(SBUS_CHX_DOWN - SBUS_CHX_BIAS);
         Remote_Data_t *subs_data = Module_Remote_get_data();
        if (subs_data != NULL)
            {
                if (subs_data->channels[4] == SBUS_CHX_UP)
                {
                    Chassis_Ctrl->chassis_mode = chassis_follow_gimbal_yaw;
                }
                else  if (subs_data->channels[4] == SBUS_CHX_BIAS)
                {
                    Chassis_Ctrl->chassis_mode = chassis_rotate;
                }
                
                else if (subs_data->channels[4] == SBUS_CHX_DOWN)
                {
                    Chassis_Ctrl->chassis_mode = chassis_rotate_reverse;
                }
            }

                    if (subs_data->channels[7] == SBUS_CHX_UP)
            {
                Chassis_Ctrl->wz = 0;
            }
            else if (subs_data->channels[7] == SBUS_CHX_BIAS)
            {
                Chassis_Ctrl->wz = 1.2;
            }
            else if (subs_data->channels[7] == SBUS_CHX_DOWN)
            {
                Chassis_Ctrl->wz = 2.5;

            }

                    /* 云台控制部分 */
            if (subs_data->channels[5] == SBUS_CHX_UP)
            {
                Gimbal_Ctrl->gimbal_mode = gimbal_gyro_mode;
                Gimbal_Ctrl->yaw -= 0.0005f * (float)(Module_Remote_get_channel(4));
                Gimbal_Ctrl->pitch -= 0.0005f * (float)(Module_Remote_get_channel(3));
            }
            else if (subs_data->channels[5] == SBUS_CHX_BIAS)
            {
                Chassis_Ctrl->chassis_mode = chassis_zero_force;
                Shoot_Ctrl->shoot_mode     = shoot_off;
                Shoot_Ctrl->friction_mode  = friction_off;
                Shoot_Ctrl->load_mode      = load_stop;
            }
            else if (subs_data->channels[5] == SBUS_CHX_DOWN)
            {
                Chassis_Ctrl->chassis_mode = chassis_zero_force;
                Gimbal_Ctrl->gimbal_mode   = gimbal_zero_force;
                Shoot_Ctrl->shoot_mode     = shoot_off;
                Shoot_Ctrl->friction_mode  = friction_off;
                Shoot_Ctrl->load_mode      = load_stop;
            }

        else
        {
            Gimbal_Ctrl->gimbal_mode   = gimbal_zero_force;
            Chassis_Ctrl->chassis_mode = chassis_zero_force;
            Shoot_Ctrl->shoot_mode     = shoot_off;
            Shoot_Ctrl->friction_mode  = friction_off;
            Shoot_Ctrl->load_mode      = load_stop;
        }
          

    }
    else
    {
        Gimbal_Ctrl->gimbal_mode   = gimbal_zero_force;
        Chassis_Ctrl->chassis_mode = chassis_zero_force;
        Shoot_Ctrl->shoot_mode     = shoot_off;
        Shoot_Ctrl->friction_mode  = friction_off;
        Shoot_Ctrl->load_mode      = load_stop;
    }
}
