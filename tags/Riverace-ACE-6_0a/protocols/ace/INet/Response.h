// $Id$

/**
 * @file Response.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_RESPONSE_H
#define ACE_INET_RESPONSE_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {

        /**
        * @class ACE_INet_Response
        *
        * @brief Base class for responses.
        *
        */
        class ACE_INET_Export Response
          {
            public:
              Response ();
              virtual ~Response ();
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_INET_RESPONSE_H */
