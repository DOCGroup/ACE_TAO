// $Id$

ACE_INLINE TAO_NS_Object_Id
TAO_NS_ID_Factory::id (void)
{
  return ++seed_;
}
