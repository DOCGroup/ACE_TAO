// -*- C++ -*-

// ===================================================================
/**
 *  @file   PrincipalAuthenticator.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_PRINCIPAL_AUTHENTICATOR_H
#define TAO_PRINCIPAL_AUTHENTICATOR_H

#include /**/ "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityReplaceableC.h"

#include "ace/Array_Base.h"

class TAO_PrincipalAuthenticator;
typedef TAO_PrincipalAuthenticator *TAO_PrincipalAuthenticator_ptr;
class TAO_PrincipalAuthenticator_var;
class TAO_PrincipalAuthenticator_Impl;

class TAO_SecurityManager;

/**
 * @class TAO_PrincipalAuthenticator
 *
 * @brief Implementation of the SecurityLevel2::PrincipalAuthenticator
 *        interface.
 *
 * This class implements the chain-of-responsibility design pattern to
 * decide which security mechanism
 */
class TAO_Security_Export TAO_PrincipalAuthenticator
  : public virtual SecurityLevel2::PrincipalAuthenticator,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_PrincipalAuthenticator (TAO_SecurityManager *manager);

  /**
   * @name SecurityLevel2::PrincipalAuthenticator Methods
   *
   * Methods exposed by the SecurityLevel2::PrincipalAuthenticator
   * interface.
   */
  //@{
  /// Return the authentication methods supported by the given
  /// mechanism.
  virtual Security::AuthenticationMethodList * get_supported_authen_methods (
      const char * mechanism
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Authenticate the principal, request privileges and create
  /// Credentials.
  /**
   * Created Credentials will be placed in the SecurityManager
   * object's own_credentials list.
   * @par
   * If authenticate() returns with a Security::SecAuthContinue,
   * e.g. due to an inability to complete authentication in a single
   * operation, then authentication must completed using the
   * continue_authentication() method.
   */
  virtual Security::AuthenticationStatus authenticate (
      Security::AuthenticationMethod method,
      const char * mechanism,
      const char * security_name,
      const CORBA::Any & auth_data,
      const Security::AttributeList & privileges,
      SecurityLevel2::Credentials_out creds,
      CORBA::Any_out continuation_data,
      CORBA::Any_out auth_specific_data
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Continue partially completed authentication process.
  virtual Security::AuthenticationStatus continue_authentication (
      const CORBA::Any & response_data,
      SecurityLevel2::Credentials_ptr creds,
      CORBA::Any_out continuation_data,
      CORBA::Any_out auth_specific_data
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /// Register a security mechanism-specific (e.g. SSLIOP, Kerberos,
  /// etc.) SecurityReplaceable::Vault implementation with the
  /// PrincipalAuthenticator object (i.e. this object).
  /**
   * @note This is a TAO-specific extension.
   */
  void register_vault (SecurityReplaceable::Vault_ptr vault
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * @name Downcast Related Methods
   *
   * Methods used for downcasting and manipulating the reference count
   * of this interface.
   */
  //@{
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_PrincipalAuthenticator_ptr _ptr_type;
  typedef TAO_PrincipalAuthenticator_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static int _tao_class_id;

  // The static operations.
  static TAO_PrincipalAuthenticator_ptr _duplicate (TAO_PrincipalAuthenticator_ptr obj);

  static TAO_PrincipalAuthenticator_ptr _narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );

  static TAO_PrincipalAuthenticator_ptr _unchecked_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );

  static TAO_PrincipalAuthenticator_ptr _nil (void)
    {
      return (TAO_PrincipalAuthenticator_ptr)0;
    }

  virtual void *_tao_QueryInterface (ptrdiff_t type);
  virtual const char* _interface_repository_id (void) const;
  //@}

protected:

  /// Destructor
  /**
   * Protected destructor to enforce the fact this class is reference
   * counted, and should not be destroyed using delete() by anything
   * other than the reference counting mechanism.
   */
  ~TAO_PrincipalAuthenticator (void);

private:

  /// Array of registered security mechanism-specific Vaults.
  ACE_Array_Base<void *> vaults_;

  /// Pointer to the SecurityManager that "owns" this
  /// PrincipalAuthenticator.
  TAO_SecurityManager * security_manager_;

};


class TAO_Security_Export TAO_PrincipalAuthenticator_var : public TAO_Base_var
{
public:
  TAO_PrincipalAuthenticator_var (void); // default constructor
  TAO_PrincipalAuthenticator_var (TAO_PrincipalAuthenticator_ptr p) : ptr_ (p) {}
  TAO_PrincipalAuthenticator_var (const TAO_PrincipalAuthenticator_var &); // copy constructor
  ~TAO_PrincipalAuthenticator_var (void); // destructor

  TAO_PrincipalAuthenticator_var &operator= (TAO_PrincipalAuthenticator_ptr);
  TAO_PrincipalAuthenticator_var &operator= (const TAO_PrincipalAuthenticator_var &);
  TAO_PrincipalAuthenticator_ptr operator-> (void) const;

  operator const TAO_PrincipalAuthenticator_ptr &() const;
  operator TAO_PrincipalAuthenticator_ptr &();
  // in, inout, out, _retn
  TAO_PrincipalAuthenticator_ptr in (void) const;
  TAO_PrincipalAuthenticator_ptr &inout (void);
  TAO_PrincipalAuthenticator_ptr &out (void);
  TAO_PrincipalAuthenticator_ptr _retn (void);
  TAO_PrincipalAuthenticator_ptr ptr (void) const;

  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_PrincipalAuthenticator_ptr tao_duplicate (TAO_PrincipalAuthenticator_ptr);
  static void tao_release (TAO_PrincipalAuthenticator_ptr);
  static TAO_PrincipalAuthenticator_ptr tao_nil (void);
  static TAO_PrincipalAuthenticator_ptr tao_narrow (
      CORBA::Object *
      ACE_ENV_ARG_DECL_NOT_USED
    );
  static CORBA::Object * tao_upcast (void *);

private:

  TAO_PrincipalAuthenticator_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  TAO_PrincipalAuthenticator_var (const TAO_Base_var &rhs);
  TAO_PrincipalAuthenticator_var &operator= (const TAO_Base_var &rhs);

};


class TAO_Security_Export TAO_PrincipalAuthenticator_out
{
public:

  TAO_PrincipalAuthenticator_out (TAO_PrincipalAuthenticator_ptr &);
  TAO_PrincipalAuthenticator_out (TAO_PrincipalAuthenticator_var &);
  TAO_PrincipalAuthenticator_out (const TAO_PrincipalAuthenticator_out &);
  TAO_PrincipalAuthenticator_out &operator= (const TAO_PrincipalAuthenticator_out &);
  TAO_PrincipalAuthenticator_out &operator= (const TAO_PrincipalAuthenticator_var &);
  TAO_PrincipalAuthenticator_out &operator= (TAO_PrincipalAuthenticator_ptr);
  operator TAO_PrincipalAuthenticator_ptr &();
  TAO_PrincipalAuthenticator_ptr &ptr (void);
  TAO_PrincipalAuthenticator_ptr operator-> (void);

private:

  TAO_PrincipalAuthenticator_ptr &ptr_;

};


#include /**/ "ace/post.h"

#endif  /* TAO_PRINCIPAL_AUTHENTICATOR_H */
