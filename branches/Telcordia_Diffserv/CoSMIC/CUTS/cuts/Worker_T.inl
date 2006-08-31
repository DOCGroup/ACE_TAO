// $Id$

//
// action_count
//
template <typename WORKER_TYPE>
CUTS_INLINE
size_t CUTS_Worker_T <WORKER_TYPE>::action_count (void) const
{
  return CUTS_Worker_T <WORKER_TYPE>::action_table_.current_size ();
}
