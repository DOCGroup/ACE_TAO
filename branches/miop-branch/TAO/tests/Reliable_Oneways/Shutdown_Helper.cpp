//
// $Id$
//
#include "Shutdown_Helper.h"

ACE_RCSID(Reliable_Oneways, Shutdown_Helper, "$Id$")

Shutdown_Helper::Shutdown_Helper (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Shutdown_Helper::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
