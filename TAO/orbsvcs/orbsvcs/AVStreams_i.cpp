// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   AVStreams_i.cpp
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    
//
// ============================================================================

#include "AVStreams_i.h"

TAO_Basic_StreamCtrl::TAO_Basic_StreamCtrl (void)
{
}

TAO_Basic_StreamCtrl::~TAO_Basic_StreamCtrl (void)
{
}

void 
TAO_Basic_StreamCtrl::stop (const AVStreams::flowSpec &the_spec,  
                            CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
}

void
TAO_Basic_StreamCtrl::start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
}

void
TAO_Basic_StreamCtrl::destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
}


CORBA::Boolean 
TAO_Basic_StreamCtrl::modify_QoS (AVStreams::streamQoS &new_qos, 
                                  const AVStreams::flowSpec &the_spec,  
                                  CORBA::Environment &env)
{
  ACE_UNUSED_ARG (new_qos);
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (env);
  return 0;
}

void
TAO_Basic_StreamCtrl::push_event (const char *the_event,  
                                  CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_event);
  ACE_UNUSED_ARG (env);
}

void
TAO_Basic_StreamCtrl::set_FPStatus (const AVStreams::flowSpec &the_spec, 
                                    const char *fp_name, 
                                    const CORBA::Any &fp_settings,  
                                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_spec);
  ACE_UNUSED_ARG (fp_name);
  ACE_UNUSED_ARG (fp_settings);
  ACE_UNUSED_ARG (env);
}

CORBA::Object_ptr 
TAO_Basic_StreamCtrl::get_flow_connection (const char *flow_name,  
                                                 CORBA::Environment &env)
{
  ACE_UNUSED_ARG (flow_name);
  ACE_UNUSED_ARG (env);
  return 0;
}

void
TAO_Basic_StreamCtrl::set_flow_connection (const char *flow_name, 
                                           CORBA::Object_ptr flow_connection,  
                                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (flow_name);
  ACE_UNUSED_ARG (flow_connection);
  ACE_UNUSED_ARG (env);
}

TAO_StreamCtrl::TAO_StreamCtrl (void)
{
}

TAO_StreamCtrl::~TAO_StreamCtrl (void)
{
}

CORBA::Boolean 
TAO_StreamCtrl::bind_devs (AVStreams::MMDevice_ptr a_party, 
                           AVStreams::MMDevice_ptr b_party, 
                           AVStreams::streamQoS &the_qos, 
                           const AVStreams::flowSpec &the_flows,  
                           CORBA::Environment &env)
{
  return 0;
}
  
CORBA::Boolean 
TAO_StreamCtrl::bind (AVStreams::StreamEndPoint_A_ptr a_party, 
                      AVStreams::StreamEndPoint_B_ptr b_party, 
                      AVStreams::streamQoS &the_qos, 
                      const AVStreams::flowSpec &the_flows,  
                      CORBA::Environment &env)
{
  return 0;
}

void 
TAO_StreamCtrl::unbind_party (AVStreams::StreamEndPoint_ptr the_ep, 
                              const AVStreams::flowSpec &the_spec,  
                              CORBA::Environment &env)
{
}
