/* -*- C++ -*- */
/**
 *  @file StructuredEvent.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_STRUCTUREDEVENT_H
#define TAO_Notify_Tests_STRUCTUREDEVENT_H

#include /**/ "ace/pre.h"
#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"

/**
 * @class TAO_Notify_Tests_StructuredEvent
 *
 * @brief Helper class to populate a CosNotification::StructuredEvent.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredEvent
{
public:
  /// Constuctor
  TAO_Notify_Tests_StructuredEvent (void);
  TAO_Notify_Tests_StructuredEvent (const char* name);
  TAO_Notify_Tests_StructuredEvent (const char* domain, const char* type);
  TAO_Notify_Tests_StructuredEvent (const char* name, const char* domain, const char* type);

  /// Destructor
  ~TAO_Notify_Tests_StructuredEvent ();

  void name (const char* name);
  void type (const char* domain, const char* type);

  const char* type (void);

  /// add <name,value> to optional header sequence.
  void opt_header (const char* name, CORBA::Any& value);

  /// add <name,value> to filterable body sequence.
  void filter (const char* name, CORBA::Any& value);

  /// add <name,value> to variable header sequence.
  void qos (const char* name, CORBA::Any& value);

  /// The remainder_of_body
  void payload (CORBA::Any& value);

  /// access the underlying event.
  CosNotification::StructuredEvent& event (void);

private:
  CosNotification::StructuredEvent event_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_STRUCTUREDEVENT_H */
