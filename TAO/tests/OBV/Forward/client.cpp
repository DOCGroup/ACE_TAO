// $Id$


#include "TreeBaseC.h"
#include "TreeControllerC.h"
#include "TreeNodeC.h"

#include "ace/Get_Opt.h"

ACE_RCSID(Forward, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}

void 
dump_node (BaseNode_ptr bn, int indent)
{
  if (bn == 0) return;

  // This is ugly I know
  int i = indent;
  for (; i != 0; i--) ACE_DEBUG ((LM_DEBUG, "  "));

  StringNode_ptr sn = StringNode::_downcast (bn);
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

  dump_node (bn->left (), indent + 1);
  dump_node (bn->right (), indent + 1);
}

void 
dump_tree (TreeController_ptr tc)
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
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

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
                                   bn_factory, 
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      bn_factory->_remove_ref (); // release ownership

      // Create and register factory for TreeController.
      TreeController_init *tc_factory = 0; 
      ACE_NEW_RETURN (tc_factory, 
                      TreeController_init,
                      1);

      orb->register_value_factory (tc_factory->tao_repository_id (),
                                   tc_factory, 
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      tc_factory->_remove_ref (); // release ownership

      // Create and register factory for StringNode.
      StringNode_init *sn_factory = 0; 
      ACE_NEW_RETURN (sn_factory, 
                      StringNode_init,
                      1);

      orb->register_value_factory (sn_factory->tao_repository_id (),
                                   sn_factory, 
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      sn_factory->_remove_ref (); // release ownership

      //Well, done with factories.

      // Obtain reference to the object.
      CORBA::Object_var tmp =
        orb->string_to_object(ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var test = Test::_narrow(tmp.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (test.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil Test reference <%s>\n",
                           ior),
                          1);
      }

      // Now build simple graph (tree in our case).
      
      TreeController_var tc;
      ACE_NEW_RETURN (tc, 
                      OBV_TreeController,
                      1);

      // Create the root node.
      {
        StringNode_var sn;
        ACE_NEW_RETURN (sn, 
                        OBV_StringNode,
                        1);
        sn->name ((const char*)("RootNode"));
        tc->root (sn);

        // Create the left leaf.
        {
          StringNode_var dummy;
          ACE_NEW_RETURN (dummy, 
                          OBV_StringNode,
                          1);
          dummy->name ((const char*)("LeftNode"));
          sn->left (dummy);          
        }

        // Create the right leaf.
        {
          StringNode_var dummy;
          ACE_NEW_RETURN (dummy, 
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
        test->reflect (tc.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Dump the resulting tree.
      dump_tree (result_tc.in ());

      test->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished\n"));

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
