// $Id$

#include "UDP.h"
#include "AVStreams_i.h"
#include "MCast.h"

#if !defined (__ACE_INLINE__)
#include "UDP.i"
#endif /* __ACE_INLINE__ */

//------------------------------------------------------------
// TAO_AV_UDP_Flow_Handler
//------------------------------------------------------------

TAO_AV_UDP_Flow_Handler::TAO_AV_UDP_Flow_Handler (void)
{
  ACE_NEW (this->transport_,
           TAO_AV_UDP_Transport (this));
}

TAO_AV_UDP_Flow_Handler::~TAO_AV_UDP_Flow_Handler (void)
{
  delete this->transport_;
}

TAO_AV_Transport *
TAO_AV_UDP_Flow_Handler::transport (void)
{
  return this->transport_;
}

int
TAO_AV_UDP_Flow_Handler::handle_input (ACE_HANDLE /*fd*/)
{
  this->protocol_object_->handle_input ();
  return 0;
}

int
TAO_AV_UDP_Flow_Handler::handle_timeout (const ACE_Time_Value &tv, 
                                         const void *arg)
{
  return TAO_AV_Flow_Handler::handle_timeout (tv,arg);
}

int
TAO_AV_UDP_Flow_Handler::set_remote_address (ACE_Addr *address)
{
  if (TAO_debug_level > 0) 
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Flow_Handler::set_remote_address\n"));
  
  ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,
                                               address);
  this->peer_addr_ = *inet_addr;
  TAO_AV_UDP_Transport *transport = ACE_dynamic_cast (TAO_AV_UDP_Transport*,
                                                      this->transport_);
  
  return transport->set_remote_address (*inet_addr);
}


ACE_HANDLE
TAO_AV_UDP_Flow_Handler::get_handle (void) const
{
  if (TAO_debug_level > 0) 
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_Flow_Handler::get_handle:%d\n",
                this->sock_dgram_.get_handle ()));

  return this->sock_dgram_.get_handle () ;
}

//------------------------------------------------------------
// TAO_AV_UDP_Transport
//------------------------------------------------------------

TAO_AV_UDP_Transport::TAO_AV_UDP_Transport (void)
  :handler_ (0)
{
}

TAO_AV_UDP_Transport::TAO_AV_UDP_Transport (TAO_AV_UDP_Flow_Handler *handler)
  :handler_ (handler),
   addr_ (0)
{
}

TAO_AV_UDP_Transport::~TAO_AV_UDP_Transport (void)
{
}

int
TAO_AV_UDP_Transport::set_remote_address (const ACE_INET_Addr &address)
{
  this->peer_addr_ = address;
  return 0;
}

int
TAO_AV_UDP_Transport::open (ACE_Addr * /*address*/)
{
  return 0;
}

int
TAO_AV_UDP_Transport::close (void)
{
  return 0;
}

int
TAO_AV_UDP_Transport::mtu (void)
{
  return ACE_MAX_DGRAM_SIZE;
}

ACE_Addr*
TAO_AV_UDP_Transport::get_peer_addr (void)
{
  return &this->peer_addr_;
}

ssize_t
TAO_AV_UDP_Transport::send (const ACE_Message_Block *mblk, ACE_Time_Value *)
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
              n = this->handler_->get_socket ()->send ((const iovec *) iov,
                                                        iovcnt,
                                                        this->peer_addr_);

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
      n = this->handler_->get_socket ()->send ((const iovec *) iov,
                                               iovcnt,
                                               this->peer_addr_);

      if (n < 1)
        return n;

      nbytes += n;
    }

  return nbytes;
}

ssize_t
TAO_AV_UDP_Transport::send (const char *buf,
                            size_t len,
                            ACE_Time_Value *)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Transport::send "));
  char addr [BUFSIZ];
  this->peer_addr_.addr_to_string (addr,BUFSIZ);
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"to %s\n",addr));

  return this->handler_->get_socket ()->send (buf, len,this->peer_addr_);
}

ssize_t
TAO_AV_UDP_Transport::send (const iovec *iov,
                          int iovcnt,
                          ACE_Time_Value *)
{
  return this->handler_->get_socket ()->send ((const iovec *) iov,
                                              iovcnt,
                                              this->peer_addr_);

}

ssize_t
TAO_AV_UDP_Transport::recv (char *buf,
                            size_t len,
                            ACE_Time_Value *)
{
  return this->handler_->get_socket ()->recv (buf, len,this->peer_addr_);
}

ssize_t
TAO_AV_UDP_Transport::recv (char *buf,
                            size_t len,
                            int flags,
                            ACE_Time_Value *timeout)
{
  return this->handler_->get_socket ()->recv (buf,
                                              len,
                                              this->peer_addr_,
                                              flags,
                                              timeout);
}

