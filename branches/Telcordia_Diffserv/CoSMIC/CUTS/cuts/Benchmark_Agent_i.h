// -*- C++ -*-

//=============================================================================
/**
 * @file    Benchmark_Agent_i.h
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BENCHMARK_AGENT_T_H_
#define _CUTS_BENCHMARK_AGENT_T_H_

#include "cuts/config.h"
#include "cuts/CUTS_stub_export.h"
#include "cuts/Benchmark_Agent.h"
#include "cuts/CUTSS.h"

//=============================================================================
/**
 * @class Benchmark_Agent_i
 */
//=============================================================================

class CUTS_STUB_Export Benchmark_Agent_i :
  virtual public ::POA_CUTS::Benchmark_Agent,
  public CUTS_Benchmark_Agent
{
public:
  /// Constructor.
  Benchmark_Agent_i (void);

  /// Destructor.
  virtual ~Benchmark_Agent_i (void);

  /// Get the performance data from the Benchmark_Agent
  virtual ::CUTS::Benchmark_Data *  collect_performance_data (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));
};

#if defined (__CUTS_INLINE__)
#include "cuts/Benchmark_Agent_i.inl"
#endif

#endif  // !defined _CUTS_BENCHMARK_AGENT_T_H_
