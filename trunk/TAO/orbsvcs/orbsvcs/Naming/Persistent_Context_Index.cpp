// $Id$

#include "Persistent_Context_Index.h"

TAO_Index::~TAO_Index (void)
{
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
