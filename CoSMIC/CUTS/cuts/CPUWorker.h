// -*- C++ -*-

//=============================================================================
/**
 * @file    CPUWorker.h
 *
 * $Id$
 *
 * This file contains the default CPU worker in CUTS.
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_CPU_WORKER_H_
#define _CUTS_CPU_WORKER_H_

#include "cuts/Action.h"
#include "cuts/Action_T.h"
#include "cuts/Worker_T.h"
#include "cuts/Worker.h"
#include "cuts/WML_Macros.h"

//=============================================================================
/**
 * @class CUTS_CPU_Worker
 *
 * @brief Basic worker that burns CPU cycles.
 */
//=============================================================================

class CUTS_Export CUTS_CPU_Worker :
  public CUTS_Worker_T <CUTS_CPU_Worker>,
  public CUTS_Worker
{
public:
  //@@CUTS::WML
  //===========================================================================
  CUTS_ACTION_DECLARE (CUTS_CPU_Worker, Run_Processor)
  {
  public:
    CUTS_ACTION_DEFAULT_CONSTRUCTOR (CUTS_CPU_Worker, Run_Processor)
    {

    }

    CUTS_ACTION_DESTRUCTOR (Run_Processor)
    {

    }

    CUTS_ACTION_EXECUTE_NO_ARGS (process);
    CUTS_DECLARE_ACTION_FACTORY (CUTS_CPU_Worker, Run_Processor);
  };
  //===========================================================================
  //@@CUTS::WML

public:
  /// Constructor.
  CUTS_CPU_Worker (void);

  /// Destructor.
  virtual ~CUTS_CPU_Worker (void);

  /// Run the CPU for the given number of repititions.
  virtual void process (void);

  /**
   * Get the number of visits handled by this CPU worker.
   */
  virtual long counter (void) const;

private:
  CUTS_ACTION_TABLE_DECLARE ();

  /// Number of visits completed by the CPU worker.
  long visits_;
};

//=============================================================================
/*
 * template <>
 * CUTS_Worker_Traits <CUTS_CPU_Worker>
 */
//=============================================================================

template <>
class CUTS_Worker_Traits <CUTS_CPU_Worker>
{
public:
  static const long worker_id_ = 2;
};

//=============================================================================
/*
 * template <>
 * CUTS_Action_Traits <CUTS_CPU_Worker::*>
 */
//=============================================================================

template <>
class CUTS_Action_Traits <CUTS_CPU_Worker::Run_Processor>
{
public:
  static const long action_id_ = 1;
};

#if defined (__CUTS_INLINE__)
#include "cuts/CPUWorker.inl"
#endif

#endif  // !defined _CUTS_CPU_WORKER_H_
