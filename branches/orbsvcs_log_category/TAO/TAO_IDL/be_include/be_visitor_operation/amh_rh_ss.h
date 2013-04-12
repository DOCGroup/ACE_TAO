//=============================================================================
/**
*  @file   amh_rh_ss.h
*
*  $Id$
*
*  Creates code for AMH-RH operations.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#ifndef AMH_RH_OPERATION_SS_H
#define AMH_RH_OPERATION_SS_H

/**
 * @class be_visitor_operation_amh_rh_ss
 *
 * @brief This is a concrete visitor to generate the server source for
 *        AMH operations
 */

#include "be_visitor_operation.h"

class be_visitor_amh_rh_operation_ss : public be_visitor_operation
{
 public:
    be_visitor_amh_rh_operation_ss (be_visitor_context *ctx);

    ~be_visitor_amh_rh_operation_ss (void);

    int visit_operation (be_operation *node);

 private:
    int marshal_params (be_operation *node);
};

#endif /* AMH_RH_OPERATION_SS_H */
