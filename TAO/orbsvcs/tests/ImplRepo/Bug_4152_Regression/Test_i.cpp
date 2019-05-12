/* -*- C++ -*-  */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

Test_i::Test_i (CORBA::ORB_ptr orb) : orb_ (CORBA::ORB::_duplicate(orb))
{
}

CORBA::Short
Test_i::get_server_num (void)
{
  return 1;
}

void
Test_i::terminate (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server received terminate and going to exit\n"));
  exit (0);
}

void
Test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server received shutdown and going to exit\n"));
  orb_->shutdown ();
}
