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
              virtual ~AuthenticationBase ();

              /// Returns authentication scheme (f.i. 'ftp' or 'http:basic').
              virtual ACE_CString const& scheme () const = 0;

              /// Returns authentication realm.
              virtual ACE_CString const& realm () const = 0;

              /// Returns user id.
              virtual ACE_CString const& user () const = 0;

              /// Changes user id.
              virtual void user (ACE_CString const& usr) = 0;

              /// Returns password.
              virtual ACE_CString const& password () const = 0;

              /// Changes password.
              virtual void password (ACE_CString const& pw) = 0;

            protected:
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
              AuthenticatorBase ();

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
