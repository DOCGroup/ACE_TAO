// -*- C++ -*-

//=============================================================================
/**
 * @file    Action_T.h
 *
 * @brief   Define various classes for specifying worker actions.
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ACTION_T_H_
#define _CUTS_ACTION_T_H_

//=============================================================================
/**
 * @class CUTS_Action_T
 *
 * @brief Base class for all actions of a worker.
 *
 * The actions must be constructed by providing a reference to the
 * parent worker. When the execute method is invoked via an extenal
 * program, e.g., CUTS emulation environment, it must call the
 * appropriate method on the contained parent.
 */
//=============================================================================

template <typename WORKER_TYPE>
class CUTS_Action_T
{
public:
  /// Traits that defines the worker type of the action.
  typedef WORKER_TYPE Worker_Type;

  /**
   * Initializing constructor.
   *
   * @param[in]   parent        Parent worker of the action.
   * @param[in]   action_name   Name of the action.
   */
  CUTS_Action_T (Worker_Type & parent);

  /// Destructor.
  virtual ~CUTS_Action_T (void);

protected:
  /// Parent of the action.
  mutable WORKER_TYPE & parent_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Action_T.inl"
#endif

#include "cuts/Action_T.cpp"

#endif  // !defined _CUTS_ACTION_T_H_
