/* $Id$ */
/*
 * The legacy ACE_SOCK_Dgram_Mcast class is a specialization of the newer
 * ACE_SOCK_Dgram_Mcast_Ex templated class.
 *
 * This file is a vestige of the original class, which is only used to satisfy
 * automated build rules and to elicit environment-specific template support 
 * behavior, if necessary.
 */

// Build the "real" class files.
#include "ace/SOCK_Dgram_Mcast.h"

// Instantiate SOCK_Dgram_Mcast_Ex internal templates, if necessary.
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_DLList_Iterator<ip_mreq>;
template class ACE_DLList<ip_mreq>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_DLList_Iterator<ip_mreq>
#pragma instantiate ACE_DLList<ip_mreq>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
