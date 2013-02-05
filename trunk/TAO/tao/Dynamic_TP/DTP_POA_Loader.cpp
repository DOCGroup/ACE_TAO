// $Id$

#include "tao/Dynamic_TP/DTP_POA_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/debug.h"
#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/Dynamic_TP/DTP_Config.h"
#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "ace/OS_NS_strings.h"
#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/PI/DLL_Resident_ORB_Initializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/ORB_Core.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DTP_POA_Loader::TAO_DTP_POA_Loader (void)
{
}

TAO_DTP_POA_Loader::~TAO_DTP_POA_Loader (void)
{
}

int
TAO_DTP_POA_Loader::init (int argc, ACE_TCHAR* argv[])
{
  //TAO_debug_level = 5;

  ACE_TRACE ("TAO_DTP_POA_Loader::init");

  // Only allow initialization once.
  static bool initialized = false;
  if (initialized)
    return 0;
  initialized = true;

  TAO_CSD_Strategy_Repository * repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
    ("TAO_CSD_Strategy_Repository");


  /* Now create a POA Configuration Map Registry to look up what
     thread pool settings are associated with each POA
  */

  if (repo == 0)
    {
      ACE_Service_Config::process_directive(
        ace_svc_desc_TAO_CSD_Strategy_Repository);
      repo = ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
        ("TAO_CSD_Strategy_Repository");
    }

  if (repo == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - DTP_POA_Loader - ")
                     ACE_TEXT ("cannot initialize strategy repo\n")));
        }
      return -1;
    }

  repo->init(0,0);

  for (int curarg = 0; curarg < argc; curarg++)
    {

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - DTP_POA_Loader - ")
                     ACE_TEXT ("parsing args\n")));
        }

      if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT ("-DTPPOAConfigMap"))
          == 0)
        {
          ++curarg;
          if (curarg >= argc)
            {
              this->report_option_value_error (ACE_TEXT ("-DTPPOAConfigMap"),
                                               ACE_TEXT("<missing>"));
              return -1;
            }
          if ((this->load_poa_map (argv[curarg], repo)) < 0)
            {
              return -1;
            }
        }
      else
        {

          /*
            If we get here then we have another map set to process and
            add to the map registry.
          */
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("DTP_POA_Loader: Missing option\n")
                          ACE_TEXT ("Usage: -DTPPOAConfigMap <comma-separated ")
                          ACE_TEXT ("list of POAs>:<POA Config Name>\n")
                          ACE_TEXT ("<%s>.\n"),
                          argv[curarg]));
            }
        }

    }

  return 0;
}


int
TAO_DTP_POA_Loader::load_poa_map (ACE_TCHAR *map,
                                  TAO_CSD_Strategy_Repository *repo)
{

  ACE_CString poa_name;
  ACE_CString config_name;
  TAO_DTP_POA_Strategy * strategy_container = 0;

  ACE_TCHAR *sep = ACE_OS::strchr (map, ':');

  if (sep == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("DTP_POA_Loader: Missing option\n")
                      ACE_TEXT ("Usage: -DTPPOAConfigMap <comma-separated ")
                      ACE_TEXT ("list of POAs>:<POA Config Name>\n<%s>.\n"),
                      map));
        }
      return -1;
    }

  config_name = ACE_TEXT_ALWAYS_CHAR (sep + 1);
  *sep = 0;

  // Now that we have a config name and a null strategy
  // implementation class we can allocate a new instance of a
  // strategy configuration container.

  ACE_NEW_RETURN(strategy_container,
                 TAO_DTP_POA_Strategy(config_name,
                                      false), -1);

  sep = ACE_OS::strchr (map, ',');
  while (sep != 0)
    {
      *sep = 0;
      poa_name = ACE_TEXT_ALWAYS_CHAR (map);
      repo->add_strategy (poa_name, strategy_container);
      map = sep + 1;
      sep = ACE_OS::strchr (map, ',');
    }
  poa_name = ACE_TEXT_ALWAYS_CHAR (map);
  repo->add_strategy(poa_name, strategy_container);

  return 0;
}

void
TAO_DTP_POA_Loader::report_option_value_error (const ACE_TCHAR* name,
                                               const ACE_TCHAR* value)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) - DTP_POA_Loader - unknown ")
                 ACE_TEXT ("argument <%s> for <%s>\n"),
                 value, name));
    }
}


/////////////////////////////////////////////////////////////////////


ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_DTP_POA_Loader)
ACE_STATIC_SVC_DEFINE (TAO_DTP_POA_Loader,
                       ACE_TEXT ("DTP_POA_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DTP_POA_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
