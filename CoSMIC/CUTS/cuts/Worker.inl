// $Id$

//
// parent
//
CUTS_INLINE
void CUTS_Worker::parent (long parent)
{
  this->parent_ = parent;
}

//
// parent
//
CUTS_INLINE
long CUTS_Worker::parent (void) const
{
  return this->parent_;
}

//
// counter
//
CUTS_INLINE
long CUTS_Worker::counter (void) const
{
  return 0;
}

//
// create_action
//
CUTS_INLINE
CUTS_Action * CUTS_Worker::create_action (const char * action_name)
{
  return 0;
}

//
// action_count
//
CUTS_INLINE
size_t CUTS_Worker::action_count (void) const
{
  return 0;
}

//
// init_action_table
//
CUTS_INLINE
void CUTS_Worker::init_action_table (void)
{

}