ssize_t
TAO_AV_UDP_Transport::recv (iovec *iov,
                            int /*iovcnt*/,
                            ACE_Time_Value *timeout)
{
  return handler_->get_socket ()->recv (iov,this->peer_addr_,0,timeout);
}


//------------------------------------------------------------
// TAO_AV_UDP_Acceptor
//------------------------------------------------------------

TAO_AV_UDP_Acceptor::TAO_AV_UDP_Acceptor (void)
{
}

TAO_AV_UDP_Acceptor::~TAO_AV_UDP_Acceptor (void)
{
}

int
TAO_AV_UDP_Acceptor::activate_svc_handler (TAO_AV_Flow_Handler *handler)
{
  ACE_Event_Handler *event_handler = handler->event_handler ();
  int result = this->av_core_->reactor ()->register_handler (event_handler,
                                                             ACE_Event_Handler::READ_MASK);
  return result;
}

int
TAO_AV_UDP_Acceptor::open (TAO_Base_StreamEndPoint *endpoint,
                           TAO_AV_Core *av_core,
                           TAO_FlowSpec_Entry *entry,
                           TAO_AV_Flow_Protocol_Factory *factory)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Acceptor::open "));
  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;
  this->flow_protocol_factory_ = factory;
  this->flowname_ = entry->flowname ();
  ACE_INET_Addr *inet_addr = (ACE_INET_Addr *) entry->address ();

  char buf[BUFSIZ];
  inet_addr->addr_to_string (buf,
                             BUFSIZ);
  
  if (TAO_debug_level > 0) 
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_Acceptor::open: %s",
                buf));

  int result = this->open_i (inet_addr);
  
  if (result < 0)
    return result;
  return 0;
}

int
TAO_AV_UDP_Acceptor::open_default (TAO_Base_StreamEndPoint *endpoint,
                                   TAO_AV_Core *av_core,
                                   TAO_FlowSpec_Entry *entry,
                                   TAO_AV_Flow_Protocol_Factory *factory)
{
  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;
  this->flow_protocol_factory_ = factory;
  this->flowname_ = entry->flowname ();
  ACE_INET_Addr *address;
  ACE_NEW_RETURN (address,
                  ACE_INET_Addr ("0"),
                  -1);
  int result = this->open_i (address);
  if (result < 0)
    return result;
  return 0;
}

int
TAO_AV_UDP_Acceptor::open_i (ACE_INET_Addr *inet_addr)
{
  int result = -1;

  ACE_INET_Addr *local_addr;
  TAO_AV_Flow_Handler *flow_handler = 0;

  if (this->entry_->is_multicast ())
    {
      TAO_AV_UDP_MCast_Flow_Handler *handler;
      ACE_NEW_RETURN (handler,
                      TAO_AV_UDP_MCast_Flow_Handler,
                      -1);
  
      flow_handler = handler;
      result = handler->get_mcast_socket ()->subscribe (*inet_addr);
      
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO_AV_UDP_MCast_connector::subscribe failed\n"),
                          -1);

      // Now disable Multicast loopback.
      // @@Should we make this a policy?
#ifdef ACE_HAS_IP_MULTICAST
      if (handler->get_mcast_socket ()->set_option (IP_MULTICAST_LOOP,
                                                    0) < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Acceptor::multicast loop disable failed\n"));
      // @@ This should also be policies.
#endif /*ACE_HAS_IP_MULTICAST*/
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
      ACE_NEW_RETURN (local_addr,
                      ACE_INET_Addr (*inet_addr),
                      -1);
    }
  else
    {
      TAO_AV_UDP_Flow_Handler *handler;
      ACE_NEW_RETURN (handler,
                      TAO_AV_UDP_Flow_Handler,
                      -1);
      flow_handler = handler;
      int result = handler->open (*inet_addr);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_Acceptor::handler_open failed\n"),-1);
      // set the socket buffer sizes to 64k.
      int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
      int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;

      if (handler->get_socket ()->set_option (SOL_SOCKET,
                                              SO_SNDBUF,
                                              (void *) &sndbufsize,
                                              sizeof (sndbufsize)) == -1
          && errno != ENOTSUP)
        return 0;

      else if (handler->get_socket ()->set_option (SOL_SOCKET,
                                                   SO_RCVBUF,
                                                   (void *) &rcvbufsize,
                                                   sizeof (rcvbufsize)) == -1
               && errno != ENOTSUP)
        return 0;

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"SOCK_Dgram::open failed\n"),-1);
      ACE_NEW_RETURN (local_addr,
                      ACE_INET_Addr,
                      -1);
      result = handler->get_socket ()->get_local_addr (*local_addr);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Dgram_Connector::open: get_local_addr failed\n"),result);
      local_addr->set (local_addr->get_port_number (),
                       local_addr->get_host_name ());
    }
  TAO_AV_Protocol_Object *object =
    this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                        this->endpoint_,
                                                        flow_handler,
                                                        flow_handler->transport ());
  flow_handler->protocol_object (object);
  //  callback->protocol_object (object);
