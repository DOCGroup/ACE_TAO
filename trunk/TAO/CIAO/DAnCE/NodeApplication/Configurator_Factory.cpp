// $Id$

#include "Configurator_Factory.h"
#include "ace/Null_Mutex.h"
#include "ciao/CIAO_common.h"
#include "ace/Arg_Shifter.h"

CIAO::NoOp_Configurator::~NoOp_Configurator (void)
{
  // Not much to do.
}

int
CIAO::NoOp_Configurator::pre_orb_initialize ()
{
  return 0;
}

int
CIAO::NoOp_Configurator::post_orb_initialize (CORBA::ORB_ptr o)
{
  return 0;
}

int
CIAO::NoOp_Configurator::init_resource_manager
(const ::Deployment::Properties & /*properties*/)
{
  // @@ Currently do thing.  We should go over the resource struct in
  // the future and throw exceptions if there are
  // un-recognizable/supported stuff in it.
  return 0;
}

CORBA::PolicyList *
CIAO::NoOp_Configurator::find_container_policies
(const ::Deployment::Properties & /*properties*/)
{
  // Not much to do.

  return 0;
}

int
CIAO::NodeApplication_Options::parse_args (int argc, char *argv[])
{
  //  ACE_Get_Opt get_opts (argc, argv, "nrk:o:");
  ACE_Arg_Shifter shifter (argc, argv);
  
  while (shifter.is_anything_left ()) {
    const char *parm = 0;

    if (shifter.cur_arg_strncasecmp ("-n") == 0) // Use callback.
      {
        this->use_callback_ = 0;
        shifter.consume_arg ();
      }
    else if (shifter.cur_arg_strncasecmp ("-r") == 0)
      {
        this->rt_support_ = 1;
        shifter.consume_arg ();
      }
    else if ((parm = shifter.get_the_parameter ("-o")) !=0)
      {
        this->ior_output_filename_ = parm;
        shifter.consume_arg ();
      }
    else if ((parm = shifter.get_the_parameter ("-o")) !=0)
      {
        this->callback_ior_ = parm;
        shifter.consume_arg ();
      }
    else if (shifter.cur_arg_strncasecmp ("-h") == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-n Do not use Callback (for testing)\n"
                           "-o <ior_output_file>\n"
                           "-k <NodeApplicationManager_callback_ior>\n"
                           "-r Request RT support\n"
                           "-h Usage help"
                           "\n",
                           argv [0]),
                          -1);
        shifter.consume_arg ();
      }
    else
      shifter.ignore_arg ();
    
  }

  if (this->use_callback_ && this->callback_ior_.length() == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Callback IOR to NodeApplicationManager is required.\n"),
                      -1);

  return 0;
}

CIAO::NodeApp_Configurator *
CIAO::NodeApplication_Options::create_nodeapp_configurator (void)
{
  typedef CIAO::NodeApp_Configurator * (*intelligent_designer)(void);

  CIAO::NodeApp_Configurator* ptr = 0;

  if (this->rt_support_)
    {
      int retval = this->config_dll_.open (ACE_DLL_PREFIX ACE_TEXT("CIAO_RTNA_Configurator"));

      if (retval != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "dll.open"),
                          0);
      intelligent_designer config_id; // "id" is for intelligent-designer

      config_id = (intelligent_designer) this->config_dll_.symbol (ACE_TEXT("create_nodeapp_configurator"));

      if (config_id == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p",
                           "dll.symbol"),
                          0);
      ptr = config_id ();
      if (ptr == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error creating RTNodeApp_Configurator\n"),
                          0);
    }
  else
    ACE_NEW_RETURN (ptr,
                    CIAO::NoOp_Configurator (),
                    0);

  return ptr;
}
