// $Id$

#include "MCast.h"
#include "Nil.h"

//------------------------------------------------------------
// TAO_AV_UDP_MCast_Acceptor
//------------------------------------------------------------

TAO_AV_UDP_MCast_Acceptor::TAO_AV_UDP_MCast_Acceptor (void)
{
}

TAO_AV_UDP_MCast_Acceptor::~TAO_AV_UDP_MCast_Acceptor (void)
{
}

int
TAO_AV_UDP_MCast_Acceptor::make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&mcast_handler)
{
  TAO_AV_Callback *callback = 0;
  if (this->endpoint_ != 0)
    {
      this->endpoint_->get_callback (this->flowname_.c_str (),
                                     callback);
      ACE_NEW_RETURN (mcast_handler,
                      TAO_AV_UDP_MCast_Flow_Handler (callback),
                      -1);
      callback->transport (mcast_handler->transport ());
      TAO_AV_Protocol_Object *object =0;
      ACE_NEW_RETURN (object,
                      TAO_AV_UDP_MCast_Object (callback,
                                               mcast_handler->transport ()),
                      -1);
      this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                            object);
      this->entry_->protocol_object (object);
    }
  return 0;
}

int
TAO_AV_UDP_MCast_Acceptor::open (TAO_Base_StreamEndPoint *endpoint,
                                 TAO_AV_Core *av_core,
                                 TAO_FlowSpec_Entry *entry)
{
  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;
  ACE_INET_Addr *mcast_addr = ACE_dynamic_cast (ACE_INET_Addr*, entry->address ());
  TAO_AV_UDP_MCast_Flow_Handler *handler;
  this->make_svc_handler (handler);
  int result = handler->get_mcast_socket ()->subscribe (*mcast_addr);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_connector::open failed\n"),-1);
  // Now disable Multicast loopback.
  // @@Should we make this a policy?
  if (handler->get_mcast_socket ()->set_option (IP_MULTICAST_LOOP,
                                            0) < 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Acceptor::multicast loop disable failed\n"));
  // @@ This should also be policies.
  int bufsize = 80 * 1024;
  if (handler->get_mcast_socket ()->ACE_SOCK::set_option (SOL_SOCKET, 
                                                          SO_RCVBUF, 
                                                          (char *)&bufsize,
                                                          sizeof(bufsize)) < 0) 
    {
      bufsize = 32 * 1024;
      if (handler->get_mcast_socket ()->ACE_SOCK::set_option (SOL_SOCKET, 
                                                              SO_RCVBUF, 
                                                              (char *)&bufsize,
                                                              sizeof(bufsize)) < 0)
      perror("SO_RCVBUF");
    }

  result = this->activate_svc_handler (handler);
  if (result < 0)
    return result;

  entry->handler (handler);
  ACE_INET_Addr *local_addr = 0;
  ACE_NEW_RETURN (local_addr,
                  ACE_INET_Addr (*mcast_addr),
                  -1);
  entry->set_local_addr (local_addr);
  return 0;
}

int
TAO_AV_UDP_MCast_Acceptor::open_default (TAO_Base_StreamEndPoint */*endpoint*/,
                                         TAO_AV_Core */*av_core*/,
                                         TAO_FlowSpec_Entry */*entry*/)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Acceptor::open_default\n"));
  return 0;
}

int
TAO_AV_UDP_MCast_Acceptor::activate_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *handler)
{
  ACE_Event_Handler *event_handler = handler;
  //  ACE_HANDLE fd = event_handler->get_handle ();
  int result = this->av_core_->reactor ()->register_handler (event_handler,
                                                 ACE_Event_Handler::READ_MASK);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Dgram_Acceptor::activate_svc_handler failed\n"),result);
  return 0;
}

int
TAO_AV_UDP_MCast_Acceptor::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_MCast_Connector
//------------------------------------------------------------
TAO_AV_UDP_MCast_Connector::TAO_AV_UDP_MCast_Connector (void)
{
}

TAO_AV_UDP_MCast_Connector::~TAO_AV_UDP_MCast_Connector (void)
{
}

int
TAO_AV_UDP_MCast_Connector::make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&mcast_handler)
{
  TAO_AV_Callback *callback = 0;
  if (this->endpoint_ != 0)
    {
      this->endpoint_->get_callback (this->flowname_.c_str (),
                                     callback);
      ACE_NEW_RETURN (mcast_handler,
                      TAO_AV_UDP_MCast_Flow_Handler (callback),
                      -1);
      callback->transport (mcast_handler->transport ());
      TAO_AV_Protocol_Object *object =0;
      ACE_NEW_RETURN (object,
                      TAO_AV_UDP_MCast_Object (callback,
                                               mcast_handler->transport ()),
                      -1);
      this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                            object);
      this->entry_->protocol_object (object);
    }
  return 0;
}

int
TAO_AV_UDP_MCast_Connector::open (TAO_Base_StreamEndPoint *endpoint,
                                  TAO_AV_Core *av_core)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Connector::open "));
  this->endpoint_ = endpoint;
  this->av_core_ = av_core;
  return 0;
}

