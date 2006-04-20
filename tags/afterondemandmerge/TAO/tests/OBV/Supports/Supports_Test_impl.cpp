// $Id$
// FUZZ: disable check_for_streams_include

#include "Supports_Test_impl.h"

#include "ace/streams.h"

/* vt_graph_impl */

vt_graph_impl::vt_graph_impl (void)
{
}

// Creates a vt_graph_impl with the given number of nodes. There will be one
// root node and the rest will be children of it.
vt_graph_impl::vt_graph_impl (int num_nodes ACE_ENV_ARG_DECL)
{
  nodes_ ().length (0);
  add_node ("ROOT" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  for (int i = 1; i < num_nodes; i++)
    {
      add_node ("CHILD" ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      nodes_ ()[0]->add_edge (nodes_ ()[i] ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Get the number of nodes in the vt_graph.
CORBA::Long vt_graph_impl::size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return nodes_ ().length ();
}

// Add a node to the graph with no edges.
void
vt_graph_impl::add_node (const char * name ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Supports_Test::Node * new_node = 0;
  ACE_NEW (new_node, node_impl (name));
  nodes_ ().length (nodes_ ().length () + 1);
  nodes_ ()[nodes_ ().length () - 1] = new_node;
}

// Print out information about each node.
void
vt_graph_impl::print (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Printing graph data... \n"));

  ACE_DEBUG ((LM_DEBUG,
	      "Number of nodes: [%d] \n", nodes_ ().length ()));

  for (size_t i = 0; i < nodes_ ().length (); i++)
    nodes_ ()[i]->print ();
}


/* vt_graph_init_impl - factory operations */

Supports_Test::vt_graph *
vt_graph_init_impl::create (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  vt_graph_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, vt_graph_impl, 0);
  return ret_val;
}

CORBA::ValueBase *
vt_graph_init_impl::create_for_unmarshal (void)
{
  vt_graph_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, vt_graph_impl, 0);
  return ret_val;
}


/* test_impl */

test_impl::test_impl (CORBA::ORB_ptr orb) : orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_impl::~test_impl (void)
{
  this->orb_->shutdown (0);
}

void
test_impl::pass_obj_graph_in (
  Supports_Test::graph * graph_param
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  ACE_ASSERT (graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 3);
  ACE_CHECK;
  graph_param->add_node ("NEW1" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 4);
  ACE_CHECK;
}

void
test_impl::pass_vt_graph_in (
  Supports_Test::vt_graph * vt_graph_param
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  ACE_ASSERT (vt_graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 3);
  ACE_CHECK;
  vt_graph_param->add_node ("NEW1" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (vt_graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 4);
  ACE_CHECK;
}

void
test_impl::pass_obj_graph_out (
  Supports_Test::graph_out graph_param
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  vt_graph_impl * the_vt_graph = 0;
  ACE_NEW (the_vt_graph, vt_graph_impl (4 ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;
  graph_param = the_vt_graph->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 4);
  ACE_CHECK;
  graph_param->add_node ("NEW1" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 5);
  ACE_CHECK;
}

void
test_impl::pass_vt_graph_out (
    Supports_Test::vt_graph_out vt_graph_param
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  vt_graph_impl * the_vt_graph = 0;
  ACE_NEW (the_vt_graph, vt_graph_impl (4 ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;
  vt_graph_param = the_vt_graph;

  ACE_ASSERT (vt_graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 4);
  ACE_CHECK;
  vt_graph_param->add_node ("NEW1" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (vt_graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 5);
  ACE_CHECK;

}

void
test_impl::pass_obj_graph_inout (
    Supports_Test::graph * &graph_param
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_ASSERT (graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 6);
  ACE_CHECK;
  graph_param->add_node ("NEW3" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 7);
  ACE_CHECK;
}

void
test_impl::pass_vt_graph_inout (
    Supports_Test::vt_graph * &vt_graph_param
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_ASSERT (vt_graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 6);
  ACE_CHECK;
  vt_graph_param->add_node ("NEW3" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (vt_graph_param->size (ACE_ENV_SINGLE_ARG_PARAMETER) == 7);
  ACE_CHECK;

}

void
test_impl::start (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
test_impl::finish (ACE_ENV_SINGLE_ARG_DECL) 
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


/* node_impl */

node_impl::node_impl (void)
{
}

// Initialize state.
node_impl::node_impl (const char * name)
{
        name_ (name);
        weight_ (0);
        degree_ (0);
        neighbors_ ().length (0);
}

// Add an edge from this node to neighbor.
void
node_impl::add_edge (Supports_Test::Node * neighbor ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  degree_ (degree_ () + 1);
  neighbors_ ().length (neighbors_ ().length () + 1);
  neighbors_ ()[neighbors_ ().length () - 1] = neighbor;
  neighbor->_add_ref ();
  return;
}

// Remove the edge from this node to neighbor.
void
node_impl::remove_edge (Supports_Test::Node * neighbor ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (unsigned int i = 0; i < neighbors_ ().length (); i++)
    if (neighbors_ ()[i] == neighbor)
      {
        neighbors_ ()[i] = neighbors_ ()[neighbors_ ().length () - 1];
        neighbors_ ().length (neighbors_ ().length () - 1);
        neighbor->_remove_ref ();
      }
}

void
node_impl::change_weight (CORBA::Long new_weight ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  weight_ (new_weight);
}

void
node_impl::print (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  cout << "  Name: " << name_ () << endl;
  cout << "    Weight: " << weight_ () << endl;
  cout << "    Degree: " << degree_ () << endl;
  cout << "    Neighbors: " << endl;
  for (size_t i = 0; i < neighbors_ ().length (); i++)
    cout << "      " << neighbors_ ()[i]->name_ () << endl;
}

/* node_init_impl - factory operations */

Supports_Test::Node *
node_init_impl::create (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  node_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, node_impl, 0);
  return ret_val;
}

CORBA::ValueBase *
node_init_impl::create_for_unmarshal (void)
{
  node_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, node_impl, 0);
  return ret_val;
}
