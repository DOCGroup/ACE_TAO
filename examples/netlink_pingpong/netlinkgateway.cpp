// $Id$

/*! \file netlinkgateway.h
     \brief netlinkgateway gateway code . 

     handles the io coming from the driver and dispatches it.

     IO coming from the driver is always at the size of msg+pagesize.
*/

#include "netlinkgateway.h"
#include "ace/Basic_Types.h"
#include "ace/Reactor.h"

#define NETLINK_UNUSED 14 //! this value must match the driver family's values


CNetlinkGateway* CSingleton<CNetlinkGateway>::m_instance=0;

CNetlinkGateway::CNetlinkGateway()
: ACE_Event_Handler(), CSingleton<CNetlinkGateway>(){}



CNetlinkGateway::~CNetlinkGateway(){}

/*! \fn  CNetlinkGateway::open()
\brief registers in the reactor and registers in the driver.
*/
int CNetlinkGateway::open()
{ 
m_Addr.set(0,0);
printf("CNetlinkGateway::open \n");

if ( 0> m_Socket.open(m_Addr,	AF_NETLINK,NETLINK_UNUSED) ){   

    ACE_ERROR((LM_ERROR,"CNetlinkGateway::open() failed to open socket\n"));
    return -1;
}

  
  reactor(ACE_Reactor::instance());
  if ( reactor() == NULL ){
  	ACE_ERROR((LM_ERROR,"CNetlinkGateway::open() failed to get reactor"));
	return (-1);
  }
  
  if (reactor()->register_handler(this, 
  			ACE_Event_Handler::READ_MASK
			) ){
   	ACE_ERROR((LM_ERROR,"CNetlinkGateway::open() failed to rgisters to reactor\n"));
	return -1;
  }
}                   

/*! \fn  CNetlinkGateway::handle_input
    \brief handle input must be a multithread function . when it is invoked
*/
int CNetlinkGateway::handle_input(ACE_HANDLE fd)
{

	NetlinkMsg msg;
	//	
	//Size of message if fixed at  PAGE_SIZE+sizeof(scipio_Msg)
	//
	int ret = m_Socket.recv(&msg,sizeof(msg),0);
	if (ret>0){     
               Ping(msg);
        }else{
	    ACE_ERROR((LM_ERROR,"CNetlinkGateway::handle_input no msg recived"));
	}
	return ret;
}


int CNetlinkGateway::handle_close(ACE_HANDLE fd,ACE_Reactor_Mask close_mask)
{
   m_Socket.close();
   return 0;
}

//
// I am not supposed to get here 
//
int CNetlinkGateway::handle_output(ACE_HANDLE fd)
{
    ACE_ERROR((LM_ERROR,"CNetlinkGateway::handle_output"));
    return 0;
}

/*! \fn  CNetlinkGateway::Pong
    \brief Pong method  send a reply message back to the driver
*/
int CNetlinkGateway::Ping(NetlinkMsg& msg){

   NetlinkMsg Reply;
   
   strcpy(msg.data,"Pong");
   Reply.h.dw_num = msg.h.dw_num++;
   Reply.h.dw_pid=getpid();
   
   int ret = m_Socket.send(&Reply,sizeof(Reply),0) ; 
   if ( 0< ret ){
         printf("pong %d\n",Reply.h.dw_num);
     }else{
        printf("pong %d failed \n",Reply.h.dw_num);
     }
   return ret;
}
