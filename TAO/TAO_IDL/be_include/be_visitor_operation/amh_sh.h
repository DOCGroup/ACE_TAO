//=============================================================================
/**
*  @file   amh_sh.h
*
*  $Id$
*
*  Creates code for AMH operations.
*
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*/
//=============================================================================

#ifndef AMH_OPERATION_SH_H
#define AMH_OPERATION_SH_H

/**
* @class be_visitor_operation_amh_sh
*
* @brief This is a concrete visitor to generate the server header for
*        AMH operations
*/
class be_visitor_amh_operation_sh : public be_visitor_operation
{
public:
  be_visitor_amh_operation_sh (be_visitor_context *ctx);
  ~be_visitor_amh_operation_sh (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);

private:
  void generate_shared_prolog (be_decl *node,
                               TAO_OutStream *os,
                               const char *skel_prefix);
};

#endif /* AMH_OPERATION_SS_H */
