//$Id$
// Implementation of TAO's Demux_Test interface

#include "demux_test_i.h"

// ctor
Demux_Test_i::Demux_Test_i (void)
{}

// dtor
Demux_Test_i::~Demux_Test_i (void)
{}

void Demux_Test_i::M302 (CORBA::Environment &env)
{
   ACE_UNUSED_ARG (env);
}

void Demux_Test_i::shutdown (CORBA::Environment &env)
{
   ACE_UNUSED_ARG (env);
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

