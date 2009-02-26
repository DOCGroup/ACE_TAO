// -*- C++ -*-

//=============================================================================
/**
 *  @file    FaultCorrelationManager_Impl.cpp
 *
 *  $Id$
 *
 *  @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#include "FaultCorrelationManager_Impl.h"

namespace DAnCE
{
  FaultCorrelationManager_Impl::FaultCorrelationManager_Impl (
    CORBA::ORB_ptr orb,
    const PROPERTY_MAP & options)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      properties_ (options.current_size ())
  {
  }

  FaultCorrelationManager_Impl::~FaultCorrelationManager_Impl()
  {
  }

  void
  FaultCorrelationManager_Impl::stop_failver_unit (const char * /* fou_id */)
  {
  }

  void
  FaultCorrelationManager_Impl::proc_failure (const char *,
                                              const char *)
  {
  }
};
