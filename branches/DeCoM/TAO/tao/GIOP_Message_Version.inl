// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_GIOP_Message_Version::TAO_GIOP_Message_Version (
    const TAO_GIOP_Message_Version &src)
  : major (src.major),
    minor (src.minor)
{
}

ACE_INLINE
TAO_GIOP_Message_Version::TAO_GIOP_Message_Version (CORBA::Octet maj,
                                                    CORBA::Octet min)
  : major (maj),
    minor (min)
{
}

ACE_INLINE TAO_GIOP_Message_Version &
TAO_GIOP_Message_Version::operator= (const TAO_GIOP_Message_Version &src)
{
  if (this != &src)
    {
      this->major = src.major;
      this->minor = src.minor;
    }

  return *this;
}


ACE_INLINE void
TAO_GIOP_Message_Version::set_version (CORBA::Octet maj, CORBA::Octet min)
{
  this->major = maj;
  this->minor = min;
}

ACE_INLINE bool
TAO_GIOP_Message_Version::operator== (const TAO_GIOP_Message_Version &src) const
{
  return this->minor == src.minor && this->major == src.major;
}

ACE_INLINE bool
TAO_GIOP_Message_Version::operator!= (const TAO_GIOP_Message_Version &src) const
{
  return !(*this == src);
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Version::major_version (void) const
{
  return this->major;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Version::minor_version (void) const
{
  return this->minor;
}

TAO_END_VERSIONED_NAMESPACE_DECL
