/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component.h
//
// = DESCRIPTION
//    Concrete visitor for components
//    This one provides the generic visitor for the be_component node.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_COMPONENT_COMPONENT_H_
#define _BE_VISITOR_COMPONENT_COMPONENT_H_

class be_visitor_component : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_component
  //
  // = DESCRIPTION
  //   This is a concrete visitor for component 
  //   that abstracts all common tasks.
  //

public:
  be_visitor_component (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component (void);
  // destructor

  virtual int visit_attribute (be_attribute *node);
  // This is the only type of declaration a component may contain.

  virtual int visit_operation (be_operation *node);
  // Operations are created by the back end for 'provides', 'uses', 'emits',
  // 'publishes' and 'consumes' declarations.

  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);
  // Structs and sequences are created by the back end for 'uses multiple'
  // declarations.
};

#endif /* _BE_VISITOR_MODULE_MODULE_H_ */
