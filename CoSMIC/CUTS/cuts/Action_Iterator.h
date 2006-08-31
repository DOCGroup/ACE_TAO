// -*- C++ -*-

//=============================================================================
/**
 * @file      Action_Iterator.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ACTION_ITERATOR_H_
#define _CUTS_ACTION_ITERATOR_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"

#include "ace/SStringfwd.h"
#include "ace/String_Base.h"

// Forward decl.
class CUTS_Action;

//=============================================================================
/**
 * @class CUTS_Action_Iterator
 *
 * @brief Base iterator class for iterating over action tables.
 *
 * This class uses the NULL object pattern, meaning each of the supported
 * operations does nothing. This iterator class is primarily used by clients
 * as a base interface. It allows clients to get the name of the actions, as
 * well as, create actions. This is useful when parsing the actions contained
 * in a worker and creating them on the fly if the name is not known ahead
 * of time.
 */
//=============================================================================

class CUTS_Export CUTS_Action_Iterator
{
public:
  /// Default constructor.
  CUTS_Action_Iterator (void);

  /// Release the resources of the iterator. This method should
  /// be used instead of the destructor.
  virtual void release (void);

  /**
   * Get the name of the action.
   *
   * @return    An empty string.
   */
  virtual const ACE_CString & action_name (void) const;

  /**
   * Create the current action. Since this is an NULL object
   * it will always return 0.
   *
   * @return      Always returns 0.
   */
  virtual CUTS_Action * create_action (void) const;

  /// Goto the next element.
  virtual void advance (void);

  /// Goto the previous element.
  virtual void reverse (void);

  /**
   * Determine if the iterator is done. This method will always
   * return \a true.
   *
   * @return      Always returns true.
   */
  virtual bool done (void) const;

protected:
  /// Default destructor.
  virtual ~CUTS_Action_Iterator (void);

private:
  /// Name of all the actions, which is an empty string.
  static ACE_CString no_name_;

  // Prevent the following operations.
  CUTS_Action_Iterator (const CUTS_Action_Iterator &);
  const CUTS_Action_Iterator & operator = (const CUTS_Action_Iterator &);
};

#endif  // !defined _CUTS_ACTION_ITERATOR_H_
