// -*- C++ -*-  $Id$

#include "Configurator_SyntaxHandler.h"
#include "SyntaxTree.h"
#include "RtSchedEventChannelC.h"
#include "Kokyu_EC.h"

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_String.h"
#include "ace/Hash_Map_Manager.h"

#include "RtecSchedulerC.h"

#include <stdlib.h> //for atol
#include <sstream>  //for istringstream

Configurator_SyntaxHandler::Configurator_SyntaxHandler (void)
  : root(0)
{
  this->nametable.open();
  this->eventqostable.open();
  this->timeoutqostable.open();
}
/*
int
init (CORBA::ORB_var orb, PortableServer::POA_var poa)
{
  this->orb = orb;
  this->poa = poa;

  ACE_NEW_RETURN(ginit,
                 Gateway_Initializer(),-1);

  return 0;
}
*/
Configurator_SyntaxHandler::~Configurator_SyntaxHandler (void)
{
  this->nametable.close();
  this->eventqostable.close();
  this->timeoutqostable.close();
}

// SyntaxVisitor FUNCTIONS //

int
Configurator_SyntaxHandler::parseVisitableSyntax (VisitableSyntax* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  ACE_CString error("Unknown VisitableSyntax: ");
  char errbuf[MAXTYPESTRINGLEN];
  visitableTypeToString(vs->getSyntaxType(),errbuf,MAXTYPESTRINGLEN);
  error += errbuf;
  ACEXML_THROW (ACEXML_SAXException (error.c_str()));
  return 0;
}

int
Configurator_SyntaxHandler::parseECConfiguration (ECConfiguration* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  // Visit Children
  //events
  EventVector::iterator eiter = vs->events.begin();
  for (; eiter != vs->events.end(); eiter++)
    {
      (*eiter)->visit(this,NULL);
    }

  //timeouts
  TimeoutVector::iterator titer = vs->timeouts.begin();
  for (; titer != vs->timeouts.end(); titer++)
    {
      (*titer)->visit(this,NULL);
    }

  //driver
  // It's important that this happens before the ECs
  // since they need stuff like whether or not we're
  // the master.
  vs->driver->visit(this,NULL);

  //localecs
  LocalECVector::iterator leciter = vs->localECs.begin();
  for (; leciter != vs->localECs.end(); leciter++)
    {
      (*leciter)->visit(this,NULL);
    }

  //remoteecs
  RemoteECVector::iterator reciter = vs->remoteECs.begin();
  for (; reciter != vs->remoteECs.end(); reciter++)
    {
      (*reciter)->visit(this,NULL);
    }

  // DEBUG -- print name table
  NameTable::iterator ntiter = this->nametable.begin();
  for (; ntiter != this->nametable.end(); ntiter++)
    {
      NameTable::ENTRY entry = *ntiter;

      char typebuf[MAXTYPESTRINGLEN];
      visitableTypeToString(entry.int_id_->getSyntaxType(),typebuf,MAXTYPESTRINGLEN);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("NameTable Entry: %s -> %s\n"),entry.ext_id_.c_str(),typebuf));
    }
  // DEBUG END

  return 0;
}

