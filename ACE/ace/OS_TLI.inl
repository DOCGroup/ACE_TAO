// -*- C++ -*-
#include "ace/OS_NS_errno.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS {

ACE_INLINE int
t_accept (ACE_HANDLE handle, ACE_HANDLE reshandle, struct t_call *call)
{
#if defined (ACE_HAS_TLI)
  return ::t_accept (handle, reshandle, call);
#else
  ACE_UNUSED_ARG (call);
  ACE_UNUSED_ARG (reshandle);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE char *
t_alloc (ACE_HANDLE handle, int struct_type, int fields)
{
#if defined (ACE_HAS_TLI)
#  if (defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) >= 500)
  // XPG5 changes t_alloc() return from char* to void*, so ACE_OSCALL_RETURN
  // doesn't compile correctly.
  char *result;
  ACE_OSCALL (::t_alloc (handle, struct_type, fields), char *, result);
  return result;
#  else
  return ::t_alloc (handle, struct_type, fields);
#  endif /* XPG4 vs XPG5 */
#else
  ACE_UNUSED_ARG (fields);
  ACE_UNUSED_ARG (struct_type);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_bind (ACE_HANDLE handle, ACE_TBIND *req, ACE_TBIND *ret)
{
#if defined (ACE_HAS_TLI)
  return ::t_bind (handle, req, ret);
#else
  ACE_UNUSED_ARG (ret);
  ACE_UNUSED_ARG (req);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_close (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  return ::t_close (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_connect(ACE_HANDLE fildes,
          struct t_call *sndcall,
          struct t_call *rcvcall)
{
#if defined (ACE_HAS_TLI)
  return ::t_connect (fildes, sndcall, rcvcall);
#else
  ACE_UNUSED_ARG (fildes);
  ACE_UNUSED_ARG (sndcall);
  ACE_UNUSED_ARG (rcvcall);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE void
t_error (const char *errmsg)
{
#if defined (ACE_HAS_TLI)
#if defined (ACE_HAS_BROKEN_T_ERROR)
  ::t_error (const_cast<char *> (errmsg));
#else
  ::t_error (errmsg);
#endif /* ACE_HAS_BROKEN_T_ERROR */
#else
  ACE_UNUSED_ARG (errmsg);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_free (char *ptr, int struct_type)
{
#if defined (ACE_HAS_TLI)
  if (!ptr)
    return 0;
  return ::t_free (ptr, struct_type);
#else
  ACE_UNUSED_ARG (struct_type);
  ACE_UNUSED_ARG (ptr);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_getinfo (ACE_HANDLE handle, struct t_info *info)
{
#if defined (ACE_HAS_TLI)
  return ::t_getinfo (handle, info);
#else
  ACE_UNUSED_ARG (info);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_getname (ACE_HANDLE handle, struct netbuf *namep, int type)
{
#if defined (ACE_HAS_XTI)
  ACE_TBIND bound, peer;
  // Depending on which address the caller wants, fill caller's values
  // into one of the t_bind netbufs. The other is set up to ignore that
  // address.
  switch (type)
    {
    case LOCALNAME:
      bound.addr.buf = namep->buf;
      bound.addr.maxlen = namep->maxlen;
      bound.addr.len = 0;
      peer.addr.buf = 0;
      peer.addr.maxlen = 0;
      peer.addr.len = 0;
      break;
    case REMOTENAME:
      bound.addr.buf = 0;
      bound.addr.maxlen = 0;
      bound.addr.len = 0;
      peer.addr.buf = namep->buf;
      peer.addr.maxlen = namep->maxlen;
      peer.addr.len = 0;
      break;
    default:
      ACE_OS::last_error (EINVAL);
      return -1;
    }
  if (t_getprotaddr (handle, &bound, &peer) == -1)
    return -1;
  // Call succeeded; put the caller's desired address length in his netbuf.
  if (type == LOCALNAME)
    namep->len = bound.addr.len;
  else
    namep->len = peer.addr.len;
  return 0;

#elif defined (ACE_HAS_SVR4_TLI)
  return ::t_getname (handle, namep, type);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (namep);
  ACE_UNUSED_ARG (type);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SVR4_TLI */
}

ACE_INLINE int
t_getstate (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  return ::t_getstate (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_listen (ACE_HANDLE handle, struct t_call *call)
{
#if defined (ACE_HAS_TLI)
  return ::t_listen (handle, call);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (call);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_look (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  return ::t_look (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE ACE_HANDLE
t_open (char *path, int oflag, struct t_info *info)
{
#if defined (ACE_HAS_TLI)
  return ::t_open (path, oflag, info);
#else
  ACE_UNUSED_ARG (path);
  ACE_UNUSED_ARG (oflag);
  ACE_UNUSED_ARG (info);

  ACE_NOTSUP_RETURN (ACE_INVALID_HANDLE);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_optmgmt (ACE_HANDLE handle, ACE_TOPTMGMT *req, ACE_TOPTMGMT *ret)
{
#if defined (ACE_HAS_TLI)
  return ::t_optmgmt (handle, req, ret);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (req);
  ACE_UNUSED_ARG (ret);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_rcv (ACE_HANDLE handle,
       char *buf,
       unsigned int nbytes,
       int *flags)
{
#if defined (ACE_HAS_TLI)
  return ::t_rcv (handle, buf, nbytes, flags);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (nbytes);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_rcvdis (ACE_HANDLE handle, struct t_discon *discon)
{
#if defined (ACE_HAS_TLI)
  return ::t_rcvdis (handle, discon);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (discon);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_rcvrel (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  return ::t_rcvrel (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_rcvudata (ACE_HANDLE handle,
            struct t_unitdata *unitdata,
            int *flags)
{
#if defined (ACE_HAS_TLI)
  return ::t_rcvudata (handle, unitdata, flags);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (unitdata);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_rcvuderr (ACE_HANDLE handle, struct t_uderr *uderr)
{
#if defined (ACE_HAS_TLI)
  return ::t_rcvuderr (handle, uderr);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (uderr);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_snd (ACE_HANDLE handle,
       const char *buf,
       unsigned int nbytes,
       int flags)
{
#if defined (ACE_HAS_TLI)
  return ::t_snd (handle, (char *) buf, nbytes, flags);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (nbytes);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_snddis (ACE_HANDLE handle, struct t_call *call)
{
#if defined (ACE_HAS_TLI)
  return ::t_snddis (handle, call);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (call);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_sndrel (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  return ::t_sndrel (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_sync (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  return ::t_sync (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
t_unbind (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  return ::t_unbind (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

}  /* end namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL
