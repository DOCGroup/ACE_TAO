//=============================================================================
/**
 *  @file   Notify_ID_Pool_T.h
 *
 *  $Id$
 *
 * A class to generate ID's.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_ID_POOL_T_H
#define TAO_NOTIFY_ID_POOL_T_H
#include "ace/pre.h"
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

/**
 * @class TAO_Notify_ID_Pool
 *
 * @brief ID_Pool
 *
 * This class is used by factories that need to associate id's with the
 * objects that they create.
 * The very first id generated is always 0.The condition is necessary to
 * support ids for default objects that require an id of 0.
 */
template <class ID_TYPE>
class TAO_Notify_ID_Pool
{
public:

  /// Constructor.
  TAO_Notify_ID_Pool (void);

  /// Destructor.
  ~TAO_Notify_ID_Pool ();

  /// Returns the current id.
  ID_TYPE get (void);

  /// Return the id back.
  void put (ID_TYPE id);

  /// The current id is moved to the active_list_.
  void next (void);

 protected:
  /// The next available id.
  ID_TYPE id_;

  /// List of ids currently in use by clients of this class.
  ACE_Unbounded_Set <ID_TYPE> active_list_;
};

 /**
  * @class TAO_Notify_ID_Pool_Ex
  *
  * @brief TAO_Notify_ID_Pool_Ex
  *
  * Extends the TAO_Notify_ID_Pool with a method to convert the active_list
  * into a sequence type.
  */
template <class ID_TYPE, class ID_TYPE_SEQ>
class TAO_Notify_ID_Pool_Ex : public TAO_Notify_ID_Pool <ID_TYPE>
{
 public:
  TAO_Notify_ID_Pool_Ex (void);
  ~TAO_Notify_ID_Pool_Ex ();

  /// Get a list of all ids in use.
  /// Essentially converts the <active_list_> to a Sequence.
  ID_TYPE_SEQ* get_sequence (ACE_ENV_SINGLE_ARG_DECL);
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
