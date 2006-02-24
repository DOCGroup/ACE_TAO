// $Id$
#include "ace/CDR_Base.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Unbounded_Set.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Instantiations of templates with basic data types that can be used
// across  ACE+TAO.
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_INT32>;
template class ACE_Unbounded_Queue<ACE_INT32>;
template class ACE_Unbounded_Queue_Iterator<ACE_INT32>;
template class ACE_Unbounded_Set<ACE_HANDLE>;
template class ACE_Unbounded_Set_Iterator<ACE_HANDLE>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_INT32>
#pragma instantiate ACE_Unbounded_Queue<ACE_INT32>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_INT32>
#pragma instantiate ACE_Unbounded_Set<ACE_HANDLE>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_HANDLE>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION*/

ACE_END_VERSIONED_NAMESPACE_DECL
