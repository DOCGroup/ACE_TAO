// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_Credentials.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_CREDENTIALS_H
#define TAO_SSLIOP_CREDENTIALS_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_X509.h"

#ifndef NO_RSA
# include "SSLIOP_RSA.h"
#endif  /* !NO_RSA */

// #ifndef NO_DSA
// # include "SSLIOP_DSA.h"
// #endif  /* !NO_DSA */

#include "orbsvcs/SecurityLevel2C.h"


class TAO_SSLIOP_Credentials;
typedef TAO_SSLIOP_Credentials* TAO_SSLIOP_Credentials_ptr;

class TAO_SSLIOP_Credentials_var : public TAO_Base_var
{
public:
  TAO_SSLIOP_Credentials_var (void); // default constructor
  TAO_SSLIOP_Credentials_var (TAO_SSLIOP_Credentials_ptr p) : ptr_ (p) {} 
  TAO_SSLIOP_Credentials_var (const TAO_SSLIOP_Credentials_var &); // copy constructor
  ~TAO_SSLIOP_Credentials_var (void); // destructor
  
  TAO_SSLIOP_Credentials_var &operator= (TAO_SSLIOP_Credentials_ptr);
  TAO_SSLIOP_Credentials_var &operator= (const TAO_SSLIOP_Credentials_var &);
  TAO_SSLIOP_Credentials_ptr operator-> (void) const;
  
  operator const TAO_SSLIOP_Credentials_ptr &() const;
  operator TAO_SSLIOP_Credentials_ptr &();
  // in, inout, out, _retn 
  TAO_SSLIOP_Credentials_ptr in (void) const;
  TAO_SSLIOP_Credentials_ptr &inout (void);
  TAO_SSLIOP_Credentials_ptr &out (void);
  TAO_SSLIOP_Credentials_ptr _retn (void);
  TAO_SSLIOP_Credentials_ptr ptr (void) const;
  
  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_SSLIOP_Credentials_ptr tao_duplicate (TAO_SSLIOP_Credentials_ptr);
  static void tao_release (TAO_SSLIOP_Credentials_ptr);
  static TAO_SSLIOP_Credentials_ptr tao_nil (void);
  static TAO_SSLIOP_Credentials_ptr tao_narrow (
      CORBA::Object *
      TAO_ENV_ARG_DECL_NOT_USED
    );
  static CORBA::Object * tao_upcast (void *);

private:
  TAO_SSLIOP_Credentials_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  TAO_SSLIOP_Credentials_var (const TAO_Base_var &rhs);
  TAO_SSLIOP_Credentials_var &operator= (const TAO_Base_var &rhs);
};


class  TAO_SSLIOP_Credentials_out
{
public:
  TAO_SSLIOP_Credentials_out (TAO_SSLIOP_Credentials_ptr &);
  TAO_SSLIOP_Credentials_out (TAO_SSLIOP_Credentials_var &);
  TAO_SSLIOP_Credentials_out (const TAO_SSLIOP_Credentials_out &);
  TAO_SSLIOP_Credentials_out &operator= (const TAO_SSLIOP_Credentials_out &);
  TAO_SSLIOP_Credentials_out &operator= (const TAO_SSLIOP_Credentials_var &);
  TAO_SSLIOP_Credentials_out &operator= (TAO_SSLIOP_Credentials_ptr);
  operator TAO_SSLIOP_Credentials_ptr &();
  TAO_SSLIOP_Credentials_ptr &ptr (void);
  TAO_SSLIOP_Credentials_ptr operator-> (void);

private:
  TAO_SSLIOP_Credentials_ptr &ptr_;
};


/**
 * @class TAO_SSLIOP_Credentials
 *
 * @brief SSLIOP-specific implementation of the
 *        SecurityLevel2::Credentials interface.
 *
 * This class encapsulates the X.509 certificate associated with a
 * given a principal.
 */
class TAO_SSLIOP_Credentials
  : public virtual SecurityLevel2::Credentials,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_SSLIOP_Credentials (X509 *cert);

#ifndef NO_RSA
  /// Constructor
  TAO_SSLIOP_Credentials (X509 *cert, RSA *rsa);
#endif  /* !NO_RSA */

