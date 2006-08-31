// $Id$

//
// name
//
CUTS_INLINE
const char * CUTS_Port_Agent::name (void) const
{
  return this->name_.c_str ();
}

//
// name
//
CUTS_INLINE
void CUTS_Port_Agent::name (const char * name)
{
  this->name_ = name;
}

//
// create_activation_record
//
CUTS_INLINE
CUTS_Activation_Record * CUTS_Port_Agent::create_activation_record (void)
{
  return this->free_list_.remove ();
}

//
// port_measurements
//
CUTS_INLINE
CUTS_Port_Measurement_Map & CUTS_Port_Agent::port_measurements (void)
{
  return this->measurement_pool_.advance ();
}

//
// destroy_activation_record
//
CUTS_INLINE
void CUTS_Port_Agent::destroy_activation_record (
  CUTS_Activation_Record * record)
{
  if (this->active_)
  {
    this->closed_list_.enqueue_tail (record);
  }
  else
  {
    this->add_to_free_list (record);
  }
}
