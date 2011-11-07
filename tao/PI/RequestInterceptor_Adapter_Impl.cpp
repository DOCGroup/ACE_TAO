// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestInterceptor_Adapter_Impl.cpp
 *
 *  $Id$
 *
 *   This file an adapter class to simplify the support of
 *   interceptors.
 */
//=============================================================================

#include "tao/PI/RequestInterceptor_Adapter_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/ORB_Core.h"
#include "tao/PI/RequestInterceptor_Adapter_Impl.h"
#include "tao/PI/PICurrent.h"
#include "tao/PI/PICurrent_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RequestInterceptor_Adapter_Impl::~TAO_RequestInterceptor_Adapter_Impl (void)
{
}

void
TAO_RequestInterceptor_Adapter_Impl::popTSC (
  TAO_ORB_Core *orb_core)
{
  // Retrieve the thread scope current (no TSS access incurred yet).
  TAO::PICurrent
    *pi_current= dynamic_cast <TAO::PICurrent *> (
      orb_core->pi_current ());

  // If the slot count is zero, there is nothing to copy.  Prevent any
  // copying (and hence TSS accesses) from occurring.
  if (pi_current && pi_current->slot_count ())
    {
      pi_current->tsc ()->pop ();
    }
}

void
TAO_RequestInterceptor_Adapter_Impl::pushTSC (
  TAO_ORB_Core *orb_core)
{
  // Retrieve the thread scope current (no TSS access incurred yet).
  TAO::PICurrent
    *pi_current= dynamic_cast <TAO::PICurrent *> (
      orb_core->pi_current ());

  // If the slot count is zero, there is nothing to copy.  Prevent any
  // copying (and hence TSS accesses) from occurring.
  if (pi_current && pi_current->slot_count ())
    {
      pi_current->tsc ()->push ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
