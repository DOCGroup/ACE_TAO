// $Id$

#include "tao/Strategies/SHMIOP_Acceptor.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Strategies/SHMIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"
#include "tao/Codeset_Manager.h"
#include "tao/CDR.h"

#include "ace/os_include/os_netdb.h"
#include "ace/OS_NS_ctype.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_SHMIOP_Acceptor::TAO_SHMIOP_Acceptor (void)
  : TAO_Acceptor (TAO_TAG_SHMEM_PROFILE),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    base_acceptor_ (this),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0),
    mmap_file_prefix_ (0),
    mmap_size_ (1024 * 1024)
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
TAO_SHMIOP_Acceptor::create_profile (const TAO::ObjectKey &object_key,
                                     TAO_MProfile &mprofile,
                                     CORBA::Short priority)
{
  // Check if multiple endpoints should be put in one profile or
  // if they should be spread across multiple profiles.
  if (priority == TAO_INVALID_PRIORITY)
    return this->create_new_profile (object_key, mprofile, priority);
  else
    return this->create_shared_profile (object_key, mprofile, priority);
}

int
TAO_SHMIOP_Acceptor::create_new_profile (const TAO::ObjectKey &object_key,
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
  TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
  if (csm)
    csm->set_codeset(pfile->tagged_components());
  return 0;
}

int
TAO_SHMIOP_Acceptor::create_shared_profile (const TAO::ObjectKey &object_key,
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
        shmiop_profile = dynamic_cast <TAO_SHMIOP_Profile *>(pfile);
        break;
      }
    }

  if (shmiop_profile == 0)
    {
      // If <mprofile> doesn't contain SHMIOP_Profile, we need to create
      // one.
      return create_new_profile (object_key, mprofile, priority);
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
    dynamic_cast <const TAO_SHMIOP_Endpoint *> (endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return 0;

      // @@ TODO The following code looks funky, why only the host
      //    name is compared?  What if there are multiple SHMIOP
      //    servers in the same address?  Why do SHMIOP_Endpoints keep
      //    a INET_Addr but not a MEM_Addr?  And why is there no lazy
      //    evaluation of IP-addresses for SHMIOP endpoints?  Is it
      //    because it is always 'localhost'?  We need answers to
      //    these questions to solve:
      //
      // http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=1220
      //
  // The following code is suspec
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
    this->version_.set_version (static_cast <CORBA::Octet>(major),
                                static_cast <CORBA::Octet>(minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  if (port)
    {
      if (ACE_OS::ace_isdigit (*port) == 0)
        return -1;                  // Port number must consist of digits

      this->address_.set (ACE_TEXT_CHAR_TO_TCHAR(port));
    }

  return this->open_i (orb_core, reactor);
}

int
TAO_SHMIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                   ACE_Reactor *reactor,
                                   int major,
                                   int minor,
                                   const char *options)
{
  if (major >=0 && minor >= 0)
    this->version_.set_version (static_cast <CORBA::Octet>(major),
                                static_cast <CORBA::Octet>(minor));

  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  // @@ Until we can support multihomed machines correctly we must
  //    pick the "default interface" and only listen on that IP
  //    address.
  this->host_ = this->address_.get_host_name ();

  return this->open_i (orb_core, reactor);
}

int
TAO_SHMIOP_Acceptor::set_mmap_options (const ACE_TCHAR *prefix, ACE_OFF_T size)
{
  this->mmap_file_prefix_ = prefix;
  this->mmap_size_ = size;

  return 0;
}

int
TAO_SHMIOP_Acceptor::open_i (TAO_ORB_Core* orb_core, ACE_Reactor *reactor)
{
  this->orb_core_ = orb_core;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_SHMIOP_CREATION_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_SHMIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_SHMIOP_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  // We only accept connection on localhost.
  if (this->base_acceptor_.open (this->address_,
                                 reactor,
                                 this->creation_strategy_,
                                 this->accept_strategy_,
                                 this->concurrency_strategy_) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - SHMIOP_Acceptor::open_i, %p\n\n"),
                    ACE_TEXT ("cannot open acceptor")));
      return -1;
    }

  this->base_acceptor_.acceptor().mmap_prefix (this->mmap_file_prefix_);
  this->base_acceptor_.acceptor().init_buffer_size (this->mmap_size_);

  if (orb_core->server_factory ()->activate_server_connections () != 0)
    this->base_acceptor_.acceptor().preferred_strategy (ACE_MEM_IO::MT);

  // @@ Should this be a catastrophic error???
  if (this->base_acceptor_.acceptor ().get_local_addr (this->address_) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - SHMIOP_Acceptor::open_i, %p\n\n"),
                    ACE_TEXT ("cannot get local addr\n")));
      return -1;
    }

  // If the ORB is instructed to use dotted decimal addresses, we respect that
  // also for shmiop
  if (orb_core->orb_params ()->use_dotted_decimal_addresses ())
    {
      // Get the ip address, we get the remote addr to put in the IOR, don't
      // calls get_host_addr() directly on address_, we then get the internal
      // address back
      const char *tmp = this->address_.get_remote_addr().get_host_addr ();

      if (tmp == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - ")
                        ACE_TEXT ("SHMIOP_Acceptor::open_i, ")
                        ACE_TEXT ("- %p, "),
                        ACE_TEXT ("cannot determine hostname\n")));
          return -1;
        }

      this->host_ = tmp;
    }
  else
    {
      // This will be the actualy host name of the original endpoint.
      ACE_TCHAR tmp_host[MAXHOSTNAMELEN+1];

      if (this->address_.get_host_name (tmp_host, sizeof tmp_host) != 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - SHMIOP_Acceptor::open_i, - %p\n"),
                        ACE_TEXT ("cannot cache hostname\n")));
          return -1;
        }
      this->host_ = ACE_TEXT_ALWAYS_CHAR(tmp_host);
    }

  // This avoids having child processes acquire the listen socket thereby
  // denying the server the opportunity to restart on a well-known endpoint.
  // This does not affect the aberrent behavior on Win32 platforms.
  (void) this->base_acceptor_.acceptor().enable (ACE_CLOEXEC);

  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - SHMIOP_Acceptor::open_i, ")
                  ACE_TEXT ("listening on : <%C:%u>\n"),
                  this->host_.c_str (),
                  this->address_.get_port_number ()));
    }

  // In the event that an accept() fails, we can examine the reason.  If
  // the reason warrants it, we can try accepting again at a later time.
  // The amount of time we wait to accept again is governed by this orb
  // parameter.
  this->set_error_retry_delay (
    this->orb_core_->orb_params ()->accept_error_delay());

  return 0;
}


