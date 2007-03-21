// -*- C++ -*-

//=============================================================================
/**
 * @file SL3_CredentialsCurator.h
 *
 * $Id$
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
    typedef CredentialsCurator* CredentialsCurator_ptr;
    typedef TAO_Pseudo_Var_T<CredentialsCurator> CredentialsCurator_var;
    typedef TAO_Pseudo_Out_T<CredentialsCurator> CredentialsCurator_out;

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
        public virtual TAO_Local_RefCounted_Object
    {
    public:
      typedef CredentialsCurator_ptr _ptr_type;
      typedef CredentialsCurator_var _var_type;
      typedef CredentialsCurator_out _out_type;

      /**
       * The type of table that maps acquisition method to acquirer
       * factory.
       */
      typedef ACE_Map_Manager<const char *,
                              TAO::SL3::CredentialsAcquirerFactory *,
                              ACE_Null_Mutex> Acquirer_Factory_Table;
      typedef Acquirer_Factory_Table::iterator Factory_Iterator;

      typedef ACE_Hash_Map_Manager_Ex <const char *,
                                       SecurityLevel3::OwnCredentials_var,
                                       ACE_Hash<const char *>,
                                       ACE_Equal_To<const char *>,
                                       ACE_Null_Mutex> Credentials_Table;
      typedef Credentials_Table::iterator Credentials_Iterator;

      /// Constructor
      CredentialsCurator (void);

      static CredentialsCurator_ptr _duplicate (CredentialsCurator_ptr obj);
      static CredentialsCurator_ptr _nil (void);
      static CredentialsCurator_ptr _narrow (CORBA::Object_ptr obj);

      /**
       * @name SecurityLevel3::CredentialsCurator Methods
       *
       * Methods required by the SecurityLevel3::CredentialsCurator
       * interface.
       */
      //@{
      virtual SecurityLevel3::AcquisitionMethodList * supported_methods ();

      virtual SecurityLevel3::CredentialsAcquirer_ptr acquire_credentials (
          const char * acquisition_method,
          const CORBA::Any & acquisition_arguments);

      virtual SecurityLevel3::OwnCredentialsList * default_creds_list ();

      virtual SecurityLevel3::CredentialsIdList * default_creds_ids ();

      virtual SecurityLevel3::OwnCredentials_ptr get_own_credentials (
          const char * credentials_id);

      virtual void release_own_credentials (const char * credentials_id);
      //@}

      /// Register CredentialsAcquirer factory.
      /**
       * @note The CredentialsCurator retains ownership of the
       *       factory.
       */
      void register_acquirer_factory (
        const char * acquisition_method,
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
      ~CredentialsCurator (void);

    private:

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
