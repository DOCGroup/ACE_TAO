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

 protected:

  ID_TYPE max_id_;
  // This is the last id circulating in the list.When there are no more id's
  // in the resuse list, we increment this value to get a new id.

  // @@ Pradeep: do you really need to do this?  I mean, with a long
  // or so you have 4 billion different IDs, recycling them is the
  // least of your problems, right?

  // @@ Carlos: absolutely. I did not realize this.
  // The recycling logic can be done away with!

  // Even if you need to recycle them, do you *really* need to keep
  // *both* the active and free collections around?  Isn't it enough
  // to know which one  is the maximum generated and the list of free
  // ones?  Anything  else is active, right?
  // Or if you know the active ones and the maximum given you know
  // that anything else is free, right?

  // Carlos: right. with one list amd the max, the elements of the other list could be determined.

  // Also: in most cases the IDs will index some sort of map, right?
  // You could obtain the active list from the set of keys in the map.

  // @@ Carlos: no, the <active_list_> list is the only list of id's.
  // well, actually because i convert these ID's to ObjectID's and put them in POA's,
  // i could get the list from the POA but there is no iterator in POA to get objectid's,
  //

  // I guess it is a classical tradeoff between CPU and memory, but I
  // would tend to conserve memory in this case...
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

  ID_TYPE_SEQ* get_sequence (CORBA::Environment &ACE_TRY_ENV);
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
