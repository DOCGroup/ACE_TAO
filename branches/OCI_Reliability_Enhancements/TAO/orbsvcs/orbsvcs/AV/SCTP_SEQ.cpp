// $Id$

#include "orbsvcs/AV/SCTP_SEQ.h"

#if defined (ACE_HAS_SCTP)

#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/Multihomed_INET_Addr.h"
#include "tao/debug.h"
#include "ace/Arg_Shifter.h"
#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Transport
//------------------------------------------------------------

TAO_AV_SCTP_SEQ_Transport::TAO_AV_SCTP_SEQ_Transport (void)
  :handler_ (0)
{
}

TAO_AV_SCTP_SEQ_Transport::TAO_AV_SCTP_SEQ_Transport (TAO_AV_SCTP_SEQ_Flow_Handler *handler)
  :handler_ (handler)
{
}

TAO_AV_SCTP_SEQ_Transport::~TAO_AV_SCTP_SEQ_Transport (void)
{
}

int
TAO_AV_SCTP_SEQ_Transport::open (ACE_Addr * /*address*/)
{
  return 0;
}

int
TAO_AV_SCTP_SEQ_Transport::close (void)
{
  return 0;
}

int
TAO_AV_SCTP_SEQ_Transport::mtu (void)
{
  return 0;
}

ACE_Addr*
TAO_AV_SCTP_SEQ_Transport::get_peer_addr (void)
{
  return 0;
}

ssize_t
TAO_AV_SCTP_SEQ_Transport::send (const ACE_Message_Block *mblk, ACE_Time_Value *)
{
  // For the most part this was copied from GIOP::send_request and
  // friends.

  //  ACE_Time_Value timeout;

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
              n = this->handler_->peer ().sendv_n ((const iovec *) iov,
                                                   iovcnt);
                                                   //&timeout);
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
      n = this->handler_->peer ().sendv_n ((const iovec *) iov,
                                           iovcnt);
      if (n < 1)
        return n;

      nbytes += n;
    }

  return nbytes;
}

ssize_t
TAO_AV_SCTP_SEQ_Transport::send (const char *buf,
                                 size_t len,
                                 ACE_Time_Value *)
{
  return this->handler_->peer ().send_n (buf, len);
}

ssize_t
TAO_AV_SCTP_SEQ_Transport::send (const iovec *iov,
                                 int iovcnt,
                                 ACE_Time_Value *)
{
  return this->handler_->peer ().sendv_n (iov,
                                          iovcnt);
}

ssize_t
TAO_AV_SCTP_SEQ_Transport::recv (char *buf,
                                 size_t len,
                                 ACE_Time_Value *)
{
  return this->handler_->peer ().recv (buf, len);
}

ssize_t
TAO_AV_SCTP_SEQ_Transport::recv (char *buf,
                                 size_t len,
                                 int flags,
                                 ACE_Time_Value *)
{
  return this->handler_->peer ().recv (buf,
                                       len,
                                       flags);
}

ssize_t
TAO_AV_SCTP_SEQ_Transport::recv (iovec *iov,
                                 int iovcnt,
                                 ACE_Time_Value *)
{
  return handler_->peer ().recvv_n (iov, iovcnt);
}

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Base_Acceptor
//------------------------------------------------------------

