// $Id$

//=============================================================================
/**
 *  @file    SyntaxVisitor.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall <thrall@cse.wustl.edu>
 */
//=============================================================================


#ifndef SYNTAXVISITOR_H
#define SYNTAXVISITOR_H

#include "ACEXML/common/SAXExceptions.h"

// Forward declarations
class VisitableSyntax;
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

class SyntaxVisitor
{
public:
  virtual int parseVisitableSyntax (VisitableSyntax vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;

  virtual int parseECConfiguration (ECConfiguration* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseEvent (Event* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseCriticality (Criticality* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseImportance (Importance* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseTimeout (Timeout* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseLocalEventChannel (LocalEventChannel* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseRemoteEventChannel (RemoteEventChannel* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseSchedulingStrategy (SchedulingStrategy* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseConsumer (Consumer* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseSubscriptions (Subscriptions* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseDependants (Dependants* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseSupplier (Supplier* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parsePublications (Publications* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseTriggers (Triggers* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseTestDriver (TestDriver* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseStartCondition (StartCondition* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseStopCondition (StopCondition* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;

  virtual int parseEventName (EventName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseSupplierName (SupplierName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseTimeoutName (TimeoutName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseIORFile (IORFile* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseWorstExecution (WorstExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseTypicalExecution (TypicalExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parsePeriod (Period* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parsePhase (Phase* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseTime (Time* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;
  virtual int parseValue (Value* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException)) = 0;

  virtual VisitableSyntax* getRootNode (void) = 0;

};

#endif //SYNTAXVISITOR_H
