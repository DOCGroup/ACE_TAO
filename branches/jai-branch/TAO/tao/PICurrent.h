// -*- C++ -*-

// ===================================================================
/**
 *  @file   PICurrent.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_PI_CURRENT_H
#define TAO_PI_CURRENT_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_INTERCEPTORS == 1

#include "ace/Array_Base.h"

#include "PortableInterceptorC.h"
#include "LocalObject.h"

#if defined(_MSC_VER)

# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
# pragma warning(disable:4250)

// This disables a "may need dll-interface" warning from MSVC.  It's
// being too paranoid in this case, too.
# pragma warning(disable:4251)

#endif /* _MSC_VER */

/// Forward declarations.
class TAO_ORB_Core;
class TAO_PICurrent_Impl;
class TAO_ServerRequest;

/**
 * @class TAO_PICurrent
 *
 * @brief Implementation of the PortableInterceptor::Current
 * interface.
 *
 * PortableInterceptor::Current is useful for passing data between
 * request interceptors, in addition to passing data from an
 * interceptor to the calling thread.
 */
class TAO_Export TAO_PICurrent
  : public virtual PortableInterceptor::Current,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_PICurrent (void);

  /**
   * @name PortableInterceptor::Current Methods
   *
   * These are methods exposed by the PortableInterceptor::Current
   * interface.
   */
  //@{
  /// Retrieve information stored in the slot table at the given
  /// SlotId.
  virtual CORBA::Any * get_slot (PortableInterceptor::SlotId id
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  /// Set information in the slot table at the given SlotId.
  virtual void set_slot (PortableInterceptor::SlotId id,
                         const CORBA::Any & data
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));
  //@}

  /// Number of slots allocated in the slot table.
  PortableInterceptor::SlotId slot_count (void) const;

  /// Retrieve the PICurrent implementation from TSS, i.e. the thread
  /// scope current (TSC).
  TAO_PICurrent_Impl * tsc (void);

  /// Verify the validity of the given SlotId.
  void check_validity (const PortableInterceptor::SlotId &id
                       ACE_ENV_ARG_DECL);

  /// Initialize the PICurrent object.
  void initialize (TAO_ORB_Core * orb_core,
                   PortableInterceptor::SlotId sc);

protected:

  /// Destructor
  /**
   * Protected destructor to enforce the fact this class is reference
   * counted, and should not be destroyed using delete() by anything
   * other than the reference counting mechanism.
   */
  ~TAO_PICurrent (void);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  //@{
  ACE_UNIMPLEMENTED_FUNC (TAO_PICurrent (const TAO_PICurrent &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_PICurrent &))
  //@}

private:

  /// Pointer to the orb core.
  TAO_ORB_Core *orb_core_;

  /// The number of allocated slots.
  PortableInterceptor::SlotId slot_count_;

};

// ------------------------------------------------------------------

/**
 * @class TAO_PICurrent_Impl
 *
 * @brief Implementation of the PortableInterceptor::Current
 *        interface.
 *
 * This class implements both the "request scope current" and the
 * "thread scope current" objects as required by Portable
 * Interceptors.
 */
class TAO_Export TAO_PICurrent_Impl
{
public:

  /// Typedef for the underyling "slot table."
  /**
   * @note The slot table is implemented as an array of pointers to
   *       void to simply avoid increasing the footprint of the ORB.
   *       A template instance of ACE_Array_Base&lt;void*&gt; is
   *       already used by TAO_ORB_Core_TSS_Resources.  Thus, no
   *       increase in footprint will occur due to this template
   *       instance.
   */
  typedef ACE_Array_Base<CORBA::Any> Table;

  /// Constructor
  TAO_PICurrent_Impl (void);

  /// Destructor
  ~TAO_PICurrent_Impl (void);

