// $Id$

ACE_INLINE
ACE_GPlot_File::ACE_GPlot_File (void)
  : closed_ (1)
{
}


ACE_INLINE
ACE_GPlot_File::~ACE_GPlot_File (void)
{
  this->close ();
}


ACE_INLINE
void
ACE_GPlot_File::set_greatest (long entry, long value)
{
  long old_value;

  // If there was no previous value, or the <value> is greater than
  // the previous value, set a new value.
  if (this->get (entry, old_value) == -1 ||
      value > old_value)
    this->set (entry, value);
}


ACE_INLINE
void
ACE_GPlot_File::set_greatest (long entry, float value)
{
  float old_value;

  // If there was no previous value, or the <value> is greater than
  // the previous value, set a new value.
  if (this->get (entry, old_value) == -1 ||
      value > old_value)
    this->set (entry, value);
}


ACE_INLINE
void
ACE_GPlot_File::set_least (long entry, long value)
{
  long old_value;

  // If there was no previous value, or the <value> is less than
  // the previous value, set a new value.
  if (this->get (entry, old_value) == -1 ||
      value < old_value)
    this->set (entry, value);
}


ACE_INLINE
void
ACE_GPlot_File::set_least (long entry, float value)
{
  float old_value;

  // If there was no previous value, or the <value> is less than
  // the previous value, set a new value.
  if (this->get (entry, old_value) == -1 ||
      value < old_value)
    this->set (entry, value);
}
