/* -*- C++ -*- */
/**
 *  @file RT_Application_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RT_APPLICATION_COMMAND_H
#define TAO_NS_RT_APPLICATION_COMMAND_H
#include "ace/pre.h"

#include "rt_notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../lib/Application_Command.h"

/**
 * @class TAO_NS_RT_Application_Command
 *
 * @brief
 *
 */
class TAO_RT_NOTIFY_TEST_Export TAO_NS_RT_Application_Command : public TAO_NS_Application_Command
{
public:
  /// Constuctor
  TAO_NS_RT_Application_Command (void);

  /// Destructor
  ~TAO_NS_RT_Application_Command ();

  virtual void handle_init (ACE_ENV_SINGLE_ARG_DECL);
};

#include "ace/post.h"
#endif /* TAO_NS_RT_APPLICATION_COMMAND_H */
