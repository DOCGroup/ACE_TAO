// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//      UIOP_Acceptor.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu> and
//     Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#include "tao/UIOP_Acceptor.h"

#if TAO_HAS_UIOP == 1

#include "tao/UIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"


ACE_RCSID(tao, UIOP_Acceptor, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class TAO_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class TAO_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate TAO_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


TAO_UIOP_Acceptor::TAO_UIOP_Acceptor (CORBA::Boolean flag)
  : TAO_Acceptor (TAO_TAG_UIOP_PROFILE),
    base_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    unlink_on_close_ (1),
    lite_flag_ (flag)
{
}

TAO_UIOP_Acceptor::~TAO_UIOP_Acceptor (void)
{
  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

int
TAO_UIOP_Acceptor::create_mprofile (const TAO_ObjectKey &object_key,
                                    TAO_MProfile &mprofile)
{
  ACE_UNIX_Addr addr;

  if (this->base_acceptor_.acceptor ().get_local_addr (addr) == -1)
    return 0;

  // we only make one
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < 1
      && mprofile.grow (count + 1) == -1)
    return -1;

  TAO_UIOP_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_UIOP_Profile (addr,
                                    object_key,
                                    this->version_,
                                    this->orb_core_),
                  -1);

  if (mprofile.give_profile (pfile) == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
      return -1;
    }

  if (this->orb_core_->orb_params ()->std_profile_components () == 0)
    return 0;

  pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

  CONV_FRAME::CodeSetComponentInfo code_set_info;
  code_set_info.ForCharData.native_code_set  = TAO_DEFAULT_CHAR_CODESET_ID;
  code_set_info.ForWcharData.native_code_set = TAO_DEFAULT_WCHAR_CODESET_ID;
  pfile->tagged_components ().set_code_sets (code_set_info);

  pfile->tagged_components ().set_tao_priority (this->priority ());

  return 0;
}

int
TAO_UIOP_Acceptor::is_collocated (const TAO_Profile *pfile)
{
  const TAO_UIOP_Profile *profile =
    ACE_dynamic_cast (const TAO_UIOP_Profile *,
                      pfile);

  // Make sure the dynamically cast pointer is valid.
  if (profile == 0)
    return 0;

  // For UNIX Files this is relatively cheap.
  ACE_UNIX_Addr address;
  if (this->base_acceptor_.acceptor ().get_local_addr (address) == -1)
    return 0;

  return profile->object_addr () == address;
}

int
TAO_UIOP_Acceptor::close (void)
{
  ACE_UNIX_Addr addr;

  if (this->base_acceptor_.acceptor ().get_local_addr (addr) == -1)
    return -1;

  if (this->unlink_on_close_)
    (void) ACE_OS::unlink (addr.get_path_name ());

  return this->base_acceptor_.close ();
}

