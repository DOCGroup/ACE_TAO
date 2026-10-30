#include "testC.h"
#include "ace/Get_Opt.h"

static ACE_TCHAR const *ior_input_file = ACE_TEXT ("file://test.ior");

static obvmod::tcOffset1 *vt1 = nullptr;
static obvmod::tcOffset003 *vt3 = nullptr;
static obvmod::tcOffset00005 *vt5 = nullptr;
static obvmod::tcOffset0000007 *vt7 = nullptr;

bool debug = false;
bool do_shutdown = false;

int
register_factories (CORBA::ORB_ptr orb)
{
  // Create and register factories.

  obvmod::tcOffset1_init *factory1 = nullptr;
  ACE_NEW_RETURN (factory1,
                  obvmod::tcOffset1_init,
                  -1);

  orb->register_value_factory (factory1->tao_repository_id (),
                               factory1);

  factory1->_remove_ref ();

  obvmod::tcOffset003_init *factory3 = nullptr;
  ACE_NEW_RETURN (factory3,
                  obvmod::tcOffset003_init,
                  -1);

  orb->register_value_factory (factory3->tao_repository_id (),
                               factory3);

  factory3->_remove_ref ();

  obvmod::tcOffset00005_init *factory5 = nullptr;
  ACE_NEW_RETURN (factory5,
                  obvmod::tcOffset00005_init,
                  -1);

  orb->register_value_factory (factory5->tao_repository_id (),
                               factory5);

  factory5->_remove_ref ();

  obvmod::tcOffset0000007_init *factory7 = nullptr;
  ACE_NEW_RETURN (factory7,
                  obvmod::tcOffset0000007_init,
                  -1);

  orb->register_value_factory (factory7->tao_repository_id (),
                               factory7);

  factory7->_remove_ref ();
  return 0;
}

int
run_test (test_ptr objref,
          CORBA::Long offset)
{
  CORBA::Any_var result = objref->get_value (offset);

  CORBA::Boolean status = false;
  CORBA::Long member_value = 0;

  switch (offset)
  {
    case 1:
      status = (result.in () >>= vt1);
      member_value = vt1->mem ();
      break;
    case 3:
      status = (result.in () >>= vt3);
      member_value = vt3->mem ();
      break;
    case 5:
      status = (result.in () >>= vt5);
      member_value = vt5->mem ();
      break;
    case 7:
      status = (result.in () >>= vt7);
      member_value = vt7->mem ();
      break;
    default:
      return -1;
  }

  if (!status)
    {
      if (debug)
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
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        debug = true;
        break;

      case 'k':
        ior_input_file = get_opts.optarg;
        break;

      case 'x':
        do_shutdown = true;
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior_input_file);

      int status = register_factories (orb.in ());

      if (status != 0)
        {
          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "OBV factory registration failed\n"));
            }

          return -1;
        }

      test_var objref = test::_narrow (obj.in ());

      for (CORBA::Long i = 1; i < 8; i += 2)
        {
          status = run_test (objref.in (), i);

          if (status == 0)
            {
              if (debug)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Incorrect member value - expected %d\n",
                              i));
                }

              return -1;
            }
        }

      if (do_shutdown)
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
