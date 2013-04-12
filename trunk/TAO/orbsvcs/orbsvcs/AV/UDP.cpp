// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/UDP.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/MCast.h"

#include "tao/debug.h"
#include "ace/OS_NS_strings.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/AV/UDP.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  // remove the event handler from the reactor.
  TAO_AV_CORE::instance()->reactor ()->remove_handler (this->event_handler(),
                                              ACE_Event_Handler::READ_MASK);

  // close the socket.
  this->close ();
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
  return this->protocol_object_->handle_input ();
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
    ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Flow_Handler::set_remote_address\n"));

  ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (address);
  this->peer_addr_ = *inet_addr;
  TAO_AV_UDP_Transport *transport = dynamic_cast<TAO_AV_UDP_Transport*> (this->transport_);

  return transport->set_remote_address (*inet_addr);
}


ACE_HANDLE
TAO_AV_UDP_Flow_Handler::get_handle (void) const
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_Flow_Handler::get_handle:%d\n",
                this->sock_dgram_.get_handle ()));

  return this->sock_dgram_.get_handle () ;
}

int
TAO_AV_UDP_Flow_Handler::change_qos(AVStreams::QoS qos)
{
  if( TAO_debug_level > 0 )
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "(%N,%l) TAO_AV_UDP_Flow_Handler::change_qos\n"));
    }

  unsigned int i=0;

  int ret = 0;
  CORBA::Long dscp = 0;
  CORBA::Long ecn = 0;
  int dscp_flag=0;
  for(i=0; i < qos.QoSParams.length(); i++)
    {

      if( ACE_OS::strcmp( qos.QoSParams[i].property_name.in(), "Diffserv_Codepoint") == 0)
        {
          qos.QoSParams[i].property_value >>= dscp;
          dscp_flag=1;
          // DSCP value can only be 6 bits wide
          if(!((dscp >= 0) && (dscp <= 63)))
            {
              dscp_flag = 0;
              ORBSVCS_DEBUG((LM_DEBUG, "(%N,%l) ECN value can only be (0-3) not %d\n", ecn));
              return -1;
            }
        }

      if( ACE_OS::strcmp( qos.QoSParams[i].property_name.in(), "ECN") == 0)
        {
          qos.QoSParams[i].property_value >>= ecn;
          // ECN value can only occupy bits 6 and 7 of the
          // IP Diffserv byte
          if(!((ecn >= 0) && (ecn <= 3)))
            {
              ORBSVCS_DEBUG((LM_DEBUG, "(%N,%l) ECN value can only be (0-3) not %d\n", ecn));
              ecn = 0;
            }

        }
    }
      // Set the Diffserv byte only if we specified
      // the Diffserv Codepoint (DSCP) or ECN via QoSParams
      // passed into this method
      if(dscp_flag || ecn)
        {
          int tos;
          tos = (int)(dscp << 2);
          if(ecn)
            {
              tos |= ecn;
            }
          ret = sock_dgram_.set_option(IPPROTO_IP, IP_TOS, (int *)&tos , (int)sizeof(tos));

          if(TAO_debug_level > 1)
            {
              ORBSVCS_DEBUG((LM_DEBUG, "(%N,%l) set tos: ret: %d\n", ret));
            }
        }

      if(TAO_debug_level > 1)
        {
           if(ret < 0 )
             {
                ORBSVCS_DEBUG((LM_DEBUG, "(%N,%l) errno: %p\n"));
             }
        }
      return ret;
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
  return 65535;
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

  iovec iov[ACE_IOV_MAX];
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
          iov[iovcnt].iov_len  = static_cast<u_long> (i->length ());
          iovcnt++;

          // The buffer is full make a OS call.  @@ TODO this should
          // be optimized on a per-platform basis, for instance, some
          // platforms do not implement writev() there we should copy
          // the data into a buffer and call send_n(). In other cases
          // there may be some limits on the size of the iovec, there
          // we should set ACE_IOV_MAX to that limit.
          if (iovcnt == ACE_IOV_MAX)
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
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Transport::send "));
  ACE_TCHAR addr [BUFSIZ];
  this->peer_addr_.addr_to_string (addr,BUFSIZ);
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"to %s\n",addr));

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
  : address_ (0),
    control_inet_address_ (0)
{
}

