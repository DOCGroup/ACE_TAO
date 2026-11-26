/**
 *  @file Name.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_NAME_H
#define TAO_Notify_Tests_NAME_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Notify_Tests_Name
 *
 * @brief Define the names for command objects and other common strings.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Name
{
public:
  static char const *const root_poa;
  static char const *const event_channel_factory;
  static char const *const naming_service;

  static ACE_TCHAR const *const command_builder;

  static ACE_TCHAR const *const application_command_factory;
  static char const *const application_command;

  static ACE_TCHAR const *const event_channel_command_factory;
  static char const *const event_channel_command;

  static ACE_TCHAR const *const supplier_admin_command_factory;
  static char const *const supplier_admin_command;

  static ACE_TCHAR const *const consumer_admin_command_factory;
  static char const *const consumer_admin_command;

  static char const *const periodic_supplier_command;
  static ACE_TCHAR const *const periodic_supplier_command_factory;

  static char const *const periodic_consumer_command;
  static ACE_TCHAR const *const periodic_consumer_command_factory;

  static char const *const poa_command;
  static ACE_TCHAR const *const poa_command_factory;

  static char const *const filter_command;
  static ACE_TCHAR const *const filter_command_factory;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_NAME_H */
