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
        class ACE_INET_Export AuthenticationBase
          {
            public:
              virtual ~AuthenticationBase ();

              virtual const ACE_CString& scheme () const = 0;

              const ACE_CString& realm () const;

              const ACE_CString& user () const;
              void user (const ACE_CString& usr);

              const ACE_CString& password () const;
              void password (const ACE_CString& pw);

            protected:
              AuthenticationBase (const ACE_CString& realm,
                                  const ACE_CString& user,
                                  const ACE_CString& pw);

            private:
              ACE_CString realm_;
              ACE_CString user_;
              ACE_CString password_;
          };

        class ACE_INET_Export AuthenticatorBase
          {
            public:
              virtual ~AuthenticatorBase ();

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
