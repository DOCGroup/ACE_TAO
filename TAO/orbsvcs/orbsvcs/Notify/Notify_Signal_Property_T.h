// $Id$
// =========================================================================
//
// = LIBRARY
//   CosNotification
//
// = FILENAME
//   Notify_Signal_Property_T.h
//
// = DESCRIPTION
//   Template class to allow conditional waits on changes of values.
//   It implements an extremely small subset of ACE_Atomic_Op<>.
//
// = AUTHOR
//    Chad Elliott <elliott_c@ociweb.com>
//
// ==========================================================================
#ifndef TAO_NOTIFY_SIGNAL_PROPERTY_T_H
#define TAO_NOTIFY_SIGNAL_PROPERTY_T_H
#include "ace/pre.h"

#include "ace/Synch_T.h"
#include "ace/Atomic_Op.h"

#if defined (_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning (push)
#endif /* _MSC_VER >= 1200 */
#pragma warning (disable:4250)
#endif /* _MSC_VER */

template <class ACE_LOCK, class TYPE>
class TAO_Notify_Signal_Property
{
public:
  // = Initialization methods.

  /// Initialize <value_> to 0.
  TAO_Notify_Signal_Property (void);

  /// Initialize <value_> to c.
  TAO_Notify_Signal_Property (const TYPE &c);

  // Destructor
  virtual ~TAO_Notify_Signal_Property (void);

  // = Accessors.

  /// Atomically post-increment <value_>.
  TYPE operator++ (int);

  /// Atomically post-decrement <value_>.
  TYPE operator-- (int);

  /// Explicitly return <value_>.
  TYPE value (void) const;

  /// Wait for the value_ to change
  virtual int wait_for_change (const ACE_Time_Value* abstime);

private:
  ACE_UNIMPLEMENTED_FUNC (
    TAO_Notify_Signal_Property (
         const TAO_Notify_Signal_Property<ACE_LOCK, TYPE> &rhs))
  ACE_UNIMPLEMENTED_FUNC (
    TAO_Notify_Signal_Property& operator= (
         const TAO_Notify_Signal_Property<ACE_LOCK, TYPE> &rhs))


  ACE_Atomic_Op <ACE_LOCK, TYPE> value_;
  ACE_SYNCH_CONDITION changed_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_Signal_Property_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_Signal_Property_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning (pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_SIGNAL_PROPERTY_T_H */
