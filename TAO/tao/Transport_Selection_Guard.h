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

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations
class TAO_Transport;
class TAO_ORB_Core;

namespace TAO
{
  /**
   * @class Transport_Selection_Guard
   *
   * @brief Used by the Transport Current feature to keep track of
   * which Transport is currently active.
   *
   * Whenever a Transport is selected: during an upcall, or prior to a
   * client invocation an instance of this class is created [on the
   * stack, or as a member of another class] to keep track of the said
   * Transport.  The class implements the RAII idiom, which makes it
   * possible to build a stack of these instances as the thread is
   * doing nested upcalls or client invocations.
   *
   * It utilizes TAO_TSS_Resources::tsg_ member pointer to keep track
   * of stack-linked Transport_Selection_Guard instances.
   *
   * If the Transport Current feature is disabled most methods are
   * no-ops and add no overhead on the critical path.
   *
   * <B>See Also:</B>
   *
   * https://svn.dre.vanderbilt.edu/viewvc/Middleware/trunk/TAO/docs/transport_current/index.html?revision=HEAD
   *
   */
  class TAO_Export Transport_Selection_Guard
  {
  public:
    static Transport_Selection_Guard* current (TAO_ORB_Core* core,
                                               size_t tss_slot_id);

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

  Transport_Selection_Guard& operator=(const Transport_Selection_Guard& rhs) {
    if (this != &rhs)
      {
#if TAO_HAS_TRANSPORT_CURRENT == 1
        prev_ = rhs.prev_;
#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */
        curr_ = rhs.curr_;
      }
    return *this;
  }


  private:
    ACE_UNIMPLEMENTED_FUNC (Transport_Selection_Guard (const Transport_Selection_Guard&))

#if TAO_HAS_TRANSPORT_CURRENT == 1
    /// This is pointing to the guard that was active prior to
    /// instantiating us.
    Transport_Selection_Guard* prev_;

#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */

    /// The "real" Transport, i.e. the one selected at present
    TAO_Transport* curr_;

    // Comparison. See Modern C++ Design by A. Alexandrescu for the gory detail.
    bool
    operator! () const
    {
      return curr_ == 0;
    }

    inline friend bool
    operator== (const Transport_Selection_Guard& lhs,
                const TAO_Transport* rhs)
    {
      return lhs.curr_ == rhs;
    }

    inline friend bool
    operator== (const TAO_Transport* lhs,
                const Transport_Selection_Guard& rhs)
    {
      return lhs == rhs.curr_;
    }

    inline friend bool
    operator!= (const Transport_Selection_Guard& lhs,
                const TAO_Transport* rhs)
    {
      return lhs.curr_ != rhs;
    }

    inline friend bool
    operator!= (const TAO_Transport* lhs,
                const Transport_Selection_Guard& rhs)
    {
      return lhs != rhs.curr_;
    }

    template <class U> inline friend bool
    operator== (const Transport_Selection_Guard& lhs,
                const U* rhs)
    {
      return lhs.curr_ == rhs;
    }

    template <class U> inline friend bool
    operator== (const U* lhs,
                const Transport_Selection_Guard& rhs)
    {
      return lhs == rhs.curr_;
    }

    template <class U> inline friend bool
    operator!= (const Transport_Selection_Guard& lhs,
                const U* rhs)
    {
      return lhs.curr_ != rhs;
    }

    template <class U> inline friend bool
    operator!= (const U* lhs,
                const Transport_Selection_Guard& rhs)
    {
      return lhs != rhs.curr_;
    }

  };

} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_SELECTION_GUARD_H */
