// $Id$
#include "Stub_Factory.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, TAO_Stub_Factory, "$Id$")

TAO_Stub_Factory::~TAO_Stub_Factory (void)
{
  // No-Op.
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Stub_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Stub_Factory>

#endif
