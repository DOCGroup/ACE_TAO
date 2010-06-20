// $Id$
#ifndef DEPLOYMENT_EVENT_INL
#define DEPLOYMENT_EVENT_INL

namespace DAnCE
{
  ACE_INLINE
  Event_Result::Event_Result (void)
    : id_ (""),
      exception_ (true)
  {
  }

  ACE_INLINE
  Event_Result::Event_Result (const std::string &id,
                              bool exception)
    : id_ (id),
      exception_ (exception)
  {
  }

  ACE_INLINE
  Event_Result::Event_Result (const std::string &id,
                              bool exception,
                              ::CORBA::Any *any)
    : id_ (id),
      exception_ (exception),
      contents_ (any)
  {
  }

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
