// $Id$

/**
 * @file Request.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_REQUEST_H
#define ACE_INET_REQUEST_H

#include /**/ "ace/pre.h"

#include "INet_Export.h"
#include "HeaderBase.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {

        /**
        * @class ACE_INet_Request
        *
        * @brief Base class for requests.
        *
        */
        class ACE_INET_Export Request
          : public virtual HeaderBase
          {
            public:
              Request ();
              virtual ~Request ();
          };

      };
  };

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_INET_REQUEST_H */
