// $Id$

/**
 * @file AuthenticationBase.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_AUTHENTICATION_BASE_H
#define ACE_INET_AUTHENTICATION_BASE_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"
#include "ace/SString.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_AuthenticationBase
        *
        * @brief Base class for authentication information
        *        holder.
        *
        */
        class ACE_INET_Export AuthenticationBase
          {
            public:
              /// Destructor
              virtual ~AuthenticationBase ();

              /// Returns authentication scheme (f.i. 'ftp' or 'http:basic').
              virtual const ACE_CString& scheme () const = 0;

              /// Returns authentication realm.
              virtual const ACE_CString& realm () const = 0;

              /// Returns user id.
              virtual const ACE_CString& user () const = 0;

              /// Changes user id.
              virtual void user (const ACE_CString& usr) = 0;

              /// Returns password.
              virtual const ACE_CString& password () const = 0;

              /// Changes password.
              virtual void password (const ACE_CString& pw) = 0;

            protected:
              /// Constructor
              AuthenticationBase ();
          };

        /**
        * @class ACE_INet_AuthenticatorBase
        *
        * @brief Base class for authenticators.
        *
        */
        class ACE_INET_Export AuthenticatorBase
          {
            public:
              /// Default constructor
              AuthenticatorBase ();

              /// Destructor
              virtual ~AuthenticatorBase ();

              /// Verifies authentication for given info and returns true if
              /// verification succeeded, false otherwise.
              /// If true, the authentication information is updated with the actual
              /// credentials.
              virtual bool authenticate(AuthenticationBase& authentication) const = 0;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/AuthenticationBase.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_AUTHENTICATION_BASE_H */