TAO_AV_UDP_Acceptor::~TAO_AV_UDP_Acceptor (void)
{
  if (this->flow_component_ == TAO_AV_Core::TAO_AV_CONTROL)
    delete this->entry_->control_handler ();

  delete this->address_;
  delete this->control_inet_address_;
}

int
TAO_AV_UDP_Acceptor::activate_svc_handler (TAO_AV_Flow_Handler *handler)
{
  ACE_Event_Handler *event_handler = handler->event_handler ();
  int result = this->av_core_->reactor ()->register_handler (event_handler,
                                                             ACE_Event_Handler::READ_MASK);

  if (this->flow_component_ == TAO_AV_Core::TAO_AV_CONTROL)
    handler->schedule_timer ();

 return result;
}

int
TAO_AV_UDP_Acceptor::open (TAO_Base_StreamEndPoint *endpoint,
                           TAO_AV_Core *av_core,
                           TAO_FlowSpec_Entry *entry,
                           TAO_AV_Flow_Protocol_Factory *factory,
                           TAO_AV_Core::Flow_Component flow_comp)
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Acceptor::open\n"));
  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;
  this->flow_component_ = flow_comp;
  this->flow_protocol_factory_ = factory;
  ACE_INET_Addr *inet_addr;
  if (flow_comp == TAO_AV_Core::TAO_AV_CONTROL)
    {
      this->flowname_ = TAO_AV_Core::get_control_flowname (entry->flowname ());
      inet_addr = (ACE_INET_Addr *) entry->control_address ();
    }
  else
    {
      this->flowname_ = entry->flowname ();
      inet_addr = (ACE_INET_Addr *) entry->address ();
    }

  if (inet_addr != 0)
    {
      ACE_TCHAR buf[BUFSIZ];
      inet_addr->addr_to_string (buf, BUFSIZ);

      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    "TAO_AV_UDP_Acceptor::open: %s\n",
                    buf));
    }

  int result = this->open_i (inet_addr, 0);

  if (result < 0)
    return result;
  return 0;
}

int
TAO_AV_UDP_Acceptor::open_default (TAO_Base_StreamEndPoint *endpoint,
                                   TAO_AV_Core *av_core,
                                   TAO_FlowSpec_Entry *entry,
                                   TAO_AV_Flow_Protocol_Factory *factory,
                                   TAO_AV_Core::Flow_Component flow_comp)
{
  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;
  this->flow_component_ = flow_comp;
  this->flow_protocol_factory_ = factory;
  if (flow_comp == TAO_AV_Core::TAO_AV_CONTROL)
    {
      this->flowname_ = TAO_AV_Core::get_control_flowname (entry->flowname ());
    }
  else
    {
      this->flowname_ = entry->flowname ();
      ACE_NEW_RETURN (this->address_,
                    ACE_INET_Addr ("0"),
                    -1);
    }

  int result = this->open_i (this->address_, 1);
  if (result < 0)
    return result;

  return 0;
}

