// -*- C++ -*-
#include "testC.h"
#include "ace/Get_Opt.h"

ACE_TCHAR const *ior_input_file = ACE_TEXT ("file://test.ior");
bool debug = false;

enum class Test_Selection
{
  State,
  Operation,
  Exception,
  All
};

Test_Selection which_test = Test_Selection::All;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:dsoe"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
              ior_input_file = get_opts.optarg;
              break;
      case 'd':
              debug = true;
              break;
      case 's':
              which_test = Test_Selection::State;
              break;
      case 'o':
              which_test = Test_Selection::Operation;
              break;
      case 'e':
              which_test = Test_Selection::Exception;
              break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <iorfile>",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

void
dump_node (BaseNode *bn, int indent)
{
  if (bn == nullptr)
    {
      return;
    }

  StringNode *sn = StringNode::_downcast (bn);

  if (debug)
    {
      // This is ugly I know
      int i = indent;
      for (; i != 0; --i) ACE_DEBUG ((LM_DEBUG, "  "));

      if (sn != nullptr)
        {
          ACE_DEBUG ((LM_DEBUG, "%x <StringNode> %C\n",
                     bn,
                     sn->name ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%x <BaseNode>\n",
                      bn));
        }
    }

  dump_node (bn->left (),
             indent + 1);
  dump_node (bn->right (),
             indent + 1);
}

void
dump_tree (TreeController *tc)
{
  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) start tree dump <%x>\n",
                  tc));
    }

  dump_node (tc->root (),
             1);

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) end tree dump <%x>\n",
                  tc));
    }
}

void
test_state (base_ptr abs)
{
  CORBA::ValueBase_var vbase = abs->_to_value ();
  TreeController *tc = TreeController::_downcast (vbase.in ());
  dump_tree (tc);
}

void
test_operation (base_ptr abs)
{
  CORBA::String_var retval = abs->base_op ("base_op");

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  retval.in ()));
    }

  CORBA::Object_var obj = abs->_to_object ();

  foo_var concrete = foo::_narrow (obj.in ());

  retval = concrete->foo_op ("foo_op");

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  retval.in ()));
    }

  retval = concrete->base_op ("base_op");

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  retval.in ()));
    }
}

void
test_exception (base_ptr abs)
{
  try
    {
      CORBA::String_var retval = abs->base_op ("bad_name");

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%C\n",
                      retval.in ()));
        }
    }
  catch (BadInput const &ex)
    {
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%C\n",
                      ex.message.in ()));
        }
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::String_var retval;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior_input_file);

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "string_to_object failed.\n"),
                            -1);
        }

      passer_var objref = passer::_narrow (obj.in ());

      if (CORBA::is_nil (objref.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "_narrow failed.\n"),
                            -1);
        }

      base_var package = base::_nil ();

      if (which_test == Test_Selection::State ||
          which_test == Test_Selection::All)
        {
          // Create and register factory for BaseNode.
          BaseNode_init *bn_factory = nullptr;
          ACE_NEW_RETURN (bn_factory,
                          BaseNode_init,
                          1);

          orb->register_value_factory (bn_factory->tao_repository_id (),
                                       bn_factory);
          bn_factory->_remove_ref (); // release ownership

          // Create and register factory for TreeController.
          TreeController_init *tc_factory = nullptr;
          ACE_NEW_RETURN (tc_factory,
                          TreeController_init,
                          1);

          orb->register_value_factory (tc_factory->tao_repository_id (),
                                       tc_factory);
          tc_factory->_remove_ref (); // release ownership

          // Create and register factory for StringNode.
          StringNode_init *sn_factory = nullptr;
          ACE_NEW_RETURN (sn_factory,
                          StringNode_init,
                          1);

          orb->register_value_factory (sn_factory->tao_repository_id (),
                                       sn_factory);
          sn_factory->_remove_ref (); // release ownership

          objref->pass_state (package.out ());

          if (CORBA::is_nil (package.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "pass_state() returned null OUT arg\n"),
                                -1);
            }

          test_state (package.in ());
        }

      if (which_test != Test_Selection::State)
        {
          objref->pass_ops (package.out ());

          if (CORBA::is_nil (package.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "pass_ops() returned null OUT arg\n"),
                                -1);
            }
        }

      if (which_test == Test_Selection::Operation ||
          which_test == Test_Selection::All)
        {
          test_operation (package.in ());
        }

      if (which_test == Test_Selection::Exception ||
          which_test == Test_Selection::All)
        {
          test_exception (package.in ());
        }

      if (which_test == Test_Selection::All)
        {
          objref->pass_nil (package.out ());

          if (!CORBA::is_nil (package.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "pass_nil() did NOT return null OUT arg\n"),
                                -1);
            }
        }

      objref->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client: exception caught - ");

      return 1;
    }

  return 0;
}
