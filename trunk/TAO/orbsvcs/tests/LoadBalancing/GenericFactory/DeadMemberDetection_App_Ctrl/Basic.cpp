// $Id$

#include "Basic.h"
#include "LB_server.h"
#include "ace/OS.h"

extern int pong_delay;

Basic::Basic (int server_id)
: server_id_ (server_id)
{
}

Basic::Basic (CORBA::Object_ptr object_group,
              CosLoadBalancing::LoadManager_ptr lm,
              CORBA::ORB_ptr orb,
              const char *loc,
              int server_id)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    server_id_ (server_id)
{
  this->object_group_ = CORBA::Object::_duplicate (object_group);
  this->lm_ = CosLoadBalancing::LoadManager::_duplicate (lm);
  this->location_ = CORBA::string_dup (loc);
}


/// Default _non_existent: always returns false.
CORBA::Boolean
Basic::_non_existent (void)
{
  if (pong_delay > 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T Basic::_non_existent server %d sleep %d seconds ... \n",
      server_id_, pong_delay));
  int i = 0;
  while (i < pong_delay)
  {
    ++i;
    ACE_OS::sleep (1);
  }
  return false;
}

char *
Basic::get_string (void)
{
  return CORBA::string_dup (this->location_.in ());
}

void
Basic::remove_member (void)
{
  try
    {
      PortableGroup::Location location (1);
      location.length (1);
      location[0].id = CORBA::string_dup (this->location_.in ());
      this->lm_->remove_member (this->object_group_.in (),
                                location);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Removed Member at Location <%s>\n",
                      this->location_.in ()));

    }
  catch (const PortableGroup::ObjectNotFound& ex)
    {
      ex._tao_print_exception ("Caught exception in remove_member");
      throw CORBA::INTERNAL ();
    }
  catch (const PortableGroup::MemberNotFound& ex)
    {
      ex._tao_print_exception ("Caught exception in remove_member");
      throw CORBA::INTERNAL ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught while destroying member\n");
    }

}

void
Basic::shutdown (void)
{
  this->orb_->shutdown (0);
}

void
Basic::exit (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Basic::exit  server %d exit \n",
                      this->server_id_));

  ACE_OS::exit (0);
}