int
TAO_AV_SCTP_SEQ_Base_Acceptor::acceptor_open (TAO_AV_SCTP_SEQ_Acceptor *acceptor,
                                              ACE_Reactor *reactor,
                                              const ACE_INET_Addr &local_addr,
                                              TAO_FlowSpec_Entry *entry)
{
  ACE_DEBUG ((LM_DEBUG,
              "In base acceptor open"));

  this->acceptor_ = acceptor;
  this->reactor_ = reactor;
  this->entry_ = entry;

  ACE_Auto_Array_Ptr<ACE_UINT32> local_ip_addr
    (new ACE_UINT32[entry->num_local_sec_addrs ()]);
  ACE_INET_Addr ip_addr;
  char** addrs = entry->get_local_sec_addr ();
  for (int i = 0; i < entry->num_local_sec_addrs (); i++)
    {
      ACE_CString addr_str (addrs[i]);
      addr_str += ":";
      ip_addr.set (addr_str.c_str ());
      local_ip_addr[i] = ip_addr.get_ip_address ();
    }


  ACE_Multihomed_INET_Addr multi_addr;
  multi_addr.set (local_addr.get_port_number (),
                  local_addr.get_ip_address (),
                  1,
                  local_ip_addr.get(),
                  entry->num_local_sec_addrs ());

  char buf[BUFSIZ];
  multi_addr.addr_to_string (buf, BUFSIZ);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_SCTP_SEQ_Base_Acceptor::open: %s",
                buf));

  if (this->open (multi_addr,reactor) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_SCTP_SEQ_Base_Acceptor::open failed\n"),-1);

  return 0;
}

int
TAO_AV_SCTP_SEQ_Base_Acceptor::make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *&handler)
{
  int const result = this->acceptor_->make_svc_handler (handler);
  if (result < 0)
    return result;
  handler->reactor (this->reactor_);
  this->entry_->handler (handler);

  return 0;
}

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Acceptor
//------------------------------------------------------------

TAO_AV_SCTP_SEQ_Acceptor::TAO_AV_SCTP_SEQ_Acceptor (void)
{
}

TAO_AV_SCTP_SEQ_Acceptor::~TAO_AV_SCTP_SEQ_Acceptor (void)
{
}

int
TAO_AV_SCTP_SEQ_Acceptor::make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *&sctp_handler)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_SCTP_SEQ_Acceptor::make_svc_handler\n"));

  if (this->endpoint_ != 0)
    {
      ACE_NEW_RETURN (sctp_handler,
                      TAO_AV_SCTP_SEQ_Flow_Handler,
                      -1);

      TAO_AV_Protocol_Object *object =
        this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                            this->endpoint_,
                                                            sctp_handler,
                                                            sctp_handler->transport ());

      sctp_handler->protocol_object (object);
      this->endpoint_->set_flow_handler (this->flowname_.c_str (),sctp_handler);
      this->entry_->protocol_object (object);
      this->entry_->handler (sctp_handler);
    }
  return 0;
}

int
TAO_AV_SCTP_SEQ_Acceptor::open (TAO_Base_StreamEndPoint *endpoint,
                                TAO_AV_Core *av_core,
                                TAO_FlowSpec_Entry *entry,
                                TAO_AV_Flow_Protocol_Factory *factory,
                                TAO_AV_Core::Flow_Component flow_comp)
{
  this->flow_protocol_factory_ = factory;

  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;

  if (flow_comp == TAO_AV_Core::TAO_AV_CONTROL)
    this->flowname_ = TAO_AV_Core::get_control_flowname (entry->flowname ());
  else
  this->flowname_ = entry->flowname ();

  ACE_Addr *address = entry->address ();

  ACE_INET_Addr *inet_addr = (ACE_INET_Addr *) address;

  char buf[BUFSIZ];
  inet_addr->addr_to_string (buf, BUFSIZ);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_SCTP_SEQ_Acceptor::open: %s",
                buf));

  //Add code for reading multihomed addresses and pass the multihomed
  //addr to the following method. ACE_Multihomed_addr derives from
  //ACE_INET_Addr, hence this should not be an issue.
  int result = this->acceptor_.acceptor_open (this,
                                              av_core->reactor (),
                                              *inet_addr,
                                              entry);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO_AV_SCTP_SEQ_Acceptor::open failed"),
                      -1);

  entry->set_local_addr (address);
  return 0;
}

