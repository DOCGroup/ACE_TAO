// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#include "ace/OS_NS_time.h"
#include "Simulator.h"

namespace ACE_RMCast
{
  Simulator::Simulator ()
  {
    ACE_OS::srand ((unsigned int)ACE_OS::time (0));
  }

  void Simulator::send (Message_ptr m)
  {
    // Note: Simulator may work in unpredictable ways mainly due
    // to the "reliable loopback" mechanism.
    //
    out_->send (m);
    return;

    /*int r (rand ());

    if ((r % 3) == 0) return;

    Lock l (mutex_);

    if (hold_.get ())
    {
      out_->send (m);
      out_->send (hold_);
    }
    else
    {
      hold_ = m;
    }*/
  }
}
