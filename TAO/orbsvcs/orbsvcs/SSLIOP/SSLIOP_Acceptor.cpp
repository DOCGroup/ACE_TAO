// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/SSLIOP/SSLIOP_Acceptor.h"
#include "orbsvcs/SSLIOP/SSLIOP_Profile.h"

#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Codeset_Manager.h"
#include "tao/CDR.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/SSLIOP/SSLIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Acceptor::Acceptor (::Security::QOP qop,
                                 const ACE_Time_Value & timeout)
  : TAO::IIOP_SSL_Acceptor (),
    ssl_acceptor_ (this),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0),
    timeout_ (timeout)
{
  // --- CSIv1 ---

  // Clear all bits in the SSLIOP::SSL association option fields.
  this->ssl_component_.target_supports = 0;
  this->ssl_component_.target_requires = 0;

  // SSLIOP requires these Security::AssociationOptions by default.
  ACE_SET_BITS (this->ssl_component_.target_requires,
                ::Security::Integrity
                | ::Security::Confidentiality
                | ::Security::NoDelegation);

  // SSLIOP supports these Security::AssociationOptions by default.
  ACE_SET_BITS (this->ssl_component_.target_supports,
                ::Security::Integrity
                | ::Security::Confidentiality
                | ::Security::EstablishTrustInTarget
                | ::Security::NoDelegation);

  // Initialize the default SSL port to zero (wild card port).
  this->ssl_component_.port = 0;

  // @@ This should go away once we support setting security
  //    association options through policies.
  if (qop == ::Security::SecQOPNoProtection)
    ACE_SET_BITS (this->ssl_component_.target_supports,
                  ::Security::NoProtection);


  // --- CSIv2 ---

  // Clear all bits in the CSIIOP::TLS_SEC_TRANS association option
  // fields.
  this->csiv2_component_.target_supports = 0;
  this->csiv2_component_.target_requires = 0;

  // SSLIOP requires these CSIIOP::AssociationOptions by default.
  ACE_SET_BITS (this->csiv2_component_.target_requires,
                CSIIOP::Integrity
                | CSIIOP::Confidentiality
                | CSIIOP::NoDelegation);

  // SSLIOP supports these CSIIOP::AssociationOptions by default.
  ACE_SET_BITS (this->csiv2_component_.target_supports,
                CSIIOP::Integrity
                | CSIIOP::Confidentiality
                | CSIIOP::EstablishTrustInTarget
                | CSIIOP::NoDelegation);

  // @@ This should go away once we support setting security
  //    association options through policies.
  if (qop == CSIIOP::NoProtection)
    ACE_SET_BITS (this->csiv2_component_.target_supports,
                  CSIIOP::NoProtection);
}

TAO::SSLIOP::Acceptor::~Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

