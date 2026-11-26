/**
 *  @file StructuredEvent.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
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
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredEvent
{
public:
  TAO_Notify_Tests_StructuredEvent ();
  TAO_Notify_Tests_StructuredEvent (char const *name);
  TAO_Notify_Tests_StructuredEvent (char const *domain, char const *type);
  TAO_Notify_Tests_StructuredEvent (char const *name, char const *domain, char const *type);

  ~TAO_Notify_Tests_StructuredEvent ();

  void name (char const *name);
  void type (char const *domain, char const *type);

  char const *type ();

  /// add <name,value> to optional header sequence.
  void opt_header (char const *name, CORBA::Any const & value);

  /// add <name,value> to filterable body sequence.
  void filter (char const *name, CORBA::Any const & value);

  /// add <name,value> to variable header sequence.
  void qos (char const *name, CORBA::Any const & value);

  /// The remainder_of_body
  void payload (CORBA::Any const & value);

  /// access the underlying event.
  CosNotification::StructuredEvent& event ();

private:
  CosNotification::StructuredEvent event_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_STRUCTUREDEVENT_H */
