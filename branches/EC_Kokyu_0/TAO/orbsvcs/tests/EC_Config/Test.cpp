// $Id$

#include "ace/Array.h"
#include "ace/Bound_Ptr.h"
#include "ace/Synch.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/DefaultHandler.h"

#include "ECConfig.h"
#include "Config_Factory.h"
#include "Test_Handler.h"

using namespace TestConfig;

typedef ACE_Strong_Bound_Ptr<Test_Config_Set,ACE_Null_Mutex> TCFG_SET_SPTR;

//NOTE: Read from a formatted file rather than hardcode the configuration. Check ACE_XML.

int
main (int argc, char *argv[])
{
  int retval = 0;

  ACEXML_TRY_NEW_ENV
    {
      ACEXML_Parser parser;

      // TODO parse args for config filename

      ACEXML_Char *filename = ACE_LIB_TEXT("test.xml");
      ACEXML_FileCharStream fcs;
      if ((retval = fcs.open(filename)) != 0) {
        ACE_DEBUG ((LM_DEBUG, "Could not open file %s\n",filename));
        return retval;
      }

      ACEXML_InputSource is (&fcs);

      Test_Handler handler (filename);
      ACEXML_DefaultHandler dflt;

      parser.setContentHandler (&handler);
      parser.setDTDHandler (&dflt);
      parser.setErrorHandler (&handler);
      parser.setEntityResolver (&dflt);

      parser.parse(&is);
      ACEXML_TRY_CHECK;

      // TODO configure according to parsed XML
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished parsing\n")));
    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      //ACE_UNUSED_ARG (ex);
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred: %s. Exiting...\n"),
                  ex.message()));
      return 1;
    }
  ACEXML_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished successfully\n")));

#if 0
  ConfigFactory::Default_Config_Factory fact;
  fact.init(argc,argv);

  Test_Config *backend = fact.create_testconfig();
  if (0 == backend) {
    ACE_DEBUG((LM_DEBUG, "Error: could not create back end!\n"));
    return 1;
  }

  TCFG_SET_SPTR cfg_ptr(new Test_Config_Set(2));

  (*cfg_ptr)[0] = new test_config_t();
  (*cfg_ptr)[0]->type = 0;
  (*cfg_ptr)[0]->period = 1000;
  (*cfg_ptr)[0]->criticality = HIGH_CRITICALITY;
  (*cfg_ptr)[0]->importance = VERY_LOW_IMPORTANCE;
  (*cfg_ptr)[0]->num_entities = 10;

  (*cfg_ptr)[1] = new test_config_t();
  (*cfg_ptr)[1]->type = 1;
  (*cfg_ptr)[1]->period = 2000;
  (*cfg_ptr)[1]->criticality = LOW_CRITICALITY;
  (*cfg_ptr)[1]->importance = VERY_LOW_IMPORTANCE;
  (*cfg_ptr)[1]->num_entities = 10;

  // PROBLEM: occasional segfault on run and configure
  int retval = 0;
  if ((retval = backend->configure(TCFG_SET_WPTR(cfg_ptr))) != 0) {
    ACE_DEBUG((LM_DEBUG, "Error configuring back end! (%d)\n",retval));
    return retval;
  }

  if ((retval = backend->run()) != 0) {
    ACE_DEBUG((LM_DEBUG, "Error running back end! (%d)\n",retval));
    return retval;
  }

  for(size_t i=0; i<cfg_ptr->size(); ++i) {
    delete (*cfg_ptr)[i];
  }

  fact.destroy_testconfig(backend);
  fact.fini();
#endif /* 0 */

  return retval;
}