int
TAO::SSLIOP::Acceptor::create_profile (const TAO::ObjectKey &object_key,
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
TAO::SSLIOP::Acceptor::create_new_profile (const TAO::ObjectKey &object_key,
                                           TAO_MProfile &mprofile,
                                           CORBA::Short priority)
{
  // Adding this->endpoint_count_ to the TAO_MProfile.
  const int count = mprofile.profile_count ();
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

      TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
      if (csm)
        csm->set_codeset (pfile->tagged_components());

      IOP::TaggedComponent component;
      component.tag = ::SSLIOP::TAG_SSL_SEC_TRANS;

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
      const CORBA::ULong length = cdr.total_length ();
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
TAO::SSLIOP::Acceptor::create_shared_profile (const TAO::ObjectKey &object_key,
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
          ssliop_profile = dynamic_cast<TAO_SSLIOP_Profile *> (pfile);
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

      TAO_SSLIOP_Endpoint * const ssliop_endp =
        dynamic_cast<TAO_SSLIOP_Endpoint *> (ssliop_profile->endpoint ());

      if (!ssliop_endp)
        return -1;

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

          TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
          if (csm)
            csm->set_codeset(ssliop_profile->tagged_components());

          IOP::TaggedComponent component;
          component.tag = ::SSLIOP::TAG_SSL_SEC_TRANS;
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
TAO::SSLIOP::Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO_SSLIOP_Endpoint *endp =
    dynamic_cast<const TAO_SSLIOP_Endpoint *> (endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return 0;

  for (size_t i = 0; i < this->endpoint_count_; ++i)
    {
      // @@ TODO The following code looks funky, why only the address
      //    is compared?  What about the IIOP address?  Why force a
      //    DNS lookup every time an SSLIOP object is decoded:
      //
      // http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=1220
      //
      if (endp->iiop_endpoint ()->object_addr () == this->addrs_[i])
        return 1;  // Collocated
    }

  return 0;  // Not collocated
}

int
TAO::SSLIOP::Acceptor::close (void)
{
  int r = this->ssl_acceptor_.close ();
  if (this->IIOP_SSL_Acceptor::close () != 0)
    r = -1;

  return r;
}

int
TAO::SSLIOP::Acceptor::open (TAO_ORB_Core *orb_core,
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

  ACE_INET_Addr addr;
  ACE_CString specified_hostname;
  if (this->parse_address (address, addr, specified_hostname) == -1)
    return -1;

  // Open the non-SSL enabled endpoints, then open the SSL enabled
  // endpoints.
  if (this->IIOP_SSL_Acceptor::open (orb_core,
                                     reactor,
                                     major,
                                     minor,
                                     address,
                                     options) != 0)
    return -1;

  // The SSL port is set in the parse_options() method. All we have
  // to do is call open_i()
  addr.set_port_number (this->ssl_component_.port);

  return this->ssliop_open_i (orb_core,
                              addr,
                              reactor);
}

int
TAO::SSLIOP::Acceptor::open_default (TAO_ORB_Core *orb_core,
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
  if (this->IIOP_SSL_Acceptor::open_default (orb_core,
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
                static_cast<ACE_UINT32> (INADDR_ANY),
                1) != 0)
    return -1;

  return this->ssliop_open_i (orb_core,
                              addr,
                              reactor);
}

int
TAO::SSLIOP::Acceptor::ssliop_open_i (TAO_ORB_Core *orb_core,
                                      const ACE_INET_Addr& addr,
                                      ACE_Reactor *reactor)
{
  this->orb_core_ = orb_core;

  ACE_NEW_RETURN (this->creation_strategy_,
                  CREATION_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  ACCEPT_STRATEGY (this->orb_core_,
                                   this->timeout_),
                  -1);

  u_short requested_port = addr.get_port_number ();
  if (requested_port == 0)
    {
      // don't care, i.e., let the OS choose an ephemeral port
      if (this->ssl_acceptor_.open (addr,
                                    reactor,
                                    this->creation_strategy_,
                                    this->accept_strategy_,
                                    this->concurrency_strategy_,
                                    0, 0, 0, 1,
                                    this->reuse_addr_) == -1)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) ")
                        ACE_TEXT ("SSLIOP_Acceptor::open_i - %p\n\n"),
                        ACE_TEXT ("cannot open acceptor")));
          return -1;
        }
    }
  else
    {
      ACE_INET_Addr a(addr);

      int found_a_port = 0;
      ACE_UINT32 last_port = requested_port + this->port_span_ - 1;
      if (last_port > ACE_MAX_DEFAULT_PORT)
        {
          last_port = ACE_MAX_DEFAULT_PORT;
        }

      for (ACE_UINT32 p = requested_port; p <= last_port; p++)
        {
          if (TAO_debug_level > 5)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) IIOP_Acceptor::open_i() ")
                        ACE_TEXT ("trying to listen on port %d\n"), p));

          // Now try to actually open on that port
          a.set_port_number ((u_short)p);
          if (this->ssl_acceptor_.open (a,
                                        reactor,
                                        this->creation_strategy_,
                                        this->accept_strategy_,
                                        this->concurrency_strategy_,
                                        0, 0, 0, 1,
                                        this->reuse_addr_) != -1)
            {
              found_a_port = 1;
              break;
            }
        }

      // Now, if we couldn't locate a port, we punt
      if (! found_a_port)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) ")
                        ACE_TEXT ("SSLIOP_Acceptor::open_i - %p\n\n"),
                        ACE_TEXT ("cannot open acceptor")));
          return -1;
        }
    }

  ACE_INET_Addr ssl_address;

  // We do this to make sure the port number the endpoint is listening
  // on gets set in the addr.
  if (this->ssl_acceptor_.acceptor ().get_local_addr (ssl_address) != 0)
    {
      // @@ Should this be a catastrophic error???
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
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
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) ")
                      ACE_TEXT ("SSLIOP_Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%s:%u>\n"),
                      this->hosts_[i],
                      this->ssl_component_.port));
        }
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
TAO::SSLIOP::Acceptor::parse_options_i (int &argc, ACE_CString ** argv)
{
  //first, do the base class parser, then parse the leftovers.
  int result = this->IIOP_SSL_Acceptor::parse_options_i(argc,argv);
  if (result == -1)
    return result;

  // then parse out our own options.
  int i = 0;
  while (i < argc)
    {
      // since the base class has already iterated over the list once,
      // it has vound any ill-formed options. Therefore we don't need
      // to do that again here.
      int slot = argv[i]->find ("=");
      ACE_CString name = argv[i]->substring (0, slot);
      ACE_CString value = argv[i]->substring (slot + 1);

      if (name == "priority")
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) Invalid SSLIOP endpoint format: ")
                             ACE_TEXT ("endpoint priorities no longer supported.\n"),
                             value.c_str ()),
                            -1);
        }
      else if (ACE_OS::strcmp (name.c_str (), "ssl_port") == 0)
        {
          int ssl_port = ACE_OS::atoi (value.c_str ());

          if (ssl_port >= 0 && ssl_port < 65536)
            this->ssl_component_.port = ssl_port;
          else
            ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid ")
                               ACE_TEXT ("IIOP/SSL endpoint ")
                               ACE_TEXT ("port: <%s>\n"),
                               value.c_str ()),
                              -1);
        }
      else
        {
          // the name is not known, skip to the next option
          i++;
          continue;
        }
      // at the end, we've consumed this argument. Shift the list and
      // put this one on the end. This technique has the effect of
      // putting them in reverse order, but that doesn't matter, since
      // these arguments are only whole strings.
      argc--;
      ACE_CString *temp = argv[i];
      for (int j = i; j <= argc-1; j++)
        argv[j] = argv[j+1];
      argv[argc] = temp;

    }
  return 0;
}

int
TAO::SSLIOP::Acceptor::verify_secure_configuration (TAO_ORB_Core *orb_core,
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
                           ::Security::NoProtection))
    {
      if (TAO_debug_level > 0)
        ORBSVCS_ERROR ((LM_ERROR,
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

TAO_END_VERSIONED_NAMESPACE_DECL
