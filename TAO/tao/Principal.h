// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Principal.h
 *
 *  $Id$
 *
 *  The CORBA::Principal pseudo-object implementation.
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  DOC group at Wash U and UCI.
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
#include "ace/Synch_T.h"

namespace CORBA
{
  /**
   * @class Principal
   *
   * @brief A "Principal" identifies an authenticated entity in the
   * network administration framework.
   *
   * Identities are used to control acccess (authorization) as well as
   * in audit trails (accountability).
   *
   * @note This CORBA feature has been deprecated by the OMG.  Use the
   *       CORBA Security Service instead.
   */
  class TAO_Export Principal
  {
  public:
    // To applications, the identifier is an opaque ID.

    //  CORBA::SEQUENCE <CORBA::Octet> id;
    CORBA::OctetSeq id;

    // @@ add "==", "<", ">" operators

    // The pseudo object operations.
    static Principal* _duplicate (Principal*);
    static Principal* _nil (void);

    // = Stuff required for memory management.
    CORBA::ULong _incr_refcnt (void);
    CORBA::ULong _decr_refcnt (void);

    Principal (void);

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
    typedef Principal_ptr _ptr_type;
    typedef Principal_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

  protected:

    /// Destructor
    /**
     * Protected destructor to enforce proper memory management
     * through the reference counting mechanism.
     */
    ~Principal (void);

  private:

    // = Prevent copying
    Principal &operator = (const CORBA::Principal_ptr &);
    Principal (const CORBA::Principal_ptr &);

  private:
    /// Number of outstanding references to this object.
    CORBA::ULong refcount_;

    /// Protect the reference count, this is OK because we do no
    /// duplicates or releases on the critical path.
    TAO_SYNCH_MUTEX refcount_mutex_;

  };

  /**
   * @class Principal_var
   *
   * @brief The T_var class for CORBA::Principal
   *
   * As any other pseudo object Principal must have a T_var class,
   * the interface an semantics are specified in the CORBA spec.
   */
  class TAO_Export Principal_var
  {
  public:
    Principal_var (void);
    Principal_var (Principal_ptr);
    Principal_var (const Principal_var &);
    ~Principal_var (void);

    Principal_var &operator= (Principal_ptr);
    Principal_var &operator= (const Principal_var &);
    Principal_ptr operator-> (void) const;

    /// in, inout, out, _retn
    operator const Principal_ptr &() const;
    operator Principal_ptr &();
    Principal_ptr in (void) const;
    Principal_ptr &inout (void);
    Principal_ptr &out (void);
    Principal_ptr _retn (void);
    Principal_ptr ptr (void) const;

  private:
    Principal_ptr ptr_;
  };

  /**
   * @class Principal_out
   *
   * @brief The T_out class for CORBA::Principal
   *
   * As any other pseudo object Principal must have a T_out class,
   * the interface an semantics are specified in the CORBA spec.
   */
  class TAO_Export Principal_out
  {
  public:
    Principal_out (Principal_ptr &);
    Principal_out (Principal_var &);
    Principal_out (Principal_out &);
    Principal_out &operator= (Principal_out &);
    Principal_out &operator= (const Principal_var &);
    Principal_out &operator= (Principal_ptr);
    operator Principal_ptr &();
    Principal_ptr &ptr (void);
    Principal_ptr operator-> (void);

  private:
    Principal_ptr &ptr_;
  };
}  // End CORBA namespace

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, CORBA::Principal*);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA::Principal*&);

#if defined (__ACE_INLINE__)
# include "tao/Principal.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PRINCIPAL_H */
