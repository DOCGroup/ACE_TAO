// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "SSLIOP_Acceptor.h"
#include "SSLIOP_Profile.h"
#include "SSLIOP_Current.h"
#include "SSLIOP_Util.h"

#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "SSLIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Acceptor,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_SSLIOP_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_SSLIOP_Connection_Handler>;
template class TAO_Creation_Strategy<TAO_SSLIOP_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>;
template class TAO_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate TAO_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_SSLIOP_Acceptor::TAO_SSLIOP_Acceptor (Security::QOP qop)
  : TAO_IIOP_SSL_Acceptor (),
    ssl_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0),
    handler_state_ ()
{
  // Clear all bits in the SSLIOP::SSL association option fields.
  this->ssl_component_.target_supports = 0;
  this->ssl_component_.target_requires = 0;

  // SSLIOP requires these Security::AssociationOptions by default.
  ACE_SET_BITS (this->ssl_component_.target_requires,
                Security::Integrity
                | Security::Confidentiality
                | Security::NoDelegation);

  // SSLIOP supports these Security::AssociationOptions by default.
  ACE_SET_BITS (this->ssl_component_.target_supports,
                Security::Integrity
                | Security::Confidentiality
                | Security::EstablishTrustInTarget
                | Security::NoDelegation);

  // Initialize the default SSL port to zero (wild card port).
  this->ssl_component_.port = 0;

  // @@ This should go away once we support setting security
  //    association options through policies.
  if (qop == Security::SecQOPNoProtection)
    ACE_SET_BITS (this->ssl_component_.target_supports,
                  Security::NoProtection);
}

TAO_SSLIOP_Acceptor::~TAO_SSLIOP_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

int
TAO_SSLIOP_Acceptor::create_profile (const TAO_ObjectKey &object_key,
                                     TAO_MProfile &mprofile,
                                     CORBA::Short priority)
{
  // Sanity check.
  if (this->endpoint_count_ == 0)
    return -1;

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
TAO_SSLIOP_Acceptor::create_new_profile (const TAO_ObjectKey &object_key,
                                         TAO_MProfile &mprofile,
                                         CORBA::Short priority)
{
  // Adding this->endpoint_count_ to the TAO_MProfile.
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < this->endpoint_count_
      && mprofile.grow (count + this->endpoint_count_) == -1)
    return -1;

  // Create a profile for each acceptor endpoint.
  for (size_t i = 0; i < this->endpoint_count_; ++i)
    {
      TAO_SSLIOP_Profile *pfile = 0;

      // @@ We need to create an SSLIOP::SSL component for the object
      //    we're creating an MProfile for.  This will allow us to
      //    properly embed secure invocation policies in the generated
      //    IOR, i.e. secure invocation policies on a per-object
      //    basis, rather than on a per-endpoint basis.  If no secure
      //    invocation policies have been set then we should use the
      //    below default SSLIOP::SSL component.
      ACE_NEW_RETURN (pfile,
                      TAO_SSLIOP_Profile (this->hosts_[i],
                                          this->addrs_[i].get_port_number (),
                                          object_key,
                                          this->addrs_[i],
                                          this->version_,
                                          this->orb_core_,
                                          &(this->ssl_component_)),
                      -1);
      pfile->endpoint ()->priority (priority);

      if (mprofile.give_profile (pfile) == -1)
        {
          pfile->_decr_refcnt ();
          pfile = 0;
          return -1;
        }

      if (this->orb_core_->orb_params ()->std_profile_components () == 0)
        continue;

      pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

      CONV_FRAME::CodeSetComponentInfo code_set_info;
      code_set_info.ForCharData.native_code_set  =
        TAO_DEFAULT_CHAR_CODESET_ID;
      code_set_info.ForWcharData.native_code_set =
        TAO_DEFAULT_WCHAR_CODESET_ID;
      pfile->tagged_components ().set_code_sets (code_set_info);

      IOP::TaggedComponent component;
      component.tag = SSLIOP::TAG_SSL_SEC_TRANS;

      // @@???? Check this code, only intended as guideline...
      TAO_OutputCDR cdr;
      cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);

      // @@ We need to create an SSLIOP::SSL component for the object
      //    we're creating an MProfile for.  This will allow us to
      //    properly embed secure invocation policies in the generated
      //    IOR, i.e. secure invocation policies on a per-object
      //    basis, rather than on a per-endpoint basis.  If no secure
      //    invocation policies have been set then we should use the
      //    below default SSLIOP::SSL component.
      cdr << this->ssl_component_;

      // TAO extension, replace the contents of the octet sequence with
      // the CDR stream
      CORBA::ULong length = cdr.total_length ();
      component.component_data.length (length);
      CORBA::Octet *buf = component.component_data.get_buffer ();
      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
          buf += i->length ();
        }

      pfile->tagged_components ().set_component (component);
    }

  return 0;
}


