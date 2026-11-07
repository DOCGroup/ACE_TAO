#include "testC.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TCHAR const *ior_input_file { ACE_TEXT ("file://test.ior") };
  bool debug {};
  bool do_shutdown {};
};

int
register_factories (CORBA::ORB_ptr orb)
{
  // Create and register factories.

  obvmod::tcOffset1_init *factory1 {};
  ACE_NEW_RETURN (factory1,
                  obvmod::tcOffset1_init,
                  -1);

  CORBA::ValueFactoryBase_var factory1_owner (factory1);
  orb->register_value_factory (factory1->tao_repository_id (),
                               factory1_owner.in ());

  obvmod::tcOffset003_init *factory3 {};
  ACE_NEW_RETURN (factory3,
                  obvmod::tcOffset003_init,
                  -1);

  CORBA::ValueFactoryBase_var factory3_owner (factory3);
  orb->register_value_factory (factory3->tao_repository_id (),
                               factory3_owner.in ());

  obvmod::tcOffset00005_init *factory5 {};
  ACE_NEW_RETURN (factory5,
                  obvmod::tcOffset00005_init,
                  -1);

  CORBA::ValueFactoryBase_var factory5_owner (factory5);
  orb->register_value_factory (factory5->tao_repository_id (),
                               factory5_owner.in ());

  obvmod::tcOffset0000007_init *factory7 {};
  ACE_NEW_RETURN (factory7,
                  obvmod::tcOffset0000007_init,
                  -1);

  CORBA::ValueFactoryBase_var factory7_owner (factory7);
  orb->register_value_factory (factory7->tao_repository_id (),
                               factory7_owner.in ());
  return 0;
}

int
run_test (test_ptr objref,
          CORBA::Long offset,
          Client_Options const& options)
{
  CORBA::Any_var result = objref->get_value (offset);

  CORBA::Boolean status {};
  CORBA::Long member_value {};

  // The extracted value pointer is owned by result and remains valid for this scope.
  switch (offset)
  {
    case 1:
      {
        obvmod::tcOffset1 *value {};
        status = (result.in () >>= value);
        if (status)
          {
            member_value = value->mem ();
          }
      }
      break;
    case 3:
      {
        obvmod::tcOffset003 *value {};
        status = (result.in () >>= value);
        if (status)
          {
            member_value = value->mem ();
          }
      }
      break;
    case 5:
      {
        obvmod::tcOffset00005 *value {};
        status = (result.in () >>= value);
        if (status)
          {
            member_value = value->mem ();
          }
      }
      break;
    case 7:
      {
        obvmod::tcOffset0000007 *value {};
        status = (result.in () >>= value);
        if (status)
          {
            member_value = value->mem ();
          }
      }
      break;
    default:
      return -1;
  }

  if (!status)
    {
      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Any extraction failed for repo id length offset"
                      " of %d from 8-byte boundary\n",
                      offset));
        }

      return -1;
    }

  return (member_value == offset);
}

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        options.debug = true;
        break;

      case 'k':
        options.ior_input_file = get_opts.optarg;
        break;

      case 'x':
        options.do_shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d "
                           "-k <ior> "
                           "-x "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);

      if (parse_args (argc, argv, options) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (options.ior_input_file);

      int status = register_factories (orb.in ());

      if (status != 0)
        {
          if (options.debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "OBV factory registration failed\n"));
            }

          return -1;
        }

      test_var objref = test::_narrow (obj.in ());

      for (CORBA::Long i = 1; i < 8; i += 2)
        {
          status = run_test (objref.in (), i, options);

          if (status == 0)
            {
              if (options.debug)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Incorrect member value - expected %d\n",
                              i));
                }

              return -1;
            }
        }

      if (options.do_shutdown)
        {
          objref->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("exception:");
      return 1;
    }

  return 0;
}
