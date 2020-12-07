// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Bug_3332_Regression_Test.cpp
//
// = DESCRIPTION
//    This is a test to verify the node unbind/remove stability.
//    Originally the deletion of a node from the rb_tree would cause some nodes
//    to be "swapped" and the node being deleted to be reused to hold data still
//    in the rb_tree, whilst the original "still in the tree" data node would be
//    unbound instead. This caused externally held pointers to existing data that
//    should still be in the tree to become invalidated unnecessarily. This test
//    ensures that the node NOT being deleted always keep their original enteries
//    (and thus only the deleted node is actually unbound and released).
//
// = AUTHOR
//    Simon Massey <sma@prismtech.com>
//
// ============================================================================

#include "test_config.h" /* Include first to enable ACE_TEST_ASSERT. */
#include "ace/RB_Tree.h"
#include "ace/Null_Mutex.h"

namespace
{
  struct Nodes
  {
    char key;
    int  value;
    ACE_RB_Tree_Node<char, int> *node;
  } nodes[] = {{'a',  1, nullptr},
               {'f',  6, nullptr},
               {'c',  3, nullptr},
               {'e',  5, nullptr},
               {'g',  7, nullptr},
               {'b',  2, nullptr},
               {'h',  8, nullptr},
               {'d',  4, nullptr},
               {'i',  9, nullptr},
               {'l', 12, nullptr},
               {'j', 10, nullptr},
               {'k', 11, nullptr},
               {'m', 13, nullptr}};
}

int
run_main (int, ACE_TCHAR *[])
{
  int result = 0;
  ACE_START_TEST (ACE_TEXT ("Bug_3332_Regression_Test"));

  ACE_RB_Tree<char, int, ACE_Less_Than<char>, ACE_Null_Mutex> tree;

  ACE_DEBUG ((LM_DEBUG, "Creating Tree\n"));
  {for (size_t i= 0u; i < sizeof(nodes)/sizeof(Nodes); ++i)
  {
    if (0 != tree.bind (nodes[i].key, nodes[i].value, nodes[i].node))
    {
      ACE_DEBUG ((LM_ERROR, ": Binding failed for %c=%d\n", nodes[i].key, nodes[i].value));
      result = 1;
    }
  }}

  if (!result)
  {
    ACE_DEBUG ((LM_DEBUG, "Validating Tree\n"));
    {for (size_t i= 0u; i < sizeof(nodes)/sizeof(Nodes); ++i)
    {
      ACE_RB_Tree_Node<char, int> *node = nullptr;

      if (tree.find (nodes[i].key, node))
      {
        ACE_DEBUG ((LM_ERROR, ": Can't find key %c in tree\n", nodes[i].key));
        result = 2;
      }
      else if (nodes[i].node != node)
      {
        ACE_DEBUG ((LM_ERROR, ": entry %c=%d has ALREADY moved from %@ to %@\n", nodes[i].key, nodes[i].value, nodes[i].node, node));
        result = 3;
      }
      else for (size_t j= i+1u; j < sizeof(nodes)/sizeof(Nodes); ++j)
      {
        if (nodes[i].node == nodes[j].node)
        {
          ACE_DEBUG ((LM_ERROR, ": Duplicate Binding (%@) for %c=%d and %c=%d\n", nodes[i].node, nodes[i].key, nodes[i].value, nodes[j].key, nodes[j].value));
          result = 4;
       }
      }
    }}

    if (!result)
    {
      {for (size_t i= 0u; i < sizeof(nodes)/sizeof(Nodes); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "Deleting node %c=%d @ %@\n", nodes[i].key, nodes[i].value, nodes[i].node));
        if (tree.unbind (nodes[i].key))
        {
          ACE_DEBUG ((LM_ERROR, ": Can't unbind key %c from tree\n", nodes[i].key));
          result = 5;
        }
        else
        {
          ACE_RB_Tree_Node<char, int> *node = nullptr;

          if (0 == tree.find (nodes[i].key, node))
          {
            ACE_DEBUG ((LM_ERROR, ": Found %c=%d @ %@ in tree\n", node->key(), node->item(), node));
            result = 6;
          }
          else {for (size_t j= i+1u; j < sizeof(nodes)/sizeof(Nodes); ++j)
          {
            if (tree.find (nodes[j].key, node))
            {
              ACE_DEBUG ((LM_ERROR, ": Can't find key %c in tree\n", nodes[j].key));
              result = 7;
            }
            else if (nodes[j].node != node)
            {
              ACE_DEBUG ((LM_ERROR, ": entry %c=%d has moved from %@ to %@\n", nodes[j].key, nodes[j].value, nodes[j].node, node));
              nodes[j].node = node;  // Don't keep reporting this same issue on further deletions.
              result = 8;
            }
            else if (nodes[j].key != node->key() || nodes[j].value != node->item())
            {
              ACE_DEBUG ((LM_ERROR, ": entry %c=%d has changed to %c=%d\n", nodes[j].key, nodes[j].value, node->key(), node->item()));
              nodes[j].key= node->key(); // Don't keep reporting this problem on further deletions.
              nodes[j].value = node->item();
              result = 9;
            }
          }}
        }
      }}
    }
  }

  ACE_DEBUG ((LM_DEBUG, "Finished\n"));
  ACE_END_TEST;
  return result;
}
