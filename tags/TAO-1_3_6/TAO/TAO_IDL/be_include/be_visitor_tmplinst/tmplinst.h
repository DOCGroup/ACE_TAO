//=============================================================================
/**
*  @file   tmplinst.h
*
*  $Id$
*
*  Base class for the concrete visitors that generate explicit template
*  instantiations for the stub side and skeleton side.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================
#ifndef TAO_BE_VISITOR_TMPLINST_TMPLINST_H
#define TAO_BE_VISITOR_TMPLINST_TMPLINST_H

/**
 * @class be_visitor_tmplinst
 *
 * @brief Base class for stub generation and skeleton generation visitors.
 */
class be_visitor_tmplinst : public be_visitor_scope
{
public:
  enum Mode
    {
      TMPL_CLASS,
      PRAGMA_INST
    };

  be_visitor_tmplinst (be_visitor_context *ctx);

  virtual ~be_visitor_tmplinst (void);

  virtual int visit_root (be_root *node);

  virtual int visit_module (be_module *node);

  virtual int visit_interface_fwd (be_interface_fwd *node);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  virtual int visit_eventtype (be_eventtype *node);

  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

  virtual int visit_structure_fwd (be_structure_fwd *node);

  virtual int visit_field (be_field *node);

  virtual int visit_union_fwd (be_union_fwd *node);

  virtual int visit_union_branch (be_union_branch *node);

  virtual int visit_typedef (be_typedef *node);

  void switch_mode (void);

protected:
  virtual idl_bool this_mode_generated (be_decl *node) const = 0;
  virtual void this_mode_generated (be_decl *node, 
                                    idl_bool val) = 0;

protected:
  Mode mode_;
  char * prefix_;
  char * suffix_;
  char * linebreak_;
};


#endif // TAO_BE_VISITOR_TMPLINST_TMPLINST_H
