//
// $Id$
//
#include "Hello.h"
#include "ace/OS_NS_unistd.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)), sleep_sec_(1)
{
}

CORBA::Short
Hello::sleep_sec()
{
  return this->sleep_sec_;
}

void
  Hello::sleep_sec(CORBA::Short sl_sec)
{
  this->sleep_sec_ = sl_sec;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Hello::sleep_sec returning\n"));
}

char *
Hello::get_string (CORBA::Long client_num)
{
  ACE_DEBUG ((LM_INFO,
    ACE_TEXT ("TAO (%P|%t) - Server: Sleeping for client->%d....\n "),
              client_num));

  ACE_OS::sleep(this->sleep_sec_);
  ACE_DEBUG ((LM_INFO,
    ACE_TEXT ("TAO (%P|%t) - Server: Returning for client->%d....\n "),
              client_num));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