//   this->endpoint_->set_protocol_object (this->flowname_.c_str (),
//                                         object);
  this->endpoint_->set_flow_handler (this->flowname_.c_str (),flow_handler);
  this->entry_->protocol_object (object);

  char buf[BUFSIZ];
  local_addr->addr_to_string (buf,BUFSIZ);
  if (TAO_debug_level > 0) 
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_ACCEPTOR::open:%s \n",
                buf));
  
  this->entry_->set_local_addr (local_addr);
  this->entry_->handler (flow_handler);
  
// call activate svc handler.
  return this->activate_svc_handler (flow_handler);
}

int
TAO_AV_UDP_Acceptor::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_Connector
//------------------------------------------------------------
TAO_AV_UDP_Connector::TAO_AV_UDP_Connector (void)
{
}

TAO_AV_UDP_Connector::~TAO_AV_UDP_Connector (void)
{
}

int
TAO_AV_UDP_Connector::open (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_AV_Flow_Protocol_Factory *factory)

{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Connector::open "));
  this->endpoint_ = endpoint;
  this->av_core_ = av_core;
  this->flow_protocol_factory_ = factory;
  return 0;
}

int
TAO_AV_UDP_Connector::connect (TAO_FlowSpec_Entry *entry,
                               TAO_AV_Transport *&transport)
{
        
  ACE_INET_Addr *local_addr;
  
  int result = -1;
  this->entry_ = entry;
  this->flowname_ = entry->flowname ();

  ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,
                                               entry->address ());
  
  TAO_AV_Flow_Handler *flow_handler = 0;
  
  if (entry->is_multicast ())
    {
      TAO_AV_UDP_MCast_Flow_Handler *handler;
      ACE_NEW_RETURN (handler,
                      TAO_AV_UDP_MCast_Flow_Handler,
                      -1);

      flow_handler = handler;
      
      result = handler->get_mcast_socket ()->subscribe (*inet_addr);
      
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_MCast_connector::open failed\n"),-1);
      // Now disable Multicast loopback.
      // @@Should we make this a policy?
#if defined (ACE_HAS_IP_MULTICAST)
      if (handler->get_mcast_socket ()->set_option (IP_MULTICAST_LOOP,
                                                    0) < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Acceptor::multicast loop disable failed\n"));
      // @@ This should also be policies.
#endif /*ACE_HAS_IP_MULTICAST*/

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
      ACE_NEW_RETURN (local_addr,
                      ACE_INET_Addr (*inet_addr),
                      -1);
    }
  else
    {

      ACE_NEW_RETURN (local_addr,
                      ACE_INET_Addr ("0"),
                      -1);
      
      TAO_AV_UDP_Flow_Handler *handler;
      ACE_NEW_RETURN (handler,
                      TAO_AV_UDP_Flow_Handler,
                      -1);
      
      flow_handler = handler;
      
      result = handler->open (*local_addr);
      
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"handler::open failed\n"),-1);
      
      // set the socket buffer sizes to 64k.
      int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
      int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;

      if (handler->get_socket ()->set_option (SOL_SOCKET,
                                              SO_SNDBUF,
                                              (void *) &sndbufsize,
                                              sizeof (sndbufsize)) == -1
          && errno != ENOTSUP)
        return 0;

      else if (handler->get_socket ()->set_option (SOL_SOCKET,
                                                   SO_RCVBUF,
                                                   (void *) &rcvbufsize,
                                                   sizeof (rcvbufsize)) == -1
               && errno != ENOTSUP)
        return 0;

      handler->set_remote_address  (inet_addr);
      
      result = handler->get_socket ()->get_local_addr (*local_addr);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Dgram_Connector::open: get_local_addr failed\n"),result);
      
      local_addr->set (local_addr->get_port_number (),
                       local_addr->get_host_name ());
    }

  TAO_AV_Protocol_Object *object =
    this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                        this->endpoint_,
                                                        flow_handler,
                                                        flow_handler->transport ());
  flow_handler->protocol_object (object);

  this->endpoint_->set_flow_handler (this->flowname_.c_str (),
                                     flow_handler);

  this->entry_->protocol_object (object);

  char buf[BUFSIZ];
  local_addr->addr_to_string (buf,BUFSIZ);

  if (TAO_debug_level > 0) 
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_CONNECTOR::connect:%s \n",buf));

  entry->set_local_addr (local_addr);
  entry->handler (flow_handler);
  transport = flow_handler->transport ();

  // call activate svc handler.
  return this->activate_svc_handler (flow_handler);
}

