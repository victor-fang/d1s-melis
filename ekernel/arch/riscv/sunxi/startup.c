/*
* Copyright (c) 2019-2025 Allwinner Technology Co., Ltd. ALL rights reserved.
*
* Allwinner is a trademark of Allwinner Technology Co.,Ltd., registered in
* the the People's Republic of China and other countries.
* All Allwinner Technology Co.,Ltd. trademarks are used with permission.
*
* DISCLAIMER
* THIRD PARTY LICENCES MAY BE REQUIRED TO IMPLEMENT THE SOLUTION/PRODUCT.
* IF YOU NEED TO INTEGRATE THIRD PARTY’S TECHNOLOGY (SONY, DTS, DOLBY, AVS OR MPEGLA, ETC.)
* IN ALLWINNERS’SDK OR PRODUCTS, YOU SHALL BE SOLELY RESPONSIBLE TO OBTAIN
* ALL APPROPRIATELY REQUIRED THIRD PARTY LICENCES.
* ALLWINNER SHALL HAVE NO WARRANTY, INDEMNITY OR OTHER OBLIGATIONS WITH RESPECT TO MATTERS
* COVERED UNDER ANY REQUIRED THIRD PARTY LICENSE.
* YOU ARE SOLELY RESPONSIBLE FOR YOUR USAGE OF THIRD PARTY’S TECHNOLOGY.
*
*
* THIS SOFTWARE IS PROVIDED BY ALLWINNER"AS IS" AND TO THE MAXIMUM EXTENT
* PERMITTED BY LAW, ALLWINNER EXPRESSLY DISCLAIMS ALL WARRANTIES OF ANY KIND,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING WITHOUT LIMITATION REGARDING
* THE TITLE, NON-INFRINGEMENT, ACCURACY, CONDITION, COMPLETENESS, PERFORMANCE
* OR MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL ALLWINNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS, OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <compiler.h>
#include <port.h>

#include <rtthread.h>
#include <hal_mem.h>
#include <hal_thread.h>

extern void common_init(void);
void system_time_init(void);
void show_melis_version(void);

static int aw_application_init(pfun_krnl_init_t app_init)
{
    void *app_init_thread;

    /* highest priority to system bringup. */
    app_init_thread = kthread_create(app_init, NULL, "kstartup", 0x4000, 25);
    kthread_start(app_init_thread);
    return 0;
}

void awos_init_bootstrap(pfun_bsp_init_t aw_system_bsp_init, pfun_krnl_init_t app_init, uint64_t ticks)
{
    void *heap_start, *heap_end;

    extern unsigned long __bss_end[];
    heap_start = (void *)__bss_end;
    heap_end   = (void *)(CONFIG_DRAM_VIRTBASE + CONFIG_DRAM_SIZE);

    rt_system_heap_init(heap_start, heap_end);

    common_init();

#ifdef CONFIG_DOUBLE_FREE_CHECK
    void esKRNL_MemLeakChk(uint32_t en);
    esKRNL_MemLeakChk(3);
#endif

    dma_alloc_coherent_init();

#ifdef CONFIG_DMA_COHERENT_HEAP
    dma_coherent_heap_init();
#endif
    system_time_init();

    if (aw_system_bsp_init)
    {
        aw_system_bsp_init();
    }

    setbuf(stdout, NULL);
    setbuf(stdin, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    show_melis_version();

    rt_system_timer_init();

    rt_system_scheduler_init();
    rt_system_timer_thread_init();

    /* initialize idle thread */
    rt_thread_idle_init();

    aw_application_init(app_init);

    rt_system_scheduler_start();

#ifdef CONFIG_BRINGUP_IDLE
    void idle_entry(void);
    idle_entry();
#endif
    /* never come here */
    CODE_UNREACHABLE;
}
