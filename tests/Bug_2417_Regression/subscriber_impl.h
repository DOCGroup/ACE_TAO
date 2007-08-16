// $Id$

#ifndef EXPLOIT_PUBLISHER_IMPL_H
#define EXPLOIT_PUBLISHER_IMPL_H

#include "subscriberS.h"

class Subscriber_impl : public POA_Subscriber
{
	public:
		Subscriber_impl(CORBA::ORB_ptr orb);
		~Subscriber_impl();

    virtual void onData (
        ::CORBA::Double value
      );
    virtual ::CORBA::Boolean isAlive (
      );

		void shutdown();
	private:
		CORBA::ORB_var orb_;
		unsigned int count;
};

#endif
