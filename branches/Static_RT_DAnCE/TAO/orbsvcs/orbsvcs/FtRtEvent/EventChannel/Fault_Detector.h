// -*- C++ -*-

//=============================================================================
/**
 *  @file   Fault_Detector.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef FAULT_DETECTOR_H
#define FAULT_DETECTOR_H

#include "ace/Task.h"
#include "ace/Reactor.h"

#include "orbsvcs/FTRT_GroupManagerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_Fault_Listener;

class Fault_Detector
{
public:
  virtual ~Fault_Detector();

  static Fault_Detector* instance();

  int init(int argc, ACE_TCHAR** argv);
  void stop();
  void set_listener(TAO_FTEC_Fault_Listener*);
  const FTRT::Location& my_location() const;
  virtual int connect(const FTRT::Location& location)=0;

private:
  virtual int init_acceptor()=0;
  virtual int parse_conf(int argc, ACE_TCHAR** argv);

protected:

  class ReactorTask : public ACE_Task_Base
  {
  public:
    // ctor
    ReactorTask();
    virtual int svc (void);
    ACE_Reactor reactor_;
  };

  FTRT::Location location_;
  ReactorTask reactor_task_;
  TAO_FTEC_Fault_Listener* listener_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif //FAULT_DETECTOR_H