int
TAO_AV_UDP_Acceptor::open_i (ACE_INET_Addr *inet_addr,
                             int is_default_addr)
{
  int result = -1;
  ACE_INET_Addr *local_addr = 0;

  TAO_AV_Flow_Handler *flow_handler = 0;

  // if using a default address and this is the control flow component, the
  //  handler and local address are already set in the flow spec entry
  if (is_default_addr &&
      (this->flow_component_ == TAO_AV_Core::TAO_AV_CONTROL)  &&
      (ACE_OS::strcasecmp(this->entry_->flow_protocol_str (), "RTP") == 0))
    {
      flow_handler = this->entry_->control_handler ();

      local_addr = dynamic_cast<ACE_INET_Addr*> (this->entry_->get_local_control_addr ());
    }
  else
    {
      // this variable is only used for RTP/UDP; RFC 1889 requires an even/odd
      //  consecutive port pair
      int get_new_port = 1;

      while (get_new_port)
        {
          // assume the ports will be OK
          get_new_port = 0;

          result = TAO_AV_UDP_Connection_Setup::setup (flow_handler,
                                                       inet_addr,
                                                       local_addr,
                                                       this->entry_->is_multicast (),
                                                       TAO_AV_UDP_Connection_Setup::ACCEPTOR);

        if (result < 0)
          {
             ORBSVCS_DEBUG((LM_DEBUG,"(%N,%l) Error during connection setup: %d\n", result));
          }

          local_addr->set (local_addr->get_port_number (),
                           local_addr->get_host_name ());

          if (is_default_addr)
            {
              if ((ACE_OS::strcasecmp(this->entry_->flow_protocol_str (), "RTP") == 0) &&
                  (this->flow_component_ == TAO_AV_Core::TAO_AV_DATA))
                {
                  if (is_default_addr && local_addr->get_port_number ()%2 != 0)
                    {
                      // RTP port should be even
                      delete local_addr;
                      local_addr = 0;
                      delete flow_handler;
                      get_new_port = 1;
                    }
                  else
                    {
                      ACE_INET_Addr *local_control_addr = 0;
                      TAO_AV_Flow_Handler *control_flow_handler = 0;

                      ACE_NEW_RETURN (this->control_inet_address_,
                                      ACE_INET_Addr ("0"),
                                      -1);

                      TAO_AV_UDP_Connection_Setup::setup(control_flow_handler,
                                                         this->control_inet_address_,
                                                         local_control_addr,
                                                         this->entry_->is_multicast (),
                                                         TAO_AV_UDP_Connection_Setup::ACCEPTOR);

                      if (local_control_addr->get_port_number () !=
                          local_addr->get_port_number () +1)
                        {
                          delete this->control_inet_address_;
                          delete local_addr;
                          local_addr = 0;
                          delete flow_handler;
                          delete local_control_addr;
                          delete control_flow_handler;
                          get_new_port = 1;
                        }
                      else
                        {
                          this->entry_->control_address (this->control_inet_address_);
                          this->entry_->set_local_control_addr (local_control_addr);
                          this->entry_->control_handler (control_flow_handler);
                        }
                    }
                }
            }
        }
    }

  TAO_AV_Protocol_Object *object =
    this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                        this->endpoint_,
                                                        flow_handler,
                                                        flow_handler->transport ());
  flow_handler->protocol_object (object);

  if (this->flow_component_ == TAO_AV_Core::TAO_AV_DATA)
    {
      this->endpoint_->set_flow_handler (this->flowname_.c_str (),flow_handler);

      this->entry_->protocol_object (object);
      this->entry_->set_local_addr (local_addr);
      this->entry_->handler (flow_handler);
      //this->entry_->address (inet_addr);
      this->entry_->address (local_addr);
    }
  else
    {
      this->endpoint_->set_control_flow_handler (this->flowname_.c_str (),flow_handler);

      this->entry_->control_protocol_object (object);
      this->entry_->set_local_control_addr (local_addr);
      this->entry_->control_handler (flow_handler);
    }

  if (local_addr != 0)
    {
      ACE_TCHAR buf[BUFSIZ];
      local_addr->addr_to_string (buf,BUFSIZ);
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    "TAO_AV_UDP_ACCEPTOR::open:%s\n",
                    buf));
    }

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
  : control_inet_address_ (0)
{
}

TAO_AV_UDP_Connector::~TAO_AV_UDP_Connector (void)
{
  if (this->flow_component_ == TAO_AV_Core::TAO_AV_CONTROL)
    {
      delete this->entry_->control_handler ();
    }

  if (this->control_inet_address_ != 0)
    delete this->control_inet_address_;
}

int
TAO_AV_UDP_Connector::open (TAO_Base_StreamEndPoint *endpoint,
                            TAO_AV_Core *av_core,
                            TAO_AV_Flow_Protocol_Factory *factory)

{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Connector::open "));
  this->endpoint_ = endpoint;
  this->av_core_ = av_core;
  this->flow_protocol_factory_ = factory;
  return 0;
}

