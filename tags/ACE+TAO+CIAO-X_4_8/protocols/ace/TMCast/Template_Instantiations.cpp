// $Id$

// Note: this file is here only until support for explicit template
// instantiations is removed from ACE, after ACE 5.5 is released.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \
    defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# include "ace/Null_Mutex.h"
# include "ace/Refcounted_Auto_Ptr.h"

# include "TransactionController.hpp"

#endif

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Refcounted_Auto_Ptr<ACE_TMCast::Message, ACE_Null_Mutex>;
template class ACE_Refcounted_Auto_Ptr<ACE_TMCast::Recv, ACE_Null_Mutex>;
template class ACE_Refcounted_Auto_Ptr<ACE_TMCast::Send, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_TMCast::Message, ACE_Null_Mutex>
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_TMCast::Recv, ACE_Null_Mutex>
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_TMCast::Send, ACE_Null_Mutex>

#elif defined (__HP_aCC)
// Make aC++ stop complaining about an empty translation unit
static int shut_up_aCC = 0;
#endif
