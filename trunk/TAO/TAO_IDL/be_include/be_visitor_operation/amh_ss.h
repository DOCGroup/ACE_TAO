//=============================================================================
/**
*  @file   amh_ss.h
*
*  $Id$
*
*  Creates code for AMH operations.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#ifndef AMH_OPERATION_SS_H
#define AMH_OPERATION_SS_H

/**
 * @class be_visitor_operation_amh_ss
 *
 * @brief This is a concrete visitor to generate the server source for
 *        AMH operations
 */
class be_visitor_amh_operation_ss : public be_visitor_operation
{
public:
  be_visitor_amh_operation_ss (be_visitor_context *ctx);

  ~be_visitor_amh_operation_ss (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);

protected:
  int generate_shared_prologue (be_decl *node,
                                TAO_OutStream *os,
                                const char *skel_prefix);
  int generate_shared_section (be_decl *node,
                               TAO_OutStream *os,
                               int argument_count);
  int generate_shared_epilogue (TAO_OutStream *os);
};

#endif /* AMH_OPERATION_SS_H */
