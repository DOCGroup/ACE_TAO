/* -*- C++ -*- */
/**
 *  @file Application_Starter.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_APPLICATION_STARTER_H
#define TAO_NS_APPLICATION_STARTER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/CORBA_macros.h"
#include "tao/ORB.h"

/**
 * @class TAO_NS_Application_Starter
 *
 * @brief
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Application_Starter : public ACE_Service_Object
{
public:
  virtual ~TAO_NS_Application_Starter () {};

  /// Init
  virtual void bootstrap (CORBA::ORB_var &orb ACE_ENV_ARG_DECL) = 0;

  /// Execute the commands.
  virtual void run (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Shutdown
  virtual void shutdown (void) = 0;
};

#define TAO_NS_APP_STARTER_DIRECTIVE \

#include "ace/post.h"
#endif /* TAO_NS_APPLICATION_STARTER_H */
