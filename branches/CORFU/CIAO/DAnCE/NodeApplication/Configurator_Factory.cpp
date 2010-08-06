// $Id$

#include "Configurator_Factory.h"
#include "NodeApp_Configurator.h"
#include "ciao/CIAO_common.h"
#include "ace/Arg_Shifter.h"

#if !defined (__ACE_INLINE__)
# include "Configurator_Factory.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplication_Options::NodeApplication_Options () : 
   use_callback_ (true), 
   rt_support_ (false)
{
}

int
CIAO::NodeApplication_Options::parse_args (int &argc, char *argv[])
{
  ACE_Arg_Shifter shifter (argc, argv);

  while (shifter.is_anything_left ())
    {
      const char *parm = 0;

      if (shifter.cur_arg_strncasecmp ("-n") == 0) // Use callback.
        {
          this->use_callback_ = false;
          shifter.consume_arg ();
        }
      else if (shifter.cur_arg_strncasecmp ("-r") == 0)
        {
          this->rt_support_ = true;
          shifter.consume_arg ();
        }
      else if (ACE_OS::strncmp (shifter.get_current (),
                                "-ORB",
                                ACE_OS::strlen ("-ORB")) == 0)
        {
          // Ignore ORB parameter
          shifter.ignore_arg ();
        }
      else if (shifter.cur_arg_strncasecmp ("-o") == 0)
        {
          // This double checking is necessary to avoid the Arg_Shifter from
          // mistaking any -ORBxxx flag as -o flag.
          if ((parm = shifter.get_the_parameter ("-o")) !=0)
            {
              this->ior_output_filename_ = parm;
            }
          shifter.consume_arg ();
        }
      else if ((parm = shifter.get_the_parameter ("-k")) !=0)
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
        {
          shifter.ignore_arg ();
        }
    }

  if (this->use_callback_ && 0 == this->callback_ior_.length ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Callback IOR to NodeApplicationManager "
                         "is required.\n"),
                        -1);
    }

  return 0;
}

CIAO::NodeApp_Configurator *
CIAO::NodeApplication_Options::create_nodeapp_configurator (void)
{
  CIAO::NodeApp_Configurator* ptr = 0;
  ACE_NEW_THROW_EX (ptr,
                    CIAO::NodeApp_Configurator (),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));
  if (this->rt_support_)
    {
      ptr->set_rt_support ();
    }

  return ptr;
  /*
  typedef CIAO::NodeApp_Configurator * (*intelligent_designer)(void);
  CIAO::NodeApp_Configurator* ptr = 0;

  if (this->rt_support_)
    {
      int const retval =
        this->config_dll_.open (
          ACE_DLL_PREFIX ACE_TEXT ("CIAO_RTNA_Configurator"),
          ACE_DEFAULT_SHLIB_MODE,
          0);

      if (0 != retval)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p\n",
                             "dll.open"),
                            0);
        }


      // Cast the void* to non-pointer type first - it's not legal to
      // cast a pointer-to-object directly to a pointer-to-function.
      void *void_ptr =
        this->config_dll_.symbol (ACE_TEXT ("create_nodeapp_configurator"));
      ptrdiff_t tmp = reinterpret_cast<ptrdiff_t> (void_ptr);

      // "id" is for intelligent-designer.
      intelligent_designer config_id =
        reinterpret_cast<intelligent_designer> (tmp);

      if (0 == config_id)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p",
                             "dll.symbol"),
                            0);
        }

      ptr = config_id ();

      if (0 == ptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error creating RTNodeApp_Configurator\n"),
                            0);
        }
    }
  else
    {
      ACE_NEW_RETURN (ptr,
                      CIAO::NoOp_Configurator (),
                      0);
    }

  return ptr;
  */
}

