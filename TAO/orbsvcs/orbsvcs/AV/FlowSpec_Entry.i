/* -*- C++ -*- */
// $Id$

// FlowSpec_Entry.i

ACE_INLINE
int
TAO_FlowSpec_Entry::set_direction (char *direction)
{
  this->direction_str_ = direction;
  if (direction == 0)
    {
      this->direction_ = TAO_AV_INVALID;
      return -1;
    }
  if (ACE_OS::strcasecmp (direction,"in") == 0)
    this->direction_ = TAO_AV_DIR_IN;
  else if (ACE_OS::strcasecmp (direction,"out") == 0)
    this->direction_ = TAO_AV_DIR_OUT;
  return 0;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::parse_flow_protocol_string (char *flow_string)
{
  if (flow_string == 0)
    return 0;
  if (ACE_OS::strncasecmp (flow_string,"sfp",3) == 0)
    {
      this->use_flow_protocol_ = 1;
      // do some flow protocol processing.
      this->flow_protocol_ = flow_string;
    }
  return 0;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::direction (void)
{
  return this->direction_;
}

ACE_INLINE
char *
TAO_FlowSpec_Entry::direction_str (void)
{
  return this->direction_str_;
}

ACE_INLINE
char *
TAO_FlowSpec_Entry::flow_protocol_str (void)
{
  return this->flow_protocol_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::flow_protocol_str (const char *str)
{
  this->flow_protocol_ = CORBA::string_dup (str);
}

ACE_INLINE
TAO_AV_Core::Protocol
TAO_FlowSpec_Entry::carrier_protocol (void)
{
  return this->protocol_;
}

ACE_INLINE
char *
TAO_FlowSpec_Entry::carrier_protocol_str (void)
{
  return this->carrier_protocol_;
}

ACE_INLINE
ACE_Addr *
TAO_FlowSpec_Entry::address (void)
{
  return this->address_;
}

ACE_INLINE
char *
TAO_FlowSpec_Entry::address_str (void)
{
  return this->address_str_;
}

ACE_INLINE
char*
TAO_FlowSpec_Entry::format (void)
{
  return this->format_;
}

ACE_INLINE
const char *
TAO_FlowSpec_Entry::flowname (void)
{
  return this->flowname_;
}


ACE_INLINE
int
TAO_FlowSpec_Entry::set_peer_addr (ACE_Addr *peer_addr)
{
  this->peer_addr_ = peer_addr;
  if (this->handler_ != 0)
    this->handler_->set_remote_address (peer_addr);
  return 0;
}

ACE_INLINE
ACE_Addr *
TAO_FlowSpec_Entry::get_peer_addr (void)
{
  return this->peer_addr_;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::set_local_addr (ACE_Addr *local_addr)
{
  this->local_addr_ = local_addr;
  return 0;
}

ACE_INLINE
ACE_Addr*
TAO_FlowSpec_Entry::get_local_addr (void)
{
  return this->local_addr_;
}

ACE_INLINE
TAO_AV_Transport*
TAO_FlowSpec_Entry::transport (void)
{
  return this->transport_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::transport (TAO_AV_Transport *transport)
{
  this->transport_ = transport;
}

ACE_INLINE
TAO_AV_Flow_Handler *
TAO_FlowSpec_Entry::handler (void)
{
  return this->handler_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::handler (TAO_AV_Flow_Handler *handler)
{
  this->handler_ = handler;
//   // Now remove the handler from the reactor if the handler is for a producer.
//   switch (this->role ())
//     {
//     case TAO_AV_PRODUCER:
//       {
//         ACE_Event_Handler *event_handler = handler->event_handler ();
//         int result  = event_handler->reactor ()->remove_handler (event_handler,
//                                                                  ACE_Event_Handler::READ_MASK);
//         if (result < 0)
//           ACE_ERROR ((LM_ERROR,"TAO_FlowSpec_Entry::handler\n"));
//         break;
//       }
//     }
}

ACE_INLINE
TAO_AV_Protocol_Object*
TAO_FlowSpec_Entry::protocol_object (void)
{
  return this->protocol_object_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::protocol_object (TAO_AV_Protocol_Object *object)
{
  this->protocol_object_ = object;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::is_multicast (void)
{
  return this->is_multicast_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::role (TAO_FlowSpec_Entry::Role role)
{
  this->role_ = role;
}
