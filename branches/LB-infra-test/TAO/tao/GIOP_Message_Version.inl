// -*- C++ -*-
//$Id$

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

ACE_INLINE
TAO_GIOP_Message_Version::~TAO_GIOP_Message_Version (void)
{
}

ACE_INLINE TAO_GIOP_Message_Version &
TAO_GIOP_Message_Version::operator= (
    const TAO_GIOP_Message_Version &src)
{
  if (this == &src)
    return *this;

  this->major = src.major;
  this->minor = src.minor;
  return *this;
}


ACE_INLINE void
TAO_GIOP_Message_Version::set_version (CORBA::Octet maj,
				       CORBA::Octet min)
{
  this->major = maj;
  this->minor = min;
}

ACE_INLINE int
TAO_GIOP_Message_Version::operator== (
    const TAO_GIOP_Message_Version &src)
{
  return this->major == src.major && this->minor == src.minor;
}

ACE_INLINE int
TAO_GIOP_Message_Version::operator!= (
    const TAO_GIOP_Message_Version &src)
{
  return !(*this == src);
}

