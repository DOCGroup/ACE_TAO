#ifndef _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH
#define _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH

#include "ast_valuetype_fwd.h"

// Representation of a forward eventtype declaration.

class TAO_IDL_FE_Export AST_EventTypeFwd : public virtual AST_ValueTypeFwd
{
public:
  AST_EventTypeFwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  virtual ~AST_EventTypeFwd ();

  // Cleanup function.
  virtual void destroy ();

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;
};

#endif           // _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH
