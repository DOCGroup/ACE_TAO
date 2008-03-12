#include "tao/Monitor/Monitor.h"

ACE_RCSID (Monitor,
           Monitor,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// This initializer is very similar to the ORBInitializer_Registry::init.
// This one is used during static initialization while the other is used
// when Monitor is a shared library.
int
TAO_Monitor_Init::Initializer (void)
{
  int const status = 0;
  return status;
}

TAO_END_VERSIONED_NAMESPACE_DECL
