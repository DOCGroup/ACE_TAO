//=============================================================================
/**
*  @file   amh_ss.h
*
*  $Id$
*
*  Creates code for AMH and AMH-RH operations.
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

 protected:
    virtual int demarshal_params (be_operation *node);
};

#endif /* AMH_OPERATION_SS_H */


#ifndef AMH_RH_OPERATION_SS_H
#define AMH_RH_OPERATION_SS_H

/**
 * @class be_visitor_operation_amh_rh_ss
 *
 * @brief This is a concrete visitor to generate the server source for
 *        AMH operations
 */
class be_visitor_amh_rh_operation_ss : public be_visitor_operation
{
 public:
    be_visitor_amh_rh_operation_ss (be_visitor_context *ctx);
    
    ~be_visitor_amh_rh_operation_ss (void);
    
    virtual int visit_operation (be_operation *node);
    
};

#endif /* AMH_RH_OPERATION_SS_H */