int
TAO_AV_SCTP_SEQ_Acceptor::open_default (TAO_Base_StreamEndPoint *endpoint,
                                        TAO_AV_Core *av_core,
                                        TAO_FlowSpec_Entry *entry,
                                        TAO_AV_Flow_Protocol_Factory *factory,
                                        TAO_AV_Core::Flow_Component flow_comp)
{
  this->flow_protocol_factory_ = factory;
  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;
  if (flow_comp == TAO_AV_Core::TAO_AV_CONTROL)
    this->flowname_ = TAO_AV_Core::get_control_flowname (entry->flowname());
  else
  this->flowname_ = entry->flowname ();

  ACE_INET_Addr *address;
  ACE_NEW_RETURN (address,
                  ACE_INET_Addr ("0"),
                  -1);

  int result = this->acceptor_.acceptor_open (this,
                                              av_core->reactor (),
                                              *address,
                                              entry);


  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO_AV_SCTP_SEQ_Acceptor::open failed"),
                      -1);

  this->acceptor_.acceptor ().get_local_addr (*address);

  address->set (address->get_port_number (),
                address->get_host_name ());

  char buf[BUFSIZ];
  address->addr_to_string (buf,BUFSIZ);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_SCTP_SEQ_Acceptor::open_default: %s\n",
                buf));

  entry->set_local_addr (address);

  return 0;
}


int
TAO_AV_SCTP_SEQ_Acceptor::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Base_Connector
//------------------------------------------------------------

int
TAO_AV_SCTP_SEQ_Base_Connector::connector_open (TAO_AV_SCTP_SEQ_Connector *connector,
                                                ACE_Reactor *reactor)
{
  this->connector_ = connector;
  this->reactor_ = reactor;

  if (this->open (reactor) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_SCTP_SEQ_Base_Connector::open failed\n"),-1);
  return 0;
}

int
TAO_AV_SCTP_SEQ_Base_Connector::make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *&sctp_handler)
{
  int result =
    this->connector_->make_svc_handler (sctp_handler);
  if (result < 0)
    return result;
  sctp_handler->reactor (this->reactor_);
  return 0;
}

int
TAO_AV_SCTP_SEQ_Base_Connector::connector_connect (TAO_AV_SCTP_SEQ_Flow_Handler *&handler,
                                                   const ACE_Multihomed_INET_Addr &remote_addr,
                                                   const ACE_Multihomed_INET_Addr &local_addr)
{
  int result = this->connect (handler,
                              remote_addr,
                              0,
                              local_addr);

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_SCTP_SEQ_Base_Connector::connect failed\n"),-1);
  return 0;
}

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Connector
//------------------------------------------------------------
TAO_AV_SCTP_SEQ_Connector::TAO_AV_SCTP_SEQ_Connector (void)
{
}

TAO_AV_SCTP_SEQ_Connector::~TAO_AV_SCTP_SEQ_Connector (void)
{
}

int
TAO_AV_SCTP_SEQ_Connector::make_svc_handler (TAO_AV_SCTP_SEQ_Flow_Handler *&sctp_handler)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_SCTP_SEQ_Connector::make_svc_handler\n"));

  //  TAO_AV_Callback *callback = 0;
  if (this->endpoint_ != 0)
    {
      //       this->endpoint_->get_callback (this->flowname_.c_str (),
      //                                      callback);
      ACE_NEW_RETURN (sctp_handler,
                      //                      TAO_AV_SCTP_SEQ_Flow_Handler (callback),
                      TAO_AV_SCTP_SEQ_Flow_Handler,
                      -1);
      TAO_AV_Protocol_Object *object =
        this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                            this->endpoint_,
                                                            sctp_handler,
                                                            sctp_handler->transport ());
      sctp_handler->protocol_object (object);
      //      callback->protocol_object (object);
      //       this->endpoint_->set_protocol_object (this->flowname_.c_str (),
      //                                             object);
      this->endpoint_->set_flow_handler (this->flowname_.c_str (),sctp_handler);
      this->entry_->protocol_object (object);
      this->entry_->handler (sctp_handler);
    }
  return 0;
}

int
TAO_AV_SCTP_SEQ_Connector::open (TAO_Base_StreamEndPoint *endpoint,
                                 TAO_AV_Core *av_core,
                                 TAO_AV_Flow_Protocol_Factory *factory)
{
  this->endpoint_ = endpoint;
  this->flow_protocol_factory_ = factory;
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_SCTP_SEQ_Connector::open "));
  int result = this->connector_.connector_open(this,
                                               av_core->reactor ());
  return result;
}

