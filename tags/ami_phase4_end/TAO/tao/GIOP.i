// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE
TAO_GIOP_LocateRequestHeader::TAO_GIOP_LocateRequestHeader (void)
  : request_id (0)
{
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Version::TAO_GIOP_Version (const TAO_GIOP_Version &src)
  : major (src.major),
    minor (src.minor)
{
}

ACE_INLINE
TAO_GIOP_Version::TAO_GIOP_Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{
}

ACE_INLINE
TAO_GIOP_Version::~TAO_GIOP_Version (void)
{
}

ACE_INLINE TAO_GIOP_Version &
TAO_GIOP_Version::operator= (const TAO_GIOP_Version &src)
{
  if (this == &src)
    return *this;

  this->major = src.major;
  this->minor = src.minor;
  return *this;
}

ACE_INLINE void
TAO_GIOP_Version::set_version (CORBA::Octet maj, CORBA::Octet min)
{
  this->major = maj;
  this->minor = min;
}

ACE_INLINE int
TAO_GIOP_Version::operator== (const TAO_GIOP_Version &src)
{
  return this->major == src.major && this->minor == src.minor;
}

ACE_INLINE int
TAO_GIOP_Version::operator!= (const TAO_GIOP_Version &src)
{
  return !(*this == src);
}

// ****************************************************************

ACE_INLINE void
TAO_GIOP_Message_State::reset (void)
{
  this->message_size = 0;
  this->current_offset = 0;
  this->more_fragments = 0;
  this->cdr.reset_contents ();
}

ACE_INLINE int
TAO_GIOP_Message_State::header_received (void) const
{
  return this->message_size != 0;
}
