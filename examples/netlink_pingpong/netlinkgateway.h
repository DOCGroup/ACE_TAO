// $Id$
/*! \file netlinkgateway.h
    \brief  netlink getway class. 
    
    This class is used to pass buffer to the driver and back.
    
    Why use netlink sockets as a communication facility 
    
*/
#ifndef NETLINKGATEWAY_H
#define NETLINKGATEWAY_H

#include "ace/SOCK_Netlink.h"
#include "ace/Reactor.h"
#include "ace/SOCK_IO.h"
#include "ace/Event_Handler.h"
#include "singleton.h"
#include "netlinkmsg.h"

/*! \class CNetlinkGateway
    \brief Reactor plug in class for netlink sockets.
    
    Handles incoming buffers/messages  and sends them back.
    unlike its name  ( lack of time ) it acts both as a gateways and a dispatcher.
*/
class CNetlinkGateway : public ACE_Event_Handler,
			public CSingleton<CNetlinkGateway>
{
    ACE_Netlink_Addr m_Addr;
    ACE_SOCK_Netlink m_Socket;

protected:

    CNetlinkGateway();

public:
    friend class CSingleton<CNetlinkGateway> ;
    
     ~CNetlinkGateway();
    //!< Get this handler's I/O handle.
    virtual ACE_HANDLE get_handle (void) const{
	return m_Socket.get_handle (); 
    }

    int	  Send(char* buf,int dwSize){ return m_Socket.send(buf,dwSize,0);}
    int   Ping(NetlinkMsg& msg);
    void  Start() { NetlinkMsg msg={0}; Ping(msg); }
    //
    // Reacotr methods 
    //
    virtual int open () ; //!< 
    //!< Called when input is available from the client.
    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
    //!< Called when output is possible.
    virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);
    //!< Called when this handler is removed from the ACE_Reactor.
    virtual int handle_close (ACE_HANDLE handle,ACE_Reactor_Mask close_mask);

};


#endif
