/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CORBA_Ref.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali (irfan@wuerl.wustl.edu)
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 *
 *  A wrapper for helping with Orbix object references.
 *
 *
 */
//=============================================================================


#ifndef ACE_CORBA_REF_H
#define ACE_CORBA_REF_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_CORBA_Ref
 *
 * @brief A wrapper for helping with Orbix object references.
 *
 * <ACE_CORBA_Ref> is parameterized by the type of orbix object
 * reference to be used.  The construtor, operator=, and the
 * destructor of <ACE_CORBA_Ref> perform implicit duplicates and
 * releases in order to help make the use of Orbix object
 * references transparent.
 */
template <class CORBA_REF>
class ACE_CORBA_Ref
{
public:
  /// Null construction.
  ACE_CORBA_Ref (void);

  /// Contruction with an orbix ref.
  /// performs a <CORBA_REF::_duplicate>.
  ACE_CORBA_Ref (CORBA_REF *ref);

  /// Assignment performs a <CORBA_REF::_duplicate>.
  CORBA_REF *operator= (CORBA_REF *ref);

  /// Type operator
  operator CORBA_REF *(void) const;

  /// Smart pointer to forward all CORBA_REF calls to the underlying
  /// Orbix reference.
  CORBA_REF *operator-> (void) const;

  /// Pointer comparison.
  int operator== (CORBA_REF *) const;

  /// Pointer comparison.
  int operator!= (CORBA_REF *) const;

  /// Destruction: calls <CORBA_REF::_release>.
  ~ACE_CORBA_Ref (void);

private:
  CORBA_REF *ref_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/CORBA_Ref.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("CORBA_Ref.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* CORBA_REF_H */
