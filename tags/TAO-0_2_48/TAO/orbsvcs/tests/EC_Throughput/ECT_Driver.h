/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   Abstract base class for the test driver, this let us implement a
//   collocated and a non-collocated test.
//
// ============================================================================

#ifndef ECT_DRIVER_H
#define ECT_DRIVER_H

#include "tao/corba.h"

class ECT_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  virtual ~ECT_Driver (void);

  virtual void shutdown_consumer (void* consumer_cookie,
                                  CORBA::Environment&) = 0;
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.

  void end_to_end (ACE_hrtime_t sample);
  void supplier_to_ec (ACE_hrtime_t sample);
  void inside_ec (ACE_hrtime_t sample);
  void ec_to_consumer (ACE_hrtime_t sample);

  void dump_latency_results (const char* description);

  class Latency_Stats
  {
    // = TITLE
    //   Maintains latency statistics.
    //
    // = DESCRIPTION
    //   This class is used to keep latency statistics of the Event
    //   Channel, in the future we could keep a histogram, but at the
    //   moment we just keep track of minimum, maximum, average and
    //   variance.
    //   The data should be  collected using the High Resolution
    //   timers.
  public:
    Latency_Stats (void);

    void dump_results (const char* description,
                       const char* description2);

    void sample (ACE_hrtime_t sample);

  private:
    u_long n_;
    ACE_hrtime_t sum_;
    ACE_hrtime_t sum2_;
    ACE_hrtime_t min_;
    ACE_hrtime_t max_;
  };

private:
  Latency_Stats end_to_end_;
  Latency_Stats supplier_to_ec_;
  Latency_Stats inside_ec_;
  Latency_Stats ec_to_consumer_;
};

#if defined (__ACE_INLINE__)
#include "ECT_Driver.i"
#endif /* __ACE_INLINE__ */

#endif /* ECT_CONSUMER_H */
