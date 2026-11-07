//=============================================================================
/**
 *  @file    Load_Balancing_Service.cpp
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#include "Load_Balancing_Service.h"
#include "Load_Balancer_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

int
Load_Balancing_Service::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("do:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o': // outputs object ior to the specified file.
        this->ior_output_file_.reset (ACE_OS::fopen (get_opts.opt_arg (), "w"));

        if (!this->ior_output_file_)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Load_Balancing_Service::init (int argc,
                              ACE_TCHAR *argv[])
{
  int result {};
  CORBA::String_var ior;

  try
    {
      result = this->orb_manager_.init (argc,
                                        argv);
      if (result == -1)
        return result;

      // Check the non-ORB arguments.
      result = this->parse_args (argc, argv);
      if (result < 0)
        return result;

      // Create, ref. count, and activate the servant.
      Object_Group_Factory_i *factory_servant {};
      ACE_NEW_RETURN (factory_servant,
                      Object_Group_Factory_i (),
                      -1);
      PortableServer::ServantBase_var s = factory_servant;
      ior = this->orb_manager_.activate (factory_servant);

      if (ior.in () == nullptr)
        return -1;
      if (TAO_debug_level > 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Load_Balancer: Object Group Factory ior is %C\n",
                    ior.in ()));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Load_Balancing_Service::init");
      return -1;
    }

  if (this->ior_output_file_)
    {
      std::fprintf (this->ior_output_file_.get (), "%s", ior.in ());
      this->ior_output_file_.reset ();
    }
  return 0;
}

int
Load_Balancing_Service::run ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Load_Balancer: Initialized\n"));

  return this->orb_manager_.run ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  Load_Balancing_Service factory;

  if (factory.init (argc, argv) == -1)
    return 1;

  try
    {
      result = factory.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Load_Balancing_Service");
      return 1;
    }

  if (result == -1)
    return 1;
  return 0;
}
