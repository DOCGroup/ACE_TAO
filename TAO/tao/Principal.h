// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    principal.h
//
// = DESCRIPTION
//    The CORBA_Principal pseudo-object implementation.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//
// ============================================================================

#ifndef TAO_PRINCIPAL_H
#define TAO_PRINCIPAL_H

#include "tao/corbafwd.h"
#include "tao/Sequence.h"

class TAO_Export CORBA_Principal
{
  // = TITLE
  //    A "Principal" identifies an authenticated entity in the
  //    network administration framework.  Identities are used to
  //    control acccess (authorization) as well as in audit trails
  //    (accountability).
public:
  // To applications, the identifier is an opaque ID.

  //  CORBA::SEQUENCE <CORBA::Octet> id;
  CORBA::OctetSeq id;

  // @@ add "==", "<", ">" operators

  // The pseudo object operations.
  static CORBA_Principal* _duplicate (CORBA_Principal*);
  static CORBA_Principal* _nil (void);

  // = Stuff required for memory management.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  CORBA_Principal (void);

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Principal_ptr _ptr_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:
  ~CORBA_Principal (void);

  // = these are not provided
  CORBA_Principal &operator = (const CORBA::Principal_ptr &);
  CORBA_Principal (const CORBA::Principal_ptr &);

private:
  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

  ACE_SYNCH_MUTEX refcount_mutex_;
  // Protect the reference count, this is OK because we do no
  // duplicates or releases on the critical path.

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

extern TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, CORBA_Principal*);

extern TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA_Principal*&);

#if defined (__ACE_INLINE__)
# include "tao/Principal.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PRINCIPAL_H */
