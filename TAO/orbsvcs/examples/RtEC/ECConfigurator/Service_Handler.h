/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Service_Handler
//
// = AUTHOR
//   Bryan Thrall
//
// ============================================================================

#ifndef SVC_HANDLER_H
#define SVC_HANDLER_H

#include "ace/CORBA_macros.h"

class Service_Handler
{
public:
  virtual void handle_service_start(ACE_ENV_SINGLE_ARG_DECL) = 0;

  virtual void handle_service_stop(ACE_ENV_SINGLE_ARG_DECL) = 0;
};

#endif //SVC_HANDLER_H
