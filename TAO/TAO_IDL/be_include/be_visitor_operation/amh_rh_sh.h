//=============================================================================
/**
*  @file   amh_rh_sh.h
*
*  $Id$
*
*  Creates operation code for AMH-RH operations.
*
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*/
//=============================================================================

#ifndef AMH_RH_OPERATION_SH_H
#define AMH_RH_OPERATION_SH_H

/**
* @class be_visitor_operation_amh_rh_sh
*
* @brief This is a concrete visitor to generate the server header for
*        AMH operations
*/

#include "be_visitor_operation.h"

class be_visitor_amh_rh_operation_sh : public be_visitor_operation
{
 public:
    be_visitor_amh_rh_operation_sh (be_visitor_context *ctx);

    ~be_visitor_amh_rh_operation_sh (void);

    int visit_operation (be_operation *node);

};

#endif /* AMH_RH_OPERATION_SH_H */
