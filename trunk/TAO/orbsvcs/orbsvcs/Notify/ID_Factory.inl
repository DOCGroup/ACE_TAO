// $Id$

ACE_INLINE TAO_NS_Object::ID
TAO_NS_ID_Factory::id (void)
{
  return ++seed_;
}
