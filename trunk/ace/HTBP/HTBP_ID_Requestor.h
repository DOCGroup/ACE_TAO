// -*- C++ -*-
// $Id$

#ifndef HTID_REQUESTOR_H
#define HTID_REQUESTOR_H
#include <ace/pre.h>

#include "HTBP_Export.h"
#include "ace/SString.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations
class ACE_SOCK_Stream;

namespace ACE
{
  namespace HTBP
  {
    // Forward declarations
    class Environment;

    /**
     * ACE_HTBP_ID_Requestor
     *
     * @brief This singleton class is to be used to get a HTID
     */
    class HTBP_Export ID_Requestor
    {
    public:
      /// Constructor.
      ID_Requestor (Environment * = 0);

      /// Accessor to HTID
      char *get_HTID ();

    private:
      /// Connects to the remote host
      int connect_to_server (ACE_SOCK_Stream *cli_stream);

      /// Send HTTP GET request to the server.
      int send_request (ACE_SOCK_Stream *cli_stream);

      unsigned port_;
      ACE_CString host_;
      ACE_CString url_;

      static ACE_CString htid_;
      static ACE_SYNCH_MUTEX htid_lock_;
    };

  };
};
#include <ace/post.h>
#endif /* HTID_REQUESTOR_H */
