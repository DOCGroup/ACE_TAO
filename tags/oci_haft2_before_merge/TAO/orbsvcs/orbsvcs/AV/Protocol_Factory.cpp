// $Id$


#include "Protocol_Factory.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"

// TAO_AV_Flow_Protocol_Factory
TAO_AV_Flow_Protocol_Factory::TAO_AV_Flow_Protocol_Factory (void)
{
}

TAO_AV_Flow_Protocol_Factory::~TAO_AV_Flow_Protocol_Factory (void)
{
}

int
TAO_AV_Flow_Protocol_Factory::init (int /* argc */,
                                    char * /* argv */ [])
{
  return -1;
}

int
TAO_AV_Flow_Protocol_Factory::match_protocol (const char * /* protocol_string */)
{
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_Flow_Protocol_Factory::make_protocol_object (TAO_FlowSpec_Entry * /* entry */,
                                                    TAO_Base_StreamEndPoint * /* endpoint */ ,
                                                    TAO_AV_Flow_Handler * /* handler */,
                                                    TAO_AV_Transport * /* transport */)
{
  return 0;
}

const char *
TAO_AV_Flow_Protocol_Factory::control_flow_factory (void)
{
  return 0;
}

//----------------------------------------------------------------------
// TAO_AV_Protocol_Object
//----------------------------------------------------------------------
TAO_AV_Protocol_Object::TAO_AV_Protocol_Object (void)
  :transport_ (0),
   callback_ (0) 
{
  // no-op.
}

TAO_AV_Protocol_Object::TAO_AV_Protocol_Object (TAO_AV_Callback *callback,
                                                TAO_AV_Transport *transport)
  :transport_ (transport),
   callback_ (callback)
{
  // no-op.
}

int
TAO_AV_Protocol_Object::open (TAO_AV_Callback *callback,
                              TAO_AV_Transport *transport)
{
  this->callback_ = callback;
  this->transport_ = transport;
  return 0;
}

TAO_AV_Protocol_Object::~TAO_AV_Protocol_Object (void)
{
}

int
TAO_AV_Protocol_Object::start (void)
{
  return this->callback_->handle_start ();
}

int
TAO_AV_Protocol_Object::stop (void)
{
  return this->callback_->handle_stop ();
}

int
TAO_AV_Protocol_Object::set_policies (const TAO_AV_PolicyList &policy_list)
{
  this->policy_list_ = policy_list;
  return 0;
}

TAO_AV_PolicyList
TAO_AV_Protocol_Object::get_policies (void)
{
  return this->policy_list_;
}

TAO_AV_Transport*
TAO_AV_Protocol_Object::transport (void)
{
  return this->transport_;
}

// void
// TAO_AV_Protocol_Object::transport (TAO_AV_Transport *transport)
// {
//   this->transport_ = transport;
// }

void
TAO_AV_Protocol_Object::control_object (TAO_AV_Protocol_Object * /* object */)
{
  return;
}

int
TAO_AV_Protocol_Object::handle_control_input (ACE_Message_Block *,
                                              const ACE_Addr &)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"TAO_AV_Protocol_Object::handle_control_input\n"));
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_AV_Transport_Factory>;
template class ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_AV_Transport_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>
#endif
