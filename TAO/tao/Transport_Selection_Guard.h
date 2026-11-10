// -*- C++ -*-

// ===================================================================
/**
 *  @file   Transport_Selection_Guard.h
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
   * https://github.com/DOCGroup/ACE_TAO/blob/master/TAO/docs/transport_current/index.html
   *
   */
  class TAO_Export Transport_Selection_Guard
  {
  public:
    static Transport_Selection_Guard *current (TAO_ORB_Core *core,
                                               size_t tss_slot_id);

    explicit Transport_Selection_Guard (TAO_Transport *t);

    ~Transport_Selection_Guard ();

    // CSD server-request cloning copies the selected transport state.
    Transport_Selection_Guard &operator= (Transport_Selection_Guard const &rhs)
    {
      if (this != &rhs)
        {
#if TAO_HAS_TRANSPORT_CURRENT == 1
          this->prev_ = rhs.prev_;
#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */
          this->curr_ = rhs.curr_;
        }
      return *this;
    }

    /// getter
    TAO_Transport *operator-> () const
    {
      return this->get ();
    }

    /// getter
    TAO_Transport &operator* () const
    {
      return *this->get ();
    }

    /// Getter
    TAO_Transport *get () const
    {
      return this->curr_;
    }

    /// Setter
    Transport_Selection_Guard &set (TAO_Transport *t)
    {
      this->curr_ = t;
      return *this;
    }

  private:
    Transport_Selection_Guard (Transport_Selection_Guard const &) = delete;
    Transport_Selection_Guard (Transport_Selection_Guard &&) = delete;
    Transport_Selection_Guard &operator= (Transport_Selection_Guard &&) = delete;

#if TAO_HAS_TRANSPORT_CURRENT == 1
    /// Non-owning pointer to the previous guard in the current thread's stack.
    Transport_Selection_Guard *prev_;

#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */

    /// Non-owning pointer to the currently selected transport.
    TAO_Transport *curr_;

    friend TAO_Export bool
    operator== (Transport_Selection_Guard const &lhs,
                TAO_Transport const *rhs);

    friend TAO_Export bool
    operator== (TAO_Transport const *lhs,
                Transport_Selection_Guard const &rhs);

    friend TAO_Export bool
    operator!= (Transport_Selection_Guard const &lhs,
                TAO_Transport const *rhs);

    friend TAO_Export bool
    operator!= (TAO_Transport const *lhs,
                Transport_Selection_Guard const &rhs);

    template <class U> inline friend bool
    operator== (Transport_Selection_Guard const &lhs,
                U *rhs)
    {
      return lhs.curr_ == rhs;
    }

    template <class U> inline friend bool
    operator== (U *lhs,
                Transport_Selection_Guard const &rhs)
    {
      return lhs == rhs.curr_;
    }

    template <class U> inline friend bool
    operator!= (Transport_Selection_Guard const &lhs,
                U *rhs)
    {
      return lhs.curr_ != rhs;
    }

    template <class U> inline friend bool
    operator!= (U *lhs,
                Transport_Selection_Guard const &rhs)
    {
      return lhs != rhs.curr_;
    }
  };

  TAO_Export bool
  operator== (Transport_Selection_Guard const &lhs,
              TAO_Transport const *rhs);

  TAO_Export bool
  operator== (TAO_Transport const *lhs,
              Transport_Selection_Guard const &rhs);

  TAO_Export bool
  operator!= (Transport_Selection_Guard const &lhs,
              TAO_Transport const *rhs);

  TAO_Export bool
  operator!= (TAO_Transport const *lhs,
              Transport_Selection_Guard const &rhs);
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_SELECTION_GUARD_H */
