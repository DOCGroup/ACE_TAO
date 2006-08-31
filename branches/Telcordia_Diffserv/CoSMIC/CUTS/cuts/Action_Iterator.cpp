// $Id$

#include "cuts/Action_Iterator.h"

//
// no_name_
//
ACE_CString CUTS_Action_Iterator::no_name_;

//
// release
//
void CUTS_Action_Iterator::release (void)
{
  delete this;
}

//
// ~CUTS_Action_Iterator
//
CUTS_Action_Iterator::~CUTS_Action_Iterator (void)
{

}

//
// CUTS_Action_Iterator
//
CUTS_Action_Iterator::CUTS_Action_Iterator (void)
{

}

//
// done
//
bool CUTS_Action_Iterator::done (void) const
{
  return true;
}

//
// advance
//
void CUTS_Action_Iterator::advance (void)
{

}

//
// reverse
//
void CUTS_Action_Iterator::reverse (void)
{

}

//
// create_action
//
CUTS_Action * CUTS_Action_Iterator::create_action (void) const
{
  return 0;
}

//
// action_name
//
const ACE_CString & CUTS_Action_Iterator::action_name (void) const
{
  return CUTS_Action_Iterator::no_name_;
}