  /// Retrieve information stored in the slot table at the given
  /// SlotId.
  CORBA::Any * get_slot (PortableInterceptor::SlotId id
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  /// Set information in the slot table at the given SlotId.
  void set_slot (PortableInterceptor::SlotId id,
                 const CORBA::Any & data
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));


  /// Get the PICurrent peer associated with this PICurrent
  /// implementation.
  TAO_PICurrent_Impl *pi_peer (void);

  /// Set the PICurrent peer associated with this PICurrent
  /// implementation.
  void pi_peer (TAO_PICurrent_Impl *peer);

  /// Return a reference to the underlying slot table.
  Table &slot_table (void);

  /// Mark the slot table as being clean or dirty.  The slot table is
  /// dirty if it has been modified since the last time it was
  /// copied.
  void dirty (CORBA::Boolean dirty);

  /// Flag that specifies if the underlying slot table has been
  /// modified since last copy.
  CORBA::Boolean dirty (void) const;

  /// Copy the contents of the given PICurrent.
  void copy (TAO_PICurrent_Impl &rhs, CORBA::Boolean deep_copy);

private:

  /// Prevent copying through the copy constructor and the assignment
  //operator.
  //@{
  ACE_UNIMPLEMENTED_FUNC (
    TAO_PICurrent_Impl (const TAO_PICurrent_Impl &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_PICurrent_Impl &))
  //@}

private:

  /// The PICurrent implementation with which this implementation's
  /// slot table interacts.
  TAO_PICurrent_Impl *pi_peer_;

  /// Array of CORBA::Anys that is the underlying "slot table."
  Table slot_table_;

  /// Table that was logically copied from a PICurrent in another
  /// scope, i.e. either the request scope or the thread scope.
  Table *lc_slot_table_;

  /// Flag that specifies if the underlying slot table has been
  /// modified since last copy.
  /**
   * @note This flag is only used when copying between a request scope
   *       current and a thread scope current.
   */
  CORBA::Boolean dirty_;

};

// ------------------------------------------------------------------

/**
 * @class TAO_PICurrent_Guard
 *
 * @brief Class used to make copying between request scope current and
 *        thread scope current exception-safe.
 *
 * Since copies between the request scope current and thread scope
 * current must also occur if an exception is thrown, e.g. made
 * available to the send_exception() interception points, the "guard"
 * idiom is used to make this action exception-safe.
 *
 * @note This Guard class is only used on the server side.
 */
class TAO_Export TAO_PICurrent_Guard
{
public:

  /// Constructor
  /**
   * This constructor sets up this guard to copy the data held in a
   * given PICurrent when transitioning from that PICurrent's scope to
   * another scope (e.g. request scope to thread scope transition
   * immediately following receive_request_service_contexts() on
   * server side).
   */
  TAO_PICurrent_Guard (TAO_ServerRequest &server_request,
                       CORBA::Boolean tsc_to_rsc);

  /// Destructor
  /**
   * The destructor copies (a logical copy whenever possible) data
   * held in a given PICurrent when transitioning from one PICurrent
   * scope to another immediately before any ending interception
   * points are invoked, and after the sending and intermediate (if
   * any) interception points are invoked.
   */
  ~TAO_PICurrent_Guard (void);

private:

  /// The PICurrent implementation whose slot table will copied.
  TAO_PICurrent_Impl *src_;

  /// The PICurrent implementation whose slot table will filled with
  /// the contents of another PICurrent's slot table.
  TAO_PICurrent_Impl *dest_;

  /// Flag that indicates if the TSC is to be copied to the RSC.
  /**
   * If false, then the RSC must be deep copied upon leaving the
   * request scope and entering the thread scope.  This is necessary
   * since the RSC slot table is no longer available upon leaving the
   * thread scope, meaning that a logical copy is not enough.
   */
  CORBA::Boolean tsc_to_rsc_;

};


#if defined (__ACE_INLINE__)
# include "PICurrent.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include "ace/post.h"

#endif /* TAO_PI_CURRENT_H */
