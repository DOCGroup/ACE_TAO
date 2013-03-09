// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Float
TAO_LB_LoadMinimum::effective_load (CORBA::Float previous_load,
                                    CORBA::Float new_load)
{
  // Apply per-balance load.  (Recompute raw load)
  previous_load += this->per_balance_load_;

  // Apply dampening.  (Recompute new raw load)
  CORBA::Float result =
    this->dampening_ * previous_load + (1 - this->dampening_) * new_load;

  ACE_ASSERT (!ACE::is_equal (this->tolerance_, 0.0f));

  // Compute the effective load.
  result /= this->tolerance_;

  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
