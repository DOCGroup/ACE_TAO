// $Id$

//=============================================================================
/**
 *  @file    SyntaxTree.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall <thrall@cse.wustl.edu>
 */
//=============================================================================


#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include "SyntaxVisitor.h"
#include "RtSchedEventChannelC.h" // Event_Type,SourceId

#include "RtecSchedulerC.h"
#include "ace/OS_String.h"
#include "ace/Hash_Map_Manager.h"

#include <vector>

// Forward decls
class VisitableSyntax;
class StringSyntax;
class IntegerSyntax;
class Driver;
class ECConfiguration;
class Event;
class Criticality;
class Importance;
class Timeout;
class LocalEventChannel;
class RemoteEventChannel;
class SchedulingStrategy;
class Consumer;
class Subscriptions;
class Dependants;
class Supplier;
class Publications;
class Triggers;
class TestDriver;
class StartCondition;
class StopCondition;
class EventName;
class SupplierName;
class TimeoutName;
class IORFile;
class WorstExecution;
class TypicalExecution;
class Period;
class Phase;
class Time;
class Value;

// Typedefs
typedef std::vector<Event*>              EventVector;
typedef std::vector<Timeout*>            TimeoutVector;
typedef std::vector<LocalEventChannel*>  LocalECVector;
typedef std::vector<RemoteEventChannel*> RemoteECVector;
typedef std::vector<Consumer*>           ConsumerVector;
typedef std::vector<Supplier*>           SupplierVector;
typedef std::vector<EventName*>          EventNameVector;
typedef std::vector<SupplierName*>       SupplierNameVector;
typedef std::vector<TimeoutName*>        TimeoutNameVector;

typedef ACE_Hash_Map_Manager<ACE_CString,VisitableSyntax*,ACE_Null_Mutex> NameTable;


// Utility classes

class VisitableSyntax
{
public:

  //Tag for the element type
  enum element {
    ECCONFIGURATION = 0,
    EVENT,
    CRITICALITY,
    IMPORTANCE,
    WORSTEXECUTION,
    TYPICALEXECUTION,
    TIMEOUT,
    PERIOD,
    PHASE,
    LOCALEVENTCHANNEL,
    REMOTEEVENTCHANNEL,
    SCHEDULINGSTRATEGY,
    CONSUMER,
    SUBSCRIPTIONS,
    EVENTNAME,
    DEPENDANTS,
    SUPPLIERNAME,
    SUPPLIER,
    PUBLICATIONS,
    TRIGGERS,
    TIMEOUTNAME,
    IORFILE,
    TESTDRIVER,
    STARTCONDITION,
    TIME,
    STOPCONDITION,
    VALUE,
    UNKNOWN_ELEMENT
  };

  virtual ~VisitableSyntax (void)
  {}

  virtual int visit (SyntaxVisitor* v, void* arg) = 0;

  virtual element getSyntaxType (void)
  {
    return UNKNOWN_ELEMENT;
  }
};

class StringSyntax: public VisitableSyntax
{
public:

  virtual ~StringSyntax (void)
  {}

  // Value
  ACE_CString str;
};

class IntegerSyntax: public VisitableSyntax
{
public:

  virtual ~IntegerSyntax (void)
  {}

  // Value
  long val;
};

class Driver: public VisitableSyntax
{
public:

  virtual ~Driver (void)
  {}

};

// Specific parse types

class ECConfiguration : public VisitableSyntax
{
public:

  ECConfiguration (void)
    :driver(NULL)
  {}

  virtual ~ECConfiguration (void)
  {}

  virtual element getSyntaxType (void)
  {
    return ECCONFIGURATION;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseECConfiguration (this,arg);
  }

  // Children
  EventVector     events;
  TimeoutVector   timeouts;
  LocalECVector   localECs;
  RemoteECVector  remoteECs;
  Driver          *driver;
};

class Event : public VisitableSyntax
{
public:

