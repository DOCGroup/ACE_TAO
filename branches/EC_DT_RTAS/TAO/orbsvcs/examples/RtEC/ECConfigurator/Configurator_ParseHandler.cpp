// -*- C++ -*-  $Id$

#include "Configurator_ParseHandler.h"
#include "SyntaxTree.h"

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_String.h"

#include "RtecSchedulerC.h"

#include <stdlib.h> //for atol
#include <sstream>  //for istringstream

Configurator_ParseHandler::Configurator_ParseHandler (const char *filename)
  : fileName_(filename)
  , ecconfiguration(0)
{
}

Configurator_ParseHandler::~Configurator_ParseHandler (void)
{
}

void
Configurator_ParseHandler::characters (const ACEXML_Char * cdata,
                                       int ,
                                       int
                                       ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  //ACE_UNUSED_ARG(cdata);

  if (!this->scope_.empty())
    {
      std::istringstream iss(cdata);

      switch (this->scope_.top()->getSyntaxType())
        {
          IntegerSyntax *intsyn;
          StringSyntax *strsyn;

        case VisitableSyntax::WORSTEXECUTION:
        case VisitableSyntax::TYPICALEXECUTION:
        case VisitableSyntax::PERIOD:
        case VisitableSyntax::PHASE:
        case VisitableSyntax::TIME:
        case VisitableSyntax::VALUE:
          intsyn = dynamic_cast<IntegerSyntax*>(this->scope_.top());
          if ((iss >> intsyn->val) == 0)
            {
              ACEXML_THROW(ACEXML_SAXException(ACE_TEXT ("Invalid number format")));
            }
          break;
        case VisitableSyntax::EVENTNAME:
        case VisitableSyntax::SUPPLIERNAME:
        case VisitableSyntax::TIMEOUTNAME:
        case VisitableSyntax::IORFILE:
          strsyn = dynamic_cast<StringSyntax*>(this->scope_.top());
          strsyn->str.set(iss.str().c_str());
          break;
        case VisitableSyntax::ECCONFIGURATION:
        case VisitableSyntax::EVENT:
        case VisitableSyntax::CRITICALITY:
        case VisitableSyntax::IMPORTANCE:
        case VisitableSyntax::TIMEOUT:
        case VisitableSyntax::LOCALEVENTCHANNEL:
        case VisitableSyntax::REMOTEEVENTCHANNEL:
        case VisitableSyntax::SCHEDULINGSTRATEGY:
        case VisitableSyntax::CONSUMER:
        case VisitableSyntax::SUBSCRIPTIONS:
        case VisitableSyntax::DEPENDANTS:
        case VisitableSyntax::SUPPLIER:
        case VisitableSyntax::PUBLICATIONS:
        case VisitableSyntax::TRIGGERS:
        case VisitableSyntax::TESTDRIVER:
        case VisitableSyntax::STARTCONDITION:
        case VisitableSyntax::STOPCONDITION:
        case VisitableSyntax::UNKNOWN_ELEMENT:
        default:
          //ignore characters outside leaf elements
          break;
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Character data outside of any scope")));
    }
}

void
Configurator_ParseHandler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  /*
  // Print out test_config_t's:
  const TestConfig::Test_Config_Set &cfgs = *this->configs_;
  char *cfg_format = "{%10d, %10d, %10d, %10d, %10d }";

  for (size_t i=0; i<cfgs.size(); ++i) {
    TestConfig::test_config_t *cfg = cfgs[i];

    if (i!=0)
      {
        //finish previous line
        ACE_DEBUG ((LM_DEBUG, "\n"));
      }
    if (cfg->comptype == TestConfig::SOURCE)
      {
        ACE_DEBUG ((LM_DEBUG, "SOURCE : "));
      }
    else if (cfg->comptype == TestConfig::SINK)
      {
        ACE_DEBUG ((LM_DEBUG, "SINK   : "));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "UNKNOWN: "));
      }
    ACE_DEBUG ((LM_DEBUG, cfg_format,
                cfg->type,
                cfg->period,
                cfg->criticality,
                cfg->importance,
                cfg->num_entities));
  }
  //finish last line
  ACE_DEBUG ((LM_DEBUG, "\n"));
  */
}

