/**
 * @file Servant_var.h
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */
#ifndef CS_SERVANT_VAR_H
#define CS_SERVANT_VAR_H

#include "ace/config-all.h"

template<typename SERVANT>
class Servant_var
{
public:
  /// Constructor
  /**
   * @param s The contained servant.  This class assumes ownership
   */
  ACE_EXPLICIT Servant_var (SERVANT *s = 0);

  /// Copy constructor
  Servant_var (const Servant_var<SERVANT> &rhs);

  /// Assignment
  Servant_var<SERVANT>& operator= (const Servant_var<SERVANT> &rhs);

  /// Destructor
  ~Servant_var ();

  //@{
  /** @name Smart pointer operations
   */
  const SERVANT *operator->() const;
  SERVANT *operator->();

  operator SERVANT *();
  operator const SERVANT *() const;
  //@}

  //@{
  /** @name Canonical CORBA T_var methods
   */
  SERVANT *in ();
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

#endif /* CS_SERVANT_VAR_H */
