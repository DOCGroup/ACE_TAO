// $Id$

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