// #ifndef NO_DSA
//   /// Constructor
//   TAO_SSLIOP_Credentials (X509 *cert, DSA *dsa);
// #endif  /* !NO_DSA */

  /// Return a duplicate (specifically a deep copy) of this credential
  /// object.
  virtual SecurityLevel2::Credentials_ptr copy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the invocation credentials type this object is.
  /**
   * If this Credentials object was created by the
   * PrincipalAuthenticator then the "own" credentials type
   * (i.e. Security::SecOwnCredentials) is returned.  Specifically,
   * the credentials represented by this Credentials object are our
   * own, not the peer's credentials.
   * @par
   * If this Credentials object corresponds to the credentials
   * received from the peer, then the "received" credentials type
   * (i.e. Security::SecReceivedCredentials) is returned.
   * @par
   * If this Credentials object corresponds to the target object's
   * credentials then the "target" credentials type
   * (i.e. Security::SecTargetCredentials) is returned.
   */
  virtual Security::InvocationCredentialsType credentials_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AuthenticationStatus authentication_state (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * mechanism (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions accepting_options_supported (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void accepting_options_supported (
      Security::AssociationOptions accepting_options_supported
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions accepting_options_required (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void accepting_options_required (
      Security::AssociationOptions accepting_options_required
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions invocation_options_supported (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void invocation_options_supported (
      Security::AssociationOptions invocation_options_supported
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions invocation_options_required (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void invocation_options_required (
      Security::AssociationOptions invocation_options_required
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean get_security_feature (
      Security::CommunicationDirection direction,
      Security::SecurityFeature feature
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean set_attributes (
      const Security::AttributeList & requested_attributes,
      Security::AttributeList_out actual_attributes
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AttributeList * get_attributes (
      const Security::AttributeTypeList & attributes
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Verify that this credential is still valid.
  virtual CORBA::Boolean is_valid (
      Security::UtcT_out expiry_time
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean refresh (
      const CORBA::Any & refresh_data
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@{
  /// Return a pointer to the underlying X.509 certificate.
  /**
   * @note Caller owns the returned object.  Use a
   *       TAO_SSLIOP_X509_var.
   */
  X509 *x509 (void);
  //@}

#ifndef NO_RSA
  //@{
  /// Return a pointer to the underlying RSA key.
  /**
   * @return Non-zero value if RSA key is used.
   *
   * @note Caller owns the returned object.  Use a
   *       TAO_SSLIOP_RSA_var.
   */
  RSA *rsa (void);
  //@}
#endif  /* !NO_RSA */

// #ifndef NO_DSA
//   //@{
//   /// Return a pointer to the underlying DSA key.
//   /**
//    * @return Non-zero value if DSA key is used.
//    *
//    * @note Caller owns the returned object.  Use a
//    *       TAO_SSLIOP_DSA_var.
//    */
//   DSA *dsa (void);
//   //@}
// #endif  /* !NO_DSA */

  CORBA::Boolean operator== (const TAO_SSLIOP_Credentials &rhs);

  CORBA::ULong hash (void) const;

  //@{
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_SSLIOP_Credentials_ptr _ptr_type;
  typedef TAO_SSLIOP_Credentials_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static int _tao_class_id;
  
  // The static operations.
  static TAO_SSLIOP_Credentials_ptr _duplicate (TAO_SSLIOP_Credentials_ptr obj);
  
  static TAO_SSLIOP_Credentials_ptr _narrow (
      CORBA::Object_ptr obj
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    );
  
  static TAO_SSLIOP_Credentials_ptr _unchecked_narrow (
      CORBA::Object_ptr obj
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    );
  
  static TAO_SSLIOP_Credentials_ptr _nil (void)
    {
      return (TAO_SSLIOP_Credentials_ptr)0;
    }
  
  virtual void *_tao_QueryInterface (ptr_arith_t type);
  //@}

protected:

  /// Destructor.
  /**
   * Destructor is protected to enforce proper reference counting.
   * Specifically, this reference counted object should only be
   * deallocated by the reference counting mechanism.
   */
  ~TAO_SSLIOP_Credentials (void);

protected:

  /// Security association options supported by this Credentials
  /// object when accepting client invocations.
  /**
   * This attribute corresponds to the SSLIOP::target_supports field
   * in the security association options in the SSLIOP tagged
   * component that is embedded in IORs created by SSLIOP-enabled
   * server side ORBs.
   */
  Security::AssociationOptions accepting_options_supported_;

  /// Security association options required by this Credentials
  /// object when accepting client invocations.
  /**
   * This attribute corresponds to the SSLIOP::target_requires field
   * in the security association options in the SSLIOP tagged
   * component that is embedded in IORs created by SSLIOP-enabled
   * server side ORBs.
   */
  Security::AssociationOptions accepting_options_required_;

  /// Security association options supported by this Credentials
  /// object when invoking operations on the target object.
  Security::AssociationOptions invocation_options_supported_;

  /// Security association options required by this Credentials
  /// object when invoking operations on the target object.
  Security::AssociationOptions invocation_options_required_;

  /// Reference to the X.509 certificate associated with this SSLIOP
  /// Credentials object.
  TAO_SSLIOP_X509_var x509_;

#ifndef NO_RSA
  TAO_SSLIOP_RSA_var rsa_;
#endif  /* !NO_RSA */

// #ifndef NO_DSA
//   TAO_SSLIOP_DSA_var dsa_;
// #endif  /* !NO_DSA */

};

#if defined (__ACE_INLINE__)
# include "SSLIOP_Credentials.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_CREDENTIALS_H */
