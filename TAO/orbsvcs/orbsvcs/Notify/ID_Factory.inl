// $Id$

ACE_INLINE TAO_Notify_Object::ID
TAO_Notify_ID_Factory::id (void)
{
  return ++seed_;
}
