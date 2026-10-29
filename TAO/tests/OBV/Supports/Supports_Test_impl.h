#ifndef TAO_SUPPORTS_TEST_IMPL_H
#define TAO_SUPPORTS_TEST_IMPL_H

#include "Supports_TestS.h"
#include "ace/Get_Opt.h"

/**
 * \class node_impl
 */
class node_impl :
  public virtual OBV_Supports_Test::Node,
  public virtual CORBA::DefaultValueRefCountBase
{
public:
  node_impl () = default;

  explicit node_impl (char const *name);

  node_impl (node_impl const&) = delete;
  node_impl (node_impl&&) = delete;
  node_impl& operator= (node_impl const&) = delete;
  node_impl& operator= (node_impl&&) = delete;

  void add_edge (Supports_Test::Node *neighbor) override;

  void remove_edge (Supports_Test::Node *neighbor) override;

  void change_weight (CORBA::Long new_weight) override;

  void print () override;
};

class node_init_impl : public Supports_Test::Node_init
{
 public:
  node_init_impl () = default;

  node_init_impl (node_init_impl const&) = delete;
  node_init_impl (node_init_impl&&) = delete;
  node_init_impl& operator= (node_init_impl const&) = delete;
  node_init_impl& operator= (node_init_impl&&) = delete;

  Supports_Test::Node *create () override;

  CORBA::ValueBase *create_for_unmarshal () override;
};


class vt_graph_impl :
  public virtual OBV_Supports_Test::vt_graph,
  public virtual POA_Supports_Test::vt_graph,
  public virtual CORBA::DefaultValueRefCountBase
{
 public:
  vt_graph_impl () = default;

  explicit vt_graph_impl (int num_nodes);

  vt_graph_impl (vt_graph_impl const&) = delete;
  vt_graph_impl (vt_graph_impl&&) = delete;
  vt_graph_impl& operator= (vt_graph_impl const&) = delete;
  vt_graph_impl& operator= (vt_graph_impl&&) = delete;

  CORBA::Long size () override;

  void add_node (char const *name) override;

  void print () override;
};

class vt_graph_init_impl :
  public Supports_Test::vt_graph_init
{
 public:
  vt_graph_init_impl () = default;

  vt_graph_init_impl (vt_graph_init_impl const&) = delete;
  vt_graph_init_impl (vt_graph_init_impl&&) = delete;
  vt_graph_init_impl& operator= (vt_graph_init_impl const&) = delete;
  vt_graph_init_impl& operator= (vt_graph_init_impl&&) = delete;

  Supports_Test::vt_graph *create () override;

  CORBA::ValueBase *create_for_unmarshal () override;
};

class test_impl :
  public virtual POA_Supports_Test::test
{
 public:
  explicit test_impl (CORBA::ORB_ptr orb);

  ~test_impl () override = default;

  test_impl (test_impl const&) = delete;
  test_impl (test_impl&&) = delete;
  test_impl& operator= (test_impl const&) = delete;
  test_impl& operator= (test_impl&&) = delete;

  void pass_obj_graph_in (Supports_Test::graph *graph_param) override;

  void pass_vt_graph_in (Supports_Test::vt_graph *vt_graph_param) override;

  void pass_obj_graph_out (Supports_Test::graph_out graph_param) override;

  void pass_vt_graph_out (Supports_Test::vt_graph_out vt_graph_param) override;

  void pass_obj_graph_inout (Supports_Test::graph *&graph_param) override;

  void pass_vt_graph_inout (Supports_Test::vt_graph *&vt_graph_param) override;

  void start () override;

  void finish () override;

 private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_SUPPORTS_TEST_IMPL_H */