  Event (void)
    : period(NULL)
    , phase(NULL)
    , criticality(NULL)
    , importance(NULL)
    , worstexecution(NULL)
    , typicalexecution(NULL)
  {}

  virtual ~Event (void)
  {}

  virtual element getSyntaxType (void)
  {
    return EVENT;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseEvent (this,arg);
  }

  // Children
  Period           *period;
  Phase            *phase;
  Criticality      *criticality;
  Importance       *importance;
  WorstExecution   *worstexecution;
  TypicalExecution *typicalexecution;

  // Attributes
  ACE_CString name;
  RtEventChannelAdmin::Event_Type type;
};

class Criticality : public VisitableSyntax
{
public:

  virtual ~Criticality (void)
  {}

  virtual element getSyntaxType (void)
  {
    return CRITICALITY;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseCriticality (this,arg);
  }

  // Attributes
  RtecScheduler::Criticality_t value;
};

class Importance : public VisitableSyntax
{
public:

  virtual ~Importance (void)
  {}

  virtual element getSyntaxType (void)
  {
    return IMPORTANCE;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseImportance (this,arg);
  }

  // Attributes
  RtecScheduler::Importance_t value;
};

class Timeout : public VisitableSyntax
{
public:

  Timeout (void)
    : period(NULL)
    , phase(NULL)
  {}

  virtual ~Timeout (void)
  {}

  virtual element getSyntaxType (void)
  {
    return TIMEOUT;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseTimeout (this,arg);
  }

  // Children
  Period *period;
  Phase  *phase;

  // Attributes
  ACE_CString name;
};

class LocalEventChannel : public VisitableSyntax
{
public:

  LocalEventChannel (void)
    : schedulingstrategy(NULL)
  {}

  virtual ~LocalEventChannel (void)
  {}

  virtual element getSyntaxType (void)
  {
    return LOCALEVENTCHANNEL;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseLocalEventChannel (this,arg);
  }

  // Children
  SchedulingStrategy *schedulingstrategy;
  ConsumerVector consumers;
  SupplierVector suppliers;

  // Attributes
  ACE_CString name;
};

class RemoteEventChannel : public VisitableSyntax
{
public:

  RemoteEventChannel (void)
    : iorfile(NULL)
  {}

  virtual ~RemoteEventChannel (void)
  {}

  virtual element getSyntaxType (void)
  {
    return REMOTEEVENTCHANNEL;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseRemoteEventChannel (this,arg);
  }

  // Children
  IORFile                *iorfile;
  ConsumerVector consumers;
  SupplierVector suppliers;

  // Attributes
  ACE_CString name;
};

class SchedulingStrategy : public VisitableSyntax
{
public:

  virtual ~SchedulingStrategy (void)
  {}

  virtual element getSyntaxType (void)
  {
    return SCHEDULINGSTRATEGY;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseSchedulingStrategy (this,arg);
  }

  // Attributes
  enum { EDF,
         MUF,
         RMS } type;
  bool enableRG;;
};

class Consumer : public VisitableSyntax
{
public:

  Consumer (void)
    : subscriptions(NULL)
    , dependants(NULL)
  {}

  virtual ~Consumer (void)
  {}

  virtual element getSyntaxType (void)
  {
    return CONSUMER;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseConsumer (this,arg);
  }

  // Children
  Subscriptions *subscriptions;
  Dependants *dependants;

  // Attributes
  ACE_CString name;
};

class Subscriptions : public VisitableSyntax
{
public:

  virtual ~Subscriptions (void)
  {}

  virtual element getSyntaxType (void)
  {
    return SUBSCRIPTIONS;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseSubscriptions (this,arg);
  }

  // Children
  EventNameVector eventnames;
};

class Dependants : public VisitableSyntax
{
public:

  virtual ~Dependants (void)
  {}

  virtual element getSyntaxType (void)
  {
    return DEPENDANTS;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseDependants (this,arg);
  }