int
TAO_AV_SCTP_SEQ_Connector::connect (TAO_FlowSpec_Entry *entry,
                                    TAO_AV_Transport *&transport,
                                    TAO_AV_Core::Flow_Component flow_comp)
{
  this->entry_ = entry;
  if (flow_comp == TAO_AV_Core::TAO_AV_CONTROL)
    this->flowname_ = TAO_AV_Core::get_control_flowname (entry->flowname ());
  else
    this->flowname_ = entry->flowname ();
  ACE_Addr *remote_addr = entry->address ();
  ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr *> (remote_addr);
  TAO_AV_SCTP_SEQ_Flow_Handler *handler = 0;

  ACE_Multihomed_INET_Addr remote_multi_addr;
  remote_multi_addr.set (inet_addr->get_port_number (),
                         inet_addr->get_ip_address (),
                         1,
                         0,
                         0);

  ACE_Multihomed_INET_Addr local_addr; //This can be a multihomed address
  ACE_INET_Addr *addr;
  if (entry->get_peer_addr () != 0)
    {
      addr = dynamic_cast<ACE_INET_Addr *> (entry->get_peer_addr ());
    }
  else
    {
      ACE_NEW_RETURN (addr,
                      ACE_INET_Addr ("0"),
                      -1);
    }

  ACE_Auto_Array_Ptr<ACE_UINT32> local_ip_addr
    (new ACE_UINT32[entry->num_peer_sec_addrs ()]);
  ACE_INET_Addr ip_addr;
  char** addrs = entry->get_peer_sec_addr ();
  for (int i = 0; i < entry->num_peer_sec_addrs (); i++)
    {
      ACE_CString addr_str (addrs[i]);
      addr_str += ":";
      ip_addr.set (addr_str.c_str ());
      local_ip_addr[i] = ip_addr.get_ip_address ();
    }

  if (entry->num_peer_sec_addrs () != 0)
    local_addr.set (addr->get_port_number (),
                    addr->get_ip_address (),
                    1,
                    local_ip_addr.get(),
                    entry->num_peer_sec_addrs ());
  else
    local_addr.set (addr->get_port_number (),
                    addr->get_ip_address (),
                    1,
                    0,
                    entry->num_peer_sec_addrs ());


  int result = this->connector_.connector_connect (handler,
                                                   remote_multi_addr,
                                                   local_addr);

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_SCTP_SEQ_connector::connect failed\n"),-1);
  entry->handler (handler);
  transport = handler->transport ();

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Local Addrs\n"));
      char buf [BUFSIZ];
      size_t size = BUFSIZ;
      ACE_INET_Addr *peer_addrs = 0;
      ACE_NEW_RETURN (peer_addrs,ACE_INET_Addr [size], -1);
      handler->peer ().get_local_addrs (peer_addrs, size);
      for (unsigned int i=0; i < size;i++)
        {
          peer_addrs [i].addr_to_string (buf,
                                         BUFSIZ);
          ACE_DEBUG ((LM_DEBUG,
                      "%s %d\n",
                      buf,
                      size));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Remote Addrs\n"));

      size = BUFSIZ;
      handler->peer ().get_remote_addrs (peer_addrs, size);
      for (unsigned int i=0; i < size;i++)
        {
          peer_addrs [i].addr_to_string (buf,
                                         BUFSIZ);
          ACE_DEBUG ((LM_DEBUG,
                      "%s %d\n",
                      buf,
                      size));
        }
      //delete peer_addrs;
    }

  return 0;
}

int
TAO_AV_SCTP_SEQ_Connector::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Protocol_Factory
//------------------------------------------------------------


TAO_AV_SCTP_SEQ_Factory::TAO_AV_SCTP_SEQ_Factory (void)
{
}

