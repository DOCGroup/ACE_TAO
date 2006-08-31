// -*- C++ -*-

//=============================================================================
/**
 * @file    Worker.h
 *
 * $Id$
 *
 * This file contains the base class for all workers in CUTS.
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_WORKER_H_
#define _CUTS_WORKER_H_

#include "cuts/config.h"
#include "cuts/Action_Iterator.h"

// forward declarations
class CUTS_Action;
class CUTS_Action_Iterator;

//=============================================================================
/**
 * @class CUTS_Worker
 *
 * @brief Abstact base class for worker objects.
 *
 * This class contains methods that are common across all worker objects
 * in CUTS.
 */
//=============================================================================

class CUTS_Export CUTS_Worker
{
public:
  /// Get the parent of the worker.
  long parent (void) const;

  /// Set the parent of the worker.
  void parent (long parent);

  /**
   * Get worker specific integral information. The meaning of
   * this value is dependent on the worker. Please consult the
   * documentation for the particular worker class to get the
   * meaning of this value returned. By default, if the class
   * does not define this file, its return value is 0.
   *
   * @return    Worker dependent integer value; 0 by default.
   */
  virtual long counter (void) const;

  /**
   * Factory operation for creating actions. If creation of the
   * action fails, 0 is returned.
   *
   * @param[in]   action_name     Name of the action.
   * @return      Pointer to the action.
   */
  virtual CUTS_Action * create_action (const char * action_name);

  /**
   * Get the number of actions supported by the worker.
   *
   * @return      The number of actions.
   */
  virtual size_t action_count (void) const;

  /**
   * Get an iterator to the first action of the worker. The iterator
   * returned is dynamically allocated.
   *
   * @return     Pointer to an iterator.
   */
  virtual CUTS_Action_Iterator * begin (void);

protected:
  /// Constructor.
  CUTS_Worker (void);

  /// Destructor.
  virtual ~CUTS_Worker (void);

  /// Initialize the action table. This method is an empty method
  /// and is only here for subclassing purposes.
  static void init_action_table (void);

  /// Id of the parent
  long parent_;

private:
  // disable the following methods
  CUTS_Worker (const CUTS_Worker &);
  const CUTS_Worker & operator = (const CUTS_Worker &);
};

//=============================================================================
/**
 * @class CUTS_Worker_Traits
 *
 * @brief Defines traits of worker objects.
 *
 * The traits are used by the benchmarking environment for logging
 * the performance of individual worker. The traits are defined
 * seperately from the actual workers because traits are not mandatory.
 * If no traits are defined, then the default values are used. Also,
 * it prevents the worker classes from being cluttered with non-functional
 * definitions.
 */
//=============================================================================

template <typename WORKER_TYPE>
class CUTS_Worker_Traits
{
public:
  /**
   * Unique id of the worker.
   *
   * @note In future implmentations, the <worker_id_> will be a
   * UUID value in string format.
   */
  static const long worker_id_ = 0;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Worker.inl"
#endif

#endif  // !defined _CUTS_WORKER_H_
