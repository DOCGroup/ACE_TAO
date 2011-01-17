// -*- C++ -*-
// $Id$

#include "tao/EndpointPolicy/IIOPEndpointValue_i.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IIOP_Endpoint.h"
#include "tao/IIOP_Acceptor.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

IIOPEndpointValue_i::IIOPEndpointValue_i ()
  :host_(""),
   port_(0)
{
}

IIOPEndpointValue_i::IIOPEndpointValue_i (const char *host, CORBA::UShort port)
  :host_(host),
   port_(port)
{
  if (this->addr_.set(port, host) != 0 || host == 0 || host[0] == 0)
    this->addr_.set_type (-1);
}

IIOPEndpointValue_i::~IIOPEndpointValue_i (void)
{
}

CORBA::Boolean
IIOPEndpointValue_i::is_equivalent (const TAO_Endpoint * endpoint) const
{
  const TAO_IIOP_Endpoint *iep =
    dynamic_cast<const TAO_IIOP_Endpoint *>(endpoint);
  if (iep == 0)
    return 0;
  return this->addr_.get_type() != -1 ? this->addr_ == iep->object_addr() :
    this->is_equivalent_i (iep->port(), iep->host());
}

CORBA::Boolean
IIOPEndpointValue_i::is_equivalent_i (CORBA::UShort port,
                                      const char *host) const
{
  bool is_eq = this->port_ == port;
  if (is_eq)
    is_eq = (this->host_.in()[0] == '\0' ||
            ACE_OS::strcasecmp(host, this->host_.in()) == 0);
  return is_eq;
}

CORBA::Boolean
IIOPEndpointValue_i::validate_acceptor(TAO_Acceptor * acceptor,
                                       bool is_multi_prot) const
{
  TAO_IIOP_Acceptor *iacc = dynamic_cast<TAO_IIOP_Acceptor *>(acceptor);
  if (iacc == 0)
    return false;

#if 0
  // @@@ mesnier_p@ociweb.com @@@
  // This test was originally intended to validate that there is an
  // acceptor available which supports the target endpoint. However,
  // variablility in the definition of a "hostname" makes this hard
  // to determine, without adding more overhead, probably to the
  // IIOP_Acceptor class.
  // I'm not sure this level of validation is really required, so
  // for now I'm just leaving it out.

  const ACE_INET_Addr * eps = iacc->endpoints ();
  CORBA::ULong eps_count = iacc->endpoint_count ();
  for (CORBA::ULong eps_idx = 0; eps_idx < eps_count; ++ eps_idx)
    {
      char hostname[513];
      eps[eps_idx].get_host_name(hostname,512);
      if (this->is_equivalent_i(eps[eps_idx].get_port_number(),hostname))
        return true;
    }
  return false;
#else
  // @@@ mesnier_p@ociweb.com @@@
  // Rather than blindly returning true, we need to reject defaulted
  // endpoint hosts except when the ORB has more than one protocol
  // acceptor defined;

  return is_multi_prot || this->host_.in()[0] != '\0';
#endif /* 0 */
}

char *
IIOPEndpointValue_i::host (void)
{
  return CORBA::string_dup (this->host_.in());
}

void
IIOPEndpointValue_i::host (const char *h)
{
  this->host_ = h;
  if (this->addr_.set(this->port_, this->host_.in ()) != 0)
    this->addr_.set_type (-1);
}

CORBA::UShort
IIOPEndpointValue_i::port (void)
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
IIOPEndpointValue_i::protocol_tag (void)
{
  return IOP::TAG_INTERNET_IOP;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