int
TAO_AV_UDP_MCast_Connector::connect (TAO_FlowSpec_Entry *entry,
                                     TAO_AV_Transport *&transport)
{
  this->entry_ = entry;
  ACE_INET_Addr *mcast_addr = ACE_dynamic_cast (ACE_INET_Addr*, entry->address ());
  TAO_AV_UDP_MCast_Flow_Handler *handler;
  this->make_svc_handler (handler);
  int result = handler->get_mcast_socket ()->subscribe (*mcast_addr);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_connector::open failed\n"),-1);
  // Now disable Multicast loopback.
  // @@Should we make this a policy?
  if (handler->get_mcast_socket ()->set_option (IP_MULTICAST_LOOP,
                                            0) < 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Acceptor::multicast loop disable failed\n"));
  int bufsize = 80 * 1024;
  if (handler->get_mcast_socket ()->ACE_SOCK::set_option (SOL_SOCKET, 
                                                          SO_RCVBUF, 
                                                          (char *)&bufsize,
                                                          sizeof(bufsize)) < 0) 
    {
      bufsize = 32 * 1024;
      if (handler->get_mcast_socket ()->ACE_SOCK::set_option (SOL_SOCKET, 
                                                              SO_RCVBUF, 
                                                              (char *)&bufsize,
                                                              sizeof(bufsize)) < 0)
      perror("SO_RCVBUF");
    }

  result = this->activate_svc_handler (handler);
  if (result < 0)
    return result;
  entry->handler (handler);
  transport = handler->transport ();
  ACE_INET_Addr *local_addr = 0;
  ACE_NEW_RETURN (local_addr,
                  ACE_INET_Addr (*mcast_addr),
                  -1);
  entry->set_local_addr (local_addr);
  return 0;
}

int
TAO_AV_UDP_MCast_Connector::activate_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *handler)
{
  ACE_Event_Handler *event_handler = handler;
  //  ACE_HANDLE fd = event_handler->get_handle ();
  int result = this->av_core_->reactor ()->register_handler (event_handler,
                                                 ACE_Event_Handler::READ_MASK);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Dgram_Acceptor::activate_svc_handler failed\n"),result);
  return 0;
}

int
TAO_AV_UDP_MCast_Connector::close (void)
{
  return 0;
}

//------------------------------------------------------------
//TAO_AV_UDP_MCast_Flow_Handler
//------------------------------------------------------------

TAO_AV_UDP_MCast_Flow_Handler::TAO_AV_UDP_MCast_Flow_Handler (TAO_AV_Callback *callback)
  :TAO_AV_Flow_Handler (callback)
{
  ACE_NEW (transport_,
           TAO_AV_UDP_MCast_Transport (this));
  ACE_NEW (dgram_mcast_,
           ACE_SOCK_Dgram_Mcast);

}

TAO_AV_UDP_MCast_Flow_Handler::~TAO_AV_UDP_MCast_Flow_Handler (void)
{
  delete this->transport_;
  delete this->dgram_mcast_;
}


int
TAO_AV_UDP_MCast_Flow_Handler::handle_input (ACE_HANDLE /*fd*/)
{
  size_t size = 2*this->transport_->mtu ();
  ACE_Message_Block *frame = 0;
  ACE_NEW_RETURN (frame,
                  ACE_Message_Block (size),
                  -1);
  int n = this->transport_->recv (frame->rd_ptr (),
                                  frame->size ());
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_MCast_Flow_Handler::handle_input recv failed\n"),-1);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_MCast_Flow_Handler::handle_input connection closed\n"),-1);
  frame->wr_ptr (n);
  this->callback_->receive_frame (frame);
  return 0;
}

ACE_INLINE ACE_HANDLE
TAO_AV_UDP_MCast_Flow_Handler::get_handle (void) const
{
  ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Flow_Handler::get_handle "));
  return this->get_mcast_socket ()->get_handle () ;
}


//------------------------------------------------------------
// TAO_AV_UDP_MCast_Transport
//------------------------------------------------------------

TAO_AV_UDP_MCast_Transport::TAO_AV_UDP_MCast_Transport (void)
  :handler_ (0)
{
}

TAO_AV_UDP_MCast_Transport::TAO_AV_UDP_MCast_Transport (TAO_AV_UDP_MCast_Flow_Handler *handler)
  :handler_ (handler)
{
}

TAO_AV_UDP_MCast_Transport::~TAO_AV_UDP_MCast_Transport (void)
{
}

int
TAO_AV_UDP_MCast_Transport::open (ACE_Addr */*address*/)
{
  return 0;
}

int
TAO_AV_UDP_MCast_Transport::close (void)
{
  return 0;
}


ACE_Addr*
TAO_AV_UDP_MCast_Transport::get_peer_addr (void)
{
  return &this->peer_addr_;
}

ACE_Addr*
TAO_AV_UDP_MCast_Transport::get_local_addr (void)
{
  this->handler_->get_mcast_socket ()->get_local_addr (this->local_addr_);
  return &this->local_addr_;
}

