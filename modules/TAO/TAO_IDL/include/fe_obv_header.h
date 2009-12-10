// $Id$

#ifndef FE_OBV_HEADER_H
#define FE_OBV_HEADER_H

#include "fe_interface_header.h"

class AST_ValueType;

// FE_OBVHeader
// Internal class for FE to describe valuetype headers.
//

class FE_OBVHeader : public FE_InterfaceHeader
{
public:

  FE_OBVHeader (UTL_ScopedName *n,
                UTL_NameList *inherits,
                UTL_NameList *supports,
                bool truncatable,
                bool is_eventtype = false);
  virtual ~FE_OBVHeader (void);

  // Data Accessors.
  AST_Interface **supports (void) const;
  long n_supports (void) const;
  AST_ValueType *inherits_concrete (void) const;
  AST_Interface *supports_concrete (void) const;
  bool truncatable (void) const;
  
  virtual void destroy (void);
  // Destroy anything allocated for this class.

protected:
  // Supported interfaces.
  AST_Interface **supports_;
  long n_supports_;

  AST_ValueType *inherits_concrete_;
  AST_Interface *supports_concrete_;

  // Currently ignored.
  bool truncatable_;

protected:
  void compile_inheritance (UTL_NameList *vtypes,
                            bool is_eventtype);
  void compile_supports (UTL_NameList *supports);
  bool check_concrete_supported_inheritance (AST_Interface *d);
};

#endif /* FE_OBV_HEADER_H */
