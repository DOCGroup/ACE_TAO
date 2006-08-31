// $Id$

#if !defined (__CUTS_INLINE__)
#include "cuts/Worker_T.inl"
#endif

#include "cuts/Action_Iterator_T.h"

//
// CUTS_Worker_T
//
template <typename WORKER_TYPE>
CUTS_Worker_T <WORKER_TYPE>::CUTS_Worker_T (void)
{
  Worker_Type::init_action_table ();
}

//
// ~CUTS_Worker_T
//
template <typename WORKER_TYPE>
CUTS_Worker_T <WORKER_TYPE>::~CUTS_Worker_T (void)
{

}

//
// action_table_
//
template <typename WORKER_TYPE>
typename CUTS_Worker_T <WORKER_TYPE>::CUTS_Action_Factory_Map
CUTS_Worker_T <WORKER_TYPE>::action_table_;

//
// create_action
//
template <typename WORKER_TYPE>
CUTS_Action * CUTS_Worker_T <WORKER_TYPE>::create_action (
  const char * action_name)
{
  CUTS_Action * action = 0;
  WORKER_TYPE * worker = 0;

  // Create the correct type of worker object.
  worker = ACE_dynamic_cast (WORKER_TYPE *, this);

  if (worker != 0)
  {
    // Locate the create_action () method in the <action_table_>
    // and create the action. We have to assign the action the
    // worker on which it must operate as well.
    CUTS_ACTION_FACTORY_FUNC action_factory = 0;
    CUTS_Worker_T <WORKER_TYPE>::action_table_.find (action_name,
                                                     action_factory);
    if (action_factory != 0)
    {
      action = action_factory (*worker);
    }
  }
  return action;
}

//
// begin
//
template <typename WORKER_TYPE>
CUTS_Action_Iterator * CUTS_Worker_T <WORKER_TYPE>::begin (void)
{
  typedef CUTS_Action_Iterator_T <WORKER_TYPE> iterator;
  iterator * iter = 0;

  ACE_NEW_RETURN (
    iter,
    iterator (ACE_dynamic_cast (WORKER_TYPE *, this),
              CUTS_Worker_T <WORKER_TYPE>::action_table_.begin ()),
    0);

  return iter;
}
