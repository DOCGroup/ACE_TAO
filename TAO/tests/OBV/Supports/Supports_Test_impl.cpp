// $Id$

#include "Supports_Test_impl.h"


/* vt_graph_impl */

vt_graph_impl::vt_graph_impl (void)
{
}

vt_graph_impl::vt_graph_impl (int num_nodes)
{
	nodes_ ().length (0);
	for (int i = 0; i < num_nodes; i++)
		{
			add_node ("OLD");
			if (i != 0) nodes_ ()[i]->add_edge (nodes_ ()[0]);
		}
}

CORBA::Long vt_graph_impl::size (void)
{
	return nodes_ ().length ();
}

void vt_graph_impl::add_node (const char * name)
{
	Supports_Test::Node * new_node = 0;
  ACE_NEW (new_node, node_impl (name));
	nodes_ ().length (nodes_ ().length () + 1);
	nodes_ ()[nodes_ ().length () - 1] = new_node;
}

void vt_graph_impl::print (void)
{
  for (int i = 0; i < nodes_ ().length (); i++)
    {
      nodes_ ()[i]->print ();
    }

  cout << endl;
}



/* vt_graph_init_impl */

Supports_Test::vt_graph * vt_graph_init_impl::create (void)
{
	vt_graph_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, vt_graph_impl, 0);
  return ret_val;
}

CORBA::ValueBase * vt_graph_init_impl::create_for_unmarshal (void)
{
	vt_graph_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, vt_graph_impl, 0);
  return ret_val;
}


/* test_impl */

test_impl::test_impl (CORBA::ORB_ptr orb) : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void test_impl::pass_obj_graph_in (Supports_Test::graph * graph_param ACE_ENV_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_ASSERT (graph_param->size () == 4);
	graph_param->add_node ("NEW1");
	ACE_ASSERT (graph_param->size () == 5);

}

void test_impl::pass_vt_graph_in (Supports_Test::vt_graph * vt_graph_param ACE_ENV_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException))
{

	ACE_ASSERT (vt_graph_param->size () == 3);
	vt_graph_param->add_node ("NEW1");
	ACE_ASSERT (vt_graph_param->size () == 4);

}

void test_impl::pass_obj_graph_out (Supports_Test::graph_out graph_param ACE_ENV_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException))
{

  vt_graph_impl * the_vt_graph = 0;
  ACE_NEW (the_vt_graph, vt_graph_impl (4));
  graph_param = the_vt_graph->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
	ACE_TRY_CHECK;

  ACE_ASSERT (graph_param->size () == 4);
	graph_param->add_node ("NEW1");
	ACE_ASSERT (graph_param->size () == 5);

}

void test_impl::pass_vt_graph_out (Supports_Test::vt_graph_out vt_graph_param ACE_ENV_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException))
{

  vt_graph_impl * the_vt_graph = 0;
	ACE_NEW (the_vt_graph, vt_graph_impl (3));
	vt_graph_param = the_vt_graph;

	ACE_ASSERT (vt_graph_param->size () == 3);
	vt_graph_param->add_node ("NEW1");
	ACE_ASSERT (vt_graph_param->size () == 4);

}

void test_impl::pass_obj_graph_inout (Supports_Test::graph * &graph_param ACE_ENV_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_ASSERT (graph_param->size () == 6);
	graph_param->add_node ("NEW!");
	ACE_ASSERT (graph_param->size () == 7);

}

void test_impl::pass_vt_graph_inout (Supports_Test::vt_graph * &vt_graph_param ACE_ENV_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException))
{

	ACE_ASSERT (vt_graph_param->size () == 5);
	vt_graph_param->add_node ("NEW!");
	ACE_ASSERT (vt_graph_param->size () == 6);

}

void test_impl::shutdown (ACE_ENV_SINGLE_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException))
{
	this->orb_->shutdown ();
}


/* node_impl */

node_impl::node_impl (void)
{
}

node_impl::node_impl (const char * name)
{
	name_ (name);
	weight_ (0);
	degree_ (0);
	neighbors_ ().length (0);
}

void node_impl::add_edge (Supports_Test::Node * neighbor)
{
	degree_ (degree_ () + 1);
	neighbors_ ().length (neighbors_ ().length () + 1);
	neighbors_ ()[neighbors_ ().length () - 1] = neighbor;
  neighbor->_add_ref ();
	return;
}
	
void node_impl::remove_edge (Supports_Test::Node * neighbor)
{
	
}

void node_impl::change_weight (CORBA::Long new_weight)
{
	weight_ (new_weight);
}

void node_impl::print (void)
{
  cout << "Name: " << name_ () << endl;
  cout << "Weight: " << weight_ () << endl;
  cout << "Degree: " << degree_ () << endl;
  cout << "Neighbors: " << endl;
  for (int i = 0; i < neighbors_ ().length (); i++)
    cout << "   " << neighbors_ ()[i]->name_ () << endl;
}

/* node_init_impl */

Supports_Test::Node * node_init_impl::create (void)
{
	node_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, node_impl, 0);
  return ret_val;
}

CORBA::ValueBase * node_init_impl::create_for_unmarshal (void)
{
	node_impl * ret_val = 0;
  ACE_NEW_RETURN (ret_val, node_impl, 0);
  return ret_val;
}
