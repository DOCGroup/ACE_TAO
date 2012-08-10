// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_PG
{
  ACE_INLINE
  Fragments_Cleanup_Strategy::Fragments_Cleanup_Strategy (int bound)
    : bound_ (bound)
  {
  }

  ACE_INLINE
  Time_Bound_Fragments_Cleanup_Strategy::
    Time_Bound_Fragments_Cleanup_Strategy (int bound)
    : Fragments_Cleanup_Strategy (bound)
  {
  }

  ACE_INLINE
  Number_Bound_Fragments_Cleanup_Strategy::
    Number_Bound_Fragments_Cleanup_Strategy (int bound)
    : Fragments_Cleanup_Strategy (bound)
  {
  }

  ACE_INLINE
  Memory_Bound_Fragments_Cleanup_Strategy::
    Memory_Bound_Fragments_Cleanup_Strategy (int bound)
    : Fragments_Cleanup_Strategy (bound)
  {
  }
} // namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL
