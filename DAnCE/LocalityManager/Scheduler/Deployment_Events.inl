// $Id$

#ifndef DEPLOYMENT_EVENTS_INL
#define DEPLOYMENT_EVENTS_INL

namespace DAnCE
{
  ACE_INLINE
  Deployment_Event::Deployment_Event (Event_Future holder,
                                      const char *inst_type)
    : holder_ (holder),
      instance_type_ (inst_type)
  {
  }

  ACE_INLINE
  Event_Future
  Deployment_Event::get_future (void)
  {
    return holder_;
  }
}

#endif

