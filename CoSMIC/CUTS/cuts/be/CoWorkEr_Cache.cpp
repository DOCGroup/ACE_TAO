// $Id$

#include "CoWorkEr_Cache.h"
#include "PICML/PICML.h"

static const char * CUTS_TESTING_SERVICE = "cuts_testing_service";

//
// instance_
//
CUTS_CoWorkEr_Cache * CUTS_CoWorkEr_Cache::instance_ = 0;

//
// CUTS_CoWorkEr_Cache
//
CUTS_CoWorkEr_Cache::CUTS_CoWorkEr_Cache (void)
{

}

//
// ~CUTS_CoWorkEr_Cache
//
CUTS_CoWorkEr_Cache::~CUTS_CoWorkEr_Cache (void)
{

}

//
// close
//
void CUTS_CoWorkEr_Cache::close (void)
{
  if (this == CUTS_CoWorkEr_Cache::instance_)
  {
    CUTS_CoWorkEr_Cache::instance_ = 0;
  }

  delete this;
}

//
// instance
//
CUTS_CoWorkEr_Cache * CUTS_CoWorkEr_Cache::instance (void)
{
  if (CUTS_CoWorkEr_Cache::instance_ == 0)
  {
    CUTS_CoWorkEr_Cache::instance_ = new CUTS_CoWorkEr_Cache ();
  }
  return CUTS_CoWorkEr_Cache::instance_;
}

//
// is_coworker
//
bool CUTS_CoWorkEr_Cache::is_coworker (const PICML::Component & component)
{
  typedef std::set <PICML::RequiredRequestPort> RequiredRequestPort_Set;
  RequiredRequestPort_Set receptacles =
    component.RequiredRequestPort_kind_children ();

  for (RequiredRequestPort_Set::const_iterator iter = receptacles.begin ();
       iter != receptacles.end ();
       iter ++)
  {
    if (((std::string) iter->name ()) == CUTS_TESTING_SERVICE)
    {
      return true;
    }
  }

  return false;
}
