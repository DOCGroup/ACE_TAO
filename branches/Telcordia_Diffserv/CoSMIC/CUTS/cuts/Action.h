// -*- C++ -*-

//=============================================================================
/**
 * @file      Action.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ACTION_H_
#define _CUTS_ACTION_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"

//=============================================================================
/**
 * @class CUTS_Action
 *
 * @brief Interface for managing action commands. All actions of any
 * CUTS_Worker object must provide this interface. The interface
 * contains the necessary operations to execute () it and release ()
 * is resources, if dynamically allocated, without knowing anything about
 * the commands implementation.
 */
//=============================================================================

class CUTS_Export CUTS_Action
{
public:
  /**
   * Execute method for the action. This method is called
   * when the emulation framwork wants to execute the action.
   * It will call the appropriate method on the <parent_> for
   * the action. If the target method should require parameters,
   * they should stored in the action at construction time
   */
  virtual void execute (void) const = 0;

  /// Release all the resources of the action. This method should
  /// always be used for deleting actions created using the
  /// create_action () factory operation.
  virtual void release (void);

protected:
  /// Default constructor.
  CUTS_Action (void);

  /// Copy constructor.
  CUTS_Action (const CUTS_Action &);

private:
  // prevent the following operations
  const CUTS_Action & operator = (const CUTS_Action &);
};

//=============================================================================
/**
 * @class CUTS_Action_Traits
 *
 * @bried Defines traits of an action.
 *
 * Specializing this class for a particular action is not mandatory.
 * However, if detailed information about an action's performance
 * is needed, this class should be specialized. Failing to specialize
 * this clas will result in the action using the default /a action_id_
 * for identification purposes.
 */
//=============================================================================

template <typename ACTION_TYPE>
class CUTS_Action_Traits
{
public:
  /// Type definition for the id of the action.
  static const long action_id_ = 0;
};

#endif  // !defined _CUTS_ACTION_H_
