// $Id$

#include "tao/Abstract_Servant_Base.h"

ACE_RCSID (tao,
           Abstract_Servant_Base,
           "$Id$")

TAO_Abstract_ServantBase::TAO_Abstract_ServantBase (void)
{
  // No-Op.
}

TAO_Abstract_ServantBase::~TAO_Abstract_ServantBase (void)
{
  // No-Op.
}

TAO_Abstract_ServantBase::TAO_Abstract_ServantBase (const TAO_Abstract_ServantBase &)
{
  // No-Op
}

TAO_Abstract_ServantBase & TAO_Abstract_ServantBase::operator= (const TAO_Abstract_ServantBase&)
{
  return *this;
}
