//
// $Id$
//
#include "McastHello.h"

ACE_RCSID(McastHello, McastHello, "$Id$")

McastHello::McastHello (CORBA::ORB_ptr orb,
                        int instance,
                        CORBA::Boolean &status)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    instance_ (instance),
    status_ (status)
{
  // Initialize to false (failure)
  this->status_ = 0;
}

void
McastHello::send_forty_two (CORBA::Long forty_two TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->status_ == 0 &&
      forty_two == 42)
    {
      this->status_ = 1;
    }
  else
    this->status_ = 0;
}

void
McastHello::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
