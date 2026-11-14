/* -*- c++ -*- */
#ifndef TERA_SS_SERVICE_HANDLER_H
#define TERA_SS_SERVICE_HANDLER_H

#include "ace/Synch.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/svc_export.h"

#include "jaws3/Protocol_Handler.h"

#include "SS_Data.h"

/**
 * Handles a request for the small server and selects its protocol state.
 *
 * This class initializes the protocol handler with the state and data used
 * by the underlying framework.
 */
class TeraSS_Service_Handler
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  , public JAWS_Protocol_Handler
{
public:
  TeraSS_Service_Handler ();

  TeraSS_Service_Handler (TeraSS_Service_Handler const &) = delete;
  TeraSS_Service_Handler (TeraSS_Service_Handler &&) = delete;
  TeraSS_Service_Handler &operator= (TeraSS_Service_Handler const &) = delete;
  TeraSS_Service_Handler &operator= (TeraSS_Service_Handler &&) = delete;

  int open (void *);

  int close (unsigned long);

private:
  TeraSS_Data data_;
};

/// Accepts connections for the small server.
class ACE_Svc_Export TeraSS_Acceptor
  : public ACE_Acceptor<TeraSS_Service_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  int init (int argc, ACE_TCHAR *argv[]);
};

ACE_SVC_FACTORY_DECLARE (TeraSS_Acceptor)

#endif /* TERA_SS_SERVICE_HANDLER_H */
