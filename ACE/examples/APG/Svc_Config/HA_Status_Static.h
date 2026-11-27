/**
 * Home Automation Status server. Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __HASTATUS_H_
#define __HASTATUS_H_

// Listing 1 code/ch19
#include "ace/Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Service_Object.h"
#include "ace/Svc_Handler.h"
#include "ace/Service_Config.h"

class ClientHandler :
      public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  ClientHandler () = default;
  ClientHandler (ClientHandler const &) = delete;
  ClientHandler (ClientHandler &&) = delete;
  ClientHandler &operator= (ClientHandler const &) = delete;
  ClientHandler &operator= (ClientHandler &&) = delete;

  // ... Same as previous examples.
};

class HA_Status : public ACE_Service_Object
{
public:
  HA_Status () = default;
  HA_Status (HA_Status const &) = delete;
  HA_Status (HA_Status &&) = delete;
  HA_Status &operator= (HA_Status const &) = delete;
  HA_Status &operator= (HA_Status &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;
  int info (ACE_TCHAR **str, size_t len) const override;

private:
  ACE_Acceptor<ClientHandler, ACE_SOCK_ACCEPTOR> acceptor_;
  ACE_INET_Addr listen_addr_;
};
// Listing 1

#endif /* __HASTATUS_H_ */
