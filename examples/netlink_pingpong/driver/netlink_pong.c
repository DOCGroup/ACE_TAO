/*! \file netlink-drv-pong.c 
    \brief netlink test driver	
    
    this is an example driver for netlink message.
    It pings back a message to the sender.
    
    This driver was compiled and tested on linux kernel 2.6.5 ( i used fedora core 2 ) . 
   Please note that some some changes were made from 2.4.x to 2.6.x in the interafce.      
*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/types.h>
#include <net/sock.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include "../netlinkmsg.h"

#define NETLINK_UNUSED 14
										   
MODULE_DESCRIPTION("ace_netlink kernel module");
MODULE_AUTHOR("raz ben yehuda");
MODULE_LICENSE("GPL");

static struct sock* ace_netlink_sock;
 

static int ace_netlink_send_message(int type, void *message,  int pid,int length, struct sock* socket)
{
	struct sk_buff *skb;
	char *data_start;
        int ret = 0 ;
		
	if (!message){
		printk("null message\n");
		return -EINVAL;
	}

	if (length > PAGE_SIZE){
		printk("length bigger than page size\n");
		return -EINVAL;
	}

	skb = alloc_skb(length,GFP_ATOMIC);
	if (!skb){
		printk("failed to alloc skb\n");
		return -ENOMEM;
	}
	data_start = skb_put(skb, length);
	memcpy(data_start, message, length);
	ret  = netlink_unicast(socket,skb,pid,0);
	printk("scipio_send_message ret = %d\n",ret);
	return ret;
}

void ace_netlink_handle_msg(void* r_msg)
{
     NetlinkMsg* request  = (NetlinkMsg*)r_msg;
     if (0!=request->h.dw_pid){
         
         int ret_bytes=0;
         NetlinkMsg Reply;
         Reply.h.dw_pid = request->h.dw_pid;
         Reply.h.dw_num = ++request->h.dw_num;
         printk("pong %d",Reply.h.dw_num);
         ret_bytes = ace_netlink_send_message(0,(void*)&Reply,Reply.h.dw_pid,sizeof(Reply),ace_netlink_sock);
         if (sizeof(Reply) != ret_bytes )
             printk("ace_netlink_handle_msg send failed\n");
         return;   
     }
     printk("ace_netlink_handle_msg mesg error\n");
}

/*! \fn    scipio_netlink_receive
     \brief a call back issued by the nerlink module in the context of the sender. 
*/
static void ace_netlink_netlink_receive(struct sock *sk, int length)
{                                         

	struct sk_buff *skb;
	while ((skb = skb_dequeue(&sk->sk_receive_queue) ) != NULL){
        
		printk("ace_netlink_netlink_receive message recieved %d\n",skb->len);
                ace_netlink_handle_msg(skb->data);  
		kfree_skb(skb);  // always release the skb through the network slab
	}
	//printk("ace_netlink_netlink_receive out\n");
}



int ace_netlink_init(void)
{
	ace_netlink_sock = netlink_kernel_create(NETLINK_UNUSED,ace_netlink_netlink_receive);
	//return netlink_attach(NETLINK_SCIPIO,scipio_netlink_receive);
	if (!ace_netlink_sock) {
		printk(KERN_ERR "unable to create netlink socket; aborting\n");
		return -ENODEV;
	}

	return 0;
}

static int ace_netlink_init_module(void)
{
	printk(  "Module ace_netlink init\n" );
	return ace_netlink_init();
}

static void ace_netlink_exit_module(void)
{
    if (ace_netlink_sock)
     	sock_release(ace_netlink_sock->sk_socket);
    
    printk(  "Module ace_netlink exit\n" );
}

module_init(ace_netlink_init_module);
module_exit(ace_netlink_exit_module);
