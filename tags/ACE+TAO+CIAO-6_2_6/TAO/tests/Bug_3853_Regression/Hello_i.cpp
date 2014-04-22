// $Id$

#include "Hello_i.h"
#include "ace/OS_NS_unistd.h"

// Implementation skeleton constructor
Demo_HelloWorld_i::Demo_HelloWorld_i (CORBA::ORB_ptr orb)
: orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Implementation skeleton destructor
Demo_HelloWorld_i::~Demo_HelloWorld_i (void)
{
}

void Demo_HelloWorld_i::sayHello (const char * msg)
{
  ACE_OS::sleep(1);
  // Add your implementation here
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%P|%t)Demo_HelloWorld_i::sayHello %C\n"), msg));
}

void
Demo_HelloWorld_i::shutdown()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)Demo_HelloWorld_i::shutdown \n"));
  this->orb_->shutdown (0);
}


