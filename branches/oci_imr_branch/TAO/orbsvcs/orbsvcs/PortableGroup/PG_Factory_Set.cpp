#include "PG_Factory_Set.h"

ACE_RCSID (PortableGroup,
           PG_Factory_Set,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<TAO_PG_Factory_Node>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<TAO_PG_Factory_Node>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
