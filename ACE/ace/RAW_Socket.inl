#include "ace/ACE.h"
#include "ace/Global_Macros.h"
#include "ace/RAW_Socket.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
ACE_RAW_SOCKET::is_send_only () const
{
  return this->protocol_ == IPPROTO_RAW;
}

ACE_INLINE int
ACE_RAW_SOCKET:: protocol () const
{
  return this->protocol_;
}

ACE_END_VERSIONED_NAMESPACE_DECL

