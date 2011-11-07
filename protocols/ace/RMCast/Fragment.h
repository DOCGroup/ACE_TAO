// $Id$
// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_FRAGMENT_H
#define ACE_RMCAST_FRAGMENT_H

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"
#include "Parameters.h"

namespace ACE_RMCast
{
  class Fragment : public Element
  {
  public:
    Fragment (Parameters const& params);

  public:
    virtual void
    send (Message_ptr m);

    Parameters const& params_;

  private:
    Mutex mutex_;
    u64 sn_;
  };
}

#endif  // ACE_RMCAST_FRAGMENT_H
