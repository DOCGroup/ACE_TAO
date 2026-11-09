
#include "test_impl.h"

UIPMC_Object_Impl::UIPMC_Object_Impl (CORBA::ORB_ptr orb,
                                      CORBA::ULong num_threads)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , barrier_ (static_cast<unsigned int> (num_threads))
{
}

void
UIPMC_Object_Impl::process ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server %t received a message\n")));
  this->barrier_.wait ();
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
  if (!this->shutdown_)
    {
      this->shutdown_ = true;
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server %t Shutting down\n")));
      try
        {
          this->orb_->shutdown (false);
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (
            "Server %t, Exception caught in shutdown():");
        }
    }
}