  // Children
  SupplierNameVector suppliernames;
};

class Supplier : public VisitableSyntax
{
public:

  Supplier (void)
    : publications(NULL)
    , triggers(NULL)
  {}

  virtual ~Supplier (void)
  {}

  virtual element getSyntaxType (void)
  {
    return SUPPLIER;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseSupplier (this,arg);
  }

  // Children
  Publications *publications;
  Triggers *triggers;

  // Attributes
  ACE_CString name;
  RtEventChannelAdmin::EventSourceID id;
};

class Publications : public VisitableSyntax
{
public:

  virtual ~Publications (void)
  {}

  virtual element getSyntaxType (void)
  {
    return PUBLICATIONS;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parsePublications (this,arg);
  }

  // Children
  EventNameVector eventnames;
};

class Triggers : public VisitableSyntax
{
public:

  virtual ~Triggers (void)
  {}

  virtual element getSyntaxType (void)
  {
    return TRIGGERS;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseTriggers (this,arg);
  }

  // Children
  TimeoutNameVector timeoutnames;
};

class TestDriver : public Driver
{
public:

  TestDriver (void)
    : startcondition(NULL)
    , stopcondition(NULL)
  {}

  virtual ~TestDriver (void)
  {}

  virtual element getSyntaxType (void)
  {
    return TESTDRIVER;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseTestDriver (this,arg);
  }

  // Children
  StartCondition *startcondition;
  StopCondition *stopcondition;
};

class StartCondition : public VisitableSyntax
{
public:

  StartCondition (void)
    : time(NULL)
  {}

  virtual ~StartCondition (void)
  {}

  virtual element getSyntaxType (void)
  {
    return STARTCONDITION;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseStartCondition (this,arg);
  }

  // Children
  Time *time;

  // Attributes
  typedef enum { GLOBALTIME,
                 DELAYAFTERSTART,
                 DELAYAFTERCONNECT } TYPE;
  TYPE type;
  bool master;
};

class StopCondition : public VisitableSyntax
{
public:

  StopCondition (void)
    : value(NULL)
  {}

  virtual ~StopCondition (void)
  {}

  virtual element getSyntaxType (void)
  {
    return STOPCONDITION;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseStopCondition (this,arg);
  }

  // Children
  Value *value;

  // Attributes
  typedef enum { DURATION,
                 EVENTNUMBER } TYPE;
  TYPE type;
};

// String value elements

class EventName : public StringSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return EVENTNAME;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseEventName (this,arg);
  }

};

class SupplierName : public StringSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return SUPPLIERNAME;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseSupplierName (this,arg);
  }

};

class TimeoutName : public StringSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return TIMEOUTNAME;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseTimeoutName (this,arg);
  }

};

class IORFile : public StringSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return IORFILE;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseIORFile (this,arg);
  }

};

// Integer value elements

class WorstExecution : public IntegerSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return WORSTEXECUTION;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseWorstExecution (this,arg);
  }

};

class TypicalExecution : public IntegerSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return TYPICALEXECUTION;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseTypicalExecution (this,arg);
  }

};

class Period : public IntegerSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return PERIOD;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parsePeriod (this,arg);
  }

};

class Phase : public IntegerSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return PHASE;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parsePhase (this,arg);
  }

};

class Time : public IntegerSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return TIME;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseTime (this,arg);
  }

};

class Value : public IntegerSyntax
{
public:

  virtual element getSyntaxType (void)
  {
    return VALUE;
  }

  virtual int visit(SyntaxVisitor* v, void* arg)
  {
    return v->parseValue (this,arg);
  }

};

const int MAXTYPESTRINGLEN = 32;

//Copies as much of the type name to the buffer as fits; buf will null-term.
void visitableTypeToString (VisitableSyntax::element type, char *buf, int buflen);

#endif //SYNTAXTREE_H
