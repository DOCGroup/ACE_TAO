// -*- C++ -*-
// $Id$

#ifndef FORWARDINGAGENT_THREAD_H
#define FORWARDINGAGENT_THREAD_H

#include /**/ "ace/pre.h"

#include "ace/Task.h"
#include "ace/Barrier.h"

#include "tao/ORB_Core.h"

#include "lwft_client_export.h"

class ForwardingAgent_i;

class LWFT_Client_Export ForwardingAgent_Thread
  : public ACE_Task_Base
{
public:
  ForwardingAgent_Thread (CORBA::ORB_ptr orb);

  virtual int svc (void);

private:
  CORBA::ORB_ptr orb_;

  ForwardingAgent_i *agent_;

  ACE_Barrier synchronizer_;
};

#include /**/ "ace/post.h"

#endif /* FORWARDINGAGENT_THREAD_H */
