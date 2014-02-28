
//=============================================================================
/**
 *  @file    connector.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Connector.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "ast_template_module.h"
#include "ast_template_module_inst.h"
#include "ast_uses.h"
#include "ast_module.h"
#include "ast_extended_port.h"

#include "utl_identifier.h"

#include "nr_extern.h"

#include "be_connector.h"
#include "be_mirror_port.h"
#include "be_provides.h"
#include "be_uses.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_attribute.h"

#include "be_visitor_connector.h"
#include "be_visitor_operation.h"
#include "be_visitor_attribute.h"
#include "be_visitor_context.h"
