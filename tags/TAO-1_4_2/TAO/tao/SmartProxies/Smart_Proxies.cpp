// $Id$

#include "Smart_Proxies.h"

ACE_RCSID(SmartProxies, Smart_Proxies, "$Id$")

#if !defined (__ACE_INLINE__)
#include "Smart_Proxies.inl"
#endif /* defined INLINE */

// Add this line to please the AIX linker, otherwise it complains
// "ld: 0711-244 ERROR: No csects or exported symbols have been saved."

#if defined (ACE_NDEBUG) && defined (AIX)
int the_bogus_value_to_please_linker_to_export_symbol;
#endif