int
TAO_SSLIOP_Acceptor::create_shared_profile (const TAO_ObjectKey &object_key,
                                            TAO_MProfile &mprofile,
                                            CORBA::Short priority)
{
  size_t index = 0;
  TAO_Profile *pfile = 0;
  TAO_SSLIOP_Profile *ssliop_profile = 0;

  // First see if <mprofile> already contains a SSLIOP profile.
  for (TAO_PHandle i = 0; i != mprofile.profile_count (); ++i)
    {
      pfile = mprofile.get_profile (i);
      if (pfile->tag () == IOP::TAG_INTERNET_IOP)
        {
          ssliop_profile = ACE_dynamic_cast (TAO_SSLIOP_Profile *,
                                             pfile);
          if (ssliop_profile == 0)
            return -1;
          break;
        }
    }

  // If <mprofile> doesn't contain SSLIOP_Profile, we need to create
  // one.
  if (ssliop_profile == 0)
    {
      // @@ We need to create an SSLIOP::SSL component for the object
      //    we're creating an MProfile for.  This will allow us to
      //    properly embed secure invocation policies in the generated
      //    IOR, i.e. secure invocation policies on a per-object
      //    basis, rather than on a per-endpoint basis.  If no secure
      //    invocation policies have been set then we should use the
      //    below default SSLIOP::SSL component.
      ACE_NEW_RETURN (ssliop_profile,
                      TAO_SSLIOP_Profile (this->hosts_[0],
                                          this->addrs_[0].get_port_number (),
                                          object_key,
                                          this->addrs_[0],
                                          this->version_,
                                          this->orb_core_,
                                          &(this->ssl_component_)),
                      -1);

      TAO_SSLIOP_Endpoint *ssliop_endp =
        ACE_dynamic_cast (TAO_SSLIOP_Endpoint *,
                          ssliop_profile->endpoint ());

      ssliop_endp->priority (priority);
      ssliop_endp->iiop_endpoint ()->priority (priority);

      if (mprofile.give_profile (ssliop_profile) == -1)
        {
          ssliop_profile->_decr_refcnt ();
          ssliop_profile = 0;
          return -1;
        }

      if (this->orb_core_->orb_params ()->std_profile_components () != 0)
        {
          ssliop_profile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

          CONV_FRAME::CodeSetComponentInfo code_set_info;
          code_set_info.ForCharData.native_code_set  =
            TAO_DEFAULT_CHAR_CODESET_ID;
          code_set_info.ForWcharData.native_code_set =
            TAO_DEFAULT_WCHAR_CODESET_ID;
          ssliop_profile->tagged_components ().set_code_sets (code_set_info);

          IOP::TaggedComponent component;
          component.tag = SSLIOP::TAG_SSL_SEC_TRANS;
          // @@???? Check this code, only intended as guideline...
          TAO_OutputCDR cdr;
          cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);

          // @@ We need to create an SSLIOP::SSL component for the
          //    object we're creating an MProfile for.  This will
          //    allow us to properly embed secure invocation policies
          //    in the generated IOR, i.e. secure invocation policies
          //    on a per-object basis, rather than on a per-endpoint
          //    basis.  If no secure invocation policies have been set
          //    then we should use the below default SSLIOP::SSL
          //    component.
          cdr << this->ssl_component_;

          // TAO extension, replace the contents of the octet sequence with
          // the CDR stream
          CORBA::ULong length = cdr.total_length ();
          component.component_data.length (length);
          CORBA::Octet *buf = component.component_data.get_buffer ();
          for (const ACE_Message_Block *i = cdr.begin ();
               i != 0;
               i = i->cont ())
            {
              ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
              buf += i->length ();
            }

          ssliop_profile->tagged_components ().set_component (component);
        }

      index = 1;
    }

  // Add any remaining endpoints to the SSLIOP_Profile.
  for (;
       index < this->endpoint_count_;
       ++index)
    {
      TAO_SSLIOP_Endpoint *ssl_endp = 0;
      TAO_IIOP_Endpoint *iiop_endp = 0;
      ACE_NEW_RETURN (iiop_endp,
                      TAO_IIOP_Endpoint (this->hosts_[index],
                                         this->addrs_[index].get_port_number (),
                                         this->addrs_[index]),
                      -1);
      iiop_endp->priority (priority);

      ACE_NEW_RETURN (ssl_endp,
                      TAO_SSLIOP_Endpoint (&(this->ssl_component_),
                                           iiop_endp),
                      -1);

      ssl_endp->priority (priority);
      ssliop_profile->add_endpoint (ssl_endp);
    }

  return 0;
}

