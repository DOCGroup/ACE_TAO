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

// The acceptor creates each service; callbacks destroy it on terminal errors.
class HA_Proactive_Service : public ACE_Service_Handler
{
public:
  HA_Proactive_Service () = default;
  HA_Proactive_Service (HA_Proactive_Service const &) = delete;
  HA_Proactive_Service (HA_Proactive_Service &&) = delete;
  HA_Proactive_Service &operator= (HA_Proactive_Service const &) = delete;
  HA_Proactive_Service &operator= (HA_Proactive_Service &&) = delete;

  ~HA_Proactive_Service () override
  {
    if (this->handle () != ACE_INVALID_HANDLE)
      ACE_OS::closesocket (this->handle ());
  }

  void open (ACE_HANDLE h, ACE_Message_Block &) override;

  // This method will be called when an asynchronous read
  // completes on a stream.
  void handle_read_stream
    (ACE_Asynch_Read_Stream::Result const &result) override;

  // This method will be called when an asynchronous write
  // completes on a stream.
  void handle_write_stream
    (ACE_Asynch_Write_Stream::Result const &result) override;

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
  HA_Proactive_Acceptor () = default;
  HA_Proactive_Acceptor (HA_Proactive_Acceptor const &) = delete;
  HA_Proactive_Acceptor (HA_Proactive_Acceptor &&) = delete;
  HA_Proactive_Acceptor &operator= (HA_Proactive_Acceptor const &) = delete;
  HA_Proactive_Acceptor &operator= (HA_Proactive_Acceptor &&) = delete;

  int validate_connection
    (ACE_Asynch_Accept::Result const &result,
     ACE_INET_Addr const &remote,
     ACE_INET_Addr const &local) override;
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
