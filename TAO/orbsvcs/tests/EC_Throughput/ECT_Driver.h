/* -*- C++ -*- */
/**
 * @file ECT_Driver.h
 * @brief Base interface for Event Channel throughput test drivers.
 */

#ifndef ECT_DRIVER_H
#define ECT_DRIVER_H

#include "tao/orbconf.h"
#include "ace/Stats.h"
#include "ace/CORBA_macros.h"
#include "ace/Throughput_Stats.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class Environment;
}
TAO_END_VERSIONED_NAMESPACE_DECL

class ECT_Driver
{
public:
  ECT_Driver () = default;
  virtual ~ECT_Driver () = default;

  ECT_Driver (ECT_Driver const&) = delete;
  ECT_Driver (ECT_Driver&&) = delete;
  ECT_Driver& operator= (ECT_Driver const&) = delete;
  ECT_Driver& operator= (ECT_Driver&&) = delete;

  /// Called after a consumer receives shutdown events from all suppliers.
  virtual void shutdown_consumer () = 0;
};

#endif /* ECT_CONSUMER_H */