ssize_t
TAO_AV_UDP_MCast_Transport::send (const ACE_Message_Block *mblk, ACE_Time_Value *)
{
  // For the most part this was copied from GIOP::send_request and
  // friends.

  iovec iov[IOV_MAX];
  int iovcnt = 0;
  ssize_t n = 0;
  ssize_t nbytes = 0;

  for (const ACE_Message_Block *i = mblk;
       i != 0;
       i = i->cont ())
    {
      // Make sure there is something to send!
      if (i->length () > 0)
        {
          iov[iovcnt].iov_base = i->rd_ptr ();
          iov[iovcnt].iov_len  = i->length ();
          iovcnt++;

          // The buffer is full make a OS call.  @@ TODO this should
          // be optimized on a per-platform basis, for instance, some
          // platforms do not implement writev() there we should copy
          // the data into a buffer and call send_n(). In other cases
          // there may be some limits on the size of the iovec, there
          // we should set IOV_MAX to that limit.
          if (iovcnt == IOV_MAX)
            {
              n = this->handler_->get_mcast_socket ()->send ((const iovec *) iov,
                                                             iovcnt);
              
              if (n < 1)
                return n;

              nbytes += n;
              iovcnt = 0;
            }
        }
    }

  // Check for remaining buffers to be sent!
  if (iovcnt != 0)
    {
      n = this->handler_->get_mcast_socket ()->send ((const iovec *) iov,
                                                     iovcnt);
      if (n < 1)
        return n;

      nbytes += n;
    }

  return nbytes;
}

ssize_t
TAO_AV_UDP_MCast_Transport::send (const char *buf,
                                  size_t len,
                                  ACE_Time_Value *)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Transport::send "));
  char addr [BUFSIZ];
  this->peer_addr_.addr_to_string (addr,BUFSIZ);
  ACE_DEBUG ((LM_DEBUG,"to %s\n",addr));

  return this->handler_->get_mcast_socket ()->send (buf, len);
}

ssize_t
TAO_AV_UDP_MCast_Transport::send (const iovec *iov,
                                  int iovcnt,
                                  ACE_Time_Value *)
{
  return this->handler_->get_mcast_socket ()->send (iov,
                                                    iovcnt,
                                                    0);

}

ACE_INLINE int
TAO_AV_UDP_MCast_Transport::mtu (void)
{
  return ACE_MAX_DGRAM_SIZE;
}


//ACE_INLINE ssize_t
//TAO_AV_UDP_MCast_Transport::send (const iovec *iov,
//                                  int iovcnt,
//                                  ACE_Time_Value *)
//{
//  return this->handler_->get_mcast_socket ()->send (iov,
//                                                    iovcnt,
//                                                    0);
//
//}
//
ACE_INLINE ssize_t
TAO_AV_UDP_MCast_Transport::recv (char *buf,
                                  size_t len,
                                  ACE_Time_Value *)
{
  return this->handler_->get_mcast_socket ()->recv (buf, len,this->peer_addr_);
}

ACE_INLINE ssize_t
TAO_AV_UDP_MCast_Transport::recv (char *buf,
                                  size_t len,
                                  int flags,
                                  ACE_Time_Value *timeout)
{
  return this->handler_->get_mcast_socket ()->recv (buf,
                                                    len,
                                                    this->peer_addr_,
                                                    flags,
                                                    timeout);
}


ssize_t
TAO_AV_UDP_MCast_Transport::recv (iovec *iov,
                                  int /*iovcnt*/,
                            ACE_Time_Value *timeout)
{
  return handler_->get_mcast_socket ()->recv (iov,this->peer_addr_,0,timeout);
}


//------------------------------------------------------------
// TAO_AV_UDP_MCast_Protocol_Factory
//------------------------------------------------------------

TAO_AV_UDP_MCast_Protocol_Factory::TAO_AV_UDP_MCast_Protocol_Factory (void)
{
}

TAO_AV_UDP_MCast_Protocol_Factory::~TAO_AV_UDP_MCast_Protocol_Factory (void)
{
}

int
TAO_AV_UDP_MCast_Protocol_Factory::match_protocol (TAO_AV_Core::Protocol protocol)
{
  return (protocol == TAO_AV_Core::TAO_AV_UDP_MCAST);
}


TAO_AV_Acceptor*
TAO_AV_UDP_MCast_Protocol_Factory::make_acceptor (void)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Protocol_Factory::make_acceptor "));
  TAO_AV_Acceptor *acceptor = 0;
  ACE_NEW_RETURN (acceptor,
                  TAO_AV_UDP_MCast_Acceptor,
                  0);
  return acceptor;
}

TAO_AV_Connector*
TAO_AV_UDP_MCast_Protocol_Factory::make_connector (void)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Protocol_Factory::make_connector "));
  TAO_AV_Connector *connector = 0;
  ACE_NEW_RETURN (connector,
                  TAO_AV_UDP_MCast_Connector,
                  0);
  return connector;
}
