// $Id$

//=============================================================================
/**
 *  @file    Test_Handler.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall <thrall@cse.wustl.edu>
 */
//=============================================================================


#ifndef TEST_HANDLER_H
#define TEST_HANDLER_H

#include "TestConfig.h"

#include "ACEXML/common/ContentHandler.h"
#include "ACEXML/common/ErrorHandler.h"

#include <stack>

enum element { TESTCONFIG,TEST_CONFIG_T,TYPE,PERIOD,CRITICALITY,IMPORTANCE,NUM_ENTITIES };

/**
 * @class Test_Handler
 *
 * @brief Test_Handler is a SAX event handler which parses testconfig.dtd XML
 *
 * This SAX event handler parses XML according to testconfig.dtd, producing a
 * set of test_config_t's.
 */
class Test_Handler : public ACEXML_ContentHandler, public ACEXML_ErrorHandler
{
public:

  typedef std::stack<element> STACK;

  /*
   * Default constructor.
   */
  Test_Handler (ACEXML_Char* fileName);

  /*
   * Default destructor.
   */
  virtual ~Test_Handler (void);

  /**
   * Returns a reference to the Handler's internal set of
   * test_config_t's. The Handler retains ownership of the
   * (dynamically allocated) test_config_t's in the set.
   */
  const TestConfig::TCFG_SET_WPTR get_configs (void) const;

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
private:
  TestConfig::TCFG_SET_SPTR configs_;

  ACEXML_Char* fileName_;
  ACEXML_Locator* locator_;

  STACK scope_;

  int didtype_,didperiod_,didcrit_,didimp_,didnum_;
  //for each test_config_t, whether the leaf element was processed or not
};

#endif /* TEST_HANDLER_H */
