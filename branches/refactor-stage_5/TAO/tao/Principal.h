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
#include /**/ "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "OctetSeqC.h"
#include "ace/Synch_T.h"

namespace CORBA
{
  class Principal;
  typedef TAO_Pseudo_Var_T<Principal> Principal_var;
  typedef TAO_Pseudo_Out_T<Principal, Principal_var> Principal_out;

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
    static Principal * _duplicate (Principal *);
    static Principal * _nil (void);

    // = Stuff required for memory management.
    CORBA::ULong _incr_refcnt (void);
    CORBA::ULong _decr_refcnt (void);

    Principal (void);

    // Useful for template programming.
    typedef Principal_ptr _ptr_type;
    typedef Principal_var _var_type;

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
}  // End CORBA namespace

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR &, CORBA::Principal *);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR &, CORBA::Principal *&);

#if defined (__ACE_INLINE__)
# include "tao/Principal.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_PRINCIPAL_H */
