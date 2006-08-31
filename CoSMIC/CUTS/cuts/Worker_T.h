// -*- C++ -*-

//=============================================================================
/**
 * @file      Worker_T.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_WORKER_T_H_
#define _CUTS_WORKER_T_H_

#include "cuts/config.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"
#include "ace/SStringfwd.h"
#include "ace/String_Base.h"

// Forward decl.
class CUTS_Action;

// Forward decl.
class CUTS_Action_Iterator;

// Forward decl.
template <typename WORKER_TYPE>
class CUTS_Action_Iterator_T;

//=============================================================================
/**
 * @class CUTS_Worker_T
 */
//=============================================================================

template <typename WORKER_TYPE>
class CUTS_Worker_T
{
  friend class CUTS_Action_Iterator_T <WORKER_TYPE>;

public:
  /// Type definition for the worker type.
  typedef WORKER_TYPE Worker_Type;

  /**
   * Create an action from the worker.
   *
   * @param[in]     action_name     Name of the action.
   * @return        Pointer to the action.
   */
  CUTS_Action * create_action (const char * action_name);

  /**
   * Get the number of actions supported by the worker.
   *
   * @return      The number of actions.
   */
  size_t action_count (void) const;

  /**
   * Get a iterator to the head of the action table.
   */
  CUTS_Action_Iterator * begin (void);

protected:
  typedef CUTS_Action * (*CUTS_ACTION_FACTORY_FUNC) (WORKER_TYPE &);

  typedef ACE_Hash_Map_Manager <ACE_CString,
                                CUTS_ACTION_FACTORY_FUNC,
                                ACE_Null_Mutex> CUTS_Action_Factory_Map;
  /// Constructor.
  CUTS_Worker_T (void);

  /// Destructor.
  virtual ~CUTS_Worker_T (void);

  /// The action table for the worker.
  static CUTS_Action_Factory_Map action_table_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Worker_T.inl"
#endif

#include "cuts/Worker_T.cpp"

#endif  // !define _CUTS_WORKER_T_H_
