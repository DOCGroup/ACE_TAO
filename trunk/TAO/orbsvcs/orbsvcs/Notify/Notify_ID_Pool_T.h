// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ID_Pool_T.h
//
// = DESCRIPTION
//   A class to generate ID's.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_ID_POOL_T_H
#define TAO_NOTIFY_ID_POOL_T_H
#include "ace/pre.h"
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

template <class ID_TYPE>
class TAO_Notify_ID_Pool
{
  // = TITLE
  //   ID_Pool
  //
  // = DESCRIPTION
  //   This class is used by factories that need to associate id's with the
  //   objects that they create.
  //   The very first id generated is always 0.The condition is necessary to
  //   support ids for default objects that require an id of 0.
  //
public:

  TAO_Notify_ID_Pool (void);
  // Constructor.

  ~TAO_Notify_ID_Pool ();
  // Destructor.

  ID_TYPE get (void);
  // Returns the current id.

  void put (ID_TYPE id);
  // Return the id back.

  void next (void);
  // The current id is moved to the active_list_.

 protected:
  ID_TYPE id_;
  // The next available id.

  ACE_Unbounded_Set <ID_TYPE> active_list_;
  // List of ids currently in use by clients of this class.
};

template <class ID_TYPE, class ID_TYPE_SEQ>
class TAO_Notify_ID_Pool_Ex : public TAO_Notify_ID_Pool <ID_TYPE>
{
  // = TITLE
  //   TAO_Notify_ID_Pool_Ex
  //
  // = DESCRIPTION
  //   Extends the TAO_Notify_ID_Pool with a method to convert the active_list
  //   into a sequence type.
  //
 public:
  TAO_Notify_ID_Pool_Ex (void);
  ~TAO_Notify_ID_Pool_Ex ();

  ID_TYPE_SEQ* get_sequence (ACE_ENV_SINGLE_ARG_DECL);
  // Get a list of all ids in use.
  // Essentially converts the <active_list_> to a Sequence.
};

#if defined (__ACE_INLINE__)
#include "Notify_ID_Pool_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_ID_Pool_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_ID_Pool_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NOTIFY_ID_POOL_T_H */