int
TAO_AV_UDP_Connector::connect (TAO_FlowSpec_Entry *entry,
                               TAO_AV_Transport *&transport,
                               TAO_AV_Core::Flow_Component flow_component)
{
  ACE_INET_Addr *local_addr = 0;
  ACE_INET_Addr *control_inet_addr = 0;

  this->entry_ = entry;
  this->flow_component_ = flow_component;

  ACE_INET_Addr *inet_addr;

  if (flow_component == TAO_AV_Core::TAO_AV_CONTROL)
    {
      this->flowname_ = TAO_AV_Core::get_control_flowname (entry->flowname());
    inet_addr = dynamic_cast<ACE_INET_Addr*> (entry->control_address ());
    }
  else
    {
      this->flowname_ = entry->flowname ();
      inet_addr = dynamic_cast<ACE_INET_Addr*> (entry->address ());
      control_inet_addr = dynamic_cast<ACE_INET_Addr*> (entry->control_address ());
    }

  TAO_AV_Flow_Handler *flow_handler = 0;

  // if this is the control flow component, the
  //  handler and local address are already set in the flow spec entry
  if ((flow_component == TAO_AV_Core::TAO_AV_CONTROL)  &&
      (ACE_OS::strcasecmp(this->entry_->flow_protocol_str (), "RTP") == 0) &&
      !entry->is_multicast ())
    {
      flow_handler = this->entry_->control_handler ();
      flow_handler->set_remote_address (inet_addr);

      local_addr = dynamic_cast<ACE_INET_Addr*> (this->entry_->get_local_control_addr ());
    }
  else
    {
      // this variable is only used for RTP/UDP; RFC 1889 requires an even/odd
      //  consecutive port pair
      int get_new_port = 1;

      while (get_new_port)
        {
          // assume the ports will be OK
          get_new_port = 0;

          ACE_Addr *addr;
          if ((addr = entry->get_peer_addr ()) != 0)
            {
              local_addr = dynamic_cast<ACE_INET_Addr*> (addr);
              ACE_TCHAR buf [BUFSIZ];
              local_addr->addr_to_string (buf, BUFSIZ);
            }

          TAO_AV_UDP_Connection_Setup::setup (flow_handler,
                                              inet_addr,
                                              local_addr,
                                              entry->is_multicast (),
                                              TAO_AV_UDP_Connection_Setup::CONNECTOR);

          if ((ACE_OS::strcasecmp(this->entry_->flow_protocol_str (), "RTP") == 0) &&
              (flow_component == TAO_AV_Core::TAO_AV_DATA) &&
              !entry->is_multicast ())
            {
              if (local_addr->get_port_number ()%2 != 0)
                {
                  // RTP port should be even
                  delete local_addr;
                  local_addr = 0;
                  delete flow_handler;
                  get_new_port = 1;
                }
              else
                {
                  ACE_INET_Addr *local_control_addr = 0;
                  TAO_AV_Flow_Handler *control_flow_handler = 0;

                  if (entry->is_multicast ())
                    control_inet_addr =  dynamic_cast<ACE_INET_Addr*> (entry->control_address ()) ;
                  else
                    {

                      if (local_addr != 0)
                        {
                          ACE_TCHAR buf [BUFSIZ];
                          ACE_TString addr_str (ACE_TEXT_CHAR_TO_TCHAR(local_addr->get_host_name ()));
                          addr_str += ACE_TEXT(":");
                          addr_str += ACE_OS::itoa (local_addr->get_port_number () + 1, buf, 10);
                          ACE_NEW_RETURN (local_control_addr,
                                          ACE_INET_Addr (addr_str.c_str ()),
                                          -1);
                          local_control_addr->addr_to_string (buf, BUFSIZ);
                        }


                      if (entry->control_address () == 0)
                        ACE_NEW_RETURN (this->control_inet_address_,
                                        ACE_INET_Addr ("0"),
                                        -1);
                      else
                        control_inet_address_ = dynamic_cast<ACE_INET_Addr*> (entry->control_address ());
                    }

                  TAO_AV_UDP_Connection_Setup::setup (control_flow_handler,
                                                      control_inet_addr,
                                                      local_control_addr,
                                                      entry->is_multicast (),
                                                      TAO_AV_UDP_Connection_Setup::CONNECTOR);

                  if (local_control_addr->get_port_number () !=
                      local_addr->get_port_number () +1)
                    {
                      delete local_addr;
                      local_addr = 0;
                      delete flow_handler;
                      delete local_control_addr;
                      delete control_flow_handler;
                      get_new_port = 1;
                    }
                  else
                    {
                      this->entry_->set_local_control_addr (local_control_addr);
                      this->entry_->control_handler (control_flow_handler);
                    }
                }
            }
        }
    }

  TAO_AV_Protocol_Object *object =
    this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                        this->endpoint_,
                                                        flow_handler,
                                                        flow_handler->transport ());

  flow_handler->protocol_object (object);

  if (flow_component == TAO_AV_Core::TAO_AV_DATA)
    {
      this->endpoint_->set_flow_handler (this->flowname_.c_str (),
                                         flow_handler);
      this->entry_->protocol_object (object);
      entry->set_local_addr (local_addr);
      entry->handler (flow_handler);
      transport = flow_handler->transport ();
    }
  else
    {
      this->endpoint_->set_control_flow_handler (this->flowname_.c_str (),
                                                 flow_handler);
      this->entry_->control_protocol_object (object);
      entry->set_local_control_addr (local_addr);
      entry->control_handler (flow_handler);
      transport = flow_handler->transport ();
    }

  if (local_addr != 0)
    {
      ACE_TCHAR buf[BUFSIZ];
      local_addr->addr_to_string (buf,BUFSIZ);

      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_CONNECTOR::connect:%s\n",buf));
    }

  // call activate svc handler.
  return this->activate_svc_handler (flow_handler);
}

