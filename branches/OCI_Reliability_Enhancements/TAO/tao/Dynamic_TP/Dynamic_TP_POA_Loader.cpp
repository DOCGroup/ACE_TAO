// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_POA_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/debug.h"
//TODO:Review these
//#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/Dynamic_TP/Dynamic_TP_Config.h"
#include "tao/Dynamic_TP/Dynamic_TP_POA_Strategy.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "ace/OS_NS_strings.h"
#include "ace/Tokenizer_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Dynamic_TP_POA_Loader::TAO_Dynamic_TP_POA_Loader (void)
  : initialized_ (false)
{
}

TAO_Dynamic_TP_POA_Loader::~TAO_Dynamic_TP_POA_Loader (void)
{
}

int
TAO_Dynamic_TP_POA_Loader::init (int argc, ACE_TCHAR* argv[])
{
  TAO_debug_level = 5;

	ACE_TRACE ("TAO_Dynamic_TP_POA_Loader::init");

  // Only allow initialization once.  TODO:  Are we going to allow only once?
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  //ACE_Service_Gestalt *gestalt = ACE_Service_Config::current ();

  ACE_Service_Object * const dynamic_tp_poa_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance ("Dynamic_TP_POA_Loader");

  //if (dynamic_tp_poa_loader != 0 && dynamic_tp_poa_loader != this)
  //  {
  //    return dynamic_tp_poa_loader->init (argc, argv);
  //  }

  /* First we need a handle to the local Dynamic_TP_Config list that has been loaded prior to executing this functionality.
     The registry associated with the object will contain a registry with instances of thread pool configurations mapped to
	 names of the configuration (key). We will use that as a lookup to associate POA names with a configuration.
  */
  
  TAO_Dynamic_TP_Config_Registry* tp_config_registry =
    ACE_Dynamic_Service<TAO_Dynamic_TP_Config_Registry>::instance
    ("Dynamic_TP_Config_Registry");

  if (tp_config_registry == 0)
  {
	  /* A configuration registry is apparently not loaded, so we cannot proceed */

        if (TAO_debug_level > 0)
        {
            ACE_DEBUG((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader - no thread pool configurations available - cannot initialize registry\n")));
        }
        return -1;

  }

  /*
	Initialize the CSD Framework so that we have a Strategy Repository to use for populating a POA Map to its respective configuration
	needed for the Dynamic Threadpool strategy.
  */
  //TAO_CSD_Framework_Loader::static_init();


    TAO_CSD_Strategy_Repository * poa_to_strategy_repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
    ("TAO_CSD_Strategy_Repository");


  /* Now create a POA Configuration Map Registry to look up what thread pool settings
     are associated with each POA
  */

  if (poa_to_strategy_repo == 0)
    {
		ACE_Service_Config::process_directive(ace_svc_desc_TAO_CSD_Strategy_Repository);
		poa_to_strategy_repo = ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance 
			("TAO_CSD_Strategy_Repository");
	  if (TAO_debug_level > 0)
        {
            ACE_DEBUG((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader - TAO_CSD_Strategy_Repository not found\n")));
        }

      if (poa_to_strategy_repo == 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG((LM_DEBUG,
                         ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader - cannot initialize registry\n")));
            }
          return -1;
        }
	  else
	  {
		   if (TAO_debug_level > 0)
            {
              ACE_DEBUG((LM_DEBUG,
                         ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader - registry initialized\n")));
            }
	  }
    }


  // Parse any service configurator parameters.
  /*
	
  */

  ACE_TCHAR *poa_config_map = 0;
  int curarg = 0;
  int r = 0;

  for (int curarg = 0; curarg < argc; curarg++)
  {

		if (TAO_debug_level > 0)
            {
              ACE_DEBUG((LM_DEBUG,
                         ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader - parsing args\n")));
            }

	if ((r = this->parse_string (curarg, argc, argv, ACE_TEXT("-DTPPOAConfigMap"), poa_config_map)) != 0)
        {
			/* If something comes back in poa_config_map then there is at least on set of maps, otherwise
			   we are missing parameters
			*/
          if (r < 0)
            {
				ACE_ERROR((LM_ERROR,ACE_TEXT("TAO (%P|%t) - Dynamic_TP_POA_Loader: POA Config Map not present\n")));
              return -1;
            }
		  else
		  {
		     
			  ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Dynamic_TP_POA_Loader: POA Config Map = %C\n"),poa_config_map));
			  /* Now parse and load a map to the registry. */
			 if((r = this->load_poa_map (curarg,argv,tp_config_registry,poa_to_strategy_repo)) != 0)
			 {
				 if (r < 0)
				 {
					 return -1;
				 }
			 }


		  }
        }
	else
      {

		  /*
		     If we get here then we have another map set to process and add to the map registry.
		  */
        if (TAO_debug_level > 0)
          {
				ACE_ERROR ((LM_ERROR,
                ACE_TEXT("Dynamic_TP_POA_Loader: Missing option\n")
				ACE_TEXT("Usage: -DTPPOAConfigMap <comma-separated list of POAs>:<POA Config Name>\n")
                ACE_TEXT("<%s>.\n"),
                        argv[curarg]));
          }
      }

  }

  /*
     Now we need to create an ACE_RB_Tree map to store the relationship of a POA thread pool configuration set
	 to each POA name that got passed in on the configuration line.
	 So we need to parse through the comma separated list of POA's and add them to the map along with the
	 configuration set name.

  */


  return 0;
}
  
      
int
TAO_Dynamic_TP_POA_Loader::load_poa_map (int &curarg,
                                         ACE_TCHAR *argv[],
										 TAO_Dynamic_TP_Config_Registry* config_reg,
                                         TAO_CSD_Strategy_Repository * poa_reg)
{

        ACE_CString poa_group;
		ACE_CString poa_name;
		ACE_CString config_name;

        // Parse the parameter
        ACE_CString arg ((const char *)argv[curarg]);
        ACE_CString::size_type pos = arg.find (':');

        if (pos == ACE_CString::npos)
            {
				ACE_ERROR ((LM_ERROR,
				ACE_TEXT("Dynamic_TP_POA_Loader: Missing option\n")
				ACE_TEXT("Usage: -DTPPOAConfigMap <comma-separated list of POAs>:<POA Config Name>\n")
				ACE_TEXT("<%s>.\n"),
                    argv[curarg]));
				return -1;
            }
        else
            {

			/* This is the class that holds a key name to a thread pool configuration
			   and a null pointer to a strategy implementation.A svc.conf file can have
			   a configuration in it that the application never uses, so lazy loading 
			   will keep that resource from being used.
			*/
			TAO_Dynamic_TP_POA_Strategy * strategy_container = 0;
			TAO_Dynamic_TP_POA_StrategyImpl * empty_strategy_impl = 0;

            poa_group = arg.substr (0, pos); // comma separated list of POAs
			ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader: POA Group that needs to be parsed: [%C]\n"), poa_group.c_str()));
			ACE_CString::size_type begin = 0;

			// This is the thread pool configuration name that is used as a key to the threading parameters in the RB Tree.
			config_name = arg.substr (pos + 1, arg.length () - pos);  // Add the new config name into the configuration map

			/* Now that we have a config name and a null strategy implementation class we can 
			   allocate a new instance of a strategy configuration container.
			*/

			ACE_NEW_RETURN(strategy_container,
				           TAO_Dynamic_TP_POA_Strategy(config_name,
						                               empty_strategy_impl,
													   true), -1);

			/* Find first delimiter */
			while(begin < poa_group.length())
			{
				ACE_CString::size_type const poa_delim_pos = poa_group.find(",",begin);

				if (poa_delim_pos == ACE_CString::npos) {
				/* We only have one parameter or this is the last element*/
					poa_name = poa_group.substr(begin,poa_group.length());
					break;

				}
				else
				{
					poa_name = poa_group.substr(begin,poa_delim_pos - begin);

					/* Insert a new entry into the CSD Framework POA/Strategy registry */
					ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader: Mapping POA [%C] to Config [%C].\n"), poa_name.c_str(),config_name.c_str()));
					poa_reg->add_strategy(poa_name, strategy_container);
					
				}				

				begin=poa_delim_pos + 1;
			}

				/* Insert the final entry into the Dynamic_TP_POA_Config_Map */
			    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader: Mapping POA [%C] to Config [%C].\n"), poa_name.c_str(),config_name.c_str()));
				poa_reg->add_strategy(poa_name, strategy_container);

          }

		return 0;
}

int
TAO_Dynamic_TP_POA_Loader::parse_string (int &curarg,
                                     int argc, ACE_TCHAR *argv[],
                                     const ACE_TCHAR *match, ACE_TCHAR *&value)
{
  if (ACE_OS::strcasecmp (argv[curarg], match) != 0)
    return 0;

  ++curarg;
  if (curarg >= argc)
    {
      this->report_option_value_error (match, ACE_TEXT("<missing>"));
      return -1;
    }

  value = argv[curarg];
  return 1;
}

void
TAO_Dynamic_TP_POA_Loader::report_option_value_error (const ACE_TCHAR* option_name,
                                                  const ACE_TCHAR* option_value)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_POA_Loader - unknown ")
                 ACE_TEXT ("argument <%s> for <%s>\n"),
                 option_value, option_name));
    }
}

/////////////////////////////////////////////////////////////////////


ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Loader)
ACE_STATIC_SVC_DEFINE (TAO_Dynamic_TP_POA_Loader,
                       ACE_TEXT ("Dynamic_TP_POA_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Dynamic_TP_POA_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)



TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
