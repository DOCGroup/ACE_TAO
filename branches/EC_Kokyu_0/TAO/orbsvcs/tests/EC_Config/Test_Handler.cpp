// -*- C++ -*-  $Id$

#include "Test_Handler.h"

#include "TestConfig.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#include <stdlib.h> //for atol
#include <sstream>  //for istringstream

Test_Handler::Test_Handler (ACEXML_Char* fileName)
  : configs_(new TestConfig::Test_Config_Set(0)),
    fileName_(ACE::strnew (fileName)),
    didtype_(0), //false
    didperiod_(0),
    didcrit_(0),
    didimp_(0),
    didnum_(0)
{

}

Test_Handler::~Test_Handler (void)
{
  delete[] this->fileName_;

  const TestConfig::Test_Config_Set &cfgs = *this->configs_;

  for(size_t i=0; i<cfgs.size(); ++i) {
    delete cfgs[i];
  }
}

const TestConfig::TCFG_SET_WPTR
Test_Handler::get_configs (void) const
{
  return TestConfig::TCFG_SET_WPTR(this->configs_);
}


void
Test_Handler::characters (const ACEXML_Char * cdata,
                          int ,
                          int
                          ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  const TestConfig::Test_Config_Set &cfgs = *this->configs_;
  TestConfig::test_config_t *curcfg = 0; //current test_config_t is last in set
  std::istringstream iss(cdata);

  switch (this->scope_.top())
    {
    case TYPE:
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TYPE Characters: %s\n"),cdata));
      curcfg = cfgs[cfgs.size()-1];
      if (curcfg == 0)
        {
          ACEXML_THROW(ACEXML_SAXException(ACE_TEXT ("No existing test_config_t")));
        }
      if ((iss >> curcfg->type) == 0)
        {
          ACEXML_THROW(ACEXML_SAXException(ACE_TEXT ("Invalid number format")));
        }
      this->didtype_ = 1; //true
      break;
    case PERIOD:
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("PERIOD Characters: %s\n"),cdata));
      curcfg = cfgs[cfgs.size()-1];
      if (curcfg == 0)
        {
          ACEXML_THROW(ACEXML_SAXException(ACE_TEXT ("No existing test_config_t")));
        }
      if ((iss >> curcfg->period) == 0)
        {
          ACEXML_THROW(ACEXML_SAXException(ACE_TEXT ("Invalid number format")));
        }
      this->didperiod_ = 1; //true
      break;
    case NUM_ENTITIES:
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("NUM_ENTITIES Characters: %s\n"),cdata));
      curcfg = cfgs[cfgs.size()-1];
      if (curcfg == 0)
        {
          ACEXML_THROW(ACEXML_SAXException(ACE_TEXT ("No existing test_config_t")));
        }
      if ((iss >> curcfg->num_entities) == 0)
        {
          ACEXML_THROW(ACEXML_SAXException(ACE_TEXT ("Invalid number format")));
        }
      this->didnum_ = 1; //true
      break;
    case CRITICALITY:
    case IMPORTANCE:
    case TEST_CONFIG_T:
    case TESTCONFIG:
    default:
      //ignore characters outside leaf elements
      /*
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ignoring characters outside leaf elements: %s\n"),
                  cdata));
      */
      break;
    }
}

void
Test_Handler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
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
    ACE_DEBUG ((LM_DEBUG, cfg_format,
                cfg->type,
                cfg->period,
                cfg->criticality,
                cfg->importance,
                cfg->num_entities));
  }
  //finish last line
  ACE_DEBUG ((LM_DEBUG, "\n"));
}

void
Test_Handler::endElement (const ACEXML_Char *,
                          const ACEXML_Char *,
                          const ACEXML_Char *
                          ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->scope_.empty() == 1)
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Element end outside of any scope")));
    }
  else
    {
      if (this->scope_.top() == TEST_CONFIG_T)
        {
          //check that all leaf elements were there
          if (this->didtype_ == 0
              || this->didperiod_ == 0
              || this->didcrit_ == 0
              || this->didimp_ == 0
              || this->didnum_ == 0)
            {
              ACEXML_THROW(ACEXML_SAXException(ACE_TEXT("Missing leaf element "
                                                        "(TYPE, PERIOD, CRITICALITY, "
                                                        "IMPORTANCE, or NUM_ENTITIES)")));
            }
        }
      this->scope_.pop();
      /*
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Popped scope; new top is %d\n"),
                  this->scope_.empty() ? -1 : this->scope_.top()));
      */
    }
}

