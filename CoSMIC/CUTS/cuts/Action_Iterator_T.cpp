// $Id$

//
// CUTS_Action_Iterator_T
//
template <typename WORKER_TYPE>
CUTS_Action_Iterator_T<WORKER_TYPE>::CUTS_Action_Iterator_T (
  WORKER_TYPE * worker, iterator iter)
: worker_ (worker),
  iter_ (iter)
{

}

//
// CUTS_Action_Iterator_T
//
template <typename WORKER_TYPE>
CUTS_Action_Iterator_T <WORKER_TYPE>::~CUTS_Action_Iterator_T (void)
{

}

//
// action_name
//
template <typename WORKER_TYPE>
const ACE_CString &
CUTS_Action_Iterator_T <WORKER_TYPE>::action_name (void) const
{
  return (*this->iter_).ext_id_;
}

//
// create_action
//
template <typename WORKER_TYPE>
CUTS_Action *
CUTS_Action_Iterator_T <WORKER_TYPE>::create_action (void) const
{
  return (*this->iter_).int_id_ (*this->worker_);
}

//
// advance
//
template <typename WORKER_TYPE>
void CUTS_Action_Iterator_T <WORKER_TYPE>::advance (void)
{
  this->iter_ ++;
}

//
// reverse
//
template <typename WORKER_TYPE>
void CUTS_Action_Iterator_T <WORKER_TYPE>::reverse (void)
{
  this->iter_ --;
}

//
// done
//
template <typename WORKER_TYPE>
bool CUTS_Action_Iterator_T <WORKER_TYPE>::done (void) const
{
  return this->iter_.done () == 1;
}
