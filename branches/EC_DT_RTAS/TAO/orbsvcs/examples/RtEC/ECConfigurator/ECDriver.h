// $Id$

//=============================================================================
/**
 *  @file    ECDriver.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall <thrall@cse.wustl.edu>
 */
//=============================================================================


#ifndef ECDRIVER_H
#define ECDRIVER_H

#include "RtSchedEventChannelC.h"
#include "SyntaxTree.h"
#include "Kokyu_EC.h" // Reactor_Task

#include "ace/Reactor.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

class Kokyu_EC; //forward decl

class ECDriver
{
public:
  typedef RtEventChannelAdmin::Time Time;

  ECDriver (void);

  virtual ~ECDriver (void);

  virtual int init(CORBA::ORB_var orb, PortableServer::POA_var poa);

  virtual void wait_for_start(Kokyu_EC* ec)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;

  void run(int argc = 0, char *argv[] = 0);

  virtual bool get_time_master (void) const = 0;
  virtual void set_time_master (bool tm) = 0;

  virtual void set_start_condition(StartCondition::TYPE type, Time starttime)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual void set_stop_condition(StopCondition::TYPE type, long limit)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;

  /**
   * The Configurator_SyntaxHandler needs a reactor to schedule
   * Gateway initialization timeouts; the ECDriver must provide it
   * and run it.
   */
  virtual ACE_Reactor* reactor(void) = 0;
protected:
  virtual void run_i(int argc, char *argv[]) = 0;

  CORBA::ORB_var orb;
  PortableServer::POA_var poa;
  PortableServer::POAManager_var poa_manager;
};

class ECTestDriver : public ECDriver
{
public:
  ECTestDriver (void);

  virtual ~ECTestDriver (void);

  virtual int init(CORBA::ORB_var orb, PortableServer::POA_var poa);

  virtual bool get_time_master (void) const;
  virtual void set_time_master (bool tm);

  virtual void set_start_condition(StartCondition::TYPE type, Time time)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual void set_stop_condition(StopCondition::TYPE type, long limit)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual void wait_for_start(Kokyu_EC* ec)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual ACE_Reactor* reactor(void);
protected:
  virtual void run_i(int argc, char *argv[]);

  bool time_master;

  Reactor_Task rt;

  StartCondition::TYPE starttype;
  Time starttime;
  StopCondition::TYPE stoptype;
  long stoplimit;
};

#endif // ECDRIVER_H
