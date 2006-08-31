// -*- C++ -*-

//=============================================================================
/**
 * @file    MemoryWorker.h
 *
 * $Id$
 *
 * This file contains the default memory worker in CUTS.
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_MEMORY_WORKER_H_
#define _CUTS_MEMORY_WORKER_H_

#include "cuts/Worker.h"
#include "cuts/Worker_T.h"
#include "cuts/Action.h"
#include "cuts/Action_T.h"
#include "cuts/WML_Macros.h"
#include "ace/Thread_Mutex.h"
#include <list>

class CUTS_Export CUTS_Memory_Worker :
  public CUTS_Worker_T <CUTS_Memory_Worker>,
  public CUTS_Worker
{
public:
  //@@CUTS::WML
  //===========================================================================
  CUTS_ACTION_DECLARE (CUTS_Memory_Worker, Allocate_Memory)
  {
  public:
    CUTS_ACTION_DEFAULT_CONSTRUCTOR (CUTS_Memory_Worker, Allocate_Memory)
      CUTS_ACTION_PARAMETER_INIT (kilobytes_, 0)
    {

    }

    CUTS_ACTION_INIT_CONSTRUCTOR_BEGIN (CUTS_Memory_Worker, Allocate_Memory)
      CUTS_ACTION_ARGUMENT (size_t, kilobytes)
      CUTS_ACTION_INIT_CONSTRUCTOR_END (CUTS_Memory_Worker)
      CUTS_ACTION_PARAMETER_INIT (kilobytes_, kilobytes)
    {

    }

    CUTS_ACTION_DESTRUCTOR (Allocate_Memory)
    {

    }

    CUTS_ACTION_EXECUTE (allocate,
                         (this->kilobytes_));


    CUTS_DECLARE_ACTION_FACTORY (CUTS_Memory_Worker, Allocate_Memory);

    CUTS_ACTION_PARAMETER_LIST_BEGIN
    CUTS_ACTION_PARAMETER_DECLARE (int, kilobytes_);
    CUTS_ACTION_PARAMETER_LIST_END
  };

  CUTS_ACTION_DECLARE (CUTS_Memory_Worker, Deallocate_Memory)
  {
  public:
    /// Constructor.
    Deallocate_Memory (CUTS_Memory_Worker & worker)
      : CUTS_Action_T <CUTS_Memory_Worker> (worker),
        kilobytes_ (0)
    {

    }

    Deallocate_Memory (CUTS_Memory_Worker & worker, size_t kilobytes)
      : CUTS_Action_T <CUTS_Memory_Worker> (worker),
        kilobytes_ (kilobytes)
    {

    }

    virtual ~Deallocate_Memory (void)
    {

    }

    /// Functor operator to perform deallocation on target worker.
    virtual void execute (void) const
    {
      this->parent_.deallocate (this->kilobytes_);
    }

    CUTS_DECLARE_ACTION_FACTORY (CUTS_Memory_Worker, Deallocate_Memory);

  private:
    /// Size of memory operation.
    size_t kilobytes_;
  };
  //===========================================================================
  //@@CUTS::WML

  /// Constructor.
  CUTS_Memory_Worker (void);

  /// Destructor.
  virtual ~CUTS_Memory_Worker (void);

  /// Perform memory allocation.
  virtual void allocate (size_t kilobytes);

  /// Perform memory deallocation.
  virtual void deallocate (size_t kilobytes);

  virtual long counter (void) const;

private:
  CUTS_ACTION_TABLE_DECLARE ();

  /// Type definition for the container of allocations.
  typedef std::list <char *> Memory_Allocations;

  /// Locking mechanism for protecting <memory_>.
  ACE_Thread_Mutex lock_;

  /// Collection of memory allocations.
  Memory_Allocations memory_;
};

//=============================================================================
/**
 * CUTS_Worker_Traits <CUTS_Memory_Worker>
 */
//=============================================================================

template <>
class CUTS_Worker_Traits <CUTS_Memory_Worker>
{
public:
  static const long worker_id_ = 1;
};

//=============================================================================
/**
 * CUTS_Action_Traits < >
 */
//=============================================================================

template <>
class CUTS_Action_Traits <CUTS_Memory_Worker::Allocate_Memory>
{
public:
  static const long action_id_ = 1;
};

template <>
class CUTS_Action_Traits <CUTS_Memory_Worker::Deallocate_Memory>
{
public:
  static const long action_id_ = 2;
};

#if defined (__CUTS_INLINE__)
#include "cuts/MemoryWorker.inl"
#endif

#endif  // !defined _CUTS_MEMORY_WORKER_H_
