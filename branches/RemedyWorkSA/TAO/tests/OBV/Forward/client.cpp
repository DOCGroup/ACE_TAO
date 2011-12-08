// $Id$

#include "TreeBaseC.h"
#include "TreeControllerC.h"
#include "TreeNodeC.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

void
dump_node (BaseNode *bn, int indent)
{
  if (bn == 0) return;

  // This is ugly I know
  int i = indent;
  for (; i != 0; i--) ACE_DEBUG ((LM_DEBUG, "  "));

  StringNode *sn = StringNode::_downcast (bn);
  if (sn != 0)
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

  dump_node (bn->left (), indent + 1);
  dump_node (bn->right (), indent + 1);
}

void
dump_tree (TreeController *tc)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) start tree dump <%x>\n",
              tc));

  dump_node (tc->root (), 1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) end tree dump <%x>\n",
              tc));
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // All factories are kindly provided by
      // compiler so we just to put everything in a right order.

      // Create and register factory for BaseNode.
      BaseNode_init *bn_factory = 0;
      ACE_NEW_RETURN (bn_factory,
                      BaseNode_init,
                      1);

      orb->register_value_factory (bn_factory->tao_repository_id (),
                                   bn_factory);
      bn_factory->_remove_ref (); // release ownership

      // Create and register factory for TreeController.
      TreeController_init *tc_factory = 0;
      ACE_NEW_RETURN (tc_factory,
                      TreeController_init,
                      1);

      orb->register_value_factory (tc_factory->tao_repository_id (),
                                   tc_factory);
      tc_factory->_remove_ref (); // release ownership

      // Create and register factory for StringNode.
      StringNode_init *sn_factory = 0;
      ACE_NEW_RETURN (sn_factory,
                      StringNode_init,
                      1);

      orb->register_value_factory (sn_factory->tao_repository_id (),
                                   sn_factory);
      sn_factory->_remove_ref (); // release ownership

      //Well, done with factories.

      // Obtain reference to the object.
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test_var test = Test::_narrow(tmp.in ());

      if (CORBA::is_nil (test.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil Test reference <%s>\n",
                           ior),
                          1);
      }

      // Now build simple graph (tree in our case).

      TreeController_var tc;
      ACE_NEW_RETURN (tc.inout (),
                      OBV_TreeController,
                      1);

      // Create the root node.
      {
        StringNode_var sn;
        ACE_NEW_RETURN (sn.inout (),
                        OBV_StringNode,
                        1);
        sn->name ((const char*)("RootNode"));
        tc->root (sn);

        // Create the left leaf.
        {
          StringNode_var dummy;
          ACE_NEW_RETURN (dummy.inout (),
                          OBV_StringNode,
                          1);
          dummy->name ((const char*)("LeftNode"));
          sn->left (dummy);
        }

        // Create the right leaf.
        {
          StringNode_var dummy;
          ACE_NEW_RETURN (dummy.inout (),
                          OBV_StringNode,
                          1);
          dummy->name ((const char*)("RightNode"));
          sn->right (dummy);

          // dummy->right (sn); // ;-)
        }
      }

      // Dump the tree.
      dump_tree (tc.in ());

      TreeController_var result_tc =
        test->reflect (tc.in ());

      // Dump the resulting tree.
      dump_tree (result_tc.in ());

      test->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished\n"));

      orb->destroy ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
