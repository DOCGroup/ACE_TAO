//=============================================================================
/**
 *  @file   amh_sh.h
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
  explicit be_visitor_amh_interface_sh (be_visitor_context *ctx);
  ~be_visitor_amh_interface_sh () override;
  be_visitor_amh_interface_sh (be_visitor_amh_interface_sh const &) = delete;
  be_visitor_amh_interface_sh (be_visitor_amh_interface_sh &&) = delete;
  be_visitor_amh_interface_sh &operator= (be_visitor_amh_interface_sh const &) = delete;
  be_visitor_amh_interface_sh &operator= (be_visitor_amh_interface_sh &&) = delete;

  int visit_interface (be_interface *node) override;
  int visit_operation (be_operation *node) override;
  int visit_attribute (be_attribute *node) override;

protected:
  void this_method (be_interface *node) override;

private:
};

#endif /* AMH_SH_H_ */
