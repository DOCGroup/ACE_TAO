/* -*- C++ -*- */
// $Id$

ACE_INLINE
int
ACE_Object_Manager::cleanup (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param,
                             int thread_lifetime)
{
  return ACE_Object_Manager::instance ()->cleanup_i (object,
                                                     cleanup_hook,
                                                     param,
                                                     thread_lifetime);
}
