// -*- C++ -*-

// ============================================================================
//
// $Id$
//
// ============================================================================

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// This helper function allows the application to determine whether
// the factory is uninitialized, or in a config or runtime mode of
// operation.

ACE_INLINE ACE_Scheduler_Factory::Factory_Status
ACE_Scheduler_Factory::status (void)
{
  return status_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
