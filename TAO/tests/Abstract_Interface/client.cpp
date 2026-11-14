// -*- C++ -*-
#include "testC.h"
#include "ace/Get_Opt.h"
#include "tao/Valuetype/ValueFactory.h"

enum class Test_Selection
{
  State,
  Operation,
  Exception,
  All
};

struct Options
{
  ACE_TCHAR const *ior_input_file {ACE_TEXT ("file://test.ior")};
  bool debug {};
  Test_Selection which_test {Test_Selection::All};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:dsoe"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior_input_file = get_opts.optarg;
        break;
      case 'd':
        options.debug = true;
        break;
      case 's':
        options.which_test = Test_Selection::State;
        break;
      case 'o':
        options.which_test = Test_Selection::Operation;
        break;
      case 'e':
        options.which_test = Test_Selection::Exception;
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
dump_node (BaseNode * const bn, int const indent, Options const &options)
{
  if (bn == nullptr)
    {
      return;
    }

  StringNode const * const sn = StringNode::_downcast (bn);

  if (options.debug)
    {
      // This is ugly I know
      for (int i {indent}; i != 0; --i)
        {
          ACE_DEBUG ((LM_DEBUG, "  "));
        }

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
             indent + 1,
             options);
  dump_node (bn->right (),
             indent + 1,
             options);
}

void
dump_tree (TreeController * const tc, Options const &options)
{
  if (options.debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) start tree dump <%x>\n",
                  tc));
    }

  dump_node (tc->root (),
             1,
             options);

  if (options.debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) end tree dump <%x>\n",
                  tc));
    }
}

void
test_state (base_ptr abs, Options const &options)
{
  CORBA::ValueBase_var const vbase = abs->_to_value ();
  TreeController * const tc = TreeController::_downcast (vbase.in ());
  dump_tree (tc, options);
}

void
test_operation (base_ptr abs, Options const &options)
{
  CORBA::String_var retval = abs->base_op ("base_op");

  if (options.debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  retval.in ()));
    }

  CORBA::Object_var const obj = abs->_to_object ();

  foo_var const concrete = foo::_narrow (obj.in ());

  retval = concrete->foo_op ("foo_op");

  if (options.debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  retval.in ()));
    }

  retval = concrete->base_op ("base_op");

  if (options.debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  retval.in ()));
    }
}

void
test_exception (base_ptr abs, Options const &options)
{
  try
    {
      CORBA::String_var const retval = abs->base_op ("bad_name");

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%C\n",
                      retval.in ()));
        }
    }
  catch (BadInput const &ex)
    {
      if (options.debug)
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
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      Options options;
      if (parse_args (argc, argv, options) != 0)
        {
          return 1;
        }

      CORBA::Object_var const obj =
        orb->string_to_object (options.ior_input_file);

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "string_to_object failed.\n"),
                            -1);
        }

      passer_var const objref = passer::_narrow (obj.in ());

      if (CORBA::is_nil (objref.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "_narrow failed.\n"),
                            -1);
        }

      base_var package = base::_nil ();

      if (options.which_test == Test_Selection::State ||
          options.which_test == Test_Selection::All)
        {
          // Create and register factory for BaseNode.
          CORBA::ValueFactory_var bn_factory;
          ACE_NEW_RETURN (bn_factory.inout (), BaseNode_init, 1);

          orb->register_value_factory (BaseNode::_tao_obv_static_repository_id (),
                                       bn_factory.in ());

          // Create and register factory for TreeController.
          CORBA::ValueFactory_var tc_factory;
          ACE_NEW_RETURN (tc_factory.inout (), TreeController_init, 1);

          orb->register_value_factory (TreeController::_tao_obv_static_repository_id (),
                                       tc_factory.in ());

          // Create and register factory for StringNode.
          CORBA::ValueFactory_var sn_factory;
          ACE_NEW_RETURN (sn_factory.inout (), StringNode_init, 1);

          orb->register_value_factory (StringNode::_tao_obv_static_repository_id (),
                                       sn_factory.in ());

          objref->pass_state (package.out ());

          if (CORBA::is_nil (package.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "pass_state() returned null OUT arg\n"),
                                -1);
            }

          test_state (package.in (), options);
        }

      if (options.which_test != Test_Selection::State)
        {
          objref->pass_ops (package.out ());

          if (CORBA::is_nil (package.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "pass_ops() returned null OUT arg\n"),
                                -1);
            }
        }

      if (options.which_test == Test_Selection::Operation ||
          options.which_test == Test_Selection::All)
        {
          test_operation (package.in (), options);
        }

      if (options.which_test == Test_Selection::Exception ||
          options.which_test == Test_Selection::All)
        {
          test_exception (package.in (), options);
        }

      if (options.which_test == Test_Selection::All)
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
