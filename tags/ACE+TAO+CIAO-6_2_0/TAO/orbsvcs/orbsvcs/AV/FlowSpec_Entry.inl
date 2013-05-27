// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
int
TAO_FlowSpec_Entry::set_direction (const char *direction)
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
TAO_FlowSpec_Entry::parse_flow_protocol_string (const char *flow_string)
{
  if (flow_string == 0)
    return 0;

  if (ACE_OS::strcmp (flow_string,"") == 0)
        return 0;

  this->use_flow_protocol_ = 1;
  // do some flow protocol processing.
  this->flow_protocol_ = flow_string;

  return 0;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::direction (void)
{
  return this->direction_;
}

ACE_INLINE
const char *
TAO_FlowSpec_Entry::direction_str (void) const
{
  return this->direction_str_.c_str();
}

ACE_INLINE
const char *
TAO_FlowSpec_Entry::flow_protocol_str (void) const
{
  return this->flow_protocol_.c_str();
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
const char *
TAO_FlowSpec_Entry::carrier_protocol_str (void) const
{
  return this->carrier_protocol_.c_str();
}

ACE_INLINE
ACE_Addr *
TAO_FlowSpec_Entry::address (void)
{
  return this->address_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::address (ACE_Addr *addr, bool cleanup)
{
  if (this->clean_up_address_)
    delete this->address_;

  this->address_ = addr;
  this->clean_up_address_ = cleanup;
}

ACE_INLINE
ACE_Addr *
TAO_FlowSpec_Entry::control_address (void)
{
  return this->control_address_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::control_address (ACE_Addr *addr)
{
  this->control_address_ = addr;
}

ACE_INLINE
const char *
TAO_FlowSpec_Entry::address_str (void) const
{
  return this->address_str_.c_str();
}

ACE_INLINE
const char*
TAO_FlowSpec_Entry::format (void) const
{
  return this->format_.c_str();
}

ACE_INLINE
const char *
TAO_FlowSpec_Entry::flowname (void) const
{
  return this->flowname_.c_str();
}


ACE_INLINE
int
TAO_FlowSpec_Entry::set_peer_addr (ACE_Addr *peer_addr)
{
  if (this->delete_peer_addr_)
    delete this->peer_addr_;

  this->delete_peer_addr_ = false;
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
int
TAO_FlowSpec_Entry::set_local_control_addr (ACE_Addr *local_addr)
{
  this->local_control_addr_ = local_addr;
  return 0;
}

ACE_INLINE
ACE_Addr*
TAO_FlowSpec_Entry::get_local_addr (void)
{
  return this->local_addr_;
}

ACE_INLINE
ACE_Addr*
TAO_FlowSpec_Entry::get_local_control_addr (void)
{
  return this->local_control_addr_;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::set_local_sec_addr (char** local_sec_addr,
                                        int size)
{
  this->local_sec_addr_ = local_sec_addr;
  this->num_local_sec_addrs_ = size;
  return 0;
}

ACE_INLINE
char**
TAO_FlowSpec_Entry::get_local_sec_addr (void)
{
  return this->local_sec_addr_;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::num_local_sec_addrs (void)
{
  return this->num_local_sec_addrs_;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::set_peer_sec_addr (char** peer_sec_addr,
                                       int size)
{
  this->peer_sec_addr_ = peer_sec_addr;
  this->num_peer_sec_addrs_ = size;
  return 0;
}

ACE_INLINE
char**
TAO_FlowSpec_Entry::get_peer_sec_addr (void)
{
  return this->peer_sec_addr_;
}

ACE_INLINE
int
TAO_FlowSpec_Entry::num_peer_sec_addrs (void)
{
  return this->num_peer_sec_addrs_;
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
TAO_AV_Transport*
TAO_FlowSpec_Entry::control_transport (void)
{
  return this->control_transport_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::control_transport (TAO_AV_Transport *control_transport)
{
  this->control_transport_ = control_transport;
}

ACE_INLINE
TAO_AV_Flow_Handler *
TAO_FlowSpec_Entry::handler (void)
{
  return this->handler_;
}

ACE_INLINE
TAO_AV_Flow_Handler *
TAO_FlowSpec_Entry::control_handler (void)
{
  return this->control_handler_;
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
//           ORBSVCS_ERROR ((LM_ERROR,"TAO_FlowSpec_Entry::handler\n"));
//         break;
//       }
//     }
}

ACE_INLINE
void
TAO_FlowSpec_Entry::control_handler (TAO_AV_Flow_Handler *handler)
{
  this->control_handler_ = handler;
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
TAO_AV_Protocol_Object*
TAO_FlowSpec_Entry::control_protocol_object (void)
{
  return this->control_protocol_object_;
}

ACE_INLINE
void
TAO_FlowSpec_Entry::control_protocol_object (TAO_AV_Protocol_Object *object)
{
  this->control_protocol_object_ = object;
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

TAO_END_VERSIONED_NAMESPACE_DECL
