// $Id$
#include "tao/Abstract_Servant_Base.h"

ACE_RCSID (tao, Abstract_Servant_Base, "$Id$")

TAO_Abstract_ServantBase::TAO_Abstract_ServantBase (void)
{
  // No-Op.
}

TAO_Abstract_ServantBase::~TAO_Abstract_ServantBase (void)
{
  // No-Op.
}



void
TAO_Abstract_ServantBase::_add_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
}

void
TAO_Abstract_ServantBase::_remove_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
}


TAO_Abstract_ServantBase::TAO_Abstract_ServantBase (const TAO_Abstract_ServantBase &)
{
  // No-Op
}

TAO_Abstract_ServantBase & TAO_Abstract_ServantBase::operator= (const TAO_Abstract_ServantBase&)
{
  return *this;
} 




