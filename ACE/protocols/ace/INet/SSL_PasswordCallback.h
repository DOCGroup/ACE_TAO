// $Id$

/**
 * @file SSL_PasswordCallback.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_SSL_PASSWORDCALLBACK_H
#define ACE_SSL_PASSWORDCALLBACK_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/INet/INet_Log.h"
#include "ace/INet/INet_SSL_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_SSL_PasswordCallback
        *
        * @brief Abstract base class for SSL private key
        *   password callback.
        *
        */
        class ACE_INET_SSL_Export SSL_PasswordCallback
          {
            public:
              SSL_PasswordCallback ();
              virtual ~SSL_PasswordCallback ();

              virtual void get_privatekey_password (ACE_CString& pwd) = 0;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_SSL_PASSWORDCALLBACK_H */
