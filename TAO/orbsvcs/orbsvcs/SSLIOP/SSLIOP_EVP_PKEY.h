// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_EVP_PKEY.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_EVP_PKEY_H
#define TAO_SSLIOP_EVP_PKEY_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/varbase.h"

#include <openssl/crypto.h>
#include <openssl/evp.h>


/**
 * @class TAO_SSLIOP_EVP_PKEY
 *
 * @brief Class/namespace that contains TAO-specific OpenSSL EVP_PKEY
 *        data structure manipulation functions.
 *
 * This class provides CORBA-like _duplicate() and release() methods.
 * They are simply EVP_PKEY CORBA-style reference count manipulation
 * methods.
 */
class TAO_SSLIOP_EVP_PKEY
{
public:

  /**
   * @name CORBA-style Reference Count Manipulation Methods
   */
  /// Increase the reference count on the given EVP_PKEY structure.
  static EVP_PKEY *_duplicate (EVP_PKEY *key);

  /// Decrease the reference count on the given EVP_PKEY structure.
  static void release (EVP_PKEY *key);

};

/**
 * @class TAO_SSLIOP_EVP_PKEY_var
 *
 * @brief "_var" class for the OpenSSL @param EVP_PKEY structure.
 *
 * This class is simply used to make operations on instances of the
 * OpenSSL @param EVP_PKEY structures exception safe.  It is only used
 * internally by the SSLIOP pluggable protocol.
 */
class TAO_SSLIOP_EVP_PKEY_var : public TAO_Base_var
{
public:

  /**
   * @name Constructors
   */
  //@{
  TAO_SSLIOP_EVP_PKEY_var (void);
  TAO_SSLIOP_EVP_PKEY_var (EVP_PKEY *x);
  TAO_SSLIOP_EVP_PKEY_var (const TAO_SSLIOP_EVP_PKEY_var &);
  //@}

  /// Destructor
  ~TAO_SSLIOP_EVP_PKEY_var (void);

  TAO_SSLIOP_EVP_PKEY_var &operator= (EVP_PKEY *);
  TAO_SSLIOP_EVP_PKEY_var &operator= (const TAO_SSLIOP_EVP_PKEY_var &);
  const EVP_PKEY *operator-> (void) const;
  EVP_PKEY *operator-> (void);

  operator const EVP_PKEY &() const;
  operator EVP_PKEY &();

  EVP_PKEY *in (void) const;
  EVP_PKEY *&inout (void);
  EVP_PKEY *&out (void);
  EVP_PKEY *_retn (void);
  EVP_PKEY *ptr (void) const;
  
private:

  // Unimplemented - prevents widening assignment.
  TAO_SSLIOP_EVP_PKEY_var (const TAO_Base_var &rhs);
  TAO_SSLIOP_EVP_PKEY_var &operator= (const TAO_Base_var &rhs);

private:

  /// The OpenSSL EVP_PKEY structure that represents a private key.
  EVP_PKEY *evp_;

};


#if defined (__ACE_INLINE__)
#include "SSLIOP_EVP_PKEY.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_EVP_PKEY_H */
