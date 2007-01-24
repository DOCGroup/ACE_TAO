// $Id$

#ifndef TAO_SUPPORTS_TEST_IMPL_H
#define TAO_SUPPORTS_TEST_IMPL_H

#include "Supports_TestS.h"
#include "ace/Get_Opt.h"

/**
 * \class node_impl
 *
 *
 */
class node_impl :
  public virtual OBV_Supports_Test::Node,
  public virtual CORBA::DefaultValueRefCountBase
{

public:

  node_impl (void);

  node_impl (const char * name);

  virtual void add_edge (Supports_Test::Node * neighbor ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void remove_edge (Supports_Test::Node * neighbor ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void change_weight (CORBA::Long new_weight ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void print (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

class node_init_impl : public Supports_Test::Node_init
{

 public:

  virtual Supports_Test::Node * create (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ValueBase * create_for_unmarshal (void);

};


class vt_graph_impl :
  public virtual OBV_Supports_Test::vt_graph,
  public virtual POA_Supports_Test::vt_graph,
  public virtual CORBA::DefaultValueRefCountBase
{

 public:

  vt_graph_impl (void);

  vt_graph_impl (int num_nodes
                 ACE_ENV_ARG_DECL);

  virtual CORBA::Long size (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void add_node (const char * name
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void print (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

class vt_graph_init_impl :
  public Supports_Test::vt_graph_init
{

 public:

  virtual Supports_Test::vt_graph * create (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ValueBase * create_for_unmarshal (void);

};

class test_impl :
  public virtual POA_Supports_Test::test
{

 public:

  test_impl (CORBA::ORB_ptr orb);

  virtual ~test_impl (void);

  virtual void pass_obj_graph_in (Supports_Test::graph * graph_param
				  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void pass_vt_graph_in (Supports_Test::vt_graph * vt_graph_param
				 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void pass_obj_graph_out (Supports_Test::graph_out graph_param
				   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void pass_vt_graph_out (Supports_Test::vt_graph_out vt_graph_param
				  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void pass_obj_graph_inout (Supports_Test::graph * &graph_param
				     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void pass_vt_graph_inout (Supports_Test::vt_graph * &vt_graph_param
				    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void start (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void finish (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:

  CORBA::ORB_var orb_;

};

#endif /* TAO_SUPPORTS_TEST_IMPL_H */
