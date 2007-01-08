// $Id$

#ifndef EXPLOIT_PUBLISHER_IMPL_H
#define EXPLOIT_PUBLISHER_IMPL_H

#include "publisherS.h"
#include "subscriberC.h"

class Publisher_impl : public POA_Publisher
{
	public:
		Publisher_impl(CORBA::ORB_ptr orb);
		~Publisher_impl();

    virtual void subscribe (
      ::Subscriber_ptr subscriber
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual void shutdown (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

	private:
		CORBA::ORB_var orb_;
		class Worker;
		friend class Worker;
		Worker * worker;
};

#endif
