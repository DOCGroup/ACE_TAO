// -*- C++ -*-

//=============================================================================
/**
 *  @file    CorbaStateUpdate.cpp
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "CorbaStateUpdate.h"
#include "ReplicatedApplicationC.h"

CorbaStateUpdate::CorbaStateUpdate(CORBA::Object_ptr application)
  : application_ (CORBA::Object::_duplicate (application))
{
}

CorbaStateUpdate::~CorbaStateUpdate()
{
}

void
CorbaStateUpdate::set_state (const ::CORBA::Any & state_value)
{
  if (!CORBA::is_nil (application_.in ()))
    {
      ReplicatedApplication_var ra =
        ReplicatedApplication::_narrow (application_.in ());
      
      ra->set_state (state_value);
    }
}
