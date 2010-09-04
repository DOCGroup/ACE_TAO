//$Id$
#include "source_i.h"

Source_i::Source_i (ThreeTier_ptr target)
  : target_(ThreeTier::_duplicate(target))
  , counter_(5)//1000000)
{
}

Source_i::~Source_i ()
{
}

bool
Source_i::parse_args(int, ACE_TCHAR *[])
{
  return true;
}

int
Source_i::run ()
{
  try
    {
      for(size_t loop = 0; loop < counter_; ++loop)
      {
        // call a 'nop' method on the server
        target_->tick();
//        target_->tock();
        // this sleep can be used give the middle tier a chance
        // to catch up.
//        ACE_OS::sleep(3);
      }
      unsigned long tickCount = target_->tickCount();
      unsigned long tockCount = target_->tockCount();

      ACE_DEBUG ((LM_DEBUG,
        "Count of ticks: %d tocks: %d\n",
                  (int)tickCount, (int)tockCount));

      target_->shutdown ();

    }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("\tSOURCE: Exception");
    return -1;
  }

  return 0;
}

