
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is the client program that tests TAO's Smart Proxy extension.
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "testC.h"
#include "Smart_Proxy_Impl.h"

struct Options
{
  ACE_TCHAR const *ior1 {ACE_TEXT("file://test1.ior")};
  ACE_TCHAR const *ior2 {ACE_TEXT("file://test2.ior")};
  bool one_shot_factory {true};
};

static int
parse_args (Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:j:f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        options.ior1 = get_opts.opt_arg ();
        break;
      case 'j':
        options.ior2 = get_opts.opt_arg ();
        break;
      case 'f':
        options.one_shot_factory = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i -j -f"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
run_test (CORBA::ORB_ptr orb_ptr,
          ACE_TCHAR const *ior)
{
  CORBA::ORB_var orb = CORBA::ORB::_duplicate (orb_ptr);
  try
    {
      CORBA::Object_var object = orb->string_to_object (ior);

      Test_var server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference is nil\n"),
                          1);

      server->method (0);

      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client-side exception:");
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Options options;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      if (parse_args (options, argc, argv) != 0)
        return 1;

      // To use the smart proxy it is necessary to allocate the
      // user-defined smart factory on the heap as the smart proxy
      // generated classes take care of destroying the object. This
      // way it a win situation for the application developer who
      // doesnt have to make sure to destroy it and also for the smart
      // proxy designer who now can manage the lifetime of the object
      // much surely.
      // The default policy keeps the factory registered for this interface.
      // Set -f 0 to create a separate smart proxy for each object instance.
      Smart_Test_Factory *test_factory {};
      ACE_NEW_RETURN (test_factory,
                      Smart_Test_Factory (options.one_shot_factory),
                      -1);

      ACE_UNUSED_ARG (test_factory);

      run_test (orb.in (), options.ior1);
      run_test (orb.in (), options.ior2);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client-side exception:");
      return 1;
    }

  return 0;
}
