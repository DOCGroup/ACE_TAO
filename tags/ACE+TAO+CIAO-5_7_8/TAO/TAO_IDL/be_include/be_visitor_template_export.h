
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_template_export.h
 *
 *  $Id$
 *
 *  This visitor generates an explicit export of a sequence's
 *  template base class. It is conditionally launched by a
 *  command line option and is motivated by a bug in Visual
 *  Studio C++ (.NET 2002, .NET 2003, and Express 2005), documented in
 *  http://support.microsoft.com/default.aspx?scid=kb;en-us;309801
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_BE_VISITOR_TEMPLATE_EXPORT_H
#define TAO_BE_VISITOR_TEMPLATE_EXPORT_H

#include "be_visitor_scope.h"

class AST_Interface;

/**
 * @class :
 *
 * @brief be_visitor_template_export
 *
 * Generates sequence template base class export instantiation.
 */
class be_visitor_template_export : public be_visitor_scope
{
public:
  be_visitor_template_export (be_visitor_context *ctx);

  virtual ~be_visitor_template_export (void);

  virtual int visit_root (be_root *node);

  virtual int visit_module (be_module *node);

  virtual int visit_typedef (be_typedef *node);

  virtual int visit_sequence (be_sequence *node);
};


#endif // TAO_BE_VISITOR_TEMPLATE_EXPORT_H
