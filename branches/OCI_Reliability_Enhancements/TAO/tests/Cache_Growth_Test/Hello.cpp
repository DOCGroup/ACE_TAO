//
// $Id$
//
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  if (this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size () > 1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) The size is growing\n"));
    }

  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
