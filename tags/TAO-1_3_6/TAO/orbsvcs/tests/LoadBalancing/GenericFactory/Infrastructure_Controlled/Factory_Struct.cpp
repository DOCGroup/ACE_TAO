#include "Factory_Struct.h"

ACE_RCSID (Infrastructure_Controlled,
           Factory_Struct,
           "$Id$");

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<Factory_Node>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<Factory_Node>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
