// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Strategies/COIOP_Acceptor.h"

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/COIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Codeset_Manager.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"

#if !defined(__ACE_INLINE__)
#include "tao/Strategies/COIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#include "ace/os_include/os_netdb.h"

ACE_RCSID (Strategies,
           COIOP_Acceptor,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_COIOP_Acceptor::TAO_COIOP_Acceptor (CORBA::Boolean flag)
  : TAO_Acceptor (TAO_TAG_COIOP_PROFILE),
    hosts_ (0),
    endpoint_count_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    lite_flag_ (flag),
    connection_handler_ (0)
{
}

TAO_COIOP_Acceptor::~TAO_COIOP_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  for (size_t i = 0; i < this->endpoint_count_; ++i)
    CORBA::string_free (this->hosts_[i]);

  delete [] this->hosts_;
}

// TODO =
//    2) For V1.[1,2] there are tagged components

int
TAO_COIOP_Acceptor::create_profile (const TAO::ObjectKey & object_key,
                                   TAO_MProfile &mprofile,
                                   CORBA::Short priority)
{
  // Sanity check.
  if (this->endpoint_count_ == 0)
    return -1;

  // Check if multiple endpoints should be put in one profile or if
  // they should be spread across multiple profiles.
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
TAO_COIOP_Acceptor::create_new_profile (const TAO::ObjectKey &object_key,
                                       TAO_MProfile &mprofile,
                                       CORBA::Short priority)
{
  // Adding this->endpoint_count_ to the TAO_MProfile.
  int const count = mprofile.profile_count ();
  if ((mprofile.size () - count) < this->endpoint_count_
      && mprofile.grow (count + this->endpoint_count_) == -1)
    return -1;

  // Create a profile for each acceptor endpoint.
  for (size_t i = 0; i < this->endpoint_count_; ++i)
    {
      TAO_COIOP_Profile *pfile = 0;
      ACE_Utils::UUID uuid;
      uuid.from_string (this->hosts_[i]);
      ACE_NEW_RETURN (pfile,
                      TAO_COIOP_Profile (uuid,
                                        object_key,
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
      // by the user not to do so, or if an IIOP 1.0 endpoint is being
      // created (IIOP 1.0 did not support tagged components).
      if (this->orb_core_->orb_params ()->std_profile_components () == 0
          || (this->version_.major == 1 && this->version_.minor == 0))
        continue;

      pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

      TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
      if (csm)
        csm->set_codeset(pfile->tagged_components());
    }

  return 0;
}

int
TAO_COIOP_Acceptor::create_shared_profile (const TAO::ObjectKey &object_key,
                                          TAO_MProfile &mprofile,
                                          CORBA::Short priority)
{
  size_t index = 0;
  TAO_Profile *pfile = 0;
  TAO_COIOP_Profile *iiop_profile = 0;

  // First see if <mprofile> already contains a COIOP profile.
  for (TAO_PHandle i = 0; i != mprofile.profile_count (); ++i)
    {
      pfile = mprofile.get_profile (i);
      if (pfile->tag () == TAO_TAG_COIOP_PROFILE)
      {
        iiop_profile = dynamic_cast<TAO_COIOP_Profile *> (pfile);
        break;
      }
    }

  // If <mprofile> doesn't contain a COIOP_Profile, we need to create
  // one.
  if (iiop_profile == 0)
    {
      ACE_Utils::UUID uuid (this->hosts_[0]);
      ACE_NEW_RETURN (iiop_profile,
                      TAO_COIOP_Profile (uuid,
                                        object_key,
      //                                  this->addrs_[0],
                                        this->version_,
                                        this->orb_core_),
                      -1);
      iiop_profile->endpoint ()->priority (priority);

      if (mprofile.give_profile (iiop_profile) == -1)
        {
          iiop_profile->_decr_refcnt ();
          iiop_profile = 0;
          return -1;
        }

      if (this->orb_core_->orb_params ()->std_profile_components () != 0
          && (this->version_.major >= 1 && this->version_.minor >= 1))
        {
          iiop_profile->tagged_components ().set_orb_type (TAO_ORB_TYPE);
          TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
          if (csm)
            csm->set_codeset(pfile->tagged_components());
        }

      index = 1;
    }

  // Add any remaining acceptor endpoints to the COIOP_Profile.
  for (;
       index < this->endpoint_count_;
       ++index)
    {
      TAO_COIOP_Endpoint *endpoint = 0;
      ACE_NEW_RETURN (endpoint,
                      TAO_COIOP_Endpoint (ACE_Utils::UUID (this->hosts_[index])
            ),//                             this->addrs_[index]),
                      -1);
      endpoint->priority (priority);
      iiop_profile->add_endpoint (endpoint);
    }

  return 0;
}

int
TAO_COIOP_Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO_COIOP_Endpoint *endp =
    dynamic_cast<const TAO_COIOP_Endpoint *> (endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return false;

  return true;  // Not collocated
}

int
TAO_COIOP_Acceptor::close (void)
{
  return 0;
}

int
TAO_COIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         ACE_Reactor *,
                         int major,
                         int minor,
                         const char *address,
                         const char *options)
{
  this->orb_core_ = orb_core;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("COIOP_Acceptor::open - ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (address == 0)
    return -1;

  if (major >=0 && minor >= 0)
    this->version_.set_version (static_cast<CORBA::Octet> (major),
                                static_cast<CORBA::Octet> (minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  this->endpoint_count_ = 1;  // Only one hostname to store

  ACE_NEW_RETURN (this->hosts_,
                  char *[this->endpoint_count_],
                  -1);

  this->hosts_[0] = CORBA::string_dup (address);

  return 0;
}

int
TAO_COIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                 ACE_Reactor *,
                                 int major,
                                 int minor,
                                 const char *options)
{
  this->orb_core_ = orb_core;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("COIOP_Acceptor::open_default - ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (major >=0 && minor >= 0)
    this->version_.set_version (static_cast<CORBA::Octet> (major),
                                static_cast<CORBA::Octet> (minor));

  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  // Check for multiple network interfaces.
//  if (this->probe_interfaces (orb_core) == -1)
  //  return -1;

  // Now that each network interface's hostname has been cached, open
  // an endpoint on each network interface using the INADDR_ANY
  // address.
  ACE_INET_Addr addr;

  if (addr.set (static_cast<unsigned short> (0),
                static_cast<ACE_UINT32> (INADDR_ANY),
                1) != 0)
    return -1;

  return 0;
}

CORBA::ULong
TAO_COIOP_Acceptor::endpoint_count (void)
{
  return this->endpoint_count_;
}

int
TAO_COIOP_Acceptor::object_key (IOP::TaggedProfile &profile,
                               TAO::ObjectKey &object_key)
{
  // Create the decoding stream from the encapsulation in the buffer,
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  TAO_InputCDR cdr (profile.profile_data.mb ());
#else
  TAO_InputCDR cdr (reinterpret_cast<char*> (profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  CORBA::Octet major;
  CORBA::Octet minor = CORBA::Octet();

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major)
        && cdr.read_octet (minor)))
  {
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) COIOP_Profile::decode - v%d.%d\n"),
                    major,
                    minor));
      }
    return -1;
  }

  CORBA::String_var uuid;

  // Get host and port. No processing here too..
  if (cdr.read_string (uuid.out ()) == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) TAO_COIOP_Acceptor::object_key - ")
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


int
TAO_COIOP_Acceptor::parse_options (const char *str)
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

  ACE_CString::size_type begin = 0;
  ACE_CString::size_type end = 0;

  for (CORBA::ULong j = 0; j < option_count; ++j)
    {
      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = len;

      if (end == begin)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) Zero length COIOP option.\n")),
                            -1);
        }
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          ACE_CString::size_type const slot = opt.find ("=");

          if (slot == len - 1
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) COIOP option <%s> is ")
                               ACE_TEXT ("missing a value.\n"),
                               ACE_TEXT_CHAR_TO_TCHAR (opt.c_str ())),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Zero length COIOP ")
                               ACE_TEXT ("option name.\n")),
                              -1);

          if (name == "priority")
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid COIOP endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported. \n")),
                                -1);
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid COIOP option: <%s>\n"),
                                 ACE_TEXT_CHAR_TO_TCHAR (name.c_str ())),
                                -1);
            }

          begin = end + 1;
        }
      else
        {
          break;  // No other options.
        }
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */
