/* -*- C++ -*- */
/**
 *  @file Name.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_NAME_H
#define TAO_NS_NAME_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_NS_Name
 *
 * @brief Define the names for command objects and other common strings.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Name
{
public:
  static const char* const root_poa;
  static const char* const event_channel_factory;
  static const char* const naming_service;

  static const char* const command_builder;

  static const char* const application_command_factory;
  static const char* const application_command;

  static const char* const event_channel_command_factory;
  static const char* const event_channel_command;

  static const char* const supplier_admin_command_factory;
  static const char* const supplier_admin_command;

  static const char* const consumer_admin_command_factory;
  static const char* const consumer_admin_command;

  static const char* const periodic_supplier_command;
  static const char* const periodic_supplier_command_factory;

  static const char* const periodic_consumer_command;
  static const char* const periodic_consumer_command_factory;

  static const char* const poa_command;
  static const char* const poa_command_factory;

  static const char* const filter_command;
  static const char* const filter_command_factory;
};

#include "ace/post.h"
#endif /* TAO_NS_NAME_H */
