/**
 * @file Servant_var.h
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_SERVANT_VAR_H
#define TAO_PERF_RTEC_SERVANT_VAR_H

#include "ace/Swap.h"

/**
 * @class Servant_var
 *
 * @brief Implement a smart pointer class for Servants
 *
 * Many servants, are reference counted objects, for example, those
 * that derive from the PortableServer::RefCountServantBase mixin.
 * Unfortunately the specification lacks such an useful smart pointer
 * class to manage their life-cycle.
 */
template<class SERVANT>
class Servant_var
{
public:
  /// Constructor
  /**
   * @param s The contained servant.  The constructor assumes
   * ownership.
   */
  ACE_EXPLICIT Servant_var (SERVANT *s = 0);

  /// Copy constructor
  Servant_var (const Servant_var<SERVANT> &rhs);

  /// Assignment
  Servant_var<SERVANT>& operator= (const Servant_var<SERVANT> &rhs);

  /// Assignment
  Servant_var<SERVANT>& operator= (SERVANT *s);

  /// Destructor
  ~Servant_var ();

  /// Helper method to increase the reference count on a servant.
  static SERVANT *duplicate (SERVANT *servart);

  //@{
  /** @name Smart pointer operations
   */
  const SERVANT * operator->() const;
  SERVANT * operator->();

  operator SERVANT *();
  operator const SERVANT * () const;
  //@}

  //@{
  /** @name Canonical CORBA T_var methods
   */
  SERVANT *in () const;
  SERVANT *&out ();
  SERVANT *&inout ();
  //@}

private:
  /// The owned object
  SERVANT *ptr_;
};

#if defined(__ACE_INLINE__)
#include "Servant_var.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Servant_var.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_PERF_RTEC_SERVANT_VAR_H */
