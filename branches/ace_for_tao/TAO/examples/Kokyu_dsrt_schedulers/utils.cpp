//$Id$

#include "utils.h"
#include "Kokyu.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/OS_NS_string.h"

void guid_copy( Kokyu::GuidType& lhs, const RTScheduling::Current::IdType& rhs)
{
  lhs.length(rhs.length ());
  ACE_OS::memcpy(lhs.get_buffer (),
                 rhs.get_buffer (),
                 rhs.length ());
}

void guid_copy( RTScheduling::Current::IdType& lhs, const Kokyu::GuidType& rhs)
{
  lhs.length(rhs.length ());
  ACE_OS::memcpy(lhs.get_buffer (),
                 rhs.get_buffer (),
                 rhs.length ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
template class ACE_Array_Iterator<Kokyu::ConfigInfo>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Lock_Adapter<ACE_Thread_Mutex>
#pragma instantiate ACE_Array_Iterator<Kokyu::ConfigInfo>
#endif
