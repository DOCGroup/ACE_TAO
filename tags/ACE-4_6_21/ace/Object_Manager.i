/* -*- C++ -*- */
// $Id$

ACE_INLINE
int
ACE_Object_Manager::at_exit (ACE_Cleanup *object,
                             void *param)
{
  return ACE_Object_Manager::instance ()->at_exit_i (
    object,
    (ACE_CLEANUP_FUNC) ace_cleanup_destroyer,
    param);
}

ACE_INLINE
int
ACE_Object_Manager::at_exit (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param)
{
  return ACE_Object_Manager::instance ()->at_exit_i (
    object,
    cleanup_hook,
    param);
}
