// -*- C++ -*-
#include "ace/OS_NS_errno.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
ACE_OS::msgctl (int msqid, int cmd, struct msqid_ds *val)
{
  ACE_OS_TRACE ("ACE_OS::msgctl");
#if defined (ACE_HAS_SYSV_IPC)
  return ::msgctl (msqid, cmd, val);
#else
  ACE_UNUSED_ARG (msqid);
  ACE_UNUSED_ARG (cmd);
  ACE_UNUSED_ARG (val);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SYSV_IPC */
}

ACE_INLINE int
ACE_OS::msgget (key_t key, int msgflg)
{
  ACE_OS_TRACE ("ACE_OS::msgget");
#if defined (ACE_HAS_SYSV_IPC)
  return ::msgget (key, msgflg);
#else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (msgflg);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SYSV_IPC */
}

ACE_INLINE ssize_t
ACE_OS::msgrcv (int int_id, void *buf, size_t len,
                long type, int flags)
{
  ACE_OS_TRACE ("ACE_OS::msgrcv");
#if defined (ACE_HAS_SYSV_IPC)
  return ::msgrcv (int_id, buf, len, type, flags);
#else
  ACE_UNUSED_ARG (int_id);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (len);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SYSV_IPC */
}

ACE_INLINE int
ACE_OS::msgsnd (int int_id, const void *buf, size_t len, int flags)
{
  ACE_OS_TRACE ("ACE_OS::msgsnd");
#if defined (ACE_HAS_SYSV_IPC)
# if defined (ACE_HAS_NONCONST_MSGSND)
  return ::msgsnd (int_id, const_cast<void *> (buf), len, flags);
# else
  return ::msgsnd (int_id, buf, len, flags);
# endif /* ACE_HAS_NONCONST_MSGSND */
#else
  ACE_UNUSED_ARG (int_id);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (len);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SYSV_IPC */
}

ACE_END_VERSIONED_NAMESPACE_DECL
