// $Id$

#include "ECDriver.h"
#include "Kokyu_EC.h"

#include "ace/Thread.h"
#include "orbsvcs/Time_Utilities.h"

ECDriver::ECDriver (void)
{
}

ECDriver::~ECDriver (void)
{
}

int
ECDriver::init(CORBA::ORB_var orb, PortableServer::POA_var poa)
{
  this->orb = orb;
  this->poa = poa;

  return 0;
}

void
ECDriver::run (int argc, char *argv[])
{
  this->run_i(argc,argv);
}

ECTestDriver::ECTestDriver (void)
{
}

ECTestDriver::~ECTestDriver (void)
{
}

int
ECTestDriver::init(CORBA::ORB_var orb, PortableServer::POA_var poa)
{
  ECDriver::init(orb,poa);

  this->rt.initialize();

  return 0;
}

bool
ECTestDriver::get_time_master (void) const
{
  return this->time_master;
}

void
ECTestDriver::set_time_master (bool tm)
{
  this->time_master = tm;
}

void
ECTestDriver::set_start_condition(StartCondition::TYPE type, Time time)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (type != StartCondition::DELAYAFTERCONNECT)
    {
      ACEXML_THROW (ACEXML_SAXException ("ECTestDriver does not support"
                                         " start condition types other than"
                                         "DELAYAFTERCONNECT"));
    }

  this->starttype = type;
  this->starttime = time;
}

void
ECTestDriver::set_stop_condition(StopCondition::TYPE type, long limit)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (type == StopCondition::EVENTNUMBER)
    {
      ACEXML_THROW (ACEXML_SAXException ("ECTestDriver does not support"
                                         " stop condition type EVENTNUMBER"));
    }

  this->stoptype = type;
  this->stoplimit = limit;
}

void
ECTestDriver::wait_for_start(Kokyu_EC* ec)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->time_master)
    {
      //set start time for all other ECs
      RtEventChannelAdmin::Time st;
      ACE_Time_Value start_time(30,0);
      start_time += ACE_OS::gettimeofday(); //now + 30 sec
      ORBSVCS_Time::Time_Value_to_TimeT(st,start_time);
      ec->set_start_time(st); //set start time for this EC
      Kokyu_EC::ECVector* remote_ecs(ec->remote_ecs());
      for(size_t i=0; i< remote_ecs->size(); ++i) //set start time for remote ECs
        {
          (*remote_ecs)[i]->set_start_time(st);
          ACE_TRY_CHECK;
        }
    }

  //now wait for start time
  ACE_Time_Value sleeptime(0,5); //we'll be within 5usec of start time when we call ec_->start()
  ACE_Time_Value now(ACE_OS::gettimeofday());
  ACE_OS::printf("Gateway_Initializer: waiting for start time to be set or to pass; now is %ldsec %ldusec, start time is %ldsec %ldusec\n",now.sec(),now.usec(),ec->start_time().sec(),ec->start_time().usec());
  while (ec->start_time() == ACE_Time_Value::zero
         || now < ec->start_time())
    {
      //while not time to start
      //ACE_DEBUG((LM_DEBUG,"Gateway_Initializer (%P|%t): waiting for start time to be set or to pass; now is %isec %iusec, start time is %isec %iusec\n",now.sec(),now.usec(),ec->start_time().sec(),ec->start_time().usec()));
      ACE_OS::sleep(sleeptime);
      now = ACE_OS::gettimeofday();
    }
}

ACE_Reactor*
ECTestDriver::reactor(void)
{
  return this->rt.reactor();
} //reactor()

void
ECTestDriver::run_i (int, char **)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
  ACE_OS::thr_setprio (thr_handle, prio);

  // for DURATION, limit is in msecs
  ACE_Time_Value stop_time(this->stoplimit/1000,this->stoplimit%1000);

  //spawn thread to run the reactor event loop
  this->rt.activate(); //need thread creation flags? or priority?
  orb->run (stop_time ACE_ENV_ARG_PARAMETER);
} //run_i()
