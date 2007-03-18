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

#ifndef TAO_Notify_Tests_COMMAND_FACTORY_H
#define TAO_Notify_Tests_COMMAND_FACTORY_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Tests_Command;

/**
 * @class TAO_Notify_Tests_Command_Factory
 *
 * @brief Base class for Factory to create command objects.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Command_Factory
{
public:
  /// Constuctor
  TAO_Notify_Tests_Command_Factory (void);

  /// Destructor
  virtual ~TAO_Notify_Tests_Command_Factory ();

  /// Create the command.
  virtual TAO_Notify_Tests_Command* create (void) = 0;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_COMMAND_FACTORY_H */