int
Configurator_SyntaxHandler::parseEvent (Event* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Event: %s\n"),vs->name.c_str()));

  if (!(vs->period
        && vs->phase
        && vs->criticality
        && vs->importance
        && vs->worstexecution
        && vs->typicalexecution))
    {
      ACE_CString error("Event has missing child: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  RtEventChannelAdmin::SchedInfo schedinfo;

  // Visit children
  vs->period->visit(this,&schedinfo);
  vs->phase->visit(this,&schedinfo);
  vs->criticality->visit(this,&schedinfo);
  vs->importance->visit(this,&schedinfo);
  vs->worstexecution->visit(this,&schedinfo);
  vs->typicalexecution->visit(this,&schedinfo);

  // Since nametable was OK, eventqostable should be too
  this->eventqostable.bind(vs->name,schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler DONE visiting Event: %s\n"),vs->name.c_str()));

  return 0;
}

int
Configurator_SyntaxHandler::parseCriticality (Criticality* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Criticality\n")));

  schedinfo->criticality = vs->value;

  return 0;
}

int
Configurator_SyntaxHandler::parseImportance (Importance* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Importance\n")));

  schedinfo->importance = vs->value;

  return 0;
}

int
Configurator_SyntaxHandler::parseTimeout (Timeout* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Timeout: %s\n"),vs->name.c_str()));

  if (!(vs->period
        && vs->phase))
    {
      ACE_CString error("Timeout has missing child: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  RtEventChannelAdmin::SchedInfo schedinfo;

  // Visit children
  vs->period->visit(this,&schedinfo);
  vs->phase->visit(this,&schedinfo);

  // Since nametable was OK, eventqostable should be too
  this->timeoutqostable.bind(vs->name,schedinfo);

  return 0;
}

int
Configurator_SyntaxHandler::parseLocalEventChannel (LocalEventChannel* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting LocalEventChannel: %s\n"),vs->name.c_str()));

  // Get info from SchedulingStrategy
  ACE_CString sched_type;
  switch (vs->schedulingstrategy->type) {
  case SchedulingStrategy::EDF:
    sched_type += "edf";
    break;
  case SchedulingStrategy::MUF:
    sched_type += "muf";
    break;
  case SchedulingStrategy::RMS:
    sched_type += "rms";
    break;
  }

  // TODO: set up local EC
  RtEventChannelAdmin::RtSchedEventChannel_var localEC;
  /*
  Kokyu_EC local_ec;
  if (local_ec.init(this->driver->time_master(),
                       sched_type.c_str(), this->poa.in(),
                       this->orb->orb_core()->reactor()) == -1)
    {
      ACE_CString error("Unable to initialize EC: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  localEC = local_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);

  CORBA::String_var ior = orb->object_to_string(local_ec.in()
                                                ACE_ENV_ARG_PARAMETER);

  ACE_CString ior_output_filename;
  FILE *ior_output_file = ACE_OS::fopen (ior_output_filename.c_str(), "w");
  if (ior_output_file == 0)
    {
      ACE_CString error("Unable to open ");
      error += vs->name;
      error += " for writing.";
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  ACE_OS::fprintf(ior_output_file, ior.in());
  ACE_OS::fclose(ior_output_file);

  this->localECs.push_back(localEC);
  */
  // visit localEC children
  //consumers
  ConsumerVector::iterator citer = vs->consumers.begin();
  for (; citer != vs->consumers.end(); citer++)
    {
      (*citer)->visit(this,localEC.in());
    }

  //suppliers
  SupplierVector::iterator siter = vs->suppliers.begin();
  for (; siter != vs->suppliers.end(); siter++)
    {
      (*siter)->visit(this,localEC.in());
    }

  return 0;
}

int
Configurator_SyntaxHandler::parseRemoteEventChannel (RemoteEventChannel* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting RemoteEventChannel: %s\n"),vs->name.c_str()));

  // get IOR filename
  ACE_CString iorfilename(vs->iorfile->str);

  // TODO: set up remote EC
  RtEventChannelAdmin::RtSchedEventChannel_var remoteEC;
  /*
  // TODO: for now, just do one gateway init
  this->ginitv[0]->init(this->orb,this->poa,localECs[0].in(),ior_output_filename,ior_input_files);
  ACE_Time_Value gateway_delay(5,000000);
  long timer_id = this->orb->orb_core()->reactor()->schedule_timer(this->ginitv[0],0,gateway_delay);
  if (timer_id < 0)
    {
      ACE_CString error("Could not schedule Gateway_Initializer timer for RemoteEC ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }
  */
  // visit remoteEC consumers and suppliers
  //consumers
  ConsumerVector::iterator citer = vs->consumers.begin();
  for (; citer != vs->consumers.end(); citer++)
    {
      (*citer)->visit(this,remoteEC.in());
    }

  //suppliers
  SupplierVector::iterator siter = vs->suppliers.begin();
  for (; siter != vs->suppliers.end(); siter++)
    {
      (*siter)->visit(this,remoteEC.in());
    }

  return 0;
}

int
Configurator_SyntaxHandler::parseSchedulingStrategy (SchedulingStrategy* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseConsumer (Consumer* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Consumer: %s\n"),vs->name.c_str()));

  if (!(vs->subscriptions
        && vs->dependants))
    {
      ACE_CString error("Consumer has missing child: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  // visit Consumer children
  QoSVector subs;
  vs->subscriptions->visit(this,&subs);

  SupplierVector dependants; //dependants push_back
  vs->dependants->visit(this,&dependants);

  // TODO: register Consumer
  RtEventChannelAdmin::RtSchedEventChannel *ec =
    static_cast<RtEventChannelAdmin::RtSchedEventChannel*> (arg);

  // TODO: map syntax dependants to actual suppliers somehow

  return 0;
}

int
Configurator_SyntaxHandler::parseSubscriptions (Subscriptions* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  QoSVector *subs = static_cast<QoSVector*> (arg);
  ACE_ASSERT(subs);

  // parse Subscriptions
  EventNameVector::iterator eniter = vs->eventnames.begin();
  for (; eniter != vs->eventnames.end(); eniter++)
    {
      QoSVector::value_type subqos;
      ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Subscription %s\n"),(*eniter)->str.c_str()));
      if (this->eventqostable.find((*eniter)->str,subqos))
        {
          ACE_CString error("Unknown subscription event: ");
          error += (*eniter)->str;
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
      subs->push_back(subqos);
    }

  return 0;
} //parseSubscriptions()

int
Configurator_SyntaxHandler::parseDependants (Dependants* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  SupplierVector *dependants = static_cast<SupplierVector*> (arg);
  ACE_ASSERT(dependants);

  SupplierNameVector::iterator sniter = vs->suppliernames.begin();
  for (; sniter != vs->suppliernames.end(); sniter++)
    {
      VisitableSyntax *syntax;
      if (this->nametable.find((*sniter)->str,syntax))
        {
          ACE_CString error("Unknown dependant: ");
          error += (*sniter)->str;
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
      Supplier *supplier = dynamic_cast<Supplier*> (syntax);
      ACE_ASSERT(supplier);
      dependants->push_back(supplier);
    }

  return 0;
}

int
Configurator_SyntaxHandler::parseSupplier (Supplier* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Supplier: %s\n"),vs->name.c_str()));

  if (!(vs->publications
        && vs->triggers))
    {
      ACE_CString error("Supplier has missing child: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  // visit Supplier children
  QoSVector pubs;
  vs->publications->visit(this,&pubs);

  QoSVector trigs;
  vs->triggers->visit(this,&trigs);

  // TODO: register Supplier
  RtEventChannelAdmin::RtSchedEventChannel *ec =
    static_cast<RtEventChannelAdmin::RtSchedEventChannel*> (arg);

  return 0;
}

int
Configurator_SyntaxHandler::parsePublications (Publications* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  QoSVector *pubs = static_cast<QoSVector*> (arg);
  ACE_ASSERT(pubs);

  // parse Publications
  EventNameVector::iterator eniter = vs->eventnames.begin();
  for (; eniter != vs->eventnames.end(); eniter++)
    {
      QoSVector::value_type pubqos;
      ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Publication %s\n"),(*eniter)->str.c_str()));
      // TODO: fix segfault in this vicinity
      if (this->eventqostable.find((*eniter)->str,pubqos))
        {
          ACE_CString error("Unknown publication event: ");
          error += (*eniter)->str;
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
    }

  return 0;
}

int
Configurator_SyntaxHandler::parseTriggers (Triggers* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  QoSVector *trigs = static_cast<QoSVector*> (arg);
  ACE_ASSERT(trigs);

  // parse Triggers
  EventNameVector::iterator eniter = vs->eventnames.begin();
  for (; eniter != vs->eventnames.end(); eniter++)
    {
      QoSVector::value_type trigqos;
      ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Trigger Event %s\n"),(*eniter)->str.c_str()));
      if (this->eventqostable.find((*eniter)->str,trigqos))
        {
          ACE_CString error("Unknown trigger event: ");
          error += (*eniter)->str;
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
      trigs->push_back(trigqos);
    }
  TimeoutNameVector::iterator tniter = vs->timeoutnames.begin();
  for (; tniter != vs->timeoutnames.end(); tniter++)
    {
      QoSVector::value_type trigqos;
      ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Trigger Timeout %s\n"),(*eniter)->str.c_str()));
      if (this->timeoutqostable.find((*eniter)->str,trigqos))
        {
          ACE_CString error("Unknown trigger timeout: ");
          error += (*eniter)->str;
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
      trigs->push_back(trigqos);
    }

  return 0;
}

int
Configurator_SyntaxHandler::parseTestDriver (TestDriver* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (!(vs->startcondition
        && vs->stopcondition))
    {
      ACE_CString error("TestDriver has missing child: ");
      error += vs->startcondition ? "StopCondition" : "StartCondition";
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  // Visit Children
  RtEventChannelAdmin::Time starttime;
  vs->startcondition->visit(this,&starttime);

  long limit;
  vs->stopcondition->visit(this,&limit);

  // TODO: Create TestDriver
  this->testdriver = NULL;

  return 0;
}

int
Configurator_SyntaxHandler::parseStartCondition (StartCondition* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (!vs->time)
    {
      ACE_CString error("StartCondition has missing child: Time");
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  // Visit Children
  vs->time->visit(this,arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseStopCondition (StopCondition* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (!vs->value)
    {
      ACE_CString error("StopCondition has missing child: Value");
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  // Visit Children
  vs->value->visit(this,arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseEventName (EventName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseSupplierName (SupplierName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseTimeoutName (TimeoutName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseIORFile (IORFile* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseWorstExecution (WorstExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting WorstExecution\n")));

  // TODO: convert ms to 100s of ns
  schedinfo->wc_time = vs->val;

  return 0;
}

int
Configurator_SyntaxHandler::parseTypicalExecution (TypicalExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting TypicalExecution\n")));

  // TODO: convert ms to 100s of ns
  schedinfo->typical_time = vs->val;

  return 0;
}

int
Configurator_SyntaxHandler::parsePeriod (Period* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Period with arg %@ as SchedInfo %@\n"),arg,schedinfo));

  // TODO: convert ms to 100s of ns
  schedinfo->period = vs->val;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler DONE visiting Period\n")));

  return 0;
}

int
Configurator_SyntaxHandler::parsePhase (Phase* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Phase\n")));

  // TODO: convert ms to 100s of ns
  schedinfo->phase = vs->val;

  return 0;
}

int
Configurator_SyntaxHandler::parseTime (Time* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::Time *time = static_cast<RtEventChannelAdmin::Time*> (arg);
  ACE_ASSERT(time);

  // TODO: convert ms to 100s of ns
  *time = vs->val;

  return 0;
}

int
Configurator_SyntaxHandler::parseValue (Value* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  long *value = static_cast<long*> (arg);
  ACE_ASSERT(value);

  *value = vs->val;

  return 0;
}

VisitableSyntax*
Configurator_SyntaxHandler::getRootNode (void)
{
  return this->root;
}

void
Configurator_SyntaxHandler::setRootNode (VisitableSyntax *vs)
{
  this->root = vs;
}

void
Configurator_SyntaxHandler::parse (void)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  this->root->visit(this,NULL);
}

ECTestDriver *
Configurator_SyntaxHandler::getTestDriver (void)
{
  return this->testdriver;
}

void
Configurator_SyntaxHandler::setNameTable(NameTable &nt)
{
  this->nametable.unbind_all();

  NameTable::ITERATOR ntiter = nt.begin();
  for (; ntiter != nt.end(); ++ntiter)
    {
      NameTable::ENTRY ntentry = *ntiter;
      this->nametable.bind(ntentry.ext_id_,ntentry.int_id_);
    }
}
