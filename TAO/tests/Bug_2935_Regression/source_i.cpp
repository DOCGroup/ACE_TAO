#include "source_i.h"

Source_i::Source_i (ThreeTier_ptr target)
  : target_ (ThreeTier::_duplicate (target))
{
}

int
Source_i::run ()
{
  try
    {
      for (size_t loop = 0; loop < counter_; ++loop)
        {
          // call a 'nop' method on the server
          target_->tick ();
//        target_->tock ();
        // this sleep can be used give the middle tier a chance
        // to catch up.
//        ACE_OS::sleep (3);
        }
      CORBA::ULong const tick_count = target_->tickCount ();
      CORBA::ULong const tock_count = target_->tockCount ();

      ACE_DEBUG ((LM_DEBUG,
                  "Count of ticks: %u tocks: %u\n",
                  static_cast<unsigned int> (tick_count),
                  static_cast<unsigned int> (tock_count)));

      target_->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("\tSOURCE: Exception");
      return -1;
    }

  return 0;
}