int
TAO_AV_UDP_Connector::activate_svc_handler (TAO_AV_Flow_Handler *handler)
{
  ACE_Event_Handler *event_handler = handler->event_handler ();
  int result = this->av_core_->reactor ()->register_handler (event_handler,
                                                             ACE_Event_Handler::READ_MASK);

  if (this->flow_component_ == TAO_AV_Core::TAO_AV_CONTROL)
    handler->schedule_timer ();

  return result;
}

int
TAO_AV_UDP_Connector::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_Connection_Setup
//------------------------------------------------------------

int
TAO_AV_UDP_Connection_Setup::setup (TAO_AV_Flow_Handler *&flow_handler,
                                    ACE_INET_Addr *inet_addr,
                                    ACE_INET_Addr *&local_addr,
                                    int is_multicast,
                                    ConnectionType ct)
{
  int result;

  if (is_multicast)
    {
      TAO_AV_UDP_MCast_Flow_Handler *handler;
      ACE_NEW_RETURN (handler,
                      TAO_AV_UDP_MCast_Flow_Handler,
                      -1);

      flow_handler = handler;

      result = handler->get_mcast_socket ()->join (*inet_addr);
      if (result < 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_MCast_connector::open failed\n"),-1);

      // Now disable Multicast loopback.
      // @@Should we make this a policy?
#if defined (ACE_HAS_IP_MULTICAST)
      if (handler->get_mcast_socket ()->set_option (IP_MULTICAST_LOOP,
                                                    0) < 0)
        if (TAO_debug_level > 0)
          ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_MCast_Acceptor::multicast loop disable failed\n"));
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
            ACE_OS::perror("SO_RCVBUF");
        }
      ACE_NEW_RETURN (local_addr,
                      ACE_INET_Addr ("0"),
                      -1);

      if (ct == ACCEPTOR)
        {
          result = handler->get_mcast_socket ()->get_local_addr (*local_addr);
          if (result < 0)
            ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_AV_Dgram_Connector::open: get_local_addr failed\n"),result);

          local_addr->set (local_addr->get_port_number (),
                           local_addr->get_host_name ());
          handler->set_peer_addr (local_addr);
        }
    }
  else
    {
      if (local_addr == 0)
        ACE_NEW_RETURN (local_addr,
                        ACE_INET_Addr ("0"),
                        -1);

      TAO_AV_UDP_Flow_Handler *handler;
      ACE_NEW_RETURN (handler,
                      TAO_AV_UDP_Flow_Handler,
                      -1);

      flow_handler = handler;

      if (ct == ACCEPTOR)
        result = handler->open (*inet_addr);
      else
        result = handler->open (*local_addr);
      if (result < 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"handler::open failed\n"),-1);

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

      if (ct == CONNECTOR)
        handler->set_remote_address  (inet_addr);

      result = handler->get_socket ()->get_local_addr (*local_addr);

      local_addr->set (local_addr->get_port_number (),
                       local_addr->get_host_name ());

      ACE_TCHAR buf [BUFSIZ];
      local_addr->addr_to_string (buf, BUFSIZ);

      if (result < 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_AV_Dgram_Connector::open: get_local_addr failed\n"),result);
    }

  return 1;
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
  if (ACE_OS::strcasecmp (protocol_string,"UDP") == 0)
    return 1;
  if (ACE_OS::strcasecmp (protocol_string,"RTP/UDP") == 0)
    return 1;
  return 0;
}

