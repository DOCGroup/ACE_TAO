// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_SIMULATOR_H
#define ACE_RMCAST_SIMULATOR_H

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"

namespace ACE_RMCast
{
  class Simulator : public Element
  {
  public:
    Simulator ();

  public:
    void
    send (Message_ptr m) override;

  private:
    Message_ptr hold_;
    Mutex mutex_;
  };
}


#endif  // ACE_RMCAST_SIMULATOR_H
