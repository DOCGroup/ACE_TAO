// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Float
TAO_LB_LeastLoaded::raw_load (CORBA::Float load)
{
  return this->dampened_load (load + this->per_balance_load_);
}


ACE_INLINE CORBA::Float
TAO_LB_LeastLoaded::raw_load (CORBA::Float load)
{
  return this->dampened_load (load + this->per_balance_load_);
}


ACE_INLINE CORBA::Float
TAO_LB_LeastLoaded::effective_load (CORBA::Float previous_load,
                                    CORBA::Float new_load)
{
  // Apply per-balance load.  (Recompute raw load)
  previous_load += this->per_balance_load_;

  // Apply dampening.  (Recompute new raw load)
  CORBA::Float result =
    this->dampening_ * previous_load + (1 - this->dampening_) * new_load;

  ACE_ASSERT (this->tolerance_ != 0);

  // Compute th effective load.
  result = raw_load / this->tolerance_;

  return result;
}
