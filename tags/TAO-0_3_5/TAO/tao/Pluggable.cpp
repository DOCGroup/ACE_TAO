// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================

#include "tao/Pluggable.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"

TAO_Connector_Registry::TAO_Connector_Registry (void)
  : iiop_connector_ (0)
{
}

TAO_Connector_Registry::~TAO_Connector_Registry (void)
{
}

TAO_Connector *
TAO_Connector_Registry::get_connector (CORBA::ULong tag)
{
  // For now, only IIOP connectors.
  if (tag != TAO_IOP_TAG_INTERNET_IOP)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Invalid connector tag %d\n",
                       tag),
                      0);
  else
    return iiop_connector_;
}

CORBA::Boolean
TAO_Connector_Registry::add_connector (TAO_Connector *connector)
{
  if (connector->tag() == TAO_IOP_TAG_INTERNET_IOP)
    {
      // do not copy, but save the reference (i.e. pointer)
      this->iiop_connector_ = connector;
      return 1;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Invalid connector tag %d\n",
                       connector->tag ()),
                      0);
}

int
TAO_Connector_Registry::open(TAO_Resource_Factory *trf, ACE_Reactor *reactor)
{
  // @@ Once again since we only accept 1 iiop connector, this is easy
  if (iiop_connector_)
    return this->iiop_connector_->open (trf, reactor);
  else
    return 0;
}

int
TAO_Connector_Registry::close_all()
{
  // @@ Loop through all registered connectors ... not too hard
  //    since there is only one!
  if (iiop_connector_)
    return this->iiop_connector_->close ();
  return 0;
}

int
TAO_Connector_Registry::preconnect (const char *the_preconnections)
{
  // It would be good to use auto_ptr<> to guard against premature
  // termination and, thus, leaks.
  int result=0;
  char *preconnections = ACE_OS::strdup (the_preconnections);

  // @@ OK, what we should do is parse the string so that we can
  // gather @@ together addresses of the same protocol together and
  // pass to the @@ appropriate connector.  But, for now we ASSUME
  // they are all @@ INET IP:Port!! HACK. fredk

  if (this->iiop_connector_)
    result = this->iiop_connector_->preconnect (preconnections);

  ACE_OS::free (preconnections);

  return result;
}

TAO_Profile *
TAO_Connector_Registry::connect (STUB_Object *&obj,
                                 CORBA::Environment &env)
{
  CORBA::ULong req_tag = TAO_IOP_TAG_INTERNET_IOP;
  TAO_Profile *profile = obj->get_fwd_profile ();

  // @@ FRED _ For now still only support ONE profile!
  if (profile == 0)
    profile = obj->profile_in_use ();

  // @@ And the profile selection policy is .... ONLY IIOP, and the @@
  // first one found!
  if (profile->tag () != req_tag)
    TAO_THROW_ENV_RETURN (CORBA::INTERNAL (CORBA::COMPLETED_NO),
                          env,
                          0);

  // obj->set_profile_in_use (profile);

  // here is where we get the appropriate connector object but we are
  // the Connector Registry so call get_connector(tag)

  TAO_Connector *connector =
    this->get_connector (req_tag);

  TAO_Transport *transport =
    connector->connect (profile, env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (transport == 0)
    return 0;

  return profile;
}

Version::~Version (void)
{
}

Version::Version (const Version &src)
  : major (src.major),
    minor (src.minor)
{
}

Version::Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{
}

void
Version::set_version (CORBA::Octet maj, CORBA::Octet min)
{
  this->major = maj;
  this->minor = min;
}

int
Version::operator== (const Version *&src)
{
  return this->major == src->major && this->minor == src->minor;
}

int
Version::operator== (const Version &src)
{
  return this->major == src.major && this->minor == src.minor;
}

Version &
Version::operator= (const Version &src)
{
  this->major = src.major;
  this->minor = src.minor;
  return *this;
}

TAO_Profile::~TAO_Profile (void)
{
}

TAO_Transport::~TAO_Transport (void)
{
}

TAO_Connector::~TAO_Connector (void)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{
}
