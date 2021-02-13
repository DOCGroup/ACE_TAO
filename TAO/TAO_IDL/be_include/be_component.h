/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_component.h
 *
 *  Extension of class AST_Component that provides additional means for C++
 *  mapping of an interface.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_BE_COMPONENT_H
#define TAO_BE_COMPONENT_H

#include "be_interface.h"
#include "ast_component.h"

class AST_PortType;

/**
 * The back end extension of the AST_Component class
 */
class be_component : public virtual AST_Component,
                     public virtual be_interface
{
public:
  be_component (UTL_ScopedName *n,
                AST_Component *base_component,
                AST_Type **supports,
                long n_supports,
                AST_Interface **supports_flat,
                long n_supports_flat);

  ~be_component ();

  /// Override of the AST method to eliminate narrowing of
  /// the return value. Non-virtual.
  be_component *base_component () const;

  /// Catch BE-specific members before delegating to the base class.
  virtual void redefine (AST_Interface *from);

  /// Cleanup function.
  virtual void destroy ();

  /// Visiting.
  virtual int accept (be_visitor *visitor);

  // Need for public access to fe_add_structure and fe_add_typedef,
  // used to add implied IDL nodes created for 'uses multiple' declarations.
  AST_Structure *be_add_structure (AST_Structure *t);
  AST_Typedef *be_add_typedef (AST_Typedef *t);

  ACE_CDR::ULong n_provides () const;
  ACE_CDR::ULong n_remote_provides () const;
  ACE_CDR::ULong n_uses () const;
  ACE_CDR::ULong n_remote_uses () const;
  bool has_uses_multiple () const;
  ACE_CDR::ULong n_publishes () const;
  ACE_CDR::ULong n_consumes () const;
  ACE_CDR::ULong n_emits () const;

  /// Update the has_* members.
  void scan (UTL_Scope *s);

  /// Generate the "public virtual" entries in the stub header.
  virtual void gen_stub_inheritance (TAO_OutStream *os);

  /// Generate the "public virtual" entries in the skel header.
  virtual void gen_skel_inheritance (TAO_OutStream *os);

  /// Generate the string compares for ancestors in _is_a().
  virtual int gen_is_a_ancestors (TAO_OutStream *os);

private:
  /// Specialized version for mirror ports.
  void mirror_scan (AST_PortType *p);

private:
  /// Used by visitors to reduce footprint.
  ACE_CDR::ULong n_provides_;
  ACE_CDR::ULong n_remote_provides_;
  ACE_CDR::ULong n_uses_;
  ACE_CDR::ULong n_remote_uses_;
  bool has_uses_multiple_;
  ACE_CDR::ULong n_publishes_;
  ACE_CDR::ULong n_consumes_;
  ACE_CDR::ULong n_emits_;
};

#endif  // if !defined
