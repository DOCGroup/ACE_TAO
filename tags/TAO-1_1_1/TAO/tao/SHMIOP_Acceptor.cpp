// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/SHMIOP_Acceptor.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/SHMIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "tao/SHMIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, SHMIOP_Acceptor, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_SHMIOP_Server_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_SHMIOP_Server_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_SHMIOP_Server_Connection_Handler>;
template class TAO_Creation_Strategy<TAO_SHMIOP_Server_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO_SHMIOP_Server_Connection_Handler>;
template class TAO_Accept_Strategy<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_SHMIOP_Server_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_SHMIOP_Server_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_SHMIOP_Server_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO_SHMIOP_Server_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO_SHMIOP_Server_Connection_Handler>
#pragma instantiate TAO_Accept_Strategy<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_SHMIOP_Acceptor::TAO_SHMIOP_Acceptor (CORBA::Boolean flag)
  : TAO_Acceptor (TAO_TAG_SHMEM_PROFILE),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    base_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0),
    mmap_file_prefix_ (0),
    mmap_size_ (1024 * 1024),
    lite_flag_ (flag)
{
}

TAO_SHMIOP_Acceptor::~TAO_SHMIOP_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

// TODO =
//    2) For V1.[1,2] there are tagged components
//    3) Create multiple profiles for wild carded endpoints (may be multiple
//       interfaces over which we can receive requests.  Thus a profile
//       must be made for each one.

int
TAO_SHMIOP_Acceptor::create_mprofile (const TAO_ObjectKey &object_key,
                                      TAO_MProfile &mprofile)
{
  // @@ we only make one for now
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < 1
      && mprofile.grow (count + 1) == -1)
    return -1;

  TAO_SHMIOP_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_SHMIOP_Profile (this->host_.c_str (),
                                      this->address_.get_port_number (),
                                      object_key,
                                      this->address_.get_remote_addr (),
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
  code_set_info.ForCharData.native_code_set  =
    TAO_DEFAULT_CHAR_CODESET_ID;
  code_set_info.ForWcharData.native_code_set =
    TAO_DEFAULT_WCHAR_CODESET_ID;
  pfile->tagged_components ().set_code_sets (code_set_info);

  pfile->tagged_components ().set_tao_priority (this->priority ());

  return 0;
}

int
TAO_SHMIOP_Acceptor::is_collocated (const TAO_Profile *pfile)
{
  const TAO_SHMIOP_Profile *profile =
    ACE_dynamic_cast(const TAO_SHMIOP_Profile *,
                     pfile);

  // Make sure the dynamically cast pointer is valid.
  if (profile == 0)
    return 0;

  // compare the port and sin_addr (numeric host address)
  return this->address_.same_host (profile->object_addr ());
}

int
TAO_SHMIOP_Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}

int
TAO_SHMIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                           int major,
                           int minor,
                           const char *port,
                           const char *options)
{
  if (major >=0 && minor >= 0)
    this->version_.set_version (ACE_static_cast (CORBA::Octet,
                                                 major),
                                ACE_static_cast (CORBA::Octet,
                                                 minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  if (port)
    this->address_.set (port);

  return this->open_i (orb_core);
}

int
TAO_SHMIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                   const char *options)
{
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  // @@ Until we can support multihomed machines correctly we must
  //    pick the "default interface" and only listen on that IP
  //    address.

  this->host_ = this->address_.get_host_name ();

  return this->open_i (orb_core);
}

int
TAO_SHMIOP_Acceptor::open_i (TAO_ORB_Core* orb_core)
{
  this->orb_core_ = orb_core;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_SHMIOP_CREATION_STRATEGY (this->orb_core_,
                                                this->lite_flag_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_SHMIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_SHMIOP_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  // We only accept connection on localhost.
  //  ACE_INET_Addr local_addr (addr.get_port_number (), ASYS_TEXT ("localhost"));
  if (this->base_acceptor_.open (this->address_,
                                 this->orb_core_->reactor (this),
                                 this->creation_strategy_,
                                 this->accept_strategy_,
                                 this->concurrency_strategy_) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("\n\nTAO (%P|%t) SHMIOP_Acceptor::open_i - %p\n\n"),
                    ASYS_TEXT ("cannot open acceptor")));
      return -1;
    }

  this->base_acceptor_.acceptor().malloc_options ().minimum_bytes_
    = this->mmap_size_;

  // @@ Should this be a catastrophic error???
  if (this->base_acceptor_.acceptor ().get_local_addr (this->address_) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("\n\nTAO (%P|%t) SHMIOP_Acceptor::open_i - %p\n\n"),
                    ASYS_TEXT ("cannot get local addr")));
      return -1;
    }

  // This will be the actualy host name of the original endpoint.
  char tmp_host[MAXHOSTNAMELEN+1];
  if (this->address_.get_host_name (tmp_host,
                                    sizeof tmp_host) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("\n\nTAO (%P|%t) SHMIOP_Acceptor::open_i - %p\n\n"),
                    ASYS_TEXT ("cannot cache hostname")));
      return -1;
    }
  this->host_ = tmp_host;

  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("\nTAO (%P|%t) SHMIOP_Acceptor::open_i - ")
                  ASYS_TEXT ("listening on %s using: <localhost:%u>\n"),
                  this->host_.c_str (),
                  this->address_.get_port_number ()));
    }
  return 0;
}

CORBA::ULong
TAO_SHMIOP_Acceptor::endpoint_count (void)
{
  // @@ for now just assume one!
  // we should take a look at the local address, if it is zero then
  // get the list of available IP interfaces and return this number.
  return 1;
}

int
TAO_SHMIOP_Acceptor::parse_options (const char *str)
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
  // Number of endpoints in the string  (initialized to 1).

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
                           ASYS_TEXT ("TAO (%P|%t) Zero length SHMIOP option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          int slot = opt.find ("=");

          if (slot == ACE_static_cast (int, len - 1)
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("TAO (%P|%t) SHMIOP option <%s> is ")
                               ASYS_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("TAO (%P|%t) Zero length SHMIOP ")
                               ASYS_TEXT ("option name.\n")),
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
                                   ASYS_TEXT ("TAO (%P|%t) Invalid SHMIOP endpoint ")
                                   ASYS_TEXT ("priority: <%s>\n"),
                                   value.c_str ()),
                                  -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("TAO (%P|%t) Invalid SHMIOP option: <%s>\n"),
                               name.c_str ()),
                              -1);
        }
    }
  return 0;
}

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