int
TAO_UIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         int major,
                         int minor,
                         const char *address,
                         const char *options)
{
  if (address == 0)
    return -1;

  if (major >= 0 && minor >= 0)
    this->version_.set_version (ACE_static_cast (CORBA::Octet,
                                                 major),
                                ACE_static_cast (CORBA::Octet,
                                                 minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;
  else
    return this->open_i (orb_core, address);
}

int
TAO_UIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                 const char *options)
{
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  ACE_Auto_String_Free tempname (ACE_OS::tempnam (0,
                                                  "TAO"));

  if (tempname.get () == 0)
    return -1;

  return this->open_i (orb_core,
                       tempname.get ());
}

int
TAO_UIOP_Acceptor::open_i (TAO_ORB_Core *orb_core,
                           const char *rendezvous)
{
  this->orb_core_ = orb_core;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_UIOP_CREATION_STRATEGY (this->orb_core_,
                                              this->lite_flag_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_UIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_UIOP_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  ACE_UNIX_Addr addr;

  this->rendezvous_point (addr, rendezvous);

  if (this->base_acceptor_.open (addr,
                                 this->orb_core_->reactor (this),
                                 this->creation_strategy_,
                                 this->accept_strategy_,
                                 this->concurrency_strategy_) == -1)
    {
      // Don't unlink an existing rendezvous point since it may be in
      // use by another UIOP server/client.
      if (errno == EADDRINUSE)
        this->unlink_on_close_ = 0;

      return -1;
    }

  // @@ If Profile creation is slow we may need to cache the
  //    rendezvous point here

  if (TAO_debug_level > 5)
    ACE_DEBUG ((LM_DEBUG,
                "\nTAO (%P|%t) UIOP_Acceptor::open_i - "
                "listening on: <%s>\n",
                addr.get_path_name ()));
  return 0;
}

void
TAO_UIOP_Acceptor::rendezvous_point (ACE_UNIX_Addr &addr,
                                     const char *rendezvous)
{
  // To guarantee portability, local IPC rendezvous points (including
  // the path and filename) should not be longer than 99 characters
  // long. Some platforms may support longer rendezvous points,
  // usually 108 characters including the null terminator, but
  // Posix.1g only requires that local IPC rendezvous point arrays
  // contain a maximum of at least 100 characters, including the null
  // terminator.  If an endpoint is longer than what the platform
  // supports then it will be truncated so that it fits, and a warning
  // will be issued.

  // Avoid using relative paths in your UIOP endpoints.  If possible,
  // use absolute paths instead.  Imagine that the server is given an
  // endpoint to create using -ORBEndpoint uiop://foobar.  A local IPC
  // rendezvous point called foobar will be created in the current
  // working directory.  If the client is not started in the directory
  // where the foobar rendezvous point exists then the client will not
  // be able to communicate with the server since its point of
  // communication, the rendezvous point, was not found. On the other
  // hand, if an absolute path was used, the client would know exactly
  // where to find the rendezvous point.  It is up to the user to make
  // sure that a given UIOP endpoint is accessible by both the server
  // and the client.

  addr.set (rendezvous);

  size_t length = ACE_OS::strlen (addr.get_path_name ());

  // Check if rendezvous point was truncated by ACE_UNIX_Addr since
  // most UNIX domain socket rendezvous points can only be less than
  // 108 characters long.
  if (length < ACE_OS::strlen (rendezvous))
    ACE_DEBUG ((LM_WARNING,
                "TAO (%P|%t) UIOP rendezvous point was truncated to <%s>\n"
                "since it was longer than %d characters long.\n",
                addr.get_path_name (),
                length));
}

CORBA::ULong
TAO_UIOP_Acceptor::endpoint_count (void)
{
  return 1;
}

int
TAO_UIOP_Acceptor::parse_options (const char *str)
{
  if (str == 0)
    return 0;  // No options to parse.  Not a problem.

  // Use an option format similar to the one used for CGI scripts in
  // HTTP URLs.
  // e.g.:  option1=foo&option2=bar

  ACE_CString options (str);

  size_t len = options.length ();

  const char option_delimiter = '&';

  // Count the number of options.

  CORBA::ULong option_count = 1;
  // Number of endpoints in the string (initialized to 1).

  // Only check for endpoints after the protocol specification and
  // before the object key.
  for (size_t i = 0; i < len; ++i)
    if (options[i] == option_delimiter)
      option_count++;

  // The idea behind the following loop is to split the options into
  // (option, name) pairs.
  // For example,
  //    `option1=foo&option2=bar'
  // will be parsed into:
  //    `option1=foo'
  //    `option2=bar'

  int begin = 0;
  int end = -1;

  for (CORBA::ULong j = 0; j < option_count; ++j)
    {
      begin += end + 1;

      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = len - begin;  // Handle last endpoint differently

      if (end == begin)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO (%P|%t) Zero length UIOP option.\n"),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt =
            options.substring (begin, end);

          int slot = opt.find ("=");

          if (slot == ACE_static_cast (int, len - 1)
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "TAO (%P|%t) UIOP option <%s> is "
                               "missing a value.\n",
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "TAO (%P|%t) Zero length UIOP "
                               "option name.\n"),
                              -1);

          if (name == "priority")
            {
              CORBA::Short corba_priority =
                ACE_static_cast (CORBA::Short,
                                 ACE_OS::atoi (value.c_str ()));

              if (corba_priority >= 0
                  /* && corba_priority < 32768 */)
                // priority_ and corba_priority will always be less
                // than 32768 since CORBA::Short is a signed 16 bit
                // integer.
                this->priority_ = corba_priority;
              else
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "TAO (%P|%t) Invalid UIOP endpoint "
                                   "priority: <%s>\n",
                                   value.c_str ()),
                                  -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "TAO (%P|%t) Invalid UIOP option: <%s>\n",
                               name.c_str ()),
                              -1);
        }
    }
  return 0;
}
#endif  /* TAO_HAS_UIOP == 1 */
