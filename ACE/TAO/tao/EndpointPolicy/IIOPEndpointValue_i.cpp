// -*- C++ -*-

#include "tao/EndpointPolicy/IIOPEndpointValue_i.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IIOP_Endpoint.h"
#include "tao/IIOP_Acceptor.h"
#include "ace/OS_NS_strings.h"

ACE_RCSID (IIOPEndpointValue,
           IIOPEndpointValue_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

IIOPEndpointValue_i::IIOPEndpointValue_i ()
  :host_(CORBA::string_dup("")),
   port_(0)
{
}

IIOPEndpointValue_i::IIOPEndpointValue_i (const char *host,
                                          CORBA::UShort port)
  :host_(host),
   port_(port)
{
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
  return this->is_equivalent_i(iep->port(), iep->host());
}

CORBA::Boolean
IIOPEndpointValue_i::is_equivalent_i (CORBA::UShort port,
                                      const char *host) const
{
  return
    this->port_ == port && ACE_OS::strcasecmp(host, this->host_.in()) == 0;
}

CORBA::Boolean
IIOPEndpointValue_i::validate_acceptor(TAO_Acceptor * acceptor) const
{
  TAO_IIOP_Acceptor *iacc = dynamic_cast<TAO_IIOP_Acceptor *>(acceptor);
  if (iacc == 0)
    return 0;

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
        return 1;
    }
  return 0;
#else
  return 1;
#endif /* 0 */
}

char *
IIOPEndpointValue_i::host ( void )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->host_.in());
}

void
IIOPEndpointValue_i::host (const char *h)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->host_ = h;
}

CORBA::UShort
IIOPEndpointValue_i::port (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->port_;
}

void
IIOPEndpointValue_i::port (CORBA::UShort p )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->port_ = p;
}


CORBA::ULong
IIOPEndpointValue_i::protocol_tag (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IOP::TAG_INTERNET_IOP;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