int
TAO_SHMIOP_Acceptor::object_key (IOP::TaggedProfile &profile,
                                 TAO::ObjectKey &object_key)
{
  // Create the decoding stream from the encapsulation in the buffer,
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  TAO_InputCDR cdr (profile.profile_data.mb ());
#else
  TAO_InputCDR cdr (reinterpret_cast<char*>(profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  CORBA::Octet major = 0;
  CORBA::Octet minor = 0;

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major) && cdr.read_octet (minor)))
  {
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - SHMIOP_Profile::decode, v%d.%d\n"),
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
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - SHMIOP_Acceptor::object_key, ")
                      ACE_TEXT ("error while decoding host/port\n")));
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
      ++option_count;

  // The idea behind the following loop is to split the options into
  // (option, name) pairs.
  // For example,
  //    `option1=foo&option2=bar'
  // will be parsed into:
  //    `option1=foo'
  //    `option2=bar'

  ACE_CString::size_type begin = 0;
  ACE_CString::size_type end = 0;

  for (CORBA::ULong j = 0; j < option_count; ++j)
    {
      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = len;

      if (end == begin)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) Zero length SHMIOP option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end - begin);

          ACE_CString::size_type const slot = opt.find ("=");

          if (slot == len - 1
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) SHMIOP option <%C> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          begin = end + 1;

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Zero length SHMIOP ")
                               ACE_TEXT ("option name.\n")),
                              -1);

          if (name == "priority")
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid SHMIOP endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported.\n")),
                                -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid SHMIOP option: <%C>\n"),
                               name.c_str ()),
                              -1);
        }
      else
        break;  // No other options.
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
