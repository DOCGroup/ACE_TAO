// $Id$

#include "Second_i.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "ace/Auto_Event.h"

Second_i::Second_i (CORBA::ORB_ptr orb,
                    CORBA::ULong len, ACE_Auto_Event &two_way_done)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    length_(len),
    two_way_done_(two_way_done)
{

}

Two_Objects_Test::Octet_Seq *
Second_i::twoway_method (void)
{
  Two_Objects_Test::Octet_Seq  *preply_mesg = 0;

  ACE_NEW_THROW_EX (preply_mesg,
                    Two_Objects_Test::Octet_Seq (this->length_),
                    CORBA::NO_MEMORY ());

  Two_Objects_Test::Octet_Seq_var reply_var =
    preply_mesg;
  reply_var->length ( this->length_);

  ACE_DEBUG ((LM_DEBUG,
              "Twoway servant : (%P|%t) two way method called.\n"));

  // @@ This will not work with the latest copy of ACE+TAO :-). Irfan
  // has moved the cache somewhere else. You may want to check the
  // latest code in BiDir cals again ;)
  // @@ Try to use the current_size () instead of total_size ()
  ACE_ASSERT(this->orb_->orb_core ()->lane_resources().
               transport_cache ().current_size () == 1);

  ACE_DEBUG ((LM_DEBUG,
              "Twoway servant : (%P|%t) about to signal one way thread.\n"));

  two_way_done_.signal ();

  return reply_var._retn ();
}

void
Second_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) about to shutdown the orb\n"));
  this->orb_->shutdown (0);
}
