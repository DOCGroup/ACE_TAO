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

TAO_Basic_StreamCtrl::TAO_Basic_StreamCtrl (const char *obj_name)
  : POA_AVStreams::Basic_StreamCtrl (obj_name)
{
}

TAO_Basic_StreamCtrl::~TAO_Basic_StreamCtrl ()
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
}

void
TAO_Basic_StreamCtrl::push_event (const AVStreams::streamEvent &the_event,  
                                  CORBA::Environment &env)
{
  ACE_UNUSED_ARG (the_event);
  ACE_UNUSED_ARG (the_spec);
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

