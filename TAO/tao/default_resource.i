// $Id$

ACE_INLINE void
TAO_Default_Resource_Factory::resource_source (int which_source)
{
  resource_source_ = which_source;
}

ACE_INLINE int
TAO_Default_Resource_Factory::resource_source (void)
{
  return resource_source_;
}
