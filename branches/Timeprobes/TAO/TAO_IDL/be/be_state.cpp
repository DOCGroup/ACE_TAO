// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state.cpp
//
// = DESCRIPTION
//    state based code generation.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// constructor
be_state::be_state (void)
{
}

// destructor
be_state::~be_state (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<be_state_struct, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_disctypedefn_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_disctypedefn_ci, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_public_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_public_ci, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_public_cs, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_private_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_operation, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_argument, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_typedef, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_array, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_sequence, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_attribute, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_exception, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<be_state_struct, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_union_disctypedefn_ch, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_union_disctypedefn_ci, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_union_public_ch, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_union_public_ci, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_union_public_cs, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_union_private_ch, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_operation, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_argument, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_typedef, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_array, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_sequence, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_attribute, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_state_exception, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
