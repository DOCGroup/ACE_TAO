//$Id$
// Implementation of TAO's Demux_Test interface

#include "demux_test_i.h"
#include "tao/ORB_Core.h"

// ctor
Demux_Test_i::Demux_Test_i (void)
{}

// dtor
Demux_Test_i::~Demux_Test_i (void)
{}

void Demux_Test_i::M302 (void)
{
}

void Demux_Test_i::shutdown (void)
{
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}
