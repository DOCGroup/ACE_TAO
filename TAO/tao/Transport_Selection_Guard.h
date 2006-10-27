// -*- C++ -*-

// ===================================================================
/**
 *  @file   Transport_Selection_Guard.h
 *
 *  $Id$
 *
 *  @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */
// ===================================================================

#ifndef TAO_TRANSPORT_SELECTION_GUARD_H
#define TAO_TRANSPORT_SELECTION_GUARD_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Environment.h"

/* #include "tao/Policy_Current_Impl.h" */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations
class TAO_Transport;

namespace TAO
{
  /**
   * Whenever a Transport is selected, during an upcall or prior to a
   * client invocation, an instance of this class is created [on the
   * stack, or as a member of another class] to keep track of the said
   * Transport.  The class implements the RAII idiom, which makes it
   * possible to build a stack of these instances as the thread is
   * doing nested upcalls or client invocations.
   */
  class TAO_Export Transport_Selection_Guard
  {
  public:

    static Transport_Selection_Guard* current (TAO_ORB_Core* core, size_t tss_slot_id);

  public:

    /// Ctor
    Transport_Selection_Guard (TAO_Transport* t);

    /// Dtor
    ~Transport_Selection_Guard (void);

    /// getter
    TAO_Transport* operator-> (void) const
    {
      return this->get ();
    };

    /// getter
    TAO_Transport& operator* (void) const
    {
      return *this->get ();
    };

    /// Getter
    TAO_Transport* get (void) const
    {
      return this->curr_;
    };

    /// Setter
    Transport_Selection_Guard& set (TAO_Transport* t)
    {
      this->curr_ = t;
      return *this;
    };

  private:
    ACE_UNIMPLEMENTED_FUNC (Transport_Selection_Guard (const Transport_Selection_Guard&))
    ACE_UNIMPLEMENTED_FUNC (Transport_Selection_Guard& operator=(const Transport_Selection_Guard&))

#if TAO_HAS_TRANSPORT_CURRENT == 1

    /// This is pointing to the guard that was active prior to
    /// instantiating us.

    Transport_Selection_Guard* prev_;

#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */

    /// The "real" Transport, i.e. the one selected at present
    TAO_Transport* curr_;
  };

} /* namespace TAO */




TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_SELECTION_GUARD_H */
// -*- C++ -*-

// ===================================================================
/**
 *  @file   Transport_Selection_Guard.h
 *
 *  $Id$
 *
 *  @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */
// ===================================================================

#ifndef TAO_TRANSPORT_SELECTION_GUARD_H
#define TAO_TRANSPORT_SELECTION_GUARD_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Environment.h"

/* #include "tao/Policy_Current_Impl.h" */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations
class TAO_Transport;

namespace TAO
{
  /**
   * Whenever a Transport is selected, during an upcall or prior to a
   * client invocation, an instance of this class is created [on the
   * stack, or as a member of another class] to keep track of the said
   * Transport.  The class implements the RAII idiom, which makes it
   * possible to build a stack of these instances as the thread is
   * doing nested upcalls or client invocations.
   */
  class TAO_Export Transport_Selection_Guard
  {
  public:

    static Transport_Selection_Guard* current (TAO_ORB_Core* core, size_t tss_slot_id);

  public:

    /// Ctor
    Transport_Selection_Guard (TAO_Transport* t);

    /// Dtor
    ~Transport_Selection_Guard (void);

    /// getter
    TAO_Transport* operator-> (void) const
    {
      return this->get ();
    };

    /// getter
    TAO_Transport& operator* (void) const
    {
      return *this->get ();
    };

    /// Getter
    TAO_Transport* get (void) const
    {
      return this->curr_;
    };

    /// Setter
    Transport_Selection_Guard& set (TAO_Transport* t)
    {
      this->curr_ = t;
      return *this;
    };

  private:
    ACE_UNIMPLEMENTED_FUNC (Transport_Selection_Guard (const Transport_Selection_Guard&))
    ACE_UNIMPLEMENTED_FUNC (Transport_Selection_Guard& operator=(const Transport_Selection_Guard&))

#if TAO_HAS_TRANSPORT_CURRENT == 1

    /// This is pointing to the guard that was active prior to
    /// instantiating us.

    Transport_Selection_Guard* prev_;

#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */

    /// The "real" Transport, i.e. the one selected at present
    TAO_Transport* curr_;
  };

} /* namespace TAO */




TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_SELECTION_GUARD_H */
