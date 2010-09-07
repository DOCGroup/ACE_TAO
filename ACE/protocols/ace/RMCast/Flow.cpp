// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#include "Flow.h"

#include "ace/OS_NS_unistd.h"   // sleep
#include "ace/OS_NS_sys_time.h" // gettimeofday

#include "ace/os_include/os_math.h" // exp

namespace ACE_RMCast
{
  Flow::
  Flow (Parameters const& params)
      : params_ (params),
        nak_time_ (0, 0),
        sample_start_time_ (0, 0),
        sample_bytes_ (0),
        current_tput_ (0.0),
        cap_tput_ (0.0)
  {
  }

  void Flow::send (Message_ptr m)
  {
    if (Data const* data = static_cast<Data const*> (m->find (Data::id)))
    {
      ACE_Time_Value now_time (ACE_OS::gettimeofday ());

      Lock l (mutex_);
      sample_bytes_ += data->size ();

      if (sample_start_time_ == ACE_Time_Value (0, 0))
      {
        sample_start_time_ = now_time;
      }
      else
      {
        ACE_Time_Value delta (now_time - sample_start_time_);

        if (delta > ACE_Time_Value (0, 2000))
        {
          current_tput_ =
            double (sample_bytes_) / (delta.sec () * 1000000 + delta.usec ());

          // cerr << "tput: " << current_tput_ << " bytes/usec" << endl;

          sample_bytes_ = 0;
          sample_start_time_ = ACE_Time_Value (0, 0);
        }
      }

      if (cap_tput_ != 0.0
          && current_tput_ != 0.0
          && current_tput_ > cap_tput_)
      {
        double dev = (current_tput_ - cap_tput_) / current_tput_;

        // cerr << "deviation: " << dev << endl;

        // Cap decay algorithm.
        //
        {
          ACE_Time_Value delta (now_time - nak_time_);

          unsigned long msec = delta.msec ();

          double x = msec / -16000.0;
          double y = 1.0 * exp (x);
          cap_tput_ = cap_tput_ / y;

          // cerr << "cap decay: " << cap_tput_ << " bytes/usec" << endl;
        }

        l.release ();


        timespec time;
        time.tv_sec = 0;
        time.tv_nsec = static_cast<unsigned long> (dev * 500000.0);

        // Don't bother to sleep if the time is less than 10 usec.
        //
        if (time.tv_nsec > 10000)
          ACE_OS::sleep (ACE_Time_Value (time));
      }
    }

    out_->send (m);
  }

  void Flow::recv (Message_ptr m)
  {
    if (NAK const* nak = static_cast<NAK const*> (m->find (NAK::id)))
    {
      Address to (static_cast<To const*> (m->find (To::id))->address ());

      if (nak->address () == to)
      {
        // This one is for us.
        //

        //cerr << "NAK from "
        //     << static_cast<From const*> (m->find (From::id))->address ()
        //     << " for " << nak->count () << " sns." << endl;


        ACE_Time_Value nak_time (ACE_OS::gettimeofday ());

        Lock l (mutex_);

        nak_time_ = nak_time;

        if (cap_tput_ == 0.0)
          cap_tput_ = current_tput_;

        if (cap_tput_ != 0.0)
        {
          cap_tput_ = cap_tput_ - cap_tput_ / 6.0;

          // cerr << "cap: " << cap_tput_ << " bytes/usec" << endl;
        }
      }
    }

    in_->recv (m);
  }
}

