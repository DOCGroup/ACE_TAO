// $Id$

//=============================================================================
/**
 *  @file    Configurator_ParseHandler.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall <thrall@cse.wustl.edu>
 */
//=============================================================================


#ifndef CONFIGURATOR_PARSEHANDLER_H
#define CONFIGURATOR_PARSEHANDLER_H

#include "ace/String_Base.h"
#include "ACEXML/common/ContentHandler.h"
#include "ACEXML/common/ErrorHandler.h"

#include <stack>

#include "SyntaxVisitor.h"
#include "SyntaxTree.h"

//TODO: We need a name table.  If name order shouldn't matter, we're
//going to need two passes: one to get the names and one to parse the
//rest of the XML.

//TODO: An element/attribute tag database would be helpful for adding
//new stuff (make this class more generic) and if tag names change.
//We could hardcode it into setup or have it parse the DTD.

/**
 * @class Configurator_ParseHandler
 *
 * @brief Configurator_ParseHandler is a SAX event handler which
 * parses ecconfig.dtd XML
 *
 * This SAX event handler parses XML according to ecconfig.dtd,
 * producing a set of test_config_t's.
 */
class Configurator_ParseHandler : public ACEXML_ContentHandler,
                                  public ACEXML_ErrorHandler,
                                  public SyntaxVisitor
{
public:

  typedef std::stack<VisitableSyntax*> STACK;

  /*
   * Default constructor.
   */
  Configurator_ParseHandler (const char *filename);

  /*
   * Default destructor.
   */
  virtual ~Configurator_ParseHandler (void);

  // Methods inherited from ACEXML_ContentHandler.

  /*
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           int start,
                           int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the end of a document.
   */
  virtual void endDocument (ACEXML_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the end of an element.
   */
  virtual void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * End the scope of a prefix-URI mapping.
   */
  virtual void endPrefixMapping (const ACEXML_Char *prefix ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  virtual void ignorableWhitespace (const ACEXML_Char *ch,
                                    int start,
                                    int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of a processing instruction.
   */
  virtual void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  virtual void setDocumentLocator (ACEXML_Locator *locator) ;

  /*
   * Receive notification of a skipped entity.
   */
  virtual void skippedEntity (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the beginning of a document.
   */
  virtual void startDocument (ACEXML_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the beginning of an element.
   */
  virtual void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  virtual void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  // Methods inherited from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
  virtual void error (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of a non-recoverable error.
   */
  virtual void fatalError (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of a warning.
   */
  virtual void warning (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  virtual int parseVisitableSyntax (VisitableSyntax *vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual int parseECConfiguration (ECConfiguration* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseEvent (Event* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseCriticality (Criticality* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseImportance (Importance* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTimeout (Timeout* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseLocalEventChannel (LocalEventChannel* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseRemoteEventChannel (RemoteEventChannel* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSchedulingStrategy (SchedulingStrategy* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseConsumer (Consumer* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSubscriptions (Subscriptions* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseDependants (Dependants* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSupplier (Supplier* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parsePublications (Publications* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTriggers (Triggers* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTestDriver (TestDriver* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseStartCondition (StartCondition* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseStopCondition (StopCondition* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual int parseEventName (EventName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSupplierName (SupplierName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTimeoutName (TimeoutName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseIORFile (IORFile* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseWorstExecution (WorstExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTypicalExecution (TypicalExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parsePeriod (Period* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parsePhase (Phase* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTime (Time* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseValue (Value* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual VisitableSyntax* getRootNode (void);

private:

  ACE_CString fileName_;
  ACEXML_Locator* locator_;

  STACK scope_;

  ECConfiguration *ecconfiguration;
};

#endif /* CONFIGURATOR_PARSEHANDLER_H */
