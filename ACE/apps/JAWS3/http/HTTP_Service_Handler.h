/* -*- c++ -*- */
#ifndef JAWS_HTTP_SERVICE_HANDLER_H
#define JAWS_HTTP_SERVICE_HANDLER_H

#include "ace/Synch.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/svc_export.h"

#include "jaws3/Protocol_Handler.h"

#include "HTTP_Data.h"

/**
 * Initializes the HTTP protocol handler with its state and per-connection
 * data for the JAWS framework.
 */
class JAWS_HTTP_Service_Handler
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  , public JAWS_Protocol_Handler
{
public:
  JAWS_HTTP_Service_Handler ();
  JAWS_HTTP_Service_Handler (JAWS_HTTP_Service_Handler const &) = delete;
  JAWS_HTTP_Service_Handler (JAWS_HTTP_Service_Handler &&) = delete;
  JAWS_HTTP_Service_Handler &operator= (JAWS_HTTP_Service_Handler const &) = delete;
  JAWS_HTTP_Service_Handler &operator= (JAWS_HTTP_Service_Handler &&) = delete;

  int open (void *) override;

  int close (unsigned long) override;

private:
  JAWS_HTTP_Data data_;
};

/// Accepts HTTP connections and creates their service handlers.
class ACE_Svc_Export JAWS_HTTP_Acceptor
  : public ACE_Acceptor<JAWS_HTTP_Service_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  JAWS_HTTP_Acceptor () = default;
  JAWS_HTTP_Acceptor (JAWS_HTTP_Acceptor const &) = delete;
  JAWS_HTTP_Acceptor (JAWS_HTTP_Acceptor &&) = delete;
  JAWS_HTTP_Acceptor &operator= (JAWS_HTTP_Acceptor const &) = delete;
  JAWS_HTTP_Acceptor &operator= (JAWS_HTTP_Acceptor &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]) override;
};

ACE_SVC_FACTORY_DECLARE (JAWS_HTTP_Acceptor)

#endif /* JAWS_HTTP_SERVICE_HANDLER_H */
