// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "SHMIOP_Acceptor.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "SHMIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "SHMIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Strategies, SHMIOP_Acceptor, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_SHMIOP_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_SHMIOP_Connection_Handler>;
template class TAO_Creation_Strategy<TAO_SHMIOP_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>;
template class TAO_Accept_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate TAO_Accept_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>

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
TAO_SHMIOP_Acceptor::create_profile (const TAO_ObjectKey &object_key,
                                     TAO_MProfile &mprofile,
                                     CORBA::Short priority)
{
  // Check if multiple endpoints should be put in one profile or
  // if they should be spread across multiple profiles.
  if (priority == TAO_INVALID_PRIORITY)
    return this->create_new_profile (object_key,
                                     mprofile,
                                     priority);
  else
    return this->create_shared_profile (object_key,
                                        mprofile,
                                        priority);
}

int
TAO_SHMIOP_Acceptor::create_new_profile (const TAO_ObjectKey &object_key,
                                         TAO_MProfile &mprofile,
                                         CORBA::Short priority)
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
  pfile->endpoint ()->priority (priority);

  if (mprofile.give_profile (pfile) == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
      return -1;
    }

  // Do not add any tagged components to the profile if configured
  // by the user not to do so, or if an SHMIOP 1.0 endpoint is being
  // created (IIOP 1.0 did not support tagged components, so we follow
  // the same convention for SHMIOP).
  if (this->orb_core_->orb_params ()->std_profile_components () == 0
      || (this->version_.major == 1 && this->version_.minor == 0))
    return 0;

  pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

  CONV_FRAME::CodeSetComponentInfo code_set_info;
  code_set_info.ForCharData.native_code_set  =
    TAO_DEFAULT_CHAR_CODESET_ID;
  code_set_info.ForWcharData.native_code_set =
    TAO_DEFAULT_WCHAR_CODESET_ID;
  pfile->tagged_components ().set_code_sets (code_set_info);

  return 0;
}

int
TAO_SHMIOP_Acceptor::create_shared_profile (const TAO_ObjectKey &object_key,
                                            TAO_MProfile &mprofile,
                                            CORBA::Short priority)
{
  TAO_Profile *pfile = 0;
  TAO_SHMIOP_Profile *shmiop_profile = 0;

  // First see if <mprofile> already contains a SHMIOP profile.
  for (TAO_PHandle i = 0; i != mprofile.profile_count (); ++i)
    {
      pfile = mprofile.get_profile (i);
      if (pfile->tag () == TAO_TAG_SHMEM_PROFILE)
      {
        shmiop_profile = ACE_dynamic_cast (TAO_SHMIOP_Profile *,
                                           pfile);
        break;
      }
    }

  if (shmiop_profile == 0)
    {
      // If <mprofile> doesn't contain SHMIOP_Profile, we need to create
      // one.
      return create_new_profile (object_key,
                                 mprofile,
                                 priority);
    }
  else
    {
      // A SHMIOP_Profile already exists - just add our endpoint to it.

      TAO_SHMIOP_Endpoint *endpoint = 0;
      ACE_NEW_RETURN (endpoint,
                      TAO_SHMIOP_Endpoint (this->host_.c_str (),
                                           this->address_.get_port_number (),
                                           this->address_.get_remote_addr ()),
                      -1);
      endpoint->priority (priority);
      shmiop_profile->add_endpoint (endpoint);

      return 0;
    }
}

int
TAO_SHMIOP_Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO_SHMIOP_Endpoint *endp =
    ACE_dynamic_cast(const TAO_SHMIOP_Endpoint *, endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return 0;

  // compare the port and sin_addr (numeric host address)
  return this->address_.same_host (endp->object_addr ());
}

int
TAO_SHMIOP_Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}

int
TAO_SHMIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                           ACE_Reactor *reactor,
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

  if (isdigit (*port) == 0)
    return -1;                  // Port number must consist of digits

  if (port)
    this->address_.set (port);

  return this->open_i (orb_core,
                       reactor);
}

int
TAO_SHMIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                   ACE_Reactor *reactor,
                                   int major,
                                   int minor,
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

  // @@ Until we can support multihomed machines correctly we must
  //    pick the "default interface" and only listen on that IP
  //    address.

  this->host_ = this->address_.get_host_name ();

  return this->open_i (orb_core,
                       reactor);
}

