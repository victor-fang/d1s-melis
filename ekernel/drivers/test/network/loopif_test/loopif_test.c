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
#include <rtthread.h>
//#include <task.h>
#include <lwip/tcp.h>

#include <stdio.h>
#include <lwip/ip_addr.h>
#include <lwip/err.h>
#include <lwip/netif.h>
#include <lwip/ip_addr.h>
#include <sys/times.h>

//#include "wifi_constants.h"
//#include "wifi/wifi_conf.h"
//#include "serial.h"

//#define LWIP_CALLBACK_API 1
err_t server_recv(void *arg,struct tcp_pcb *pcb,struct pbuf *p,err_t err);

err_t server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	printf("loop interface debug %s,%d\n",__func__,__LINE__);
	tcp_recv(pcb,server_recv);
	return ERR_OK;
}

err_t server_recv(void *arg,struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	char *data_recv = NULL;
	int index = 0;

	struct pbuf *q = NULL;
	printf("loop interface debug %s,%d\n",__func__,__LINE__);

	if(p != NULL)
	{
		data_recv = (char *)mem_calloc(p->tot_len + 1,1);
		if(NULL != data_recv) {
			q = p;
			while(q != NULL) {
				memcpy(&data_recv[index],q->payload,q->len);
				index += q->len;
				q = q->next;
			}
			printf("[Server]Get MSG:%s\n",data_recv);
			mem_free(data_recv);
			data_recv = NULL;
		}else {
			printf("[Server]mem_calloc error,len=%u\r\n",p->tot_len);
		}

		tcp_recved(pcb,p->tot_len);
		pbuf_free(p);
	}else if(err == ERR_OK) {
		return tcp_close(pcb);
	}
	printf("loop interface debug %s,%d\n",__func__,__LINE__);
	return ERR_OK;
}

void tcpserver_init(void)
{
	struct tcp_pcb *pcb = NULL;
	pcb = tcp_new();
	printf("loop interface debug %s,%d\n",__func__,__LINE__);
	tcp_bind(pcb,IP_ADDR_ANY,6060);

	pcb = tcp_listen(pcb);
	printf("loop interface debug %s,%d\n",__func__,__LINE__);

	tcp_accept(pcb,server_accept);
}

err_t loopclient_poll(void *arg,struct tcp_pcb *tpcb)
{
	char loopdata[100] = {0,};
	static unsigned int count = 0;
	int len = 0;

	printf("loop interface debug %s,%d\n",__func__,__LINE__);
	len = sprintf(loopdata,"Loop interface Test,loop count = %u",count ++);

	tcp_write(tpcb,loopdata,len,TCP_WRITE_FLAG_COPY);

	return ERR_OK;
}

err_t loopclient_recv(void *arg,struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	if(p != NULL) {
		printf("[Client]receive data, len = %d\r\n",p->tot_len);
		tcp_recved(pcb,p->tot_len);
		pbuf_free(p);
	}else if(err == ERR_OK) {
		return tcp_close(pcb);
	}
	return ERR_OK;
}
err_t loopclient_connect(void *arg,struct tcp_pcb *tpcb,err_t err)
{
	printf("loop interface debug %s,%d\n",__func__,__LINE__);
	tcp_recv(tpcb,loopclient_recv);
//	tcp_poll(tpcb,loopclient_poll,10);
	return ERR_OK;
}

struct tcp_pcb *pcb = NULL;
void loopclient_init(void)
{
	ip_addr_t ipaddr;
//	struct ip_addr ipaddr;
	IP4_ADDR(&ipaddr,127,0,0,1);

	printf("loop interface debug %s,%d\n",__func__,__LINE__);
	pcb = tcp_new();

	tcp_connect(pcb,&ipaddr,6060,loopclient_connect);
}

struct netif *netif = NULL;
void loop_test_task(void *unuse)
{
	printf("enter loop test task\n");
	tcpserver_init();

	loopclient_init();
	netif = netif_list;
	printf("loop interface debug %s,%d\n",__func__,__LINE__);
	while(1) {
//		netif_poll_all();
//		loopclient_poll(NULL,pcb);
//		sys_check_timeouts();
  /* loop through netifs */
		while (netif != NULL) {
		  netif_poll(netif);
		  /* proceed to next network interface */
		  netif = netif->next;
		}
		sleep(10);
		tcp_poll(pcb,loopclient_poll,10);
	}
}
