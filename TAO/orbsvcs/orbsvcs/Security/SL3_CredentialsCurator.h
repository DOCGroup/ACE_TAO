// -*- C++ -*-

//=============================================================================
/**
 * @file SL3_CredentialsCurator.h
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SL3_CREDENTIALS_CURATOR_H
#define TAO_SL3_CREDENTIALS_CURATOR_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel3C.h"

#include "tao/LocalObject.h"

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Map_Manager.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SL3
  {
    class CredentialsAcquirerFactory;

    class CredentialsCurator;
    using CredentialsCurator_ptr = CredentialsCurator *;
    using CredentialsCurator_var = TAO_Pseudo_Var_T<CredentialsCurator>;
    using CredentialsCurator_out = TAO_Pseudo_Out_T<CredentialsCurator>;

    /**
     * @class CredentialsCurator
     *
     * @brief Implementation of the SecurityLevel3::CredentialsCurator
     *        object.
     *
     * This class provides a means for creating and managing
     * OwnCredentials.
     */
    class TAO_Security_Export CredentialsCurator
      : public virtual SecurityLevel3::CredentialsCurator,
        public virtual ::CORBA::LocalObject
    {
    public:
      using _ptr_type = CredentialsCurator_ptr;
      using _var_type = CredentialsCurator_var;
      using _out_type = CredentialsCurator_out;

      /**
       * The type of table that maps acquisition method to acquirer
       * factory.
       */
      using Acquirer_Factory_Table = ACE_Map_Manager<char const *,
                                                      TAO::SL3::CredentialsAcquirerFactory *,
                                                      ACE_Null_Mutex>;
      using Factory_Iterator = Acquirer_Factory_Table::iterator;

      using Credentials_Table = ACE_Hash_Map_Manager_Ex<char const *,
                                                         SecurityLevel3::OwnCredentials_var,
                                                         ACE_Hash<char const *>,
                                                         ACE_Equal_To<char const *>,
                                                         ACE_Null_Mutex>;
      using Credentials_Iterator = Credentials_Table::iterator;

      /// Constructor
      CredentialsCurator ();

      static CredentialsCurator_ptr _duplicate (CredentialsCurator_ptr obj);
      static CredentialsCurator_ptr _nil ();
      static CredentialsCurator_ptr _narrow (CORBA::Object_ptr obj);

      /**
       * @name SecurityLevel3::CredentialsCurator Methods
       *
       * Methods required by the SecurityLevel3::CredentialsCurator
       * interface.
       */
      //@{
      SecurityLevel3::AcquisitionMethodList * supported_methods () override;

      SecurityLevel3::CredentialsAcquirer_ptr acquire_credentials (
          char const * acquisition_method,
          CORBA::Any const & acquisition_arguments) override;

      SecurityLevel3::OwnCredentialsList * default_creds_list () override;

      SecurityLevel3::CredentialsIdList * default_creds_ids () override;

      SecurityLevel3::OwnCredentials_ptr get_own_credentials (
          char const * credentials_id) override;

      void release_own_credentials (char const * credentials_id) override;
      //@}

      /// Register CredentialsAcquirer factory.
      /**
       * @note The CredentialsCurator retains ownership of the
       *       factory.
       */
      void register_acquirer_factory (
        char const * acquisition_method,
        TAO::SL3::CredentialsAcquirerFactory * factory);

      /// TAO-specific means of adding credentials to this
      /// CredentialsCurator's "own credentials" list.
      void _tao_add_own_credentials (
        SecurityLevel3::OwnCredentials_ptr credentials);

    protected:
      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~CredentialsCurator () override;

    private:
      CredentialsCurator (CredentialsCurator const &) = delete;
      CredentialsCurator (CredentialsCurator &&) = delete;
      CredentialsCurator &operator= (CredentialsCurator const &) = delete;
      CredentialsCurator &operator= (CredentialsCurator &&) = delete;

      /// Lock used to synchronize access to underlying tables.
      TAO_SYNCH_MUTEX lock_;

      /// Table of CredentialsAcquirer factories.
      Acquirer_Factory_Table acquirer_factories_;

      /// Table of OwnCredentials.
      Credentials_Table credentials_table_;
    };
  } // End SL3 namespace
}  // End TAO namespace


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SL3_CREDENTIALS_CURATOR_H */
