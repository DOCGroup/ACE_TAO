// -*- C++ -*-

//=============================================================================
/**
 * @file    Action_Iterator_T.h
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ACTION_ITERATOR_T_H_
#define _CUTS_ACTION_ITERATOR_T_H_

#include "cuts/Action_Iterator.h"

//=============================================================================
/**
 * @class CUTS_Action_Iterator_T
 *
 * @brief Implementaion of the CUTS_Action_Iterator interface.
 */
//=============================================================================

template <typename WORKER_TYPE>
class CUTS_Action_Iterator_T : public CUTS_Action_Iterator
{
  /// Type definition for the map iterator.
  typedef typename
    CUTS_Worker_T <WORKER_TYPE>::CUTS_Action_Factory_Map::iterator
    iterator;

public:
  /**
   * Initializing constructor.
   *
   * @param[in]     worker      Pointer to the parent worker.
   * @param[in]     iter        Iterator for the action table.
   */
  CUTS_Action_Iterator_T (WORKER_TYPE * worker, iterator iter);

  /**
   * Get the name of the current action.
   */
  const ACE_CString & action_name (void) const;

  /**
   * Create the current action.
   *
   * @return      Pointer to the action.
   */
  CUTS_Action * create_action (void) const;

  /// Move to the next action.
  void advance (void);

  /// Move to the previous action.
  void reverse (void);

  /**
   * Determine if all elements have been seen.
   *
   * @retval      true      All elements have been seen.
   * @retval      false     All elements have not been seen.
   */
  bool done (void) const;

protected:
  /// Default destructor.
  virtual ~CUTS_Action_Iterator_T (void);

private:
  /// Pointer to the parent worker.
  WORKER_TYPE * worker_;

  /// Iterator to the action table.
  iterator iter_;
};

#include "cuts/Action_Iterator_T.cpp"

#endif  // !defined _CUTS_ACTION_ITERATOR_T_H_
