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

class Service_Handler
{
public:
  virtual void handle_service_start(const RtecEventComm::EventSet& events
                                    ACE_ENV_ARG_DECL) = 0;

  virtual void handle_service_stop(const RtecEventComm::EventSet& events
                                    ACE_ENV_ARG_DECL) = 0;
};

#endif //SVC_HANDLER_H
