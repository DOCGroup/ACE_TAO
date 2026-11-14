// -*- C++ -*-
#include "tao/EndpointPolicy/IIOPEndpointValue_i.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IIOP_Endpoint.h"
#include "tao/IIOP_Acceptor.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

IIOPEndpointValue_i::IIOPEndpointValue_i ()
  : host_ (""),
    port_ {}
{
}

IIOPEndpointValue_i::IIOPEndpointValue_i (char const* host, CORBA::UShort port)
  : host_ (host),
    port_ (port)
{
  if (this->addr_.set (port, host) != 0 || host == nullptr || host[0] == '\0')
    this->addr_.set_type (-1);
}

CORBA::Boolean
IIOPEndpointValue_i::is_equivalent (TAO_Endpoint const* endpoint) const
{
  TAO_IIOP_Endpoint const * const iep =
    dynamic_cast<TAO_IIOP_Endpoint const*> (endpoint);
  if (iep == nullptr)
    return false;
  return this->addr_.get_type() != -1 ? this->addr_ == iep->object_addr() :
    this->is_equivalent_i (iep->port(), iep->host());
}

CORBA::Boolean
IIOPEndpointValue_i::is_equivalent_i (CORBA::UShort port,
                                      char const* host) const
{
  bool const is_eq = this->port_ == port
    && (this->host_.in()[0] == '\0'
        || ACE_OS::strcasecmp (host, this->host_.in ()) == 0);
  return is_eq;
}

CORBA::Boolean
IIOPEndpointValue_i::validate_acceptor(TAO_Acceptor * acceptor,
                                       bool is_multi_prot) const
{
  TAO_IIOP_Acceptor * const iacc = dynamic_cast<TAO_IIOP_Acceptor *> (acceptor);
  if (iacc == nullptr)
    return false;

  // @@@ mesnier_p@ociweb.com @@@
  // Rather than blindly returning true, we need to reject defaulted
  // endpoint hosts except when the ORB has more than one protocol
  // acceptor defined;

  return is_multi_prot || this->host_.in()[0] != '\0';
}

char *
IIOPEndpointValue_i::host ()
{
  return CORBA::string_dup (this->host_.in());
}

void
IIOPEndpointValue_i::host (char const* h)
{
  this->host_ = h;
  if (this->addr_.set(this->port_, this->host_.in ()) != 0)
    this->addr_.set_type (-1);
}

CORBA::UShort
IIOPEndpointValue_i::port ()
{
  return this->port_;
}

void
IIOPEndpointValue_i::port (CORBA::UShort p )
{
  this->port_ = p;
  if (this->addr_.set(this->port_, this->host_.in ()) != 0)
    this->addr_.set_type (-1);
}

CORBA::ULong
IIOPEndpointValue_i::protocol_tag ()
{
  return IOP::TAG_INTERNET_IOP;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
