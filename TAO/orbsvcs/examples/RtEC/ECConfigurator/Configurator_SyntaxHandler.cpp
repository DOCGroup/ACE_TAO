// -*- C++ -*-  $Id$

#include "Configurator_SyntaxHandler.h"
#include "SyntaxTree.h"

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_String.h"

#include "RtecSchedulerC.h"

#include <stdlib.h> //for atol
#include <sstream>  //for istringstream

Configurator_SyntaxHandler::Configurator_SyntaxHandler (void)
  : root(0)
{
}

Configurator_SyntaxHandler::~Configurator_SyntaxHandler (void)
{
}

// SyntaxVisitor FUNCTIONS //

int
Configurator_SyntaxHandler::parseVisitableSyntax (VisitableSyntax* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  ACE_CString error("Unknown syntax token: ");
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
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseEvent (Event* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseCriticality (Criticality* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseImportance (Importance* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseTimeout (Timeout* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseLocalEventChannel (LocalEventChannel* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseRemoteEventChannel (RemoteEventChannel* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

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
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseSubscriptions (Subscriptions* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseDependants (Dependants* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseSupplier (Supplier* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parsePublications (Publications* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseTriggers (Triggers* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseTestDriver (TestDriver* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseStartCondition (StartCondition* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseStopCondition (StopCondition* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

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
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseTypicalExecution (TypicalExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parsePeriod (Period* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parsePhase (Phase* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseTime (Time* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  return 0;
}

int
Configurator_SyntaxHandler::parseValue (Value* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

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
