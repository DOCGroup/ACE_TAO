// -*- C++ -*-  $Id$

#include "Configurator_SyntaxHandler.h"
#include "SyntaxTree.h"
#include "RtSchedEventChannelC.h"
#include "Kokyu_EC.h"
#include "ECDriver.h"
#include "ECSupplier.h"

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_String.h"
#include "ace/Hash_Map_Manager.h"

#include "tao/ORB_Core.h"
#include "orbsvcs/Time_Utilities.h" // ORBSVCS_Time

#include "RtecSchedulerC.h"

#include <stdlib.h> //for atol
#include <sstream>  //for istringstream
#include <pair.h>
#include <algorithm> //for_each, count_if

struct OutputInserter
{
  typedef Gateway_Initializer::FileNameVector FileNameVector;
  typedef Configurator_SyntaxHandler::ConnectionVector::value_type ECConnection;
  typedef RemoteEventChannel::CONNECTION ConnType;

  OutputInserter(FileNameVector& fnv) : fnvec(fnv) {}

  void operator() (ECConnection& conn)
  {
    ConnType t = conn.first;
    if (t == RemoteEventChannel::OUTPUT
        || t == RemoteEventChannel::TWOWAY)
      {
        fnvec.push_back(strdup(conn.second));
      }
  }

private:
  FileNameVector& fnvec;
};

struct InputCounter
{
  typedef Configurator_SyntaxHandler::ConnectionVector::value_type Value;
  bool operator() (Value& v) { return v.first == RemoteEventChannel::INPUT
                                 || v.first == RemoteEventChannel::TWOWAY; }
};

Configurator_SyntaxHandler::Configurator_SyntaxHandler (void)
  : root(0)
  , driver(0)
{
  this->nametable.open();
  this->eventqostable.open();
  this->timeoutqostable.open();
  this->suppliermap.open();
}

int
Configurator_SyntaxHandler::init (CORBA::ORB_var orb, PortableServer::POA_var poa)
{
  this->orb = orb;
  this->poa = poa;

  return 0;
}