void
Configurator_ParseHandler::endElement (const ACEXML_Char *namespaceURI,
                          const ACEXML_Char *localName,
                          const ACEXML_Char *qName
                          ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  /*
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endElement (%s, %s, %s) ***************\n"),
              namespaceURI, localName, qName));
  */

  ACE_UNUSED_ARG(namespaceURI);
  ACE_UNUSED_ARG(localName);
  ACE_UNUSED_ARG(qName);

  if (this->scope_.empty() == 1)
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Element end outside of any scope")));
    }
  else
    {
      this->scope_.pop();
    }

}

void
Configurator_ParseHandler::endPrefixMapping (const ACEXML_Char *
                                ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Configurator_ParseHandler::ignorableWhitespace (const ACEXML_Char *,
                                   int,
                                   int
                                   ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Configurator_ParseHandler::processingInstruction (const ACEXML_Char *,
                                             const ACEXML_Char *
                                             ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Configurator_ParseHandler::setDocumentLocator (ACEXML_Locator * locator)
{
  this->locator_ = locator;
}

void
Configurator_ParseHandler::skippedEntity (const ACEXML_Char *
                                     ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Configurator_ParseHandler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Configurator_ParseHandler::startElement (const ACEXML_Char *uri,
                                    const ACEXML_Char *name,
                                    const ACEXML_Char *qName,
                                    ACEXML_Attributes *alist
                                    ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startElement (%s, %s, %s) ***************\n"),
              uri, name, qName));
  /*
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("    %s = \"%s\"\n"),
                    alist->getQName (i), alist->getValue (i)));
      }
  */

  ACE_UNUSED_ARG(uri);
  ACE_UNUSED_ARG(name);

  VisitableSyntax *syn = 0;

  if (ACE_OS_String::strcmp (qName, ACE_TEXT ("ECConfiguration")) == 0)
    {
      ACE_NEW(syn,ECConfiguration);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Event")) == 0)
    {
      ACE_NEW(syn,Event);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Criticality")) == 0)
    {
      ACE_NEW(syn,Criticality);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Importance")) == 0)
    {
      ACE_NEW(syn,Importance);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("WorstExecution")) == 0)
    {
      ACE_NEW(syn,WorstExecution);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("TypicalExecution")) == 0)
    {
      ACE_NEW(syn,TypicalExecution);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Timeout")) == 0)
    {
      ACE_NEW(syn,Timeout);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Period")) == 0)
    {
      ACE_NEW(syn,Period);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Phase")) == 0)
    {
      ACE_NEW(syn,Phase);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("LocalEventChannel")) == 0)
    {
      ACE_NEW(syn,LocalEventChannel);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("RemoteEventChannel")) == 0)
    {
      ACE_NEW(syn,RemoteEventChannel);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("SchedulingStrategy")) == 0)
    {
      ACE_NEW(syn,SchedulingStrategy);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Consumer")) == 0)
    {
      ACE_NEW(syn,Consumer);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Subscriptions")) == 0)
    {
      ACE_NEW(syn,Subscriptions);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("EventName")) == 0)
    {
      ACE_NEW(syn,EventName);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Dependants")) == 0)
    {
      ACE_NEW(syn,Dependants);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("SupplierName")) == 0)
    {
      ACE_NEW(syn,SupplierName);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Supplier")) == 0)
    {
      ACE_NEW(syn,Supplier);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Publications")) == 0)
    {
      ACE_NEW(syn,Publications);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Triggers")) == 0)
    {
      ACE_NEW(syn,Triggers);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("TimeoutName")) == 0)
    {
      ACE_NEW(syn,TimeoutName);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("IORFile")) == 0)
    {
      ACE_NEW(syn,IORFile);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("TestDriver")) == 0)
    {
      ACE_NEW(syn,TestDriver);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("StartCondition")) == 0)
    {
      ACE_NEW(syn,StartCondition);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Time")) == 0)
    {
      ACE_NEW(syn,Time);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("StopCondition")) == 0)
    {
      ACE_NEW(syn,StopCondition);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("Value")) == 0)
    {
      ACE_NEW(syn,Value);
    }
  else
    {
      ACE_CString error("Element start of unknown type: ");
      error += qName;
      ACEXML_THROW (ACEXML_SAXException (error.c_str()));
    }

  syn->visit(this,alist);
}

void
Configurator_ParseHandler::startPrefixMapping (const ACEXML_Char *,
                                          const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

// Methods inherited from ACEXML_ErrorHandler.

/*
 * Receive notification of a recoverable error.
 */
void
Configurator_ParseHandler::error (ACEXML_SAXParseException & ex ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{

  ACE_DEBUG ((LM_DEBUG, "%s:%d:%d ", this->fileName_.c_str(),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
Configurator_ParseHandler::fatalError (ACEXML_SAXParseException& ex ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{

  ACE_DEBUG ((LM_DEBUG, "%s:%d:%d ", this->fileName_.c_str(),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
Configurator_ParseHandler::warning (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// SyntaxVisitor FUNCTIONS //

int
Configurator_ParseHandler::parseVisitableSyntax (VisitableSyntax* vs, void* arg)
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
Configurator_ParseHandler::parseECConfiguration (ECConfiguration* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  this->ecconfiguration = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseEvent (Event* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::ECCONFIGURATION)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Event not child of ECConfiguration")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("name"));
  if (val != 0)
    {
      vs->name = val;
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for Event")));
    }

  ECConfiguration *parent = dynamic_cast<ECConfiguration*> (this->scope_.top());
  parent->events.push_back(vs);

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseCriticality (Criticality* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Criticality not child of Event")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("value"));
  if (val != 0)
    {
      if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_LOW")) == 0)
        {
          vs->value = RtecScheduler::VERY_LOW_CRITICALITY;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("LOW")) == 0)
        {
          vs->value = RtecScheduler::LOW_CRITICALITY;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("MEDIUM")) == 0)
        {
          vs->value = RtecScheduler::MEDIUM_CRITICALITY;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("HIGH")) == 0)
        {
          vs->value = RtecScheduler::HIGH_CRITICALITY;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_HIGH")) == 0)
        {
          vs->value = RtecScheduler::VERY_HIGH_CRITICALITY;
        }
      else
        {
          ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid value for \"value\""
                                                      " attribute for Criticality")));
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for Criticality")));
    }

  Event *parent = dynamic_cast<Event*> (this->scope_.top());
  parent->criticality = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseImportance (Importance* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Importance not child of Event")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("value"));
  if (val != 0)
    {
      if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_LOW")) == 0)
        {
          vs->value = RtecScheduler::VERY_LOW_IMPORTANCE;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("LOW")) == 0)
        {
          vs->value = RtecScheduler::LOW_IMPORTANCE;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("MEDIUM")) == 0)
        {
          vs->value = RtecScheduler::MEDIUM_IMPORTANCE;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("HIGH")) == 0)
        {
          vs->value = RtecScheduler::HIGH_IMPORTANCE;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_HIGH")) == 0)
        {
          vs->value = RtecScheduler::VERY_HIGH_IMPORTANCE;
        }
      else
        {
          ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid value for \"value\""
                                                      " attribute for Importance")));
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for Importance")));
    }

  Event *parent = dynamic_cast<Event*> (this->scope_.top());
  parent->importance = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseTimeout (Timeout* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::ECCONFIGURATION)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Timeout not a child of ECConfiguration")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("name"));
  if (val != 0)
    {
      vs->name = val;
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for Timeout")));
    }

  ECConfiguration *parent = dynamic_cast<ECConfiguration*> (this->scope_.top());
  parent->timeouts.push_back(vs);

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseLocalEventChannel (LocalEventChannel* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::ECCONFIGURATION)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("LocalEventChannel not child of ECConfiguration")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("name"));
  if (val != 0)
    {
      vs->name = val;
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for LocalEventChannel")));
    }

  ECConfiguration *parent = dynamic_cast<ECConfiguration*> (this->scope_.top());
  parent->localECs.push_back(vs);

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseRemoteEventChannel (RemoteEventChannel* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::ECCONFIGURATION)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("RemoteEventChannel not child of ECConfiguration")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("name"));
  if (val != 0)
    {
      vs->name = val;
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for LocalEventChannel")));
    }

  ECConfiguration *parent = dynamic_cast<ECConfiguration*> (this->scope_.top());
  parent->remoteECs.push_back(vs);

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseSchedulingStrategy (SchedulingStrategy* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::LOCALEVENTCHANNEL)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("SchedulingStrategy not child of LocalEventChannel")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("type"));
  if (val != 0)
    {
      if (ACE_OS_String::strcmp(val,ACE_TEXT("EDF")) == 0)
        {
          vs->type = SchedulingStrategy::EDF;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("MUF")) == 0)
        {
          vs->type = SchedulingStrategy::MUF;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("RMS")) == 0)
        {
          vs->type = SchedulingStrategy::RMS;
        }
      else
        {
          ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attribute \"type\" for SchedulingStrategy")));
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for LocalEventChannel")));
    }
  val = alist->getValue(ACE_TEXT("enableRG"));
  if (val != 0)
    {
      if (ACE_OS_String::strcmp(val,ACE_TEXT("yes")) == 0)
        {
          vs->enableRG = true;
        }
      else
        {
          vs->enableRG = false;
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for LocalEventChannel")));
    }

  LocalEventChannel *parent = dynamic_cast<LocalEventChannel*> (this->scope_.top());
  parent->schedulingstrategy = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseConsumer (Consumer* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(vs);
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::LOCALEVENTCHANNEL
      || this->scope_.top()->getSyntaxType() != VisitableSyntax::REMOTEEVENTCHANNEL)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Consumer not child of LocalEventChannel or RemoteEventChannel")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("name"));
  if (val != 0)
    {
      vs->name = val;
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for Consumer")));
    }

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::LOCALEVENTCHANNEL)
    {
      LocalEventChannel *parent = dynamic_cast<LocalEventChannel*> (this->scope_.top());
      parent->consumers.push_back(vs);
    }
  else // must be RemoteEventChannel
    {
      RemoteEventChannel *parent = dynamic_cast<RemoteEventChannel*> (this->scope_.top());
      parent->consumers.push_back(vs);
    }

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseSubscriptions (Subscriptions* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::CONSUMER)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Subscriptions not child of Consumer")));

  Consumer *parent = dynamic_cast<Consumer*> (this->scope_.top());
  parent->subscriptions = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseDependants (Dependants* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::CONSUMER)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Dependants not child of Consumer")));

  Consumer *parent = dynamic_cast<Consumer*> (this->scope_.top());
  parent->dependants = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseSupplier (Supplier* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::LOCALEVENTCHANNEL
      || this->scope_.top()->getSyntaxType() != VisitableSyntax::REMOTEEVENTCHANNEL)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Supplier not child of LocalEventChannel")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("name"));
  if (val != 0)
    {
      vs->name = val;
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for Supplier")));
    }

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::LOCALEVENTCHANNEL)
    {
      LocalEventChannel *parent = dynamic_cast<LocalEventChannel*> (this->scope_.top());
      parent->suppliers.push_back(vs);
    }
  else // must be RemoteEventChannel
    {
      RemoteEventChannel *parent = dynamic_cast<RemoteEventChannel*> (this->scope_.top());
      parent->suppliers.push_back(vs);
    }

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parsePublications (Publications* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::SUPPLIER)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Publications not child of Supplier")));

  Supplier *parent = dynamic_cast<Supplier*> (this->scope_.top());
  parent->publications = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseTriggers (Triggers* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::SUPPLIER)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Triggers not child of Supplier")));

  Supplier *parent = dynamic_cast<Supplier*> (this->scope_.top());
  parent->triggers = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseTestDriver (TestDriver* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::ECCONFIGURATION)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("TestDriver not child of ECConfiguration")));

  ECConfiguration *parent = dynamic_cast<ECConfiguration*> (this->scope_.top());
  parent->driver = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseStartCondition (StartCondition* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::TESTDRIVER)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("StartCondition not child of TestDriver")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("type"));
  if (val != 0)
    {
      if (ACE_OS_String::strcmp(val,ACE_TEXT("GlobalTime")) == 0)
        {
          vs->type = StartCondition::GLOBALTIME;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("DelayAfterStart")) == 0)
        {
          vs->type = StartCondition::DELAYAFTERSTART;
        }
      else if (ACE_OS_String::strcmp(val,ACE_TEXT("DelayAfterConnect")) == 0)
        {
          vs->type = StartCondition::DELAYAFTERCONNECT;
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for StartCondition")));
    }
  val = alist->getValue(ACE_TEXT("master"));
  if (val != 0)
    {
      if (ACE_OS_String::strcmp(val,ACE_TEXT("yes")) == 0)
        {
          vs->master = true;
        }
      else
        {
          vs->master = false;
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for StartCondition")));
    }

  TestDriver *parent = dynamic_cast<TestDriver*> (this->scope_.top());
  parent->startcondition = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseStopCondition (StopCondition* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::TESTDRIVER)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("StopCondition not child of TestDriver")));

  ACEXML_Attributes *alist = static_cast<ACEXML_Attributes*>(arg);
  const ACEXML_Char *val = alist->getValue(ACE_TEXT("type"));
  if (val != 0)
    {
      if (ACE_OS_String::strcmp(val,ACE_TEXT("Duration")) == 0)
        {
          vs->type = StopCondition::DURATION;
        }
      else
        {
          vs->type = StopCondition::EVENTNUMBER;
        }
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for StopCondition")));
    }

  TestDriver *parent = dynamic_cast<TestDriver*> (this->scope_.top());
  parent->stopcondition = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseEventName (EventName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::SUBSCRIPTIONS
      || this->scope_.top()->getSyntaxType() != VisitableSyntax::PUBLICATIONS
      || this->scope_.top()->getSyntaxType() != VisitableSyntax::TRIGGERS)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("EventName not child of Subscriptions, Publications, or Triggers")));

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::SUBSCRIPTIONS)
    {
      Subscriptions *parent = dynamic_cast<Subscriptions*> (this->scope_.top());
      parent->eventnames.push_back(vs);
    }
  else if (this->scope_.top()->getSyntaxType() != VisitableSyntax::PUBLICATIONS)
    {
      Publications *parent = dynamic_cast<Publications*> (this->scope_.top());
      parent->eventnames.push_back(vs);
    }
  else if (this->scope_.top()->getSyntaxType() != VisitableSyntax::TRIGGERS)
    {
      Triggers *parent = dynamic_cast<Triggers*> (this->scope_.top());
      parent->eventnames.push_back(vs);
    }

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseSupplierName (SupplierName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::DEPENDANTS)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("SupplierName not child of Dependants")));

  Dependants *parent = dynamic_cast<Dependants*> (this->scope_.top());
  parent->suppliernames.push_back(vs);

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseTimeoutName (TimeoutName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::TRIGGERS)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("TimeoutName not child of Triggers")));

  Triggers *parent = dynamic_cast<Triggers*> (this->scope_.top());
  parent->timeoutnames.push_back(vs);

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseIORFile (IORFile* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::REMOTEEVENTCHANNEL)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("IORFile not child of RemoteEventChannel")));

  RemoteEventChannel *parent = dynamic_cast<RemoteEventChannel*> (this->scope_.top());
  parent->iorfile = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseWorstExecution (WorstExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("WorstExecution not child of Event")));

  Event *parent = dynamic_cast<Event*> (this->scope_.top());
  parent->worstexecution = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseTypicalExecution (TypicalExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("TypicalExecution not child of Event")));

  Event *parent = dynamic_cast<Event*> (this->scope_.top());
  parent->typicalexecution = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parsePeriod (Period* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT
      || this->scope_.top()->getSyntaxType() != VisitableSyntax::TIMEOUT)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Period not child of Event or Timeout")));

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT)
    {
      Event *parent = dynamic_cast<Event*> (this->scope_.top());
      parent->period = vs;
    }
  else if (this->scope_.top()->getSyntaxType() != VisitableSyntax::TIMEOUT)
    {
      Timeout *parent = dynamic_cast<Timeout*> (this->scope_.top());
      parent->period = vs;
    }

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parsePhase (Phase* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT
      || this->scope_.top()->getSyntaxType() != VisitableSyntax::TIMEOUT)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Phase not child of Event or Timeout")));

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::EVENT)
    {
      Event *parent = dynamic_cast<Event*> (this->scope_.top());
      parent->phase = vs;
    }
  else if (this->scope_.top()->getSyntaxType() != VisitableSyntax::TIMEOUT)
    {
      Timeout *parent = dynamic_cast<Timeout*> (this->scope_.top());
      parent->phase = vs;
    }

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseTime (Time* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::STARTCONDITION)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Time not child of StartCondition")));

  StartCondition *parent = dynamic_cast<StartCondition*> (this->scope_.top());
  parent->time = vs;

  this->scope_.push(vs);
  return 0;
}

int
Configurator_ParseHandler::parseValue (Value* vs, void* arg)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG(arg);

  if (this->scope_.top()->getSyntaxType() != VisitableSyntax::STOPCONDITION)
    ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Value not child of StopCondition")));

  StopCondition *parent = dynamic_cast<StopCondition*> (this->scope_.top());
  parent->value = vs;

  this->scope_.push(vs);
  return 0;
}

VisitableSyntax*
Configurator_ParseHandler::getRootNode (void)
{
  return this->ecconfiguration;
}
