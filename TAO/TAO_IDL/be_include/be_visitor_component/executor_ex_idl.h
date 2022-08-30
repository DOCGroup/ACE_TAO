/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    executor_ex_idl.h
 *
 *  Provides code generation for components in the CIAO executor IDL.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_EXECUTOR_EX_IDL_H_
#define _BE_COMPONENT_EXECUTOR_EX_IDL_H_

/**
 * This is a concrete visitor to generate the
 * CIAO executor class IDL.
 */
class be_visitor_executor_ex_idl
  : public be_visitor_component_scope
{
public:
  be_visitor_executor_ex_idl (be_visitor_context *ctx);

  ~be_visitor_executor_ex_idl ();

  int visit_component (be_component *node) override;
  int visit_connector (be_connector *node) override;

  /// To regenerate attribute declarations.
  int visit_attribute (be_attribute *node) override;

  /// To regenerate the name of the attribute's referenced
  /// (anonymous) type. Anonymous arrays are not allowed.
  int visit_string (be_string *node) override;
  int visit_sequence (be_sequence *node) override;

  int visit_provides (be_provides *node) override;
  int visit_consumes (be_consumes *node) override;

private:
  void gen_supported ();
  void gen_exception_list (UTL_ExceptList *exceptions,
                           const char *prefix = "",
                           bool closed = true);
};

#endif /* _BE_COMPONENT_EXECUTOR_EX_IDL_H_ */

