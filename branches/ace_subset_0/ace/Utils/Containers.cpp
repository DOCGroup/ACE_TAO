// $Id$

#include "ace/OS/OS.h"
#include "ace/Utils/Containers.h"

ACE_RCSID(ace, Containers, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Utils/Containers.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Double_Linked_List<ACE_DLList_Node>;
template class ACE_Double_Linked_List_Iterator_Base<ACE_DLList_Node>;
template class ACE_Double_Linked_List_Iterator<ACE_DLList_Node>;
template class ACE_Double_Linked_List_Reverse_Iterator<ACE_DLList_Node>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Double_Linked_List<ACE_DLList_Node>
#pragma instantiate ACE_Double_Linked_List_Iterator_Base<ACE_DLList_Node>
#pragma instantiate ACE_Double_Linked_List_Iterator<ACE_DLList_Node>
#pragma instantiate ACE_Double_Linked_List_Reverse_Iterator<ACE_DLList_Node>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