int
TAO_SSLIOP_Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO_SSLIOP_Endpoint *endp =
    ACE_dynamic_cast (const TAO_SSLIOP_Endpoint *,
                      endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return 0;

  for (size_t i = 0; i < this->endpoint_count_; ++i)
    {
      // compare the port and sin_addr (numeric host address)
      if (endp->iiop_endpoint ()->object_addr () == this->addrs_[i])
        return 1;  // Collocated
    }

  return 0;  // Not collocated
}

int
TAO_SSLIOP_Acceptor::close (void)
{
  int r = this->ssl_acceptor_.close ();
  if (this->TAO_IIOP_SSL_Acceptor::close () != 0)
    r = -1;

  return r;
}

int
TAO_SSLIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                           ACE_Reactor *reactor,
                           int major,
                           int minor,
                           const char *address,
                           const char *options)
{
  // Ensure that neither the endpoint configuration nor the ORB
  // configuration violate security measures.
  if (this->verify_secure_configuration (orb_core,
                                         major,
                                         minor) != 0)
    return -1;

  // Open the non-SSL enabled endpoints, then open the SSL enabled
  // endpoints.
  if (this->TAO_IIOP_SSL_Acceptor::open (orb_core,
                                         reactor,
                                         major,
                                         minor,
                                         address,
                                         options) != 0)
    return -1;

  // The SSL port is set in the parse_options() method. All we have
  // to do is call open_i()
  ACE_INET_Addr addr (this->ssl_component_.port,
                      this->addrs_[0].get_host_addr ());

  return this->ssliop_open_i (orb_core,
                              addr,
                              reactor);
}

int
TAO_SSLIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                   ACE_Reactor *reactor,
                                   int major,
                                   int minor,
                                   const char *options)
{
  // Ensure that neither the endpoint configuration nor the ORB
  // configuration violate security measures.
  if (this->verify_secure_configuration (orb_core,
                                         major,
                                         minor) != 0)
    return -1;

  // Open the non-SSL enabled endpoints, then open the SSL enabled
  // endpoints.
  if (this->TAO_IIOP_SSL_Acceptor::open_default (orb_core,
                                                 reactor,
                                                 major,
                                                 minor,
                                                 options) == -1)
    return -1;

  // Now that each network interface's hostname has been cached, open
  // an endpoint on each network interface using the INADDR_ANY
  // address.
  ACE_INET_Addr addr;

  // this->ssl_component_.port is initialized to zero or it is set in
  // this->parse_options().
  if (addr.set (this->ssl_component_.port,
                ACE_static_cast(ACE_UINT32, INADDR_ANY),
                1) != 0)
    return -1;

  return this->ssliop_open_i (orb_core,
                              addr,
                              reactor);
}

