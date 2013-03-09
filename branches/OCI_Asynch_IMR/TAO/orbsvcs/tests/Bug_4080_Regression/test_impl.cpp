//
// $Id$
//

#include "test_impl.h"

UIPMC_Object_Impl::UIPMC_Object_Impl (CORBA::ORB_ptr orb,
                                      CORBA::ULong num_threads)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , barrier_ ((unsigned int) num_threads)
  , lock_ ()
{
}

UIPMC_Object_Impl::~UIPMC_Object_Impl (void)
{
}

void
UIPMC_Object_Impl::process ()
{
  ACE_DEBUG ((LM_DEBUG, "Server %t received a message\n"));
  this->barrier_.wait ();
  static bool shutdown=false;
  if (!shutdown)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
      if (!shutdown)
        {
          shutdown=true;
          ACE_DEBUG ((LM_DEBUG, "Server %t Shutting down\n"));
          try
            {
              this->orb_->shutdown (0);
            }
          catch (const CORBA::Exception& ex)
            {
              ex._tao_print_exception (
                "Server %t, Exception caught in shutdown():");
            }
        }
    }
}
