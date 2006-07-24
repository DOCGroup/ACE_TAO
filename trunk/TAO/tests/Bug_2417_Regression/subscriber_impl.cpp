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
Subscriber_impl::onData (
      ::CORBA::Double
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ))
{
	++count;
	if (count > 100)
		shutdown();
}

::CORBA::Boolean
Subscriber_impl::isAlive (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ))
{
	return true;
}

void Subscriber_impl::shutdown()
{
	this->orb_->shutdown (0);
}

