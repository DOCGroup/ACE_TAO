//=============================================================================
/**
 *  @file   amh_sh.h
 *
 *  $Id$
 *
 *  Specialized interface visitor for AMH
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef AMH_SH_H_
#define AMH_SH_H_

class be_visitor_amh_interface_sh : public be_visitor_interface_sh
{
 public:
    be_visitor_amh_interface_sh (be_visitor_context *ctx);

    int visit_interface (be_interface *node);
  int visit_operation (be_operation *node);
  int visit_attribute (be_attribute *node);

    ~be_visitor_amh_interface_sh (void);

 protected:
    virtual void this_method (be_interface *node);

 private:
    /// Create an AMH node 'on the fly' from the
    /// original interface node
    be_interface* create_amh_class (ACE_CString name);

    /// Add attributes and operations from original-interface
    /// into the newly created AMH-node
    int add_original_members (be_interface *node,
                              be_interface *amh_node);

    /// For every operation in the original-interface, create
    /// an AMH-operation and add it to the newly created AMH-node
    int add_amh_operation (be_operation *orig_operation,
                           be_interface *amh_node);
};

#endif /* AMH_SH_H_ */
