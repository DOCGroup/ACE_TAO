/* -*- C++ -*- */
/**
 *  @file RTCORBA_Application_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_APPLICATION_COMMAND_H
#define TAO_NS_RTCORBA_APPLICATION_COMMAND_H
#include "ace/pre.h"

#include "rtcorba_notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Application_Command.h"

/**
 * @class TAO_NS_RTCORBA_Application_Command
 *
 * @brief
 *
 */
class TAO_RTCORBA_NOTIFY_TEST_Export TAO_NS_RTCORBA_Application_Command : public TAO_NS_Application_Command
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_Application_Command (void);

  /// Destructor
  ~TAO_NS_RTCORBA_Application_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i (ACE_ENV_SINGLE_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_Application_Command.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_APPLICATION_COMMAND_H */
