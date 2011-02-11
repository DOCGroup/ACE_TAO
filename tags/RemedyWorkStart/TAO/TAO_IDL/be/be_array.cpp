
//=============================================================================
/**
 *  @file    be_array.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Array that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_array.h"
#include "be_codegen.h"
#include "be_scope.h"
#include "be_helper.h"
#include "be_visitor.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "global_extern.h"

#include "ace/Log_Msg.h"

be_array::be_array (UTL_ScopedName *n,
                    unsigned long ndims,
                    UTL_ExprList *dims,
                    bool local,
                    bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_array,
              n,
              true),
    AST_Type (AST_Decl::NT_array,
              n),
    AST_ConcreteType (AST_Decl::NT_array,
                      n),
    AST_Array (n,
               ndims,
               dims,
               local,
               abstract),
    be_decl (AST_Decl::NT_array,
             n),
    be_type (AST_Decl::NT_array,
             n)
{
  if (!this->imported ())
    {
      idl_global->array_seen_ = true;
    }
}

be_array::~be_array (void)
{
}

int
be_array::create_name (void)
{
  char namebuf [NAMEBUFSIZE];
  unsigned long i;
  UTL_ScopedName *n = 0;
  be_decl *scope = 0;

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);
  // Retrieve the base type.
  // The name always starts this way.
  be_type *bt = be_type::narrow_from_decl (this->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "create_name - "
                         "bad base type\n"),
                        0);
    }

  ACE_OS::sprintf (namebuf,
                   "_tao_array_%s",
                   bt->local_name ()->get_string ());

  // Now append dimensions.
  for (i = 0; i < this->n_dims (); ++i)
    {
      AST_Expression *expr = this->dims ()[i];

      // Dimension value.
      if ((expr == 0) || ((expr != 0) && (expr->ev () == 0)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "create_name - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          ACE_OS::sprintf (namebuf,
                           "%s_%d",
                           namebuf,
                           ((int)expr->ev ()->u.ulval));
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "create_name - "
                             "bad dimension value\n"),
                            -1);
        }
    }

  // Now see if we have a fully scoped name and if so, generate one.
  scope = be_scope::narrow_from_scope (this->defined_in ())->decl ();

  if (scope)
    {
      // Make a copy of the enclosing scope's  name.
      n = (UTL_ScopedName *)scope->name ()->copy ();

      Identifier *id = 0;
      ACE_NEW_RETURN (id,
                      Identifier (ACE_OS::strdup (namebuf)),
                      -1);

      UTL_ScopedName *sn = 0;
      ACE_NEW_RETURN (sn,
                      UTL_ScopedName (id,
                                      0),
                      -1);

      // Add our local name as the last component.
      n->nconc (sn);
      // Set the fully scoped name.
      this->set_name (n);
    }
  else
    {
      // We better be not here because we must be inside some scope,
      // at least the ROOT scope.
      return -1;
    }

  return 0;
}

// Overriden method.
void
be_array::compute_tc_name (void)
{
  // Array TypeCodes can only be accessed through an alias
  // TypeCode.  Generate a TypeCode name that is meant for internal
  // use alone.

  Identifier * tao_id = 0;
  ACE_NEW (tao_id,
           Identifier ("TAO"));

  ACE_NEW (this->tc_name_,
           UTL_ScopedName (tao_id,
                           0));

  ACE_CString local_tc_name =
    ACE_CString ("tc_")
    + ACE_CString (this->flat_name ());

  Identifier * typecode_scope = 0;
  ACE_NEW (typecode_scope,
           Identifier ("TypeCode"));

  UTL_ScopedName * tc_scope_conc_name = 0;
  ACE_NEW (tc_scope_conc_name,
           UTL_ScopedName (typecode_scope,
                           0));

  this->tc_name_->nconc (tc_scope_conc_name);

  Identifier * id = 0;
  ACE_NEW (id,
           Identifier (local_tc_name.c_str ()));

  UTL_ScopedName * conc_name = 0;
  ACE_NEW (conc_name,
           UTL_ScopedName (id,
                           0));

  this->tc_name_->nconc (conc_name);
}

// Code generation.

int
be_array::gen_dimensions (TAO_OutStream *os,
                          unsigned short slice)
{
   // Loop index.
   unsigned long i;

  // Print our dimensions.
  for (i = (slice ? 1 : 0); i < this->n_dims (); ++i)
    {
      // Retrieve the ith.
      AST_Expression *expr = this->dims ()[i];

      // Dimension value.
      if ((expr == 0) || ((expr != 0) && (expr->ev () == 0)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_dimensions - "
                             "bad array dimension\n"),
                            -1);
        }

      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          *os << "[" << ((int)expr->ev ()->u.ulval) << "]";
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_dimensions - "
                             "bad dimension value\n"),
                            -1);
        }
    }

  return 0;
}

// Overridden method
void
be_array::gen_ostream_operator (TAO_OutStream *os,
                                bool use_underscore)
{
  be_scope* scope = be_scope::narrow_from_scope (this->defined_in ());
  be_decl* parent = scope->decl ();
  ACE_CString arg_name (ACE_CString (parent->full_name ())
                        + "::"
                        + (use_underscore ? "_" : "")
                        + this->local_name ()->get_string ()
                        + "_forany &_tao_array");

  // Using 'const' with xxx_forany here prevents the compiler from
  // automatically converting back to xxx_slice *.
  *os << be_nl
      << "std::ostream& operator<< (" << be_idt << be_idt_nl
      << "std::ostream &strm, " << be_nl
      << "const " << arg_name.c_str () << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "strm << \"" << this->name () << "\";" <<  be_nl_2;

  ACE_CDR::ULong ndims = this->n_dims ();
  ACE_CDR::ULong i = 0;

  for (i = 0; i < ndims; ++i)
    {
      *os << "strm << \"[\";" << be_nl_2;

      AST_Expression *expr = this->dims ()[i];

      // Generate a loop for each dimension.
      *os << "for ( ::CORBA::ULong i" << i << " = 0; i" << i << " < "
          << expr->ev ()->u.ulval << "; ++i" << i << ")" << be_idt_nl
          << "{" << be_idt_nl
          << "if (i" << i << " != 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "strm << \", \";" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;
    }

  *os << "strm << ";

  ACE_CString instance_name ("_tao_array.in ()");

  for (i = 0; i < ndims; ++i)
    {
      char *working = instance_name.rep ();
      instance_name += "[i";
      instance_name += ACE_OS::itoa (i, working, 10);
      instance_name += "]";
    }

  be_type *bt = be_type::narrow_from_decl (this->base_type ());
  bt->gen_member_ostream_operator (os,
                                   instance_name.c_str (),
                                   use_underscore,
                                   false);

  *os << ";";

  for (i = 0; i < ndims; ++i)
    {
      *os << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "strm << \"]\";";
    }

  *os << be_nl
      << "return strm;" << be_uidt_nl
      << "}" << be_nl;
}

void
be_array::gen_member_ostream_operator (TAO_OutStream *os,
                                       const char *instance_name,
                                       bool use_underscore,
                                       bool accessor)
{
  be_scope* scope = be_scope::narrow_from_scope (this->defined_in ());
  be_decl* parent = scope->decl ();
  ACE_CString decl_name (ACE_CString (parent->full_name ())
                         + "::"
                         + (use_underscore ? "_" : "")
                         + this->local_name ()->get_string ());

 // The container is always const, so the member is const as well,
 // but we have to cast it away for the forany constructor.
 *os << decl_name.c_str () << "_forany ("
     << "const_cast< " << decl_name.c_str () << "_slice *> (";

  this->be_type::gen_member_ostream_operator (os,
                                              instance_name,
                                              use_underscore,
                                              accessor);

  *os << "))";
}

int
be_array::accept (be_visitor *visitor)
{
  return visitor->visit_array (this);
}

void
be_array::destroy (void)
{
  this->be_type::destroy ();
  this->AST_Array::destroy ();
}



IMPL_NARROW_FROM_DECL (be_array)
