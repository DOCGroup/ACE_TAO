/* -*- C++ -*- */
/**
 *  @file Application_Command_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_APPLICATION_COMMAND_FACTORY_H
#define TAO_NS_APPLICATION_COMMAND_FACTORY_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "Command_Factory.h"

class TAO_NS_Command;

/**
 * @class TAO_NS_Application_Command_Factory
 *
 * @brief
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Application_Command_Factory : public TAO_NS_Command_Factory, public ACE_Service_Object
{
public:
  /// Constuctor
  TAO_NS_Application_Command_Factory (void);

  /// Destructor
  ~TAO_NS_Application_Command_Factory ();

    /// = Service_Object virtual method overloads.
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);

  /// Create the EC command.
  TAO_NS_Command* create (void);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_NOTIFY_TEST, TAO_NS_Application_Command_Factory)

ACE_FACTORY_DECLARE (TAO_NOTIFY_TEST, TAO_NS_Application_Command_Factory)

#if defined (__ACE_INLINE__)
#include "Application_Command_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_APPLICATION_COMMAND_FACTORY_H */
