// $Id$

ACE_INLINE
TAO_EC_Kokyu_Factory::TAO_EC_Kokyu_Factory (void)
  :  TAO_EC_Default_Factory (),
     disp_sched_policy_ (ACE_SCHED_FIFO),
     disp_sched_scope_ (ACE_SCOPE_THREAD)
{
}
