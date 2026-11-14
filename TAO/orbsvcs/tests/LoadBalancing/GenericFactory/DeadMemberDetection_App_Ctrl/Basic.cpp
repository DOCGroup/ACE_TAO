#include "Basic.h"
#include "LB_server.h"
#include "ace/OS_NS_unistd.h"

Basic::Basic (int server_id)
  : server_id_ (server_id)
{
}

Basic::Basic (CORBA::Object_ptr object_group,
              CosLoadBalancing::LoadManager_ptr lm,
              CORBA::ORB_ptr orb,
              std::string const &location,
              int server_id,
              int pong_delay)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    location_ (location),
    server_id_ (server_id),
    pong_delay_ (pong_delay)
{
  this->object_group_ = CORBA::Object::_duplicate (object_group);
  this->lm_ = CosLoadBalancing::LoadManager::_duplicate (lm);
}

CORBA::Boolean
Basic::_non_existent ()
{
  if (this->pong_delay_ > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t)%T Basic::_non_existent server %d sleep %d seconds ...\n"),
                this->server_id_,
                this->pong_delay_));

  for (int i {}; i < this->pong_delay_; ++i)
    ACE_OS::sleep (1);

  return false;
}

char *
Basic::get_string ()
{
  return CORBA::string_dup (this->location_.c_str ());
}

void
Basic::remove_member ()
{
  try
    {
      PortableGroup::Location location (1);
      location.length (1);
      location[0].id = CORBA::string_dup (this->location_.c_str ());
      this->lm_->remove_member (this->object_group_.in (), location);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t)%T - Removed Member at Location <%C>\n"),
                  this->location_.c_str ()));
    }
  catch (const PortableGroup::ObjectNotFound &ex)
    {
      ex._tao_print_exception ("Caught exception in remove_member");
      throw CORBA::INTERNAL ();
    }
  catch (const PortableGroup::MemberNotFound &ex)
    {
      ex._tao_print_exception ("Caught exception in remove_member");
      throw CORBA::INTERNAL ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception caught while destroying member\n");
    }
}

void
Basic::shutdown ()
{
  this->orb_->shutdown (false);
}

void
Basic::exit ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t)%T - Basic::exit server %d exit\n"),
              this->server_id_));
  ACE_OS::exit (0);
}
