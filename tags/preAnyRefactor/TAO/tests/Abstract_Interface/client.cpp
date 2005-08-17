// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Abstract_Interface,
           client,
           "$Id$")

const char *ior_input_file = "file://test.ior";
int debug = 0;

enum test_select
{
  TEST_STATE,
  TEST_OPERATION,
  TEST_EXCEPTION,
  TEST_ALL
};

test_select which_test = TEST_ALL;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:dsoe");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
              ior_input_file = get_opts.optarg;
              break;
      case 'd':
              debug = 1;
              break;
      case 's':
              which_test = TEST_STATE;
              break;
      case 'o':
              which_test = TEST_OPERATION;
              break;
      case 'e':
              which_test = TEST_EXCEPTION;
              break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                                             "-k <iorfile>",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

void
dump_node (BaseNode* bn,
           int indent)
{
  if (bn == 0)
    {
      return;
    }

  // This is ugly I know
  int i = indent;
  for (; i != 0; --i) ACE_DEBUG ((LM_DEBUG, "  "));

  StringNode* sn = StringNode::_downcast (bn);

  if (debug)
    {
      if (sn != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "%x <StringNode> %s\n",
                     bn,
                     sn->name ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%x <BaseNode> \n",
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
test_operation (base_ptr abs
                ACE_ENV_ARG_DECL)
{
  CORBA::String_var retval = abs->base_op ("base_op"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  retval.in ()));
    }

  CORBA::Object_var obj = abs->_to_object ();

  foo_var concrete = foo::_narrow (obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  retval = concrete->foo_op ("foo_op"
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  retval.in ()));
    }

  retval = concrete->base_op ("base_op"
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  retval.in ()));
    }
}

void
test_exception (base_ptr abs
                ACE_ENV_ARG_DECL)
{
  CORBA::String_var retval = abs->base_op ("bad_name"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  retval.in ()));
    }
}

int
main (int argc, char *argv[])
{
  CORBA::String_var retval;

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior_input_file
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "string_to_object failed.\n"),
                            -1);
        }

      passer_var objref = passer::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (objref.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "_narrow failed.\n"),
                            -1);
        }

      base_var package = base::_nil ();

      if (which_test == TEST_STATE || which_test == TEST_ALL)
        {
          // Create and register factory for BaseNode.
          BaseNode_init *bn_factory = 0;
          ACE_NEW_RETURN (bn_factory,
                          BaseNode_init,
                          1);

          orb->register_value_factory (bn_factory->tao_repository_id (),
                                       bn_factory
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          bn_factory->_remove_ref (); // release ownership

          // Create and register factory for TreeController.
          TreeController_init *tc_factory = 0;
          ACE_NEW_RETURN (tc_factory,
                          TreeController_init,
                          1);

          orb->register_value_factory (tc_factory->tao_repository_id (),
                                       tc_factory
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          tc_factory->_remove_ref (); // release ownership

          // Create and register factory for StringNode.
          StringNode_init *sn_factory = 0;
          ACE_NEW_RETURN (sn_factory,
                          StringNode_init,
                          1);

          orb->register_value_factory (sn_factory->tao_repository_id (),
                                       sn_factory
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          sn_factory->_remove_ref (); // release ownership

          objref->pass_state (package.out ()
                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (package.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "pass_state() returned null OUT arg\n"),
                                -1);
            }

          test_state (package.in ());
        }

      if (which_test != TEST_STATE)
        {
          objref->pass_ops (package.out ()
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (package.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "pass_ops() returned null OUT arg\n"),
                                -1);
            }
        }

      if (which_test == TEST_OPERATION || which_test == TEST_ALL)
        {
          test_operation (package.in ()
                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (which_test == TEST_EXCEPTION || which_test == TEST_ALL)
        {
          which_test = TEST_EXCEPTION;
          test_exception (package.in ()
                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (BadInput, ex)
    {
      if (which_test != TEST_EXCEPTION)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Client: exception caught - ");

          return 1;
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s\n",
                      ex.message.in ()));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client: exception caught - ");

      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