TAO_AV_SCTP_SEQ_Factory::~TAO_AV_SCTP_SEQ_Factory (void)
{
}


int
TAO_AV_SCTP_SEQ_Factory::match_protocol (const char *protocol_string)
{
  if (ACE_OS::strcasecmp (protocol_string,"SCTP_SEQ") == 0)
    return 1;
  return 0;
}

TAO_AV_Acceptor*
TAO_AV_SCTP_SEQ_Factory::make_acceptor (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_SCTP_SEQ_Factory::make_acceptor\n"));
  TAO_AV_Acceptor *acceptor = 0;
  ACE_NEW_RETURN (acceptor,
                  TAO_AV_SCTP_SEQ_Acceptor,
                  0);
  return acceptor;
}

TAO_AV_Connector*
TAO_AV_SCTP_SEQ_Factory::make_connector (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_SCTP_SEQ_Factory::make_connector\n"));
  TAO_AV_Connector *connector = 0;
  ACE_NEW_RETURN (connector,
                  TAO_AV_SCTP_SEQ_Connector,
                  0);
  return connector;
}


int
TAO_AV_SCTP_SEQ_Factory::init (int, ACE_TCHAR *[])
{

  return 0;
}

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Object
//------------------------------------------------------------

int
TAO_AV_SCTP_SEQ_Object::handle_input (void)
{
  int n = this->transport_->recv (this->frame_.rd_ptr (),
                                  this->frame_.size ());
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_SCTP_SEQ_Flow_Handler::handle_input recv failed\n"),-1);
  if (n == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_SCTP_SEQ_Flow_Handler::handle_input connection closed\n"),-1);
  this->frame_.wr_ptr (this->frame_.rd_ptr () + n);

  return this->callback_->receive_frame (&this->frame_);
}

int
TAO_AV_SCTP_SEQ_Object::send_frame (ACE_Message_Block *frame,
                                    TAO_AV_frame_info * /*frame_info*/)
{
  int result = this->transport_->send (frame);
  return result;
}

int
TAO_AV_SCTP_SEQ_Object::send_frame (const iovec *iov,
                                    int iovcnt,
                                    TAO_AV_frame_info * /*frame_info*/)
{
  return this->transport_->send (iov,iovcnt);
}

int
TAO_AV_SCTP_SEQ_Object::send_frame (const char*buf,
                                    size_t len)
{
  int result = this->transport_->send (buf, len, 0);
  return result;
}


TAO_AV_SCTP_SEQ_Object::TAO_AV_SCTP_SEQ_Object (TAO_AV_Callback *callback,
                                                TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
  // @@ Is this a good size?
  this->frame_.size (BUFSIZ);
}

TAO_AV_SCTP_SEQ_Object::~TAO_AV_SCTP_SEQ_Object (void)
{
  // No-op
}
int
TAO_AV_SCTP_SEQ_Object::destroy (void)
{
  this->callback_->handle_destroy ();
  delete this;
  return 0;
}

//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Flow_Handler
//------------------------------------------------------------

TAO_AV_SCTP_SEQ_Flow_Handler::TAO_AV_SCTP_SEQ_Flow_Handler (TAO_AV_Callback * /*callback*/)
  //  :TAO_AV_Flow_Handler (callback)
{
  ACE_NEW (this->transport_,
           TAO_AV_SCTP_SEQ_Transport (this));
}

TAO_AV_SCTP_SEQ_Flow_Handler::~TAO_AV_SCTP_SEQ_Flow_Handler (void)
{
  delete this->transport_;
}

TAO_AV_Transport *
TAO_AV_SCTP_SEQ_Flow_Handler::transport (void)
{
  return this->transport_;
}

int
TAO_AV_SCTP_SEQ_Flow_Handler::change_qos (AVStreams::QoS qos)
{
  if( TAO_debug_level > 0 )
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%N,%l) TAO_AV_SCTP_SEQ_Flow_Handler::change_qos\n"));
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
          ACE_DEBUG ((LM_DEBUG,
                      "DSCP %d\n",
                      dscp));
          dscp_flag=1;
          // DSCP value can only be 6 bits wide
          if(!((dscp >= 0) && (dscp <= 63)))
            {
              dscp_flag = 0;
              ACE_DEBUG((LM_DEBUG, "(%N,%l) ECN value can only be (0-3) not %d\n", ecn));
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
              ACE_DEBUG((LM_DEBUG, "(%N,%l) ECN value can only be (0-3) not %d\n", ecn));
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
      ret = this->peer ().set_option(IPPROTO_IP, IP_TOS, (int *)&tos , (int)sizeof(tos));

      if(TAO_debug_level > 0)
        {
          ACE_DEBUG((LM_DEBUG, "(%N,%l) set tos: ret: %d %d\n", tos, ret));
        }
    }

  if(TAO_debug_level > 0)
    {
      if(ret < 0 )
        {
          ACE_DEBUG((LM_DEBUG, "(%N,%l) errno: %p\n"));
        }
    }
  return ret;
}

