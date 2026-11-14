// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#include "Protocol.hpp"

namespace ACE_TMCast
{
  class FaultDetector
  {
  public:
    FaultDetector () = default;

  public:
    class Failed {};


    void
    insync ()
    {
      if (alone_)
        alone_ = false;

      silence_period_ = 0;
    }

    void
    outsync ()
    {
      if (!alone_ && ++silence_period_ >= Protocol::FATAL_SILENCE_FRAME)
      {
        // cerr << "Silence period has been passed." << endl;
        // cerr << "Decalring the node failed." << endl;
        throw Failed ();
      }
    }

  private:
    bool alone_ {true}; // true if we haven't heard from any members yet.
    short silence_period_ {-1};
  };
}
