/**
 * $Id$
 *
 * Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __REQUEST_HANDLER_H_
#define __REQUEST_HANDLER_H_

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Thread_Manager;
ACE_END_VERSIONED_NAMESPACE_DECL

class Request_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
  {
    // = TITLE
    //   This class is the Svc_Handler used by <Acceptor>.
  public:
    Request_Handler (ACE_Thread_Manager *tm = 0);
    // The default constructor makes sure the right reactor is used.

  protected:
    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask = 0);

  private:
    size_t  nr_msgs_rcvd_;
  };

#endif /* __REQUEST_HANDLER_H_ */
