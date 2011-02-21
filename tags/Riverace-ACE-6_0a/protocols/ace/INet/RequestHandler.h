// $Id$

/**
 * @file RequestHandler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_REQUEST_HANDLER_H
#define ACE_INET_REQUEST_HANDLER_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"
#include "ace/INet/Request.h"
#include "ace/INet/Response.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {

        /**
        * @class ACE_INet_RequestHandler
        *
        * @brief Abstract base for request handlers.
        *
        */
        class ACE_INET_Export RequestHandler
          {
            public:
              RequestHandler ();
              virtual ~RequestHandler ();

              virtual Request& request () = 0;

              virtual const Request& request () const = 0;

              virtual Response& response () = 0;

              virtual const Response& response () const = 0;
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_INET_REQUEST_HANDLER_H */
