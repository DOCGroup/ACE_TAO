// $Id$

#include "ace/Get_Opt.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/DefaultHandler.h"

#include "tao/ORB.h"

#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"

#include "Configurator_ParseHandler.h"
#include "Configurator_SyntaxHandler.h"
#include "ECDriver.h"

struct Arguments
{
  ACE_CString filename_;
};

int parse_args (int argc, char *argv[],Arguments &args);

int
main (int argc, char *argv[])
{
  int retval;

  TAO_EC_Kokyu_Factory::init_svcs ();
  TAO_EC_Gateway_IIOP_Factory::init_svcs ();

  ACEXML_TRY_NEW_ENV
    {
      ACEXML_Parser parser;
      Arguments args;
      args.filename_.set(ACE_TEXT("consumerec.xml"));

      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // parse args for config filename
      if (parse_args(argc,argv,args) == -1)
        {
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACEXML_FileCharStream *fcs = new ACEXML_FileCharStream();
      if ((retval = fcs->open(args.filename_.c_str())) != 0) {
        //ACE_DEBUG ((LM_DEBUG, "Could not open file %s\n",args.filename_.c_str()));
        return retval;
      }

      ACEXML_InputSource is (fcs); //takes responsibility of fcs

      Configurator_ParseHandler xmlhandler (args.filename_.c_str());
      ACEXML_DefaultHandler dflt;

      parser.setContentHandler (&xmlhandler);
      parser.setDTDHandler (&dflt);
      parser.setErrorHandler (&xmlhandler);
      parser.setEntityResolver (&dflt);

      parser.parse(&is);
      ACEXML_TRY_CHECK;

      if ((retval = fcs->close()) != 0) {
        //ACE_DEBUG ((LM_DEBUG, "Could not close file %s\n",args.filename_.c_str()));
        return retval;
      }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished parsing\n")));

      Configurator_SyntaxHandler cfgtor;
      cfgtor.init(orb.in(),poa.in());
      cfgtor.setRootNode(xmlhandler.getRootNode());
      cfgtor.setNameTable(xmlhandler.getNameTable());
      cfgtor.parse();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished configuring\n")));

      ECDriver *drv = cfgtor.getDriver();
      drv->run();

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

int parse_args (int argc, char *argv[], Arguments &args)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        args.filename_.set(get_opts.opt_arg());
        //ACE_DEBUG((LM_DEBUG,ACE_TEXT("Filename argument: %s\n"),args.filename_.c_str()));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f <filename>] "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
