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

#if !defined (TAO_PRINCIPAL_H)
#  define TAO_PRINCIPAL_H

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

  // = Stuff required for memory management.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  CORBA_Principal (void);

private:
  CORBA::ULong refcount_;

  ~CORBA_Principal (void);

  // = these are not provided
  CORBA_Principal &operator = (const CORBA::Principal_ptr &);
  CORBA_Principal (const CORBA::Principal_ptr &);

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

extern TAO_Export TAO_OutputCDR&
operator<<(TAO_OutputCDR&, CORBA_Principal*);

extern TAO_Export TAO_InputCDR&
operator>>(TAO_InputCDR&, CORBA_Principal*&);

#endif /* TAO_PRINCIPAL_H */
