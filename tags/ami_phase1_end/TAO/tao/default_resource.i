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

ACE_INLINE void
TAO_Default_Resource_Factory::poa_source (int which_source)
{
  poa_source_ = which_source;
}

ACE_INLINE int
TAO_Default_Resource_Factory::poa_source (void)
{
  return poa_source_;
}

ACE_INLINE int
TAO_Default_Resource_Factory::reactor_lock (void)
{
  return reactor_lock_;
}
