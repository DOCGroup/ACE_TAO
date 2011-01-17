// -*- C++ -*-
//
// $Id$


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_IIOP_Endpoint *
TAO_SSLIOP_Endpoint::iiop_endpoint (void) const
{
  return this->iiop_endpoint_;
}

ACE_INLINE void
TAO_SSLIOP_Endpoint::iiop_endpoint (TAO_IIOP_Endpoint *iiop_endpoint,
                                      bool destroy)
{
  if (iiop_endpoint != 0)
    {
      TAO_IIOP_Endpoint *new_endpoint = 0;

      if (destroy)
        {
          TAO_Endpoint *endpoint = iiop_endpoint->duplicate ();

          new_endpoint = dynamic_cast<TAO_IIOP_Endpoint *> (endpoint);

        }
      else
        new_endpoint = iiop_endpoint;

      if (this->destroy_iiop_endpoint_)
        delete this->iiop_endpoint_;

      this->iiop_endpoint_ = new_endpoint;
      this->destroy_iiop_endpoint_ = destroy;
    }
}

ACE_INLINE const ::SSLIOP::SSL &
TAO_SSLIOP_Endpoint::ssl_component (void) const
{
  return this->ssl_component_;
}

ACE_INLINE ::Security::QOP
TAO_SSLIOP_Endpoint::qop (void) const
{
  return this->qop_;
}

ACE_INLINE ::Security::EstablishTrust
TAO_SSLIOP_Endpoint::trust (void) const
{
  return this->trust_;
}

ACE_INLINE TAO::SSLIOP::OwnCredentials *
TAO_SSLIOP_Endpoint::credentials (void) const
{
  return this->credentials_.in ();
}

ACE_INLINE int
TAO_SSLIOP_Endpoint::credentials_set (void) const
{
  return this->credentials_set_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
