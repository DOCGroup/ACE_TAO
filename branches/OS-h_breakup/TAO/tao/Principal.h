// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Principal.h
 *
 *  $Id$
 *
 *  The CORBA_Principal pseudo-object implementation.
 *
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 */
//=============================================================================


#ifndef TAO_PRINCIPAL_H
#define TAO_PRINCIPAL_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "OctetSeqC.h"

/**
 * @class CORBA_Principal
 *
 * @brief A "Principal" identifies an authenticated entity in the
 * network administration framework.  Identities are used to
 * control acccess (authorization) as well as in audit trails
 * (accountability).
 */
class TAO_Export CORBA_Principal
{
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
  typedef CORBA_Principal_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:
  ~CORBA_Principal (void);

  // = these are not provided
  CORBA_Principal &operator = (const CORBA::Principal_ptr &);
  CORBA_Principal (const CORBA::Principal_ptr &);

private:
  /// Number of outstanding references to this object.
  CORBA::ULong refcount_;

  /// Protect the reference count, this is OK because we do no
  /// duplicates or releases on the critical path.
  TAO_SYNCH_MUTEX refcount_mutex_;

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

/**
 * @class CORBA_Principal_var
 *
 * @brief The T_var class for Principal
 *
 * As any other pseudo object Principal must have a T_var class,
 * the interface an semantics are specified in the CORBA spec.
 * = NOTE
 * We use CORBA_Principal_ptr as the _ptr type instead of
 * CORBA::Principal_ptr, this is an attempt to reduced the cyclic
 * dependencies in TAO.
 */
class TAO_Export CORBA_Principal_var
{
public:
  CORBA_Principal_var (void);
  CORBA_Principal_var (CORBA_Principal_ptr);
  CORBA_Principal_var (const CORBA_Principal_var &);
  ~CORBA_Principal_var (void);

  CORBA_Principal_var &operator= (CORBA_Principal_ptr);
  CORBA_Principal_var &operator= (const CORBA_Principal_var &);
  CORBA_Principal_ptr operator-> (void) const;

  /// in, inout, out, _retn
  operator const CORBA_Principal_ptr &() const;
  operator CORBA_Principal_ptr &();
  CORBA_Principal_ptr in (void) const;
  CORBA_Principal_ptr &inout (void);
  CORBA_Principal_ptr &out (void);
  CORBA_Principal_ptr _retn (void);
  CORBA_Principal_ptr ptr (void) const;

private:
  CORBA_Principal_ptr ptr_;
};

/**
 * @class CORBA_Principal_out
 *
 * @brief The T_out class for Principal
 *
 * As any other pseudo object Principal must have a T_out class,
 * the interface an semantics are specified in the CORBA spec.
 * = NOTE
 * We use CORBA_Principal_ptr as the _ptr type instead of
 * CORBA::Principal_ptr, this is an attempt to reduced the cyclic
 * dependencies in TAO.
 */
class TAO_Export CORBA_Principal_out
{
public:
  CORBA_Principal_out (CORBA_Principal_ptr &);
  CORBA_Principal_out (CORBA_Principal_var &);
  CORBA_Principal_out (CORBA_Principal_out &);
  CORBA_Principal_out &operator= (CORBA_Principal_out &);
  CORBA_Principal_out &operator= (const CORBA_Principal_var &);
  CORBA_Principal_out &operator= (CORBA_Principal_ptr);
  operator CORBA_Principal_ptr &();
  CORBA_Principal_ptr &ptr (void);
  CORBA_Principal_ptr operator-> (void);

private:
  CORBA_Principal_ptr &ptr_;
};

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, CORBA_Principal*);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA_Principal*&);

#if defined (__ACE_INLINE__)
# include "tao/Principal.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PRINCIPAL_H */
