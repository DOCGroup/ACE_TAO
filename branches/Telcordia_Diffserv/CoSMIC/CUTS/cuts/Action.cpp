// $Id$

#include "cuts/Action.h"

//
// CUTS_Action
//
CUTS_Action::CUTS_Action (void)
{

}

//
// CUTS_Action
//
CUTS_Action::CUTS_Action (const CUTS_Action &)
{

}

//
// release
//
void CUTS_Action::release (void)
{
  delete this;
}
