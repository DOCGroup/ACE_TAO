/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    servant_svh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation for the Servant
 *  class declaration
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_SERVANT_SVH_H_
#define _BE_COMPONENT_SERVANT_SVH_H_

/**
 * @class be_visitor_servant_svh
 *
 * @brief be_visitor_servant_svh
 *
 * This is a concrete visitor to generate the servant class decl
 * for a component.
 */
class be_visitor_servant_svh : public be_visitor_component_scope
{
public:
  be_visitor_servant_svh (be_visitor_context *ctx);

  ~be_visitor_servant_svh (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_consumes (be_consumes *node);

private:
  /// Base class overrides and CIAO-specific code.
  void gen_non_type_specific (void);
};

#endif /* _BE_COMPONENT_SERVANT_SVH_H_ */

