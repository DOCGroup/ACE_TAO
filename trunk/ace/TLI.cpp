// TLI.cpp
// $Id$

/* Defines the member functions for the base class of the ACE_TLI
   abstraction. */ 

#define ACE_BUILD_DLL
#include "ace/TLI.h"
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_TLI)

ACE_ALLOC_HOOK_DEFINE(ACE_TLI)

void
ACE_TLI::dump (void) const
{
  ACE_TRACE ("ACE_TLI::dump");
}

ACE_TLI::ACE_TLI (void)
{
  ACE_TRACE ("ACE_TLI::ACE_TLI");
#if defined (ACE_HAS_SVR4_TLI)
// Solaris 2.4 ACE_TLI option handling is broken.  Thus, we must do
// the memory allocation ourselves...  Thanks to John P. Hearn
// (jph@ccrl.nj.nec.com) for the help.

  this->so_opt_req.opt.maxlen = sizeof (opthdr) + sizeof (long);
  ACE_NEW (this->so_opt_req.opt.buf, char[this->so_opt_req.opt.maxlen]);

  this->so_opt_ret.opt.maxlen = sizeof (opthdr) + sizeof (long);
  this->so_opt_ret.opt.buf = new char[this->so_opt_ret.opt.maxlen];

  if (this->so_opt_ret.opt.buf == 0)
    {
      delete [] this->so_opt_req.opt.buf;
      this->so_opt_req.opt.buf = 0;
      return;
    }
#endif /* ACE_HAS_SVR4_TLI */
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

ACE_TLI::~ACE_TLI (void)
{
  ACE_TRACE ("ACE_TLI::~ACE_TLI");
#if defined (ACE_HAS_SVR4_TLI)
  if (this->so_opt_req.opt.buf)
    {
      delete [] this->so_opt_req.opt.buf;
      delete [] this->so_opt_ret.opt.buf;
      this->so_opt_req.opt.buf = 0;
      this->so_opt_ret.opt.buf = 0;
    }
#endif /* ACE_HAS_SVR4_TLI */
}

ACE_TLI::ACE_TLI (const char device[], int oflag, struct t_info *info)
{
  ACE_TRACE ("ACE_TLI::ACE_TLI");
  if (this->open (device, oflag, info) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_TLI::ACE_TLI"));
}

int
ACE_TLI::get_local_addr (ACE_Addr &sa) const
{
  ACE_TRACE ("ACE_TLI::get_local_addr");
#if defined (ACE_HAS_SVR4_TLI)
  struct netbuf name;

  name.maxlen = sa.get_size ();
  name.buf    = (char *) sa.get_addr ();
  
  if (ACE_OS::ioctl (this->get_handle (), TI_GETMYNAME, &name) == -1) 
/*  if (ACE_OS::t_getname (this->get_handle (), &name, LOCALNAME) == -1) */
#else /* SunOS4 sucks... */
  if (0)
#endif /* ACE_HAS_SVR4_TLI */
    return -1;
  else
    return 0;
}

int 
ACE_TLI::close (void)
{
  ACE_TRACE ("ACE_TLI::close");
  ACE_HANDLE result = 0;

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
  ACE_TRACE ("ACE_TLI::set_option");
#if defined (ACE_HAS_SVR4_TLI)
  /* Set up options for ACE_TLI */

  struct opthdr *opthdr = 0; /* See <sys/socket.h> for details on this format */

  this->so_opt_req.flags = T_NEGOTIATE;
  this->so_opt_req.opt.len = sizeof *opthdr + OPTLEN (optlen);

  if (this->so_opt_req.opt.len > this->so_opt_req.opt.maxlen)
    {
      t_errno = TBUFOVFLW;
      return -1;
    }

  opthdr        = (struct opthdr *) this->so_opt_req.opt.buf;
  opthdr->level = level;
  opthdr->name  = option;
  opthdr->len   = OPTLEN (optlen);
  ACE_OS::memcpy (OPTVAL (opthdr), optval, optlen);

  return ACE_OS::t_optmgmt (this->get_handle (), &this->so_opt_req, &this->so_opt_ret);
#else
  return -1;
#endif /* ACE_HAS_SVR4_TLI */
}

int 
ACE_TLI::get_option (int level, int option, void *optval, int &optlen)
{
  ACE_TRACE ("ACE_TLI::get_option");
#if defined (ACE_HAS_SVR4_TLI)
  struct opthdr *opthdr = 0; /* See <sys/socket.h> for details on this format */

  this->so_opt_req.flags = T_CHECK;
  this->so_opt_ret.opt.len = sizeof *opthdr + OPTLEN (optlen);

  if (this->so_opt_ret.opt.len > this->so_opt_ret.opt.maxlen)
    {
      t_errno = TBUFOVFLW;
      return -1;
    }

  opthdr        = (struct opthdr *) this->so_opt_req.opt.buf;
  opthdr->level = level;
  opthdr->name  = option;
  opthdr->len   = OPTLEN (optlen);
  if (ACE_OS::t_optmgmt (this->get_handle (), &this->so_opt_req, &this->so_opt_ret) == -1)
    return -1;
  else
    {
      ACE_OS::memcpy (optval, OPTVAL (opthdr), optlen);
      return 0;
    }
#else
  return -1;
#endif /* ACE_HAS_SVR4_TLI */
}

#endif /* ACE_HAS_TLI */
