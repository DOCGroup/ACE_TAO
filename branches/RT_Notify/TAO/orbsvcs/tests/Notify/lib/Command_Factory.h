/* -*- C++ -*- */
/**
 *  @file Command_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_COMMAND_FACTORY_H
#define TAO_NS_COMMAND_FACTORY_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_NS_Command;

/**
 * @class TAO_NS_Command_Factory
 *
 * @brief Base class for Factory to create command objects.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Command_Factory
{
public:
  /// Constuctor
  TAO_NS_Command_Factory (void);

  /// Destructor
  ~TAO_NS_Command_Factory ();

  /// Create the command.
  virtual TAO_NS_Command* create (void) = 0;
};

#if defined (__ACE_INLINE__)
#include "Command_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_COMMAND_FACTORY_H */
