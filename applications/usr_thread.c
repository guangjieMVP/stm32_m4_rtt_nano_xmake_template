/*
 * @brief :   
 * @date :  2022-02-xx
 * @version : v1.0.0
 * @copyright(c) 2020 : OptoMedic company Co.,Ltd. All rights reserved
 * @Change Logs:   
 * @date         author         notes:  
 */
#include <rtthread.h>
#include "global_cfg.h"
#include "bsp.h"

#define LED_THREAD_PRIORITY         25
#define LED_THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE            5

static rt_thread_t led_tid = RT_NULL;

/* LED Thread ����ں��� */
static void led_thread_entry(void *arg)
{
    while (1)
    {
        BSP_LED_Ctrl(1);
        rt_thread_mdelay(500);
        BSP_LED_Ctrl(0);
        rt_thread_mdelay(500);
    }
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[512];
static struct rt_thread thread2;
/* �߳� 2 ��� */
static void thread2_entry(void *arg)
{
    rt_uint32_t count = 0;

    /* �߳� 2 ӵ�нϸߵ����ȼ�������ռ�߳� 1 �����ִ�� */
    for (count = 0; count < 10 ; count++)
    {
        /* �߳� 2 ��ӡ����ֵ */
        printf("thread2 count: %d\r\n", count);
        rt_thread_mdelay(1000);
    }
    printf("thread2 exit\r\n");
    /* �߳� 2 ���н�����Ҳ���Զ���ϵͳ���� */
}

void usr_thread_init(void)
{

#ifdef GBL_USE_LED_THREAD   /* ��̬�ڴ洴���߳� */
    led_tid = rt_thread_create("led",
                            led_thread_entry, RT_NULL,
                            LED_THREAD_STACK_SIZE,
                            LED_THREAD_PRIORITY, THREAD_TIMESLICE);
     /* �������߳̿��ƿ飬��������߳� */
    if (led_tid != RT_NULL)    
        rt_thread_startup(led_tid);
    if (led_tid == RT_NULL) printf("led_thread failed\n");
#endif

#ifdef GBL_USE_TEST_PRINT_THREAD   
    rt_thread_init(&thread2,   /* ��̬�ڴ洴���߳� */
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   LED_THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    rt_thread_startup(&thread2);
#endif

}