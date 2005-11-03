// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Strategy::TP_Strategy(unsigned num_threads)
  : num_threads_(num_threads)
{
  // Assumes that num_threads > 0.
}


ACE_INLINE
void
TAO::CSD::TP_Strategy::set_num_threads(unsigned num_threads)
{
  // Simple Mutator.  Assumes that num_threads > 0.
  this->num_threads_ = num_threads;
}

TAO_END_VERSIONED_NAMESPACE_DECL
