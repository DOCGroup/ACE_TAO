// $Id$

/* Defines the member functions for the base class of the ACE_ATM_Stream
   abstraction. */

#define ACE_BUILD_DLL
#include "ace/ATM_Stream.h"

ACE_RCSID(ace, ATM_Stream, "$Id$")

#if defined (ACE_HAS_ATM)

#if !defined (__ACE_INLINE__)
#include "ace/ATM_Stream.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_ATM_Stream)

void
ACE_ATM_Stream::dump (void) const
{
  ACE_TRACE ("ACE_ATM_Stream::dump");
}

char*
ACE_ATM_Stream::get_peer_name (void) const
{
  ACE_TRACE ("ACE_ATM_Stream::get_peer_name");
#if defined (ACE_HAS_FORE_ATM_XTI)
//   // Use t_getprotaddr for XTI/ATM
//   struct t_bind *localaddr = (struct t_bind *) ACE_OS::t_alloc (get_handle (),
//                                                                 T_BIND,
//                                                                 T_ADDR);
//   struct t_bind *peeraddr = (struct t_bind *) ACE_OS::t_alloc (get_handle (),
//                                                                T_BIND,
//                                                                T_ADDR);
//   ::t_getprotaddr(get_handle (),
//                   localaddr,
//                   peeraddr);

//   char* connected_name = (char*) ACE_OS::malloc(peeraddr->addr.len + 1);
//   ACE_OS::strcpy(connected_name,
//                  peeraddr->addr.buf);
//   ACE_OS::t_free ((char *) localaddr,
//                   T_BIND);
//   ACE_OS::t_free ((char *) peeraddr,
//                   T_BIND);
//   return (connected_name);

#error "This doesn't seem to work. May need to jimmy-rig something with the"
#error "/etc/xti_hosts file - Ugh!"
  ACE_ATM_Addr sa;
  struct netbuf name;
  name.maxlen = sa.get_size ();
  name.buf = (char *) sa.get_addr ();
  ACE_OS::t_getname (this->get_handle (), &name, REMOTENAME);
  //  ACE_OS::ioctl (this->get_handle (),
  //               TI_GETPEERNAME,
  //               &name);
  return (name.buf);

#elif defined (ACE_HAS_FORE_ATM_WS2)
  // Use getpeername for WinSock2
  struct sockaddr name;
  socklen_t nameSize = sizeof(name);
  if (ACE_OS::getpeername(this->get_handle (),
                          &name,
                          &nameSize) != 0)
    {
      return 0;
    }
  return ??
#else
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

int
ACE_ATM_Stream::get_vpi_vci (ACE_UINT16 &vpi,
                             ACE_UINT16 &vci) const
{
  ACE_TRACE ("ACE_ATM_Stream::get_vpi_vci");
#if defined (ACE_HAS_FORE_ATM_XTI)
  struct t_atm_conn_prop conn_prop;
  char* connect_opts = (char *)&conn_prop;
  int opt_size = sizeof(t_atm_conn_prop);
  struct t_info info;
  struct t_optmgmt opt_req, opt_ret;

  if (ACE_OS::t_getinfo(stream_.get_handle(),
                        &info) < 0)
    {
      ACE_OS::t_error("t_getinfo");
      return -1;
    }

  char *buf_req = (char *) ACE_OS::malloc(info.options);
  if (buf_req == (char *) NULL)
    {
      ACE_OS::fprintf(stderr,
                      "Unable to allocate %ld bytes for options\n",
                      info.options);
      return -1;
    }

  char *buf_ret = (char *) ACE_OS::malloc(info.options);
  if (buf_ret == (char *) NULL)
    {
      ACE_OS::fprintf(stderr,
                      "Unable to allocate %ld bytes for options\n",
                      info.options);
      return -1;
    }

  ACE_OS::memset(&opt_req, 0, sizeof(opt_req));
  ACE_OS::memset(&opt_ret, 0, sizeof(opt_ret));

  struct t_opthdr *popt = (struct t_opthdr *) buf_req;
  struct t_opthdr *popt_ret = (struct t_opthdr *) buf_ret;

  popt->len= sizeof(struct t_opthdr) + opt_size;

  // We are only concerned with SVCs so no other check or values are needed
  //  here.
  popt->level = T_ATM_SIGNALING;
  popt->name = T_ATM_CONN_PROP;
  popt->status = 0;

  opt_req.opt.len = popt->len;
  opt_req.opt.buf = (char *)popt;
  opt_req.flags = T_CURRENT;

  popt = T_OPT_NEXTHDR(buf_req,
                       info.options,
                       popt);
  opt_ret.opt.maxlen  = info.options;
  opt_ret.opt.buf = (char *)popt_ret;

  if (ACE_OS::t_optmgmt(stream_.get_handle(),
                        &opt_req,
                        &opt_ret) < 0) {
    ACE_OS::t_error("t_optmgmt");
    return -1;
  }

  ACE_OS::memcpy(connect_opts,
                 (char *)popt_ret + sizeof(struct t_opthdr),
                 opt_size);

  ACE_OS::free(buf_ret);
  ACE_OS::free(buf_req);

  vpi = conn_prop.vpi;
  vci = conn_prop.vci;
  return (0);
#elif defined (ACE_HAS_FORE_ATM_WS2)
#else
  return (-1);
#endif /* ACE_HAS_FORE_ATM_XTI */
}

#endif /* ACE_HAS_ATM */
