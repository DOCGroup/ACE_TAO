// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_RSA.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_RSA_H
#define TAO_SSLIOP_RSA_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <openssl/crypto.h>

// If OpenSSL was not compiled with RSA key support then do not
// compile in support for the RSA related classes in this file.
#ifndef NO_RSA

#include "tao/varbase.h"

#include <openssl/rsa.h>

/**
 * @class TAO_SSLIOP_RSA
 *
 * @brief Class/namespace that contains TAO-specific X.509 manipulation
 *        functions.
 *
 * This class provides CORBA-like _duplicate() and release() methods.
 * They are simply RSA reference count manipulation methods.
 */
class TAO_SSLIOP_RSA
{
public:

  /**
   * @name CORBA-style Reference Count Manipulation Methods
   */
  /// Increase the reference count on the given RSA structure.
  static RSA *_duplicate (RSA *cert);

  /// Decrease the reference count on the given RSA structure.
  static void release (RSA *cert);

};

/**
 * @class TAO_SSLIOP_RSA_var
 *
 * @brief "_var" class for the OpenSSL @param RSA structure.
 *
 * This class is simply used to make operations on instances of the
 * OpenSSL @param RSA structures exception safe.  It is only used
 * internally by the SSLIOP pluggable protocol.
 */
class TAO_SSLIOP_RSA_var : public TAO_Base_var
{
public:

  /**
   * @name Constructors
   */
  //@{
  TAO_SSLIOP_RSA_var (void);
  TAO_SSLIOP_RSA_var (RSA *x);
  TAO_SSLIOP_RSA_var (const TAO_SSLIOP_RSA_var &);
  //@}

  /// Destructor
  ~TAO_SSLIOP_RSA_var (void);

  TAO_SSLIOP_RSA_var &operator= (RSA *);
  TAO_SSLIOP_RSA_var &operator= (const TAO_SSLIOP_RSA_var &);
  const RSA *operator-> (void) const;
  RSA *operator-> (void);

  operator const RSA &() const;
  operator RSA &();

  RSA *in (void) const;
  RSA *&inout (void);
  RSA *&out (void);
  RSA *_retn (void);
  RSA *ptr (void) const;
  
private:

  // Unimplemented - prevents widening assignment.
  TAO_SSLIOP_RSA_var (const TAO_Base_var &rhs);
  TAO_SSLIOP_RSA_var &operator= (const TAO_Base_var &rhs);

private:

  /// The OpenSSL RSA structure that represents a RSA key.
  RSA *rsa_;

};


#if defined (__ACE_INLINE__)
#include "SSLIOP_RSA.inl"
#endif  /* __ACE_INLINE__ */

#endif  /* !NO_DSA */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_RSA_H */
