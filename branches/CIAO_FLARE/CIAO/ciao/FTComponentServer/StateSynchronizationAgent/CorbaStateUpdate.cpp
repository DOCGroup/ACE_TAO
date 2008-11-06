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

CorbaStateUpdate::CorbaStateUpdate(ReplicatedApplication_ptr application)
  : application_ (ReplicatedApplication::_duplicate (application))
{
}

CorbaStateUpdate::~CorbaStateUpdate()
{
}

void
CorbaStateUpdate::set_state (const ::CORBA::Any & state_value)
{
  if (!CORBA::is_nil (application_.in ()))
    application_->set_state (state_value);
}
