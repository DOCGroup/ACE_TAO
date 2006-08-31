// $Id$

//
// operations
//
CUTS_INLINE
const CUTS_Worker_Measurement::Operation_Map &
CUTS_Worker_Measurement::operations (void) const
{
  return this->operations_;
}

CUTS_INLINE
CUTS_Worker_Measurement::Operation_Map &
CUTS_Worker_Measurement::operations (void)
{
  return this->operations_;
}

//
// operator =
//
CUTS_INLINE
const CUTS_Worker_Measurement &
CUTS_Worker_Measurement::operator = (const CUTS_Worker_Measurement & src)
{
  this->operations_ = src.operations_;
  return *this;
}

//
// record_operation
//
CUTS_INLINE
void CUTS_Worker_Measurement::record_operation (size_t reps, long opid,
                                                const ACE_Time_Value & tm)
{
  this->operations_[opid](reps, tm);
}

