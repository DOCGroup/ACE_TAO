/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    CORBA_Ref.h
//
// = AUTHOR
//    Irfan Pyarali (irfan@wuerl.wustl.edu).
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//    A wrapper for helping with Orbix object references.
//
// ============================================================================

#ifndef ACE_CORBA_REF_H
#define ACE_CORBA_REF_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class CORBA_REF>
class ACE_CORBA_Ref
{
  // = TITLE
  //    A wrapper for helping with orbix object references.
  //
  // = DESCRIPTION
  //    ACE_CORBA_Ref is parameterized by the type of orbix object
  //    reference to be used.  The construtor, operator=, and the
  //    destructor of ACE_CORBA_Ref perform implicit duplicates and
  //    releases in order to help make the use of orbix object
  //    references transparent.
public:
  ACE_CORBA_Ref (void);
  // Null construction.

  ACE_CORBA_Ref (CORBA_REF *ref);
  // Contruction with an orbix ref.
  // performs a ref->_duplicate().

  CORBA_REF *operator= (CORBA_REF *ref);
  // Assignment performs a ref->_duplicate().

  operator CORBA_REF *(void) const;
  // Type operator

  CORBA_REF *operator-> (void) const;
  // Smart pointer to forward all CORBA_REF calls to the underlying
  // orbix reference.

  int operator== (CORBA_REF *) const;
  // Pointer comparison.

  int operator!= (CORBA_REF *) const;
  // Pointer comparison.

  ~ACE_CORBA_Ref (void);
  // Destruction: calls ref_->_release

private:
  CORBA_REF *ref_;
};

#if defined (__ACE_INLINE__)
#include "ace/CORBA_Ref.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/CORBA_Ref.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("CORBA_Ref.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CORBA_REF_H */