TAO_AV_Acceptor*
TAO_AV_UDP_Factory::make_acceptor (void)
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Factory::make_acceptor\n"));
  TAO_AV_Acceptor *acceptor = 0;
  ACE_NEW_RETURN (acceptor,
                  TAO_AV_UDP_Acceptor,
                  0);
  return acceptor;
}

TAO_AV_Connector*
TAO_AV_UDP_Factory::make_connector (void)
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Factory::make_connector\n"));
  TAO_AV_Connector *connector = 0;
  ACE_NEW_RETURN (connector,
                  TAO_AV_UDP_Connector,
                  0);
  return connector;
}

int
TAO_AV_UDP_Factory::init (int /* argc */,
                          ACE_TCHAR * /* argv */ [])
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
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"(%N,%l) TAO_AV_UDP_Flow_Handler::handle_input recv failed: errno: %m\n"),-1);

  this->frame_.wr_ptr (this->frame_.rd_ptr () + n);

  return this->callback_->receive_frame (&this->frame_);
}

int
TAO_AV_UDP_Object::send_frame (ACE_Message_Block *frame,
                               TAO_AV_frame_info * /*frame_info*/)
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Object::send_frame\n"));
  int const result = this->transport_->send (frame);
  if (result < 0)
    return result;
  return 0;
}

int
TAO_AV_UDP_Object::send_frame (const iovec *iov,
                               int iovcnt,
                               TAO_AV_frame_info * /*frame_info*/)
{
  int const result = this->transport_->send (iov,iovcnt);
  if (result < 0)
    return result;
  return 0;
}

int
TAO_AV_UDP_Object::send_frame (const char*buf,
                               size_t len)
{
  int const result = this->transport_->send (buf, len, 0);
  if (result < 0)
    return result;
  return 0;
}

TAO_AV_UDP_Object::TAO_AV_UDP_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
  this->frame_.size (this->transport_->mtu ());
}

TAO_AV_UDP_Object::~TAO_AV_UDP_Object (void)
{
  //no-op
}

int
TAO_AV_UDP_Object::destroy (void)
{
  this->callback_->handle_destroy ();
  delete this;

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
                               ACE_TCHAR * /* argv */ [])
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
  if( endpoint->get_callback (entry->flowname (), callback) ) {
    ORBSVCS_ERROR_RETURN ((LM_ERROR, "(%N,%l) Invalid callback\n"), 0);
  }

  TAO_AV_UDP_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_UDP_Object (callback,
                                     transport),
                  0);
  callback->open (object,
                  handler);
  endpoint->set_protocol_object (entry->flowname (),
                                 object);

  endpoint->protocol_object_set ();

  return object;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_UDP_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_UDP_Flow_Factory,
                       ACE_TEXT ("UDP_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_UDP_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_UDP_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_UDP_Factory,
                       ACE_TEXT ("UDP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_UDP_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)