Configurator_SyntaxHandler::~Configurator_SyntaxHandler (void)
{
  this->nametable.close();
  this->eventqostable.close();
  this->timeoutqostable.close();
  this->suppliermap.close();
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
  EventTypeVector* ev;
  ACE_NEW_RETURN(ev,
                 EventTypeVector,-1);
  EventVector::iterator eiter = vs->events.begin();
  for (; eiter != vs->events.end(); eiter++)
    {
      (*eiter)->visit(this,ev);
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
  ConnectionVector connections;
  RemoteECVector::iterator reciter = vs->remoteECs.begin();
  for (; reciter != vs->remoteECs.end(); reciter++)
    {
      (*reciter)->visit(this,&connections);
    }

  // Initialize gateways from local ECs to remote ECs
  KokyuECVector::iterator keciter = this->kokyuECs.begin();
  for(; keciter != this->kokyuECs.end(); ++keciter)
    {
      KokyuECVector::value_type kokyuEC = *keciter;
      kokyuEC->setEventTypes(ev); //now it owns ev

      // only want Gateway for Output and TwoWay remote ECs, but want
      // to wait for Input and TwoWay ECs to connect

      Gateway_Initializer::FileNameVector output_ior_files;
      // insert all Output and TwoWay EC IOR filenames into output_ior_files
      std::for_each(connections.begin(),connections.end(),
                    OutputInserter(output_ior_files));
      // count the number of Input and TwoWay ECs
      int numInputs = std::count_if(connections.begin(),connections.end(),
                                    InputCounter());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("num Output connections: %d, num Input connections: %d\n"),output_ior_files.size(),numInputs));

      Gateway_Initializer *ginit;
      ACE_NEW_RETURN(ginit,
                     Gateway_Initializer(),-1);
      // ginit takes ownership of strings copied by OutputInserter
      ginit->init(this->orb,this->poa,kokyuEC,this->driver,
                  kokyuEC->get_name(),output_ior_files,numInputs);
      ACE_Time_Value gateway_delay(5,000000);
      long timer_id = this->driver->reactor()->schedule_timer(ginit,0,gateway_delay);
      if (timer_id < 0)
        {
          ACE_CString error("Could not schedule Gateway_Initializer timer for Local EC ");
          error += kokyuEC->get_name();
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
      this->ginitv.push_back(ginit);
    }

  // delete name strings in connections
  ConnectionVector::iterator citer = connections.begin();
  for (; citer != connections.end(); ++citer)
    delete (*citer).second;

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
  EventTypeVector* ev = static_cast<EventTypeVector*> (arg);
  ACE_ASSERT(ev);

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

  // Set EventType
  schedinfo.type = vs->type;
  ev->push_back(vs->type);

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

  if (!vs->period)
    {
      ACE_CString error("Timeout has missing child: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  RtEventChannelAdmin::SchedInfo schedinfo;

  // Visit children
  vs->period->visit(this,&schedinfo);
  if (vs->phase)
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
    sched_type = "edf";
    break;
  case SchedulingStrategy::MUF:
    sched_type = "muf";
    break;
  case SchedulingStrategy::RMS:
    sched_type = "rms";
    break;
  }

  // Set up local EC
  RtEventChannelAdmin::RtSchedEventChannel_var localEC;

  Kokyu_EC *local_ec;
  ACE_NEW_RETURN(local_ec,
                 Kokyu_EC(),-1);
  local_ec->set_name(vs->name.c_str()); //copied by local_ec

  if (local_ec->init(this->driver->get_time_master(),
                     sched_type.c_str(), this->poa.in(),
                     this->orb->orb_core()->reactor()) == -1)
    {
      ACE_CString error("Unable to initialize EC: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  localEC = local_ec->_this(ACE_ENV_SINGLE_ARG_PARAMETER);

  CORBA::String_var ior = orb->object_to_string(localEC.in()
                                                ACE_ENV_ARG_PARAMETER);

  ACE_CString ior_output_filename(vs->name);
  ior_output_filename += ".ior";
  FILE *ior_output_file = ACE_OS::fopen (ior_output_filename.c_str(), "w");
  if (ior_output_file == 0)
    {
      ACE_CString error("Unable to open ");
      error += ior_output_filename;
      error += " for writing.";
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  ACE_OS::fprintf(ior_output_file, ior.in());
  ACE_OS::fclose(ior_output_file);

  this->kokyuECs.push_back(local_ec);

  // visit localEC children
  // suppliers go first because consumers might need them as dependants
  //suppliers
  SupplierVector::iterator siter = vs->suppliers.begin();
  for (; siter != vs->suppliers.end(); siter++)
    {
      (*siter)->visit(this,local_ec);
    }

  //consumers
  ConsumerVector::iterator citer = vs->consumers.begin();
  for (; citer != vs->consumers.end(); citer++)
    {
      (*citer)->visit(this,local_ec);
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
  ConnectionVector *connections =
    static_cast<ConnectionVector*> (arg);
  ACE_ASSERT(connections);
  connections->push_back(make_pair(vs->conn,iorfilename.rep())); //copies string
  /* TODO: for now, don't allow consumers and suppliers on remote EC
  // Set up remote EC
  RtEventChannelAdmin::RtSchedEventChannel_var remoteEC;

  Kokyu_EC::init_remote_ec(iorfilename.c_str(),this->orb.in(),
                           remoteEC.out());

  // Visit remoteEC Children
  // suppliers go first because consumers might need them as dependants
  //suppliers
  SupplierVector::iterator siter = vs->suppliers.begin();
  for (; siter != vs->suppliers.end(); siter++)
    {
      (*siter)->visit(this,remoteEC.in());
    }

  //consumers
  ConsumerVector::iterator citer = vs->consumers.begin();
  for (; citer != vs->consumers.end(); citer++)
    {
      (*citer)->visit(this,remoteEC.in());
    }
  */
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
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Consumer subscriptions at %@\n"),vs->subscriptions));

  if (!vs->subscriptions)
    {
      ACE_CString error("Consumer has missing child: ");
      error += vs->name;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  // visit Consumer children
  Kokyu_EC::QoSVector subs;
  vs->subscriptions->visit(this,&subs);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Consumer dependants at %@\n"),vs->dependants));
  SupplierVector dependants; //dependants push_back
  if (vs->dependants)
    vs->dependants->visit(this,&dependants);

  // Register Consumer
  Kokyu_EC *ec = static_cast<Kokyu_EC*> (arg);

  // TODO: negotiate event types
  RtEventChannelAdmin::Time max_exec;
  ECSupplier::EventTypeVector subtypes;
  Kokyu_EC::QoSVector::iterator subiter = subs.begin();
  int count;
  for (count=0; subiter != subs.end(); ++subiter,++count)
    {
      Kokyu_EC::QoSVector::value_type qos = *subiter;

      if (max_exec < qos.wc_time || !count) // find maximum worst-case time
        max_exec = qos.wc_time;

      subtypes.push_back(qos.type);
    }

  // DEBUG
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Consumer subscription types: ")));
  ECSupplier::EventTypeVector::iterator typeiter;
  for (typeiter = subtypes.begin(); typeiter != subtypes.end(); ++typeiter)
    {
      ECSupplier::EventTypeVector::value_type type = *typeiter;

      ACE_DEBUG ((LM_DEBUG, "%d  ",type));
    }
  ACE_DEBUG((LM_DEBUG,"\n"));
  // END DEBUG

  ACE_Time_Value exec_time; // Consumer executes max worst-case time
  ORBSVCS_Time::TimeT_to_Time_Value (exec_time,max_exec);
  ECConsumer *consumer;
  ACE_NEW_RETURN(consumer,
                 ECConsumer(subtypes),-1);
  consumer->setWorkTime(exec_time);

  // for each dependant, set up the dependency
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Consumer setting up dependencies: %d"), dependants.size()));
  if (dependants.size())
    {
      SupplierVector::iterator siter = dependants.begin();
      for (; siter != dependants.end(); ++siter)
        {
          SupplierVector::value_type supplier = *siter;
          SupplierMap::VALUE ecsupplier;
          this->suppliermap.find(supplier->name,ecsupplier);
          ACE_ASSERT(ecsupplier); // shouldn't have got here with no Supplier

          // if multiple timeouts, this won't reg supplier multiple times
          consumer->pushDependant(ecsupplier);
          ec->add_consumer_with_supplier(consumer,
                                         vs->name.c_str(),
                                         subs,
                                         ecsupplier,
                                         supplier->name.c_str(),
                                         ecsupplier->getPublishedTypes()
                                         ACE_ENV_ARG_PARAMETER
                                         );
        }
    }
  else
    { // no dependants to register
      ec->add_consumer(consumer,
                       vs->name.c_str(),
                       subs
                       ACE_ENV_ARG_PARAMETER
                       );
    }
  this->consumermap.bind(vs->name,consumer);

  return 0;
}

int
Configurator_SyntaxHandler::parseSubscriptions (Subscriptions* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  Kokyu_EC::QoSVector *subs = static_cast<Kokyu_EC::QoSVector*> (arg);
  ACE_ASSERT(subs);

  // parse Subscriptions
  EventNameVector::iterator eniter = vs->eventnames.begin();
  for (; eniter != vs->eventnames.end(); eniter++)
    {
      Kokyu_EC::QoSVector::value_type subqos;
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

  if (!vs->publications)
    {
      ACEXML_THROW (ACEXML_SAXException ("Supplier has missing Publications child"));
    }

  // visit Supplier children
  Kokyu_EC::QoSVector pubs;
  vs->publications->visit(this,&pubs);

  Kokyu_EC::QoSVector trigs;
  if (vs->triggers)
    vs->triggers->visit(this,&trigs);

  // Register Supplier
  /*
  RtEventChannelAdmin::RtSchedEventChannel *ec =
    static_cast<RtEventChannelAdmin::RtSchedEventChannel*> (arg);
  */
  Kokyu_EC *ec = static_cast<Kokyu_EC*> (arg);

  ECSupplier::EventTypeVector pubtypes;
  Kokyu_EC::QoSVector::iterator pubiter = pubs.begin();
  for (; pubiter != pubs.end(); ++pubiter)
    {
      Kokyu_EC::QoSVector::value_type schedinfo = *pubiter;
      pubtypes.push_back(pubiter->type);
    }

  ECSupplier *supplier;
  ACE_NEW_RETURN(supplier,
                 ECSupplier(vs->id,pubtypes),-1);

  //for each trigger timeout, set up a timer
  Kokyu_EC::QoSVector::iterator qositer = trigs.begin();
  for (; qositer != trigs.end(); ++qositer)
    {
      Kokyu_EC::QoSVector::value_type qos = *qositer;
      // Timeout QoS only has period and phase
      ECSupplier_Timeout_Handler *timeout;
      ACE_NEW_RETURN(timeout,
                     ECSupplier_Timeout_Handler(supplier),-1);

      ACE_Time_Value phase;
      ORBSVCS_Time::TimeT_to_Time_Value (phase,qos.phase);
      ACE_Time_Value period;
      ORBSVCS_Time::TimeT_to_Time_Value (period,qos.period);

      // if multiple timeouts, this won't reg supplier multiple times
      ec->add_supplier_with_timeout(supplier,
                                    vs->name.c_str(),
                                    pubtypes,
                                    timeout,
                                    phase,
                                    period,
                                    RtecScheduler::LOW_CRITICALITY,
                                    RtecScheduler::LOW_IMPORTANCE
                                    ACE_ENV_ARG_PARAMETER
                                    );
    }
  this->suppliermap.bind(vs->name,supplier);

  return 0;
}

int
Configurator_SyntaxHandler::parsePublications (Publications* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  Kokyu_EC::QoSVector *pubs = static_cast<Kokyu_EC::QoSVector*> (arg);
  ACE_ASSERT(pubs);

  // TODO: negotiate event types
  // parse Publications
  EventNameVector::iterator eniter = vs->eventnames.begin();
  for (; eniter != vs->eventnames.end(); eniter++)
    {
      Kokyu_EC::QoSVector::value_type pubqos;
      ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Publication %s\n"),(*eniter)->str.c_str()));
      if (this->eventqostable.find((*eniter)->str,pubqos))
        {
          ACE_CString error("Unknown publication event: ");
          error += (*eniter)->str;
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
      // else pubqos retrieved
      pubs->push_back(pubqos);
    }

  return 0;
}

int
Configurator_SyntaxHandler::parseTriggers (Triggers* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  Kokyu_EC::QoSVector *trigs = static_cast<Kokyu_EC::QoSVector*> (arg);
  ACE_ASSERT(trigs);

  // parse Triggers
  TimeoutNameVector::iterator tniter = vs->timeoutnames.begin();
  for (; tniter != vs->timeoutnames.end(); tniter++)
    {
      Kokyu_EC::QoSVector::value_type trigqos;
      ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Trigger Timeout %s\n"),(*tniter)->str.c_str()));
      if (this->timeoutqostable.find((*tniter)->str,trigqos))
        {
          ACE_CString error("Unknown trigger timeout: ");
          error += (*tniter)->str;
          ACEXML_THROW (ACEXML_SAXException (error.c_str()));
        }
      // else trigqos retrieved
      trigs->push_back(trigqos);
    }

  return 0;
}

int
Configurator_SyntaxHandler::parseTestDriver (TestDriver* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting TestDriver: %@\n"),vs));

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

  // Create TestDriver
  if (this->driver)
    {
      ACE_CString error("More than one TestDriver");
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }
  ACE_NEW_RETURN (this->driver,
                  ECTestDriver(),-1);
  this->driver->init(this->orb,this->poa);
  this->driver->set_time_master(vs->startcondition->master);
  this->driver->set_start_condition(vs->startcondition->type,starttime);
  this->driver->set_stop_condition(vs->stopcondition->type,limit);

  return 0;
}

int
Configurator_SyntaxHandler::parseStartCondition (StartCondition* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting StartCondition: %@\n"),vs));

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
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting StopCondition: %@\n"),vs));

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

  // convert ms to 100s of ns
  schedinfo->wc_time = vs->val*1000;

  return 0;
}

int
Configurator_SyntaxHandler::parseTypicalExecution (TypicalExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting TypicalExecution\n")));

  // convert ms to 100s of ns
  schedinfo->typical_time = vs->val*10000;

  return 0;
}

int
Configurator_SyntaxHandler::parsePeriod (Period* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::SchedInfo *schedinfo = static_cast<RtEventChannelAdmin::SchedInfo*> (arg);
  ACE_ASSERT(schedinfo);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Period with arg %@ as SchedInfo %@\n"),arg,schedinfo));

  // convert ms to 100s of ns
  schedinfo->period = vs->val*10000;

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

  // convert ms to 100s of ns
  schedinfo->phase = vs->val*10000;

  return 0;
}

int
Configurator_SyntaxHandler::parseTime (Time* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  RtEventChannelAdmin::Time *time = static_cast<RtEventChannelAdmin::Time*> (arg);
  ACE_ASSERT(time);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Time: %@\n"),vs));

  // convert ms to 100s of ns
  *time = vs->val*10000;

  return 0;
}

int
Configurator_SyntaxHandler::parseValue (Value* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  long *value = static_cast<long*> (arg);
  ACE_ASSERT(value);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("SyntaxHandler visiting Value: %@\n"),vs));

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

ECDriver *
Configurator_SyntaxHandler::getDriver (void)
{
  return this->driver;
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
