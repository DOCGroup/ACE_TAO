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

    void dump_results (const char* test_name,
                       const char* sub_test);

    void sample (ACE_hrtime_t sample);

    void accumulate (const Latency_Stats& stats);
    // Useful to merge several Latency_Stats.

  private:
    u_long n_;
    ACE_hrtime_t sum_;
    ACE_hrtime_t sum2_;
    ACE_hrtime_t min_;
    ACE_hrtime_t max_;
  };

  class Throughput_Stats
  {
    // = TITLE
    //   Maintains throughput statistics.
    //
    // = DESCRIPTION
    //   This class is used to keep throughput statistics of the Event
    //   Channel.
    //   The data should be  collected using the High Resolution
    //   timers.
  public:
    Throughput_Stats (void);

    void dump_results (const char* test_name,
                       const char* sub_test);

    void start (void);
    // Start measuring the time.

    void stop (void);
    // The test has completed

    void sample (void);
    // An event has been received

    void accumulate (const Throughput_Stats& stats);
    // Useful to merge several Throughput_Stats.

  private:
    CORBA::ULong n_;
    int done_;
    ACE_hrtime_t start_;
    ACE_hrtime_t stop_;
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