void
Test_Handler::endPrefixMapping (const ACEXML_Char *
                                ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Test_Handler::ignorableWhitespace (const ACEXML_Char *,
                                   int,
                                   int
                                   ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Test_Handler::processingInstruction (const ACEXML_Char *,
                                             const ACEXML_Char *
                                             ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Test_Handler::setDocumentLocator (ACEXML_Locator * locator)
{
  this->locator_ = locator;
}

void
Test_Handler::skippedEntity (const ACEXML_Char *
                                     ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Test_Handler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
Test_Handler::startElement (const ACEXML_Char *uri,
                                    const ACEXML_Char *name,
                                    const ACEXML_Char *qName,
                                    ACEXML_Attributes *alist
                                    ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  /*
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startElement (%s, %s, %s) ***************\n"),
              uri, name, qName));

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

  const TestConfig::Test_Config_Set &cfgs = *this->configs_;

  element newscope;
  if (ACE_OS_String::strcmp (qName, ACE_TEXT ("type")) == 0)
    {
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pushing TYPE scope\n")));
      newscope = TYPE;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("period")) == 0)
    {
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pushing PERIOD scope\n")));
      newscope = PERIOD;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("criticality")) == 0)
    {
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pushing CRITICALITY scope\n")));
      newscope = CRITICALITY;
      if (alist != 0)
        {
          const ACEXML_Char *val = alist->getValue(ACE_TEXT("value"));
          if (val != 0)
            {
              TestConfig::test_config_t *curcfg = 0; //current test_config_t is last in set
              curcfg = cfgs[cfgs.size()-1];
              if (curcfg == 0)
                {
                  ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("No existing test_config_t")));
                }
              if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_LOW")) == 0)
                {
                  curcfg->criticality = TestConfig::VERY_LOW_CRITICALITY;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("LOW")) == 0)
                {
                  curcfg->criticality = TestConfig::LOW_CRITICALITY;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("MEDIUM")) == 0)
                {
                  curcfg->criticality = TestConfig::MEDIUM_CRITICALITY;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("HIGH")) == 0)
                {
                  curcfg->criticality = TestConfig::HIGH_CRITICALITY;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_HIGH")) == 0)
                {
                  curcfg->criticality = TestConfig::VERY_HIGH_CRITICALITY;
                }
              else
                {
                  ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid value for \"value\""
                                                           " attribute for criticality")));
                }
            }
          else
            {
              ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for criticality")));
            }
          this->didcrit_ = 1; //true
        }
      else
        {
          ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("No attributes for criticality")));
        }
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("importance")) == 0)
    {
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pushing IMPORTANCE scope\n")));
      newscope = IMPORTANCE;
      if (alist != 0)
        {
          const ACEXML_Char *val = alist->getValue(ACE_TEXT("value"));
          if (val != 0)
            {
              TestConfig::test_config_t *curcfg = 0; //current test_config_t is last in set
              curcfg = cfgs[cfgs.size()-1];
              if (curcfg == 0)
                {
                  ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("No existing test_config_t")));
                }
              if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_LOW")) == 0)
                {
                  curcfg->importance = TestConfig::VERY_LOW_IMPORTANCE;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("LOW")) == 0)
                {
                  curcfg->importance = TestConfig::LOW_IMPORTANCE;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("MEDIUM")) == 0)
                {
                  curcfg->importance = TestConfig::MEDIUM_IMPORTANCE;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("HIGH")) == 0)
                {
                  curcfg->importance = TestConfig::HIGH_IMPORTANCE;
                }
              else if (ACE_OS_String::strcmp(val,ACE_TEXT("VERY_HIGH")) == 0)
                {
                  curcfg->importance = TestConfig::VERY_HIGH_IMPORTANCE;
                }
              else
                {
                  ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid value for \"value\""
                                                           " attribute for importance")));
                }
            }
          else
            {
              ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Invalid attributes for importance")));
            }
          this->didimp_ = 1; //true
        }
      else
        {
          ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("No attributes for importance")));
        }
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("num_entities")) == 0)
    {
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pushing NUM_ENTITIES scope\n")));
      newscope = NUM_ENTITIES;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("test_config_t")) == 0)
    {
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pushing TEST_CONFIG_T scope\n")));
      newscope = TEST_CONFIG_T;
      //create test_config_t for filling in by the sub-elements of this element
      TestConfig::test_config_t *newcfg = 0;
      ACE_NEW(newcfg,TestConfig::test_config_t());
      TestConfig::Test_Config_Set &cfgs = *this->configs_;
      size_t cfgsize = cfgs.size();
      cfgs.size(cfgsize+1);
      cfgs[cfgsize] = newcfg;
      this->didtype_ = 0; //false
      this->didperiod_ = 0; //false
      this->didcrit_ = 0; //false
      this->didimp_ = 0; //false
      this->didnum_ = 0; //false
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("testconfig")) == 0)
    {
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pushing TESTCONFIG scope\n")));
      newscope = TESTCONFIG;
    }
  else
    {
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT("Element start of unknown type")));
    }
  this->scope_.push(newscope);
}

void
Test_Handler::startPrefixMapping (const ACEXML_Char *,
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
Test_Handler::error (ACEXML_SAXParseException & ex ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{

  ACE_DEBUG ((LM_DEBUG, "%s:%d:%d ", this->fileName_,
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
Test_Handler::fatalError (ACEXML_SAXParseException& ex ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{

  ACE_DEBUG ((LM_DEBUG, "%s:%d:%d ", this->fileName_,
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
Test_Handler::warning (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}
