// $Id$

#include "ace/ACE.h"
#include "be_visitor_typecode/typecode_defn.h"
#include "be_stream_factory.h"
#include "be_visitor_factory.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Common_Visitor_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Compiled_Visitor_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;

template class ACE_Node <be_interface *>;
template class ACE_Unbounded_Queue <be_interface *>;
template class ACE_Unbounded_Queue_Iterator <be_interface *>;

template class ACE_Node<be_visitor_typecode_defn::QNode*>;
template class ACE_Unbounded_Queue<be_visitor_typecode_defn::QNode*>;
template class ACE_Unbounded_Queue_Iterator<be_visitor_typecode_defn::QNode*>;

template class ACE_Node <be_interface_fwd *>;
template class ACE_Unbounded_Queue <be_interface_fwd *>;
template class ACE_Unbounded_Queue_Iterator <be_interface_fwd *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Common_Visitor_Factory, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Compiled_Visitor_Factory, ACE_SYNCH_RECURSIVE_MUTEX>

#pragma instantiate ACE_Node <be_interface *>
#pragma instantiate ACE_Unbounded_Queue <be_interface *>
#pragma instantiate ACE_Unbounded_Queue_Iterator <be_interface *>

#pragma instantiate ACE_Node<be_visitor_typecode_defn::QNode*>
#pragma instantiate ACE_Unbounded_Queue<be_visitor_typecode_defn::QNode*>
#pragma instantiate ACE_Unbounded_Queue_Iterator<be_visitor_typecode_defn::QNode*>

#pragma instantiate ACE_Node <be_interface_fwd *>
#pragma instantiate ACE_Unbounded_Queue <be_interface_fwd *>
#pragma instantiate ACE_Unbounded_Queue_Iterator <be_interface_fwd *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

