// $Id$

#include "ace/OS.h"
#include "ace/Containers.h"

ACE_RCSID(ace, Containers, "$Id$")

// Can't put this in Containers.cpp.
// Probably belongs elsewhere, but this will do in a pinch.
#if ! defined (ACE_LACKS_TEMPLATE_AS_TEMPLATE_PARAMETER)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Double_Linked_List<ACE_DLList_Node>;
template class ACE_Double_Linked_List_Iterator<ACE_DLList_Node>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Double_Linked_List<ACE_DLList_Node>
#pragma instantiate ACE_Double_Linked_List_Iterator<ACE_DLList_Node>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ! defined (ACE_LACKS_TEMPLATE_AS_TEMPLATE_PARAMETER) */