int
TAO_AV_UDP_Connector::activate_svc_handler (TAO_AV_Flow_Handler *handler)
{
  ACE_Event_Handler *event_handler = handler->event_handler ();
  int result = this->av_core_->reactor ()->register_handler (event_handler,
                                                             ACE_Event_Handler::READ_MASK);
  return result;
}

int
TAO_AV_UDP_Connector::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_Protocol_Factory
//------------------------------------------------------------

TAO_AV_UDP_Factory::TAO_AV_UDP_Factory (void)
{
}

TAO_AV_UDP_Factory::~TAO_AV_UDP_Factory (void)
{
}

int
TAO_AV_UDP_Factory::match_protocol (const char *protocol_string)
{
  if (ACE_OS::strstr (protocol_string,"UDP") != 0)
    return 1;
  return 0;
}

TAO_AV_Acceptor*
TAO_AV_UDP_Factory::make_acceptor (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Factory::make_acceptor "));
  TAO_AV_Acceptor *acceptor = 0;
  ACE_NEW_RETURN (acceptor,
                  TAO_AV_UDP_Acceptor,
                  0);
  return acceptor;
}

TAO_AV_Connector*
TAO_AV_UDP_Factory::make_connector (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Factory::make_connector "));
  TAO_AV_Connector *connector = 0;
  ACE_NEW_RETURN (connector,
                  TAO_AV_UDP_Connector,
                  0);
  return connector;
}

int
TAO_AV_UDP_Factory::init (int /* argc */,
                          char * /* argv */ [])
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_Object
//------------------------------------------------------------

int
TAO_AV_UDP_Object::handle_input (void)
{
  int n = this->transport_->recv (this->frame_.rd_ptr (),
                                  this->frame_.size ());
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_Flow_Handler::handle_input recv failed\n"),-1);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_Flow_Handler::handle_input connection closed\n"),-1);
  this->frame_.wr_ptr (this->frame_.rd_ptr () + n);

  return this->callback_->receive_frame (&this->frame_);
}

int
TAO_AV_UDP_Object::send_frame (ACE_Message_Block *frame,
                               TAO_AV_frame_info * /*frame_info*/)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Object::send_frame\n"));
  int result = this->transport_->send (frame);
  if (result < 0)
    return result;
  return 0;
}

int 
TAO_AV_UDP_Object::send_frame (const iovec *iov,
                               int iovcnt,
                               TAO_AV_frame_info * /*frame_info*/)
{
  return this->transport_->send (iov,iovcnt);
}

int
TAO_AV_UDP_Object::send_frame (const char*buf,
                               size_t len)
{
  int result = this->transport_->send (buf, len, 0);
  if (result < 0)
    return result;
  return 0;
}

TAO_AV_UDP_Object::TAO_AV_UDP_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
  this->frame_.size (2 * this->transport_->mtu ());
}

TAO_AV_UDP_Object::~TAO_AV_UDP_Object (void)
{
  //no-op
}

int
TAO_AV_UDP_Object::destroy (void)
{
  this->callback_->handle_destroy ();
  return 0;
}


//------------------------------------------------------------
// TAO_AV_UDP_Flow_Factory
//------------------------------------------------------------
TAO_AV_UDP_Flow_Factory::TAO_AV_UDP_Flow_Factory (void)
{
}

TAO_AV_UDP_Flow_Factory::~TAO_AV_UDP_Flow_Factory (void)
{
}

int
TAO_AV_UDP_Flow_Factory::init (int /* argc */,
                               char * /* argv */ [])
{
  return 0;
}

int
TAO_AV_UDP_Flow_Factory::match_protocol (const char *flow_string)
{
  if (ACE_OS::strcasecmp (flow_string,"UDP") == 0)
    return 1;
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_UDP_Flow_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                               TAO_Base_StreamEndPoint *endpoint,
                                               TAO_AV_Flow_Handler *handler,
                                               TAO_AV_Transport *transport)
{
  TAO_AV_Callback *callback = 0;
  endpoint->get_callback (entry->flowname (),
                          callback);


  TAO_AV_UDP_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_UDP_Object (callback,
                                     transport),
                  0);
  callback->open (object,
                  handler);
  endpoint->set_protocol_object (entry->flowname (),
                                 object);
  return object;
}

ACE_FACTORY_DEFINE (AV, TAO_AV_UDP_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_UDP_Flow_Factory,
                       ACE_TEXT ("UDP_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_UDP_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (AV, TAO_AV_UDP_Factory)

ACE_STATIC_SVC_DEFINE (TAO_AV_UDP_Factory,
                       ACE_TEXT ("UDP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_UDP_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)


