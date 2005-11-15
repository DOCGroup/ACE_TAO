// $Id$

// Note: this file is here only until support for explicit template
// instantiations is removed from ACE, after ACE 5.5 is released.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \
    defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# include "ace/Hash_Map_Manager.h"
# include "Acknowledge.h"

#endif

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<unsigned long, ACE_RMCast::Acknowledge::Descr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<unsigned long, ACE_RMCast::Acknowledge::Descr, ACE_Hash<unsigned long>, ACE_Equal_To<unsigned long>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate class ACE_Hash_Map_Manager<unsigned long, ACE_RMCast::Acknowledge::Descr, ACE_Null_Mutex>
# pragma instantiate ACE_Hash_Map_Manager_Ex<unsigned long, ACE_RMCast::Acknowledge::Descr, ACE_Hash<unsigned long>, ACE_Equal_To<unsigned long>, ACE_Null_Mutex>

#elif defined (__HP_aCC)
// Make aC++ stop complaining about an empty translation unit
static int shut_up_aCC = 0;
#endif