int
TAO_AV_SCTP_SEQ_Flow_Handler::open (void * /*arg*/)
{
  ACE_CDR::Long nodelay = 1;

#if defined (SCTP_NODELAY)
  if (this->peer ().set_option (IPPROTO_SCTP,
                                SCTP_NODELAY,
                                (void *) &nodelay,
                                sizeof (nodelay)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "NODELAY failed\n"),
                      -1);
#endif /* SCTP_NODELAY */

  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  char server[MAXHOSTNAMELEN + 16];

  (void) addr.addr_to_string (server, sizeof (server));

  if (TAO_debug_level > 0)
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) connection to server <%s> on %d\n",
                  server, this->peer ().get_handle ()));

  this->peer ().disable (ACE_NONBLOCK);

  // Register the handler with the reactor.
  if (this->reactor ()
      && this->reactor ()->register_handler
      (this,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("unable to register client handler")),
                      -1);
  return 0;
}

int
TAO_AV_SCTP_SEQ_Flow_Handler::handle_input (ACE_HANDLE /*fd*/)
{
  return this->protocol_object_->handle_input ();
}

int
TAO_AV_SCTP_SEQ_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                              const void *arg)
{
  return TAO_AV_Flow_Handler::handle_timeout (tv,arg);
}


//------------------------------------------------------------
// TAO_AV_SCTP_SEQ_Flow_Factory
//------------------------------------------------------------
TAO_AV_SCTP_SEQ_Flow_Factory::TAO_AV_SCTP_SEQ_Flow_Factory (void)
{
}

TAO_AV_SCTP_SEQ_Flow_Factory::~TAO_AV_SCTP_SEQ_Flow_Factory (void)
{
}

int
TAO_AV_SCTP_SEQ_Flow_Factory::init (int /* argc */,
                                    ACE_TCHAR * /* argv */ [])
{
  return 0;
}

int
TAO_AV_SCTP_SEQ_Flow_Factory::match_protocol (const char *flow_string)
{
  if (ACE_OS::strcasecmp (flow_string,"SCTP_SEQ") == 0)
    return 1;
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_SCTP_SEQ_Flow_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                    TAO_Base_StreamEndPoint *endpoint,
                                                    TAO_AV_Flow_Handler *handler,
                                                    TAO_AV_Transport *transport)
{
  TAO_AV_Callback *callback = 0;
  if( endpoint->get_callback (entry->flowname (), callback) ) {
    ACE_ERROR_RETURN ((LM_ERROR, "(%N,%l) Invalid callback\n"), 0);
  }

  TAO_AV_SCTP_SEQ_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_SCTP_SEQ_Object (callback,
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

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_SCTP_SEQ_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_SCTP_SEQ_Flow_Factory,
                       ACE_TEXT ("SCTP_SEQ_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_SCTP_SEQ_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_SCTP_SEQ_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_SCTP_SEQ_Factory,
                       ACE_TEXT ("SCTP_SEQ_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_SCTP_SEQ_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

#endif // ACE_HAS_SCTP
