// -*- C++ -*-

//==========================================================================
/**
 *  @file    Dynamic.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 *  @author Irfan Pyarali.
 */
//==========================================================================

#ifndef ACE_DYNAMIC_H
#define ACE_DYNAMIC_H
#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Dynamic
 *
 * @brief Checks to see if an object was dynamically allocated.
 *
 * This class holds the pointer in a thread-safe manner between
 * the call to operator new and the call to the constructor.
 */
class ACE_Export ACE_Dynamic
{
public:
  // = Initialization and termination method.
  /// Constructor.
  ACE_Dynamic (void);

  /// Destructor.
  ~ACE_Dynamic (void);

  /**
   * Sets a flag that indicates that the object was dynamically
   * created. This method is usually called in operator new and then
   * checked and reset in the constructor.
   */
  void set (void);

  /// 1 if we were allocated dynamically, else 0.
  int is_dynamic (void);

  /// Resets state flag.
  void reset (void);

  static ACE_Dynamic *instance (void);

private:
  /**
   * Flag that indicates that the object was dynamically created. This
   * method is usually called in operator new and then checked and
   * reset in the constructor.
   */
  int is_dynamic_;
};

#if defined (__ACE_INLINE__)
#include "ace/Dynamic.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_DYNAMIC_H */
