// $Id$

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, params, "$Id$")

TAO_ORB_Parameters::TAO_ORB_Parameters (void)
  : name_service_ior_ (0),
    name_service_port_ (0),
    trading_service_ior_ (0),
    trading_service_port_ (0),
    sock_rcvbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    sock_sndbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    cdr_memcpy_tradeoff_ (TAO_DEFAULT_CDR_MEMCPY_TRADEOFF)
{
}

TAO_ORB_Parameters::~TAO_ORB_Parameters (void)
{
}

void
TAO_ORB_Parameters::addr (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

// Get the address on which we're listening.

const ACE_INET_Addr &
TAO_ORB_Parameters::addr (void)
{
  return this->addr_;
}

void
TAO_ORB_Parameters::name_service_ior (CORBA::String ns)
{
  this->name_service_ior_ = ns;
}

// Set the IOR of our name service.

CORBA::String
TAO_ORB_Parameters::name_service_ior (void)
{
  return this->name_service_ior_;
}

// Get the IOR of our name service.

void
TAO_ORB_Parameters::name_service_port (CORBA::UShort port)
{
  this->name_service_port_ = port;
}

// Set the port of our name service.

CORBA::UShort
TAO_ORB_Parameters::name_service_port (void)
{
  return this->name_service_port_;
}


void
TAO_ORB_Parameters::trading_service_ior (CORBA::String ns)
{
  this->trading_service_ior_ = ns;
}

// Set the IOR of our trading service.

CORBA::String
TAO_ORB_Parameters::trading_service_ior (void)
{
  return this->trading_service_ior_;
}

// Get the IOR of our trading service.

void
TAO_ORB_Parameters::trading_service_port (CORBA::UShort port)
{
  this->trading_service_port_ = port;
}

// Set the port of our trading service.

CORBA::UShort
TAO_ORB_Parameters::trading_service_port (void)
{
  return this->trading_service_port_;
}

void
TAO_OA_Parameters::demux_strategy (const char* strategy)
{
  // Determine the demux strategy based on the given name
  if (!ACE_OS::strcmp (strategy, "linear"))
    this->demux_ = TAO_LINEAR;
  else if (!ACE_OS::strcmp (strategy, "dynamic_hash"))
    this->demux_ = TAO_DYNAMIC_HASH;
  else if (!ACE_OS::strcmp (strategy, "user_def"))
    this->demux_ = TAO_USER_DEFINED;
  else if (!ACE_OS::strcmp (strategy, "active_demux"))
    this->demux_ = TAO_ACTIVE_DEMUX;
  else
    // Provide fallback!
    this->demux_ = TAO_DYNAMIC_HASH;
}
