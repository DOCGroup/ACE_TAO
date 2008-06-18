// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
int
TAO_Singleton_Manager::at_exit (ACE_Cleanup *object,
                                void *param)
{
  return TAO_Singleton_Manager::instance ()->at_exit_i (
    object,
    (ACE_CLEANUP_FUNC) ACE_CLEANUP_DESTROYER_NAME,
    param);
}

ACE_INLINE
int
TAO_Singleton_Manager::at_exit (void *object,
                                ACE_CLEANUP_FUNC cleanup_hook,
                                void *param)
{
  return TAO_Singleton_Manager::instance ()->at_exit_i (
    object,
    cleanup_hook,
    param);
}

TAO_END_VERSIONED_NAMESPACE_DECL
