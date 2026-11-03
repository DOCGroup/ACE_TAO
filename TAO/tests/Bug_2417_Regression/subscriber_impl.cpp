#include "subscriber_impl.h"

Subscriber_impl::Subscriber_impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , count_ {}
{
}

void
Subscriber_impl::onData (::CORBA::Double)
{
  unsigned int const count = ++this->count_;
  if (count > 100)
    this->shutdown ();
}

::CORBA::Boolean
Subscriber_impl::isAlive ()
{
  return true;
}

void
Subscriber_impl::shutdown ()
{
  this->orb_->shutdown (false);
}
