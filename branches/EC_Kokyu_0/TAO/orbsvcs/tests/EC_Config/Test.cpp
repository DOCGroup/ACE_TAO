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

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished parsing\n")));

      // TODO configure according to parsed XML
      ConfigFactory::Default_Config_Factory fact;
      fact.init(argc,argv);

      Test_Config *backend = fact.create_testconfig();
      if (0 == backend) {
        ACE_DEBUG((LM_DEBUG, "Error: could not create back end!\n"));
        return 1;
      }

      // PROBLEM: occasional segfault on run and configure
      TCFG_SET_WPTR cfg_ptr(handler.get_configs());

      int retval = 0;
      if ((retval = backend->configure(cfg_ptr)) != 0) {
        ACE_DEBUG((LM_DEBUG, "Error configuring back end! (%d)\n",retval));
        return retval;
      }

      if ((retval = backend->run()) != 0) {
        ACE_DEBUG((LM_DEBUG, "Error running back end! (%d)\n",retval));
        return retval;
      }

      fact.destroy_testconfig(backend);
      fact.fini();

    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred: %s. Exiting...\n"),
                  ex.message()));
      return 1;
    }
  ACEXML_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished successfully\n")));

  return retval;
}