int
TAO_SSLIOP_Acceptor::ssliop_open_i (TAO_ORB_Core *orb_core,
                                    const ACE_INET_Addr& addr,
                                    ACE_Reactor *reactor)
{
  this->orb_core_ = orb_core;

  int giop_lite = 0;
  // Explicitly disable GIOPlite support since it introduces security
  // holes.

  if (TAO_SSLIOP_Util::setup_handler_state (this->orb_core_,
                                            &(this->tcp_properties_),
                                            this->handler_state_) != 0)
      return -1;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_SSLIOP_CREATION_STRATEGY (this->orb_core_,
                                                &(this->handler_state_),
                                                giop_lite),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_SSLIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_SSLIOP_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  if (this->ssl_acceptor_.open (addr,
                                reactor,
                                this->creation_strategy_,
                                this->accept_strategy_,
                                this->concurrency_strategy_) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("SSLIOP_Acceptor::open_i - %p\n\n"),
                    ACE_TEXT ("cannot open acceptor")));
      return -1;
    }

  ACE_INET_Addr ssl_address;

  // We do this to make sure the port number the endpoint is listening
  // on gets set in the addr.
  if (this->ssl_acceptor_.acceptor ().get_local_addr (ssl_address) != 0)
    {
      // @@ Should this be a catastrophic error???
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("SSLIOP_Acceptor::open_i - %p\n\n"),
                    ACE_TEXT ("cannot get local addr")));
      return -1;
    }

  // Reset the SSL endpoint port to the one chosen by the OS (or by
  // the user if provided.
  this->ssl_component_.port = ssl_address.get_port_number ();

  (void) this->ssl_acceptor_.acceptor().enable (ACE_CLOEXEC);
  // This avoids having child processes acquire the listen socket
  // thereby denying the server the opportunity to restart on a
  // well-known endpoint.  This does not affect the aberrent behavior
  // on Win32 platforms.

  if (TAO_debug_level > 5)
    {
      for (size_t i = 0; i < this->endpoint_count_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) ")
                      ACE_TEXT ("SSLIOP_Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%s:%u>\n"),
                      this->hosts_[i],
                      this->ssl_component_.port));
        }
    }

  return 0;
}

int
TAO_SSLIOP_Acceptor::parse_options (const char *str)
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

  // @@ We should add options to set the security association options,
  //    or are those controlled by Policies?
  // @@ They are controlled by the SecureInvocation policies defined
  //    in the Security Service specification.
  for (CORBA::ULong j = 0; j < option_count; ++j)
    {
      begin += end + 1;

      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = len - begin;  // Handle last endpoint differently

      if (end == begin)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) Zero length")
                           ACE_TEXT ("IIOP/SSL option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          int slot = opt.find ("=");

          if (slot == ACE_static_cast (int, len - 1)
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) IIOP/SSL")
                               ACE_TEXT ("option <%s> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "TAO (%P|%t) Zero length IIOP/SSL "
                               "option name.\n"),
                              -1);

          if (name == "priority")
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid SSLIOP endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported. \n"),
                                 value.c_str ()),
                                -1);
            }
          else if (ACE_OS::strcmp (name.c_str (), "ssl_port") == 0)
            {
              int ssl_port = ACE_OS::atoi (value.c_str ());

              if (ssl_port >= 0 && ssl_port < 65536)
                this->ssl_component_.port = ssl_port;
              else
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("TAO (%P|%t) Invalid ")
                                   ACE_TEXT ("IIOP/SSL endpoint ")
                                   ACE_TEXT ("port: <%s>\n"),
                                   value.c_str ()),
                                  -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid ")
                               ACE_TEXT ("IIOP/SSL ")
                               ACE_TEXT ("option: <%s>\n"),
                               name.c_str ()),
                              -1);
        }
    }
  return 0;
}

int
TAO_SSLIOP_Acceptor::verify_secure_configuration (TAO_ORB_Core *orb_core,
                                                  int major,
                                                  int minor)
{
  // Sanity check.
  if (major < 1)
    {
      // There is no such thing as IIOP 0.x.
      errno = EINVAL;
      return -1;
    }

  // In order to support a secure connection, the SSLIOP::SSL tagged
  // component must be embedded in the IOR.  This isn't possible if
  // the user elects to disable standard profile components.
  // Similarly, IIOP 1.0 does not support tagged components, which
  // makes it impossible to embed the SSLIOP::SSL tagged component
  // within the IOR.  If the given object explicitly disallows
  // insecure invocations and standard profile components are
  // disabled, then return with an error since secure invocations
  // cannot be supported without standard profile components.
  //
  // Note that it isn't enough to support NoProtection.  NoProtection
  // must be required since "support" does not preclude the secure
  // port from being used.

  if ((orb_core->orb_params ()->std_profile_components () == 0
       || (major == 1 && minor == 0))
      && ACE_BIT_DISABLED (this->ssl_component_.target_requires,
                           Security::NoProtection))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Cannot support secure ")
                    ACE_TEXT ("IIOP over SSL connection if\n")
                    ACE_TEXT ("(%P|%t) standard profile ")
                    ACE_TEXT ("components are disabled\n")
                    ACE_TEXT ("(%P|%t) or IIOP 1.0 endpoint is ")
                    ACE_TEXT ("used.\n")));

      errno = EINVAL;
      return -1;
    }

  return 0;
}
