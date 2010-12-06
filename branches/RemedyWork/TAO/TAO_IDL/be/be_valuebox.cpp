
//=============================================================================
/**
 *  @file    be_valuebox.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Valuebox that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================

#include "be_valuebox.h"
#include "be_type.h"
#include "be_visitor.h"
#include "global_extern.h"

be_valuebox::be_valuebox (AST_Type *boxed_type,
                          UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_valuebox,
              n),
    AST_Type (AST_Decl::NT_valuebox,
              n),
    AST_ConcreteType (AST_Decl::NT_valuebox,
                      n),
    AST_ValueBox (n,
                  boxed_type),
    be_decl (AST_Decl::NT_valuebox,
             n),
    be_type (AST_Decl::NT_valuebox,
             n)
{
  // Always the case (according to C++ mapping specification).
  this->size_type (AST_Type::VARIABLE);

  // Set the flag  that says we have a valuetype in this IDL file.
  // This allows the correct #include to be generated (see be_codegen.cpp).
  idl_global->valuebase_seen_ = true;

  // Set the flag that says we have a valuetype in this IDL file.
  // This allows the correct #include to be generated (see be_codegen.cpp).
  idl_global->valuetype_seen_ = true;
}

// Accept a visitor.
int
be_valuebox::accept (be_visitor *visitor)
{
  return visitor->visit_valuebox (this);
}


void
be_valuebox::destroy (void)
{
  // Call the destroy methods of our base classes.
  this->AST_ValueBox::destroy ();
  this->be_type::destroy ();
}



IMPL_NARROW_FROM_DECL (be_valuebox)
