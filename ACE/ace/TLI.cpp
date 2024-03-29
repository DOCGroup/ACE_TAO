// Defines the member functions for the base class of the ACE_TLI
// abstraction.
#include "ace/TLI.h"
#include "ace/Log_Category.h"
#include "ace/OS_Memory.h"
#include "ace/OS_TLI.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_socket.h"
#include <memory>

#if defined (ACE_HAS_TLI)

#if !defined (__ACE_INLINE__)
#include "ace/TLI.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_TLI)

void
ACE_TLI::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_TLI::dump");
#endif /* ACE_HAS_DUMP */
}

ACE_TLI::ACE_TLI ()
{
  ACE_TRACE ("ACE_TLI::ACE_TLI");
}

ACE_HANDLE
ACE_TLI::open (const char device[], int oflag, struct t_info *info)
{
  ACE_TRACE ("ACE_TLI::open");
  if (oflag == 0)
    oflag = O_RDWR;
  this->set_handle (ACE_OS::t_open ((char *) device, oflag, info));

  return this->get_handle ();
}

ACE_TLI::~ACE_TLI ()
{
  ACE_TRACE ("ACE_TLI::~ACE_TLI");
}

ACE_TLI::ACE_TLI (const char device[], int oflag, struct t_info *info)
{
  ACE_TRACE ("ACE_TLI::ACE_TLI");
  if (this->open (device, oflag, info) == ACE_INVALID_HANDLE)
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_TLI::ACE_TLI")));
}

int
ACE_TLI::get_local_addr (ACE_Addr &sa) const
{
  ACE_TRACE ("ACE_TLI::get_local_addr");
  struct netbuf name;

  name.maxlen = sa.get_size ();
  name.buf    = (char *) sa.get_addr ();

  if (ACE_OS::t_getname (this->get_handle (), &name, LOCALNAME) == -1)
    return -1;
  else
    return 0;
}

int
ACE_TLI::close ()
{
  ACE_TRACE ("ACE_TLI::close");
  int result = 0; // Geisler: result must be int

  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      result = ACE_OS::t_close (this->get_handle ());
      this->set_handle (ACE_INVALID_HANDLE);
    }
  return result;
}

int
ACE_TLI::set_option (int level, int option, void *optval, int optlen)
{
  /* Set up options for ACE_TLI */
  ACE_TRACE ("ACE_TLI::set_option");

#if defined (ACE_HAS_XTI)
  // ret will get the negotiated option back after attempting to set it.
  // Assume this will fit in the requested size.
  struct t_optmgmt req, ret;
  ACE_NEW_RETURN (req.opt.buf, char[sizeof (struct t_opthdr) + optlen], -1);
#  if (_XOPEN_SOURCE - 0 >= 500)
  std::unique_ptr<char> req_opt_buf_p (reinterpret_cast<char*> (req.opt.buf));
#  else
  std::unique_ptr<char[]> req_opt_buf_p (req.opt.buf);
#  endif /* XPG5 vs XPG4 */
  struct t_opthdr *opthdr =
    reinterpret_cast<struct t_opthdr *> (req.opt.buf);
  ACE_NEW_RETURN (ret.opt.buf, char[sizeof (struct t_opthdr) + optlen], -1);
#  if (_XOPEN_SOURCE - 0 >= 500)
  std::unique_ptr<char> ret_opt_buf_p (reinterpret_cast<char*> (ret.opt.buf));
#  else
  std::unique_ptr<char[]> ret_opt_buf_p (ret.opt.buf);
#  endif /* XPG5 vs XPG4 */

  req.flags = T_NEGOTIATE;
  req.opt.len = sizeof *opthdr + optlen;
  ret.opt.maxlen = req.opt.len;
  opthdr->level = level;
  opthdr->name  = option;
  opthdr->len   = req.opt.len;   // We only request one option at a time.
  ACE_OS::memcpy (&opthdr[1], optval, optlen);
  return ACE_OS::t_optmgmt (this->get_handle (), &req, &ret);
#else
  ACE_UNUSED_ARG (level);
  ACE_UNUSED_ARG (option);
  ACE_UNUSED_ARG (optval);
  ACE_UNUSED_ARG (optlen);
  return -1;
#endif /* ACE_HAS_XTI */
}

int
ACE_TLI::get_option (int level, int option, void *optval, int &optlen)
{
  ACE_TRACE ("ACE_TLI::get_option");
#if defined (ACE_HAS_XTI)
  // ret will get the option requested in req.
  struct t_optmgmt req, ret;
  ACE_NEW_RETURN (req.opt.buf, char[sizeof (struct t_opthdr)], -1);
#  if (_XOPEN_SOURCE - 0 >= 500)
  std::unique_ptr<char> req_opt_buf_p (reinterpret_cast<char*> (req.opt.buf));
#  else
  std::unique_ptr<char[]> req_opt_buf_p (req.opt.buf);
#  endif /* XPG5 vs XPG4 */
  struct t_opthdr *opthdr =
    reinterpret_cast<struct t_opthdr *> (req.opt.buf);
  ACE_NEW_RETURN (ret.opt.buf, char[sizeof (struct t_opthdr) + optlen], -1);
#  if (_XOPEN_SOURCE - 0 >= 500)
  std::unique_ptr<char> ret_opt_buf_p (reinterpret_cast<char*> (ret.opt.buf));
#  else
  std::unique_ptr<char[]> ret_opt_buf_p (ret.opt.buf);
#  endif /* XPG5 vs XPG4 */

  req.flags = T_CURRENT;
  req.opt.len = sizeof *opthdr;
  ret.opt.maxlen = sizeof (struct t_opthdr) + optlen;
  opthdr->level = level;
  opthdr->name  = option;
  opthdr->len   = sizeof (*opthdr);    // Just the header on the request
  if (ACE_OS::t_optmgmt (this->get_handle (), &req, &ret) == -1)
    return -1;
  else
    {
      opthdr = reinterpret_cast<struct t_opthdr *> (ret.opt.buf);
      if (opthdr->status == T_NOTSUPPORT)
        {
          errno = ENOTSUP;
          return -1;
        }
      else
        {
          ACE_OS::memcpy (optval, &opthdr[1], optlen);
          return 0;
        }
    }
#else
  ACE_UNUSED_ARG (level);
  ACE_UNUSED_ARG (option);
  ACE_UNUSED_ARG (optval);
  ACE_UNUSED_ARG (optlen);
  return -1;
#endif /* ACE_HAS_XTI */
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_TLI */
