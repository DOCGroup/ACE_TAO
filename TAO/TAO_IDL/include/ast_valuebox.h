#ifndef _AST_VALUEBOX_AST_VALUEBOX_HH
#define _AST_VALUEBOX_AST_VALUEBOX_HH

#include "ast_concrete_type.h"

class TAO_IDL_FE_Export AST_ValueBox :  public virtual AST_ConcreteType
{
public:
  AST_ValueBox (UTL_ScopedName *n,
                AST_Type       *boxed_type);

  virtual ~AST_ValueBox ();

  // Data Accessors.
  AST_Type            *boxed_type () const;

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy ();

  static AST_Decl::NodeType const NT;

protected:
  AST_Type                    *pd_boxed_type;
};

#endif           // _AST_VALUEBOX_AST_VALUEBOX_HH
