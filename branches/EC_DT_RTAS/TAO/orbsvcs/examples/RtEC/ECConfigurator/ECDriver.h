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

  virtual bool get_time_master (void) const;
  virtual void set_time_master (bool tm);

  virtual void set_start_condition(StartCondition::TYPE type, Time time)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual void set_stop_condition(StopCondition::TYPE type, long limit)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual void wait_for_start(Kokyu_EC* ec)
    ACE_THROW_SPEC ((ACEXML_SAXException));

protected:
  virtual void run_i(int argc, char *argv[]);

  bool time_master;

  StartCondition::TYPE starttype;
  Time starttime;
  StopCondition::TYPE stoptype;
  long stoplimit;
};

#endif // ECDRIVER_H
