// -*- C++ -*-
// $Id$

ACE_INLINE const TAO::ObjectKey &
TAO_DIOP_Profile::object_key (void) const
{
  return this->object_key_;
}

ACE_INLINE TAO::ObjectKey *
TAO_DIOP_Profile::_key (void) const
{
  TAO::ObjectKey *key = 0;

  ACE_NEW_RETURN (key,
                  TAO::ObjectKey (this->object_key_),
                  0);

  return key;
}




