//
// $Id$
//

#include "Client_Task.h"

Client_Task::Client_Task (const char *ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))

{
}

int
Client_Task::svc (void)
{
  int result = 0;
  try
    {
      // All factories are kindly provided by
      // compiler so we just to put everything in a right order.

      // Create and register factory for BaseNode.
      BaseNode_init *bn_factory = 0;
      ACE_NEW_RETURN (bn_factory,
                      BaseNode_init,
                      1);

      this->corb_->register_value_factory (bn_factory->tao_repository_id (),
                                   bn_factory);
      bn_factory->_remove_ref (); // release ownership

      // Create and register factory for TreeController.
      TreeController_init *tc_factory = 0;
      ACE_NEW_RETURN (tc_factory,
                      TreeController_init,
                      1);

      this->corb_->register_value_factory (tc_factory->tao_repository_id (),
                                   tc_factory);
      tc_factory->_remove_ref (); // release ownership

      // Create and register factory for StringNode.
      StringNode_init *sn_factory = 0;
      ACE_NEW_RETURN (sn_factory,
                      StringNode_init,
                      1);

      this->corb_->register_value_factory (sn_factory->tao_repository_id (),
                                   sn_factory);
      sn_factory->_remove_ref (); // release ownership

      //Well, done with factories.

      // Obtain reference to the object.
      CORBA::Object_var tmp =
        this->corb_->string_to_object(this->input_);

      Test_var test = Test::_narrow(tmp.in ());

      if (CORBA::is_nil (test.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil Test reference <%s>\n",
                           this->input_),
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

      // Make copy

      TreeController_var tc_copy;
      ACE_NEW_RETURN (tc_copy.inout (),
                      OBV_TreeController,
                      1);

      // Create the root node.
      {
        StringNode_var sn;
        ACE_NEW_RETURN (sn.inout (),
                        OBV_StringNode,
                        1);
        sn->name ((const char*)("RootNode"));
        tc_copy->root (sn);

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

      if (!is_equal_tree (tc.in (), tc_copy.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: tc != tc_copy\n"));
          result = 1;
        }

      TreeController_var tc_result =
        test->reflect (tc.in ());

      // The following two ifs will fail until bug 1390 is fixed.
      if (is_equal_tree (tc.in (), tc_result.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: tc == tc_result\n"));
          result = 1;
        }
      if (result == 0 && !is_equal_tree (tc.in (), tc_copy.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: tc != tc_copy\n"));
          result = 1;
        }

      test->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return result;

}

int
Client_Task::is_equal_tree (TreeController * tree1, TreeController * tree2)
{
  return is_equal_node (tree1->root (), tree2->root ());
}


int
Client_Task::is_equal_node (BaseNode * node1, BaseNode * node2)
{
  int ret_val = 0;
  if (!(node1 || node2))
    return 1;
  if (node1 && node2)
    {
      StringNode * sn1 = StringNode::_downcast (node1);
      StringNode * sn2 = StringNode::_downcast (node2);
      if (sn1 && sn2)
        if (!ACE_OS::strcmp (sn1->name (), sn2->name ()))
          ret_val = 1;
      if (!(sn1 || sn2))
        ret_val = 1;
      return (ret_val &&
              is_equal_node (node1->left (), node2->left ()) &&
              is_equal_node (node1->right (), node2->right ()));
    }

  return 0;
}

void
Client_Task::dump_node (BaseNode *bn, int indent)
{
  if (bn == 0) return;

  // This is ugly I know
  int i = indent;
  for (; i != 0; i--) ACE_DEBUG ((LM_DEBUG, "  "));

  StringNode *sn = StringNode::_downcast (bn);
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
Client_Task::dump_tree (TreeController *tc)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) start tree dump <%x>\n",
              tc));

  dump_node (tc->root (), 1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) end tree dump <%x>\n",
              tc));
}
