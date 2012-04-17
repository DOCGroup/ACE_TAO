// $Id$
// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_PARAMETERS_H
#define ACE_RMCAST_PARAMETERS_H

#include "ace/Time_Value.h"


namespace ACE_RMCast
{
  class Parameters
  {
  public:
    Parameters (
      // Loss and reordering simulator.
      //
      bool simulator = false,

      // MTU(1500) - IP-hdr - UDP-hdr
      //
      unsigned short max_packet_size = 1470,

      // How often various worker threads wakeup to do
      // cacellation checks, queue run-throughs, etc.
      //
      ACE_Time_Value const& tick  = ACE_Time_Value (0, 2000),

      // How long to wait, in ticks, before sending NAK.
      //
      unsigned long nak_timeout = 1,

      // How long to wait, in ticks, before sending NRTM.
      //
      unsigned long nrtm_timeout = 10,

      // How long to retain a message for retransmission, in ticks.
      //
      unsigned long retention_timeout = 500,  // 1 sec

      size_t addr_map_size = 50
    )
        : simulator_ (simulator),
          max_packet_size_ (max_packet_size),
          tick_ (tick),
          nak_timeout_ (nak_timeout),
          nrtm_timeout_ (nrtm_timeout),
          retention_timeout_ (retention_timeout),
          addr_map_size_(addr_map_size)
    {
    }

  public:
    bool
    simulator () const
    {
      return simulator_;
    }

    unsigned short
    max_packet_size () const
    {
      return max_packet_size_;
    }

    ACE_Time_Value const&
    tick () const
    {
      return tick_;
    }

    unsigned long
    nak_timeout () const
    {
      return nak_timeout_;
    }

    unsigned long
    nrtm_timeout () const
    {
      return nrtm_timeout_;
    }

    unsigned long
    retention_timeout () const
    {
      return retention_timeout_;
    }

    size_t
    addr_map_size () const
    {
      return addr_map_size_;
    }

  private:
    bool simulator_;
    unsigned short max_packet_size_;
    ACE_Time_Value tick_;
    unsigned long nak_timeout_;
    unsigned long nrtm_timeout_;
    unsigned long retention_timeout_;
    size_t addr_map_size_;
  };
}


#endif  // ACE_RMCAST_PARAMETERS_H
