// $Id$
// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_FLOW_H
#define ACE_RMCAST_FLOW_H

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"
#include "Parameters.h"

namespace ACE_RMCast
{
  class Flow : public Element
  {
  public:
    Flow (Parameters const& );

  public:
    virtual void
    send (Message_ptr m);

    virtual void
    recv (Message_ptr m);

  private:
    // Parameters const& params_;

    Mutex mutex_;
    ACE_Time_Value nak_time_;

    // Throughput sampling.
    //
    ACE_Time_Value sample_start_time_;
    unsigned long sample_bytes_;
    double current_tput_;
    double cap_tput_;
  };
}


#endif  // ACE_RMCAST_FLOW_H
