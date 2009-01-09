// $Id$

#include "subscriber_impl.h"
#include "ace/OS_NS_stdio.h"

Subscriber_impl::Subscriber_impl(CORBA::ORB_ptr orb)
:  orb_ (CORBA::ORB::_duplicate (orb))
, count(0)
{
}

Subscriber_impl::~Subscriber_impl()
{
}

void
Subscriber_impl::onData (::CORBA::Double)
{
  ++count;
  if (count > 100)
    this->shutdown();
}

::CORBA::Boolean
Subscriber_impl::isAlive ()
{
  return true;
}

void Subscriber_impl::shutdown()
{
  this->orb_->shutdown (0);
}
