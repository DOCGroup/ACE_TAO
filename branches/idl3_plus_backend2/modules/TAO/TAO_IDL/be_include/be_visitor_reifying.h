//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_reifying.h
//
// = DESCRIPTION
//    This visitor visits a be_param_holder, finds the corresponding
//    template argument, looks up or creates the type, and stores
//    it for retrieval and use in the template module
//    instantiation visitor
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef TAO_BE_VISITOR_REIFYING_H
#define TAO_BE_VISITOR_REIFYING_H

#include "be_visitor_decl.h"

#include "utl_scoped_name.h"

class AST_Decl;
class be_param_holder;

class be_visitor_reifying : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_reifying
  //
  // = DESCRIPTION
  //   Reifies a template argument that has been substituted for
  //   a template parameter reference. Complete reification may
  //   include further reification, for example of array
  //   dimensions, or creating a copy of the template argument
  //   node (if it is itself defined inside a template module)
  //   for addition to the scope of the template module
  //   instantiation.
  //
public:
  be_visitor_reifying (be_visitor_context *ctx);

  virtual ~be_visitor_reifying (void);
  
  AST_Decl *reified_node (void) const;

  virtual int visit_interface (be_interface *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_component (be_component *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_array (be_array *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_string (be_string *node);
  virtual int visit_constant (be_constant *node);
  virtual int visit_param_holder (be_param_holder *node);
  
private:
  UTL_ScopedName *template_module_rel_name (AST_Decl *d);
  
private:
  AST_Decl *reified_node_;
};

#endif // TAO_BE_VISITOR_REIFYING_H