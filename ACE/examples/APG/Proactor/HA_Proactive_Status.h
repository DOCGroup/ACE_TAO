/* -*- C++ -*- */
/*
** Example program from The ACE Programmer's Guide, Chapter 8.
** Copyright 2003 Addison-Wesley. All Rights Reserved.
*/

#ifndef __HA_PROACTIVE_STATUS_H
#define __HA_PROACTIVE_STATUS_H

#include "ace/OS_NS_sys_socket.h"
#include "ace/Asynch_IO.h"
#include "ace/Service_Object.h"
// Listing 1 code/ch08

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)

class HA_Proactive_Service : public ACE_Service_Handler
{
public:
  ~HA_Proactive_Service () override
   {
     if (this->handle () != ACE_INVALID_HANDLE)
       ACE_OS::closesocket (this->handle ());
   }

  void open (ACE_HANDLE h, ACE_Message_Block &) override;

  // This method will be called when an asynchronous read
  // completes on a stream.
  void handle_read_stream
    (const ACE_Asynch_Read_Stream::Result &result) override;

  // This method will be called when an asynchronous write
  // completes on a stream.
  void handle_write_stream
    (const ACE_Asynch_Write_Stream::Result &result) override;

private:
  ACE_Asynch_Read_Stream reader_;
  ACE_Asynch_Write_Stream writer_;
};
// Listing 1

// Listing 2 code/ch08
#include "ace/Asynch_Acceptor.h"
#include "ace/INET_Addr.h"

class HA_Proactive_Acceptor :
    public ACE_Asynch_Acceptor<HA_Proactive_Service>
{
public:
  int validate_connection
    (const ACE_Asynch_Accept::Result &result,
     const ACE_INET_Addr &remote,
     const ACE_INET_Addr &local) override;
};
// Listing 2

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS */

#if 0
// Listing 3 code/ch08
template <class HANDLER>
class ACE_Asynch_Acceptor : public ACE_Handler
      ...
protected:
  virtual HANDLER *make_handler ()
  {
    return new HANDLER;
  }
// Listing 3
#endif

class HA_Proactive_Status : public ACE_Service_Object
{
public:
  // Initializes object when dynamic linking occurs.
  int init (int argc, ACE_TCHAR *argv[]) override;

  // Terminates object when dynamic unlinking occurs.
  int fini () override;
};

#endif /* __HA_PROACTIVE_STATUS_H */
