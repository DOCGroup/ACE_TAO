/* -*- C++ -*- */
// $Id$

ACE_INLINE
int
ACE_Object_Manager::cleanup (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param,
                             ACE_hthread_t thread_handle)
{
  return ACE_Object_Manager::instance ()->cleanup_i (object,
                                                     cleanup_hook,
                                                     param,
                                                     thread_handle);
}