int
TAO_SHMIOP_Acceptor::set_mmap_options (const ACE_TCHAR *prefix,
                                       off_t size)
{
  this->mmap_file_prefix_ = prefix;
  this->mmap_size_ = size;

  return 0;
}

int
TAO_SHMIOP_Acceptor::open_i (TAO_ORB_Core* orb_core,
                             ACE_Reactor *reactor)
{
  this->orb_core_ = orb_core;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_SHMIOP_CREATION_STRATEGY (this->orb_core_,
                                                0,
                                                this->lite_flag_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_SHMIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_SHMIOP_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  // We only accept connection on localhost.
  //  ACE_INET_Addr local_addr (addr.get_port_number (), ACE_TEXT ("localhost"));
  if (this->base_acceptor_.open (this->address_,
                                 reactor,
                                 this->creation_strategy_,
                                 this->accept_strategy_,
                                 this->concurrency_strategy_) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) SHMIOP_Acceptor::open_i - %p\n\n"),
                    ACE_TEXT ("cannot open acceptor")));
      return -1;
    }

  this->base_acceptor_.acceptor().mmap_prefix (this->mmap_file_prefix_);
  this->base_acceptor_.acceptor().malloc_options ().minimum_bytes_
    = this->mmap_size_;

  if (orb_core->server_factory ()->activate_server_connections () != 0)
    this->base_acceptor_.acceptor().preferred_strategy (ACE_MEM_IO::MT);

  // @@ Should this be a catastrophic error???
  if (this->base_acceptor_.acceptor ().get_local_addr (this->address_) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) SHMIOP_Acceptor::open_i - %p\n\n"),
                    ACE_TEXT ("cannot get local addr")));
      return -1;
    }

  // This will be the actualy host name of the original endpoint.
  char tmp_host[MAXHOSTNAMELEN+1];
  if (this->address_.get_host_name (tmp_host,
                                    sizeof tmp_host) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) SHMIOP_Acceptor::open_i - %p\n\n"),
                    ACE_TEXT ("cannot cache hostname")));
      return -1;
    }
  this->host_ = tmp_host;

  (void) this->base_acceptor_.acceptor().enable (ACE_CLOEXEC);
  // This avoids having child processes acquire the listen socket thereby
  // denying the server the opportunity to restart on a well-known endpoint.
  // This does not affect the aberrent behavior on Win32 platforms.

  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nTAO (%P|%t) SHMIOP_Acceptor::open_i - ")
                  ACE_TEXT ("listening on %s using: <localhost:%u>\n"),
                  this->host_.c_str (),
                  this->address_.get_port_number ()));
    }
  return 0;
}


int
TAO_SHMIOP_Acceptor::object_key (IOP::TaggedProfile &profile,
                                 TAO_ObjectKey &object_key)
{
  // Create the decoding stream from the encapsulation in the buffer,
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  TAO_InputCDR cdr (profile.profile_data.mb ());
#else
  TAO_InputCDR cdr (ACE_reinterpret_cast(char*,profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  CORBA::Octet major, minor;

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major)
        && cdr.read_octet (minor)))
  {
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) SHMIOP_Profile::decode - v%d.%d\n"),
                    major,
                    minor));
      }
    return -1;
  }

  CORBA::String_var host;
  CORBA::UShort port = 0;

  // Get host and port. No processing here too..
  if (cdr.read_string (host.out ()) == 0
      || cdr.read_ushort (port) == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) TAO_Tagged_Profile::decode - ")
                      ACE_TEXT ("error while decoding host/port")));
        }
      return -1;
    }

  // ... and object key.
  if ((cdr >> object_key) == 0)
    return -1;

  // We are NOT bothered about the rest.

  return 1;
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
                           ACE_TEXT ("TAO (%P|%t) Zero length SHMIOP option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          int slot = opt.find ("=");

          if (slot == ACE_static_cast (int, len - 1)
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) SHMIOP option <%s> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Zero length SHMIOP ")
                               ACE_TEXT ("option name.\n")),
                              -1);

          if (name == "priority")
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid SHMIOP endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported. \n"),
                                 value.c_str ()),
                                -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid SHMIOP option: <%s>\n"),
                               name.c_str ()),
                              -1);
        }
    }
  return 0;
}

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
