/********************************************************
* @file       app_mpu.c
* @author     szhj13
* @version    V1.0
* @date       2021-08-12
* @brief      the entry of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "BAT32G157.h"

#include "drv_mpu.h"
#include "drv_mpu_dmp_motion_driver.h"
#include "app_mpu.h"


/* Private typedef --------------------------------------*/
/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (20)
#define q30  1073741824.0f

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
float Yaw,Roll,Pitch;
long temperature;
struct int_param_s int_param;

static signed char gyro_orientation[9] = {
                                           -1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1
                                         };
                                           
static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx )
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}

static void run_self_test(void)
{
	
    int result;
//    char test_packet[4] = {0};
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		printf("setting bias succesfully ......\r\n");
    }
	else
	{
		printf("bias has not been modified ......\r\n");
	}
}

void App_MPU_Init(void )
{
    if(!mpu_init(&int_param))
    {
        printf("mpu initialization complete......\r\n ");
   	    //mpu_set_sensor
   	    if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
   	    {
   	  	    printf("mpu_set_sensor complete ......\r\n");
   	    }
        else
	    {
	  	    printf("mpu_set_sensor come across error ......\r\n");
	    }
         //mpu_configure_fifo
	    if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	    {
	  	    printf("mpu_configure_fifo complete ......\r\n");
	    }
	    else
	    {
	  	    printf("mpu_configure_fifo come across error ......\r\n");
	    }
         //mpu_set_sample_rate
	    if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))
	    {
	  	    printf("mpu_set_sample_rate complete ......\r\n");
	    }
	    else
	    {
	  	    printf("mpu_set_sample_rate error ......\r\n");
	    }

        //dmp_load_motion_driver_firmvare
	    if(!dmp_load_motion_driver_firmware())
	    {
	  	    printf("dmp_load_motion_driver_firmware complete ......\r\n");
	    }
	    else
	    {
	  	    printf("dmp_load_motion_driver_firmware come across error ......\r\n");
	    }

         //dmp_set_orientation
	    if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
	    {
	  	    printf("dmp_set_orientation complete ......\r\n");
	    }
	    else
	    {
	  	    printf("dmp_set_orientation come across error ......\r\n");
	    }
        //dmp_enable_feature
	    if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
	        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
	        DMP_FEATURE_GYRO_CAL))
	    {
	  	    printf("dmp_enable_feature complete ......\r\n");
	    }
	    else
	    {
	  	    printf("dmp_enable_feature come across error ......\r\n");
	    }

        //dmp_set_fifo_rate
	    if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
	    {
	  	    printf("dmp_set_fifo_rate complete ......\r\n");
	    }
	    else
	    {
	  	    printf("dmp_set_fifo_rate come across error ......\r\n");
	    }

        run_self_test();

        if(!mpu_set_dmp_state(1))
	    {
	  	    printf("mpu_set_dmp_state complete ......\r\n");
	    }
	    else
	    {
	  	    printf("mpu_set_dmp_state come across error ......\r\n");
	    }
    }
    else
	{
		printf("mpu initialization come across error......\r\n ");
	}
}

void App_DMP_update(void)
{
   	if(0 == dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))
	{
		if (sensors & INV_WXYZ_QUAT )
		{
		 
			 q0=quat[0] / q30;
			 q1=quat[1] / q30;
			 q2=quat[2] / q30;
			 q3=quat[3] / q30;
			 Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
			 Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll		 
			 Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
		}
	 }
     else
     {
//			Uart_Send_Status(0, 0, 0);
     }
}

