// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE PortableServer::Servant_var<TAO_ECG_Complex_Address_Server>
TAO_ECG_Complex_Address_Server::create (int is_source_mapping)
{
  PortableServer::Servant_var<TAO_ECG_Complex_Address_Server> s;
  ACE_NEW_RETURN (s,
                  TAO_ECG_Complex_Address_Server (is_source_mapping),
                  s);
  return s;
}

TAO_END_VERSIONED_NAMESPACE_DECL
