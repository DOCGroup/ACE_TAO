/**
 * $Id$
 *
 * Home Automation Status server. Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __HASTATUS_H_
#define __HASTATUS_H_

#include "ace/OS.h"
#include "ace/Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Service_Object.h"
#include "ace/Svc_Handler.h"

#include "HASTATUS_export.h"

// Listing 10
class ClientHandler :
    public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

    ClientHandler()
            : super()
    {
        // Exclude 10
        ACE_DEBUG(( LM_INFO,
                    "ClientHandler ctor\n"
                    ));
        // Exclude 10
    }
// Listing 10

    ~ClientHandler()
    {
        ACE_DEBUG(( LM_INFO,
                    "ClientHandler dtor\n"
                    ));
    }

// Listing 13
    int handle_input (ACE_HANDLE)
    {
        char buf[64];
        int bytesReceived;

        if( (bytesReceived =
             this->peer_.recv( buf, sizeof(buf)-1 )) < 1 )
        {
            ACE_DEBUG(( LM_INFO,
                        "ClientHandler handle_input: "
                        "Received %d bytes. Leaving.\n",
                        bytesReceived
                        ));
            return -1;
        }

        buf[bytesReceived] = 0;
        ACE_DEBUG(( LM_INFO,
                    "ClientHandler handle_input: %s\n",
                    buf
                    ));

        return 0;
    }
};
// Listing 13


class HASTATUS_Export HA_Status : public ACE_Service_Object
{
public:
  virtual int init (int argc, ACE_TCHAR *argv[]);

private:
  ACE_Acceptor<ClientHandler, ACE_SOCK_ACCEPTOR> acceptor_;
  ACE_INET_Addr listen_addr_;
};

#endif /* __HASTATUS_H_ */
