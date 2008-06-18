// $Id$

#include "be_visitor_typecode/typecode_defn.h"
#include "be_stream_factory.h"
#include "be_visitor_factory.h"
#include "ast_interface.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Unbounded_Queue.h"

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template class ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_OutStream_Factory,ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Visitor_Factory,ACE_SYNCH_RECURSIVE_MUTEX>;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
