// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface.cpp
//
// = DESCRIPTION
//    Extension of class AST_Interface that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale,
//    Michael Kircher
//
// ============================================================================

#include "be_interface.h"
#include "be_interface_strategy.h"
#include "be_attribute.h"
#include "be_operation.h"
#include "be_visitor.h"
#include "be_helper.h"
#include "be_stream_factory.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "ast_generator.h"
#include "ast_component.h"
#include "global_extern.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include "ace/Process.h"

ACE_RCSID (be,
           be_interface,
           "$Id$")

// Default constructor.
be_interface::be_interface (void)
  : skel_count_ (0),
    in_mult_inheritance_ (-1),
    original_interface_ (0),
    has_mixed_parentage_ (-1)
{
  ACE_NEW (this->strategy_,
           be_interface_default_strategy (this));
}

// Constructor used to build the AST.
be_interface::be_interface (UTL_ScopedName *n,
                            AST_Interface **ih,
                            long nih,
                            AST_Interface **ih_flat,
                            long nih_flat,
                            idl_bool local,
                            idl_bool abstract)
  : AST_Interface (n,
                   ih,
                   nih,
                   ih_flat,
                   nih_flat,
                   local,
                   abstract),
    AST_Decl (AST_Decl::NT_interface,
              n),
    UTL_Scope (AST_Decl::NT_interface),
    COMMON_Base (local,
                 abstract),
    skel_count_ (0),
    in_mult_inheritance_ (-1),
    original_interface_ (0),
    has_mixed_parentage_ (-1)
{
  ACE_NEW (this->strategy_,
           be_interface_default_strategy (this));
}

be_interface::~be_interface (void)
{
  // We know that it cannot be 0, but..
  if (this->strategy_ != 0)
    {
      delete this->strategy_;
      this->strategy_ = 0;
    }
}

be_interface_strategy *
be_interface::set_strategy (be_interface_strategy *new_strategy)
{
  be_interface_strategy *old = this->strategy_;

  if (new_strategy != 0)
    {
      this->strategy_ = new_strategy;
    }

  return old;
}


const char *
be_interface::local_name (void) const
{
  // Return the local name.
  return this->strategy_->local_name ();
}

const char *
be_interface::full_name (void)
{
  // Return the stringified full name.
  return this->strategy_->full_name ();
}

const char *
be_interface::flat_name (void) const
{
  // Return the flattened full scoped name.
  return this->strategy_->flat_name ();
}

const char *
be_interface::repoID (void) const
{
  // Retrieve the repository ID.
  return this->strategy_->repoID ();
}

const char *
be_interface::full_skel_name (void) const
{
  // Retrieve the fully scoped skel class name.
  return this->strategy_->full_skel_name ();
}

const char *
be_interface::full_coll_name (int type) const
{
  // Retrieve the fully qualified collocated class name
  return this->strategy_->full_coll_name (type);
}

const char *
be_interface::local_coll_name (int type) const
{
  // Retrieve the fully qualified collocated class name.
  return this->strategy_->local_coll_name (type);
}

const char *
be_interface::relative_skel_name (const char *skel_name)
{
  // Relative skeleton name.
  return this->strategy_->relative_skel_name (skel_name);
}


void
be_interface::compute_full_skel_name (const char *prefix,
                                      char *&skelname)
{
  if (skelname != 0)
    {
      return;
    }
  else
    {
      long namelen = ACE_OS::strlen (prefix);
      long first = I_TRUE;
      long second = I_FALSE;
      char *item_name = 0;

      // In the first loop compute the total length.
      for (UTL_IdListActiveIterator i (this->name ());
           !i.is_done ();
           i.next ())
        {
          if (!first)
            {
              namelen += 2; // for "::"
            }
          else if (second)
            {
              first = second = I_FALSE;
            }

          // Print the identifier.
          item_name = i.item ()->get_string ();
          namelen += ACE_OS::strlen (item_name);

          // Additional 4 for the POA_ characters.
          if (first)
            {
              if (ACE_OS::strcmp (item_name, "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }
        }

      ACE_NEW (skelname,
               char [namelen+1]);
      skelname[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;
      ACE_OS::strcat (skelname, prefix);

      for (UTL_IdListActiveIterator j (this->name ());
           !j.is_done ();
           j.next ())
        {
          if (!first)
            {
              ACE_OS::strcat (skelname, "::");
            }
          else if (second)
            {
              first = second = I_FALSE;
            }

          // Print the identifier.
          item_name = j.item ()->get_string ();
          ACE_OS::strcat (skelname, item_name);

          if (first)
            {
              if (ACE_OS::strcmp (item_name, "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }
        }
    }
}

const char*
be_interface::relative_name (const char *localname,
                             const char *othername)
{
  // Some compilers do not like generating a fully scoped name for a
  // type that was defined in the same enclosing scope in which it was
  // defined.  We have to emit just the partial name, relative to our
  // "localname".

  // The tricky part here is that it is not enough to check if the
  // typename we are using was defined in the current scope. But we
  // need to ensure that it was not defined in any of our ancestor
  // scopes as well. If that is the case, then we can generate a fully
  // scoped name for that type, else we use the ACE_NESTED_CLASS macro.

  // Thus we need some sort of relative name to be generated.

  static char macro [NAMEBUFSIZE];

  // Hold the fully scoped name.
  char def_name [NAMEBUFSIZE];
  char use_name [NAMEBUFSIZE];

  // These point to the curr and next component in the scope.
  char *def_curr = def_name;
  char *def_next;
  char *use_curr = use_name;
  char *use_next;

  ACE_OS::memset (macro,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (def_name,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (use_name,
                  '\0',
                  NAMEBUFSIZE);

  // Traverse every component of the def_scope and use_scope beginning at the
  // root and proceeding towards the leaf trying to see if the components
  // match. Continue until there is a match and keep accumulating the path
  // traversed. This forms the first argument to the ACE_NESTED_CLASS
  // macro. Whenever there is no match, the remaining components of the
  // def_scope form the second argument.

  ACE_OS::strcpy (def_name,
                  localname);
  ACE_OS::strcpy (use_name,
                  othername);

  while (def_curr && use_curr)
    {
      // Find the first occurrence of a ::
      // and advance the next pointers accordingly.
      def_next = ACE_OS::strstr (def_curr, "::");
      use_next = ACE_OS::strstr (use_curr, "::");

      if (def_next)
        {
          *def_next = 0;
        }

      if (use_next)
        {
          *use_next = 0;
        }

      if (!ACE_OS::strcmp (def_curr, use_curr))
        {
          // They have same prefix, append to arg1.
          def_curr = (def_next ? (def_next+2) : 0); // Skip the ::
          use_curr = (use_next ? (use_next+2) : 0); // Skip the ::
        }
      else
        {
          // We had overwritten a ':' by a '\0' for string comparison. We
          // revert back because we want the rest of the relative name to be
          // used.
          if (def_next)
            {
              *def_next = ':';
            }

          if (use_next)
            {
              *use_next = ':';
            }

          // No match. This is the end of the first argument. Get out
          // of the loop as no more comparisons are necessary.
          break;
        }
    }

  // Start the 2nd argument of the macro.

  // Copy the remaining def_name (if any left)
  if (def_curr)
    {
      ACE_OS::strcat (macro,
                      def_curr);
    }

  return macro;
}


// Am I in some kind of a multiple inheritance?
int be_interface::in_mult_inheritance (void)
{
  if (this->in_mult_inheritance_ == -1)
    {
      // Compute once for all.
      // Determine if we are in some form of a multiple inheritance.
      if (this->traverse_inheritance_graph (
              be_interface::in_mult_inheritance_helper,
              0
            ) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_interface::in_mult_inheritance "
                             "error determining mult inheritance\n"),
                            -1);
        }
    }

  return this->in_mult_inheritance_;
}

void be_interface::in_mult_inheritance (int mi)
{
  if (this->in_mult_inheritance_ == -1)
    {
      this->in_mult_inheritance_ = mi;
    }
}

// Gen copy constructors,
void
be_interface::gen_copy_ctors (TAO_OutStream *os)
{
  this->traverse_inheritance_graph (
      be_interface::gen_copy_ctors_helper,
      os
    );

  return;
}

int
be_interface::gen_copy_ctors_helper (be_interface *node,
                                     be_interface *base,
                                     TAO_OutStream *os)
{
  static int first = 0;

  if (node != base)
    {
      if (first)
        {
          *os << be_global->impl_class_prefix () << base->flat_name ()
              << be_global->impl_class_suffix () << " (t)"
              << ", " << base->full_skel_name () << " (t)";

          first = 0;
        }
      else
        {
          *os << ", " << be_global->impl_class_prefix () << base->flat_name ()
              << be_global->impl_class_suffix () << " (t)"
              << ", " << base->full_skel_name () << " (t)";   ;
        }
    }
  else
    {
      *os << ":";
      first = 1;
    }

  return 1;
}

// Generate default constructors.
void
be_interface::gen_def_ctors (TAO_OutStream *os)
{
  this->traverse_inheritance_graph (
      be_interface::gen_def_ctors_helper,
      os
    );

  return;
}


int
be_interface::gen_def_ctors_helper (be_interface* node,
                                    be_interface* base,
                                    TAO_OutStream *os)
{

  static int first = 0;

  if (node != base)
    {
      if (first)
        {
          *os << be_global->impl_class_prefix () << base->flat_name ()
              << be_global->impl_class_suffix () << " ()";

          first = 0;
        }
      else
        {
          *os << ", " << be_global->impl_class_prefix () << base->flat_name ()
              << be_global->impl_class_suffix () << " ()";
        }
    }
  else
    {
      *os << ":";
      first = 1;
    }

   return 1;
}


void
be_interface::gen_stub_ctor (TAO_OutStream *os)
{
  // Generate the constructor from stub and servant.
  if (!this->is_local ())
    {
      *os << be_nl << be_nl
          << "ACE_INLINE" << be_nl;
      *os << this->name () << "::"
          << this->local_name () << " ("
          << be_idt << be_idt_nl
          << "TAO_Stub *objref," << be_nl
          << "CORBA::Boolean _tao_collocated," << be_nl
          << "TAO_Abstract_ServantBase *servant," << be_nl
          << "TAO_ORB_Core *oc" << be_uidt_nl
                << ")"
                << be_nl;
      *os << ": ACE_NESTED_CLASS (CORBA, Object) (objref, _tao_collocated, servant, oc)";

      if (this->has_mixed_parentage_)
        {
          *os << be_idt;

          int status =
            this->traverse_inheritance_graph (
                      be_interface::gen_abstract_init_helper,
                      os,
                      I_TRUE
                    );

          if (status == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "be_interface::gen_stub_ctor - "
                          "inheritance graph traversal failed\n"));
            }

          *os << "," << be_nl
              << "ACE_NESTED_CLASS (CORBA, AbstractBase) (objref, _tao_collocated, servant)"
              << be_uidt << be_uidt;
        }
      else
        {
          *os << be_uidt;
        }

      *os << be_nl << "{" << be_idt_nl
          << "this->" << this->flat_name ()
          << "_setup_collocation (_tao_collocated);";

      *os << be_uidt_nl
          << "}";
    }
}

// Generate the var definition. If <interface_name> is not 0, generate
// the var defn for that name. Otherwise, do it for the interface you
// are visiting (this).
int
be_interface::gen_var_defn (char *interface_name)
{
  char namebuf [NAMEBUFSIZE];  // names

  // Decide which name to use.
  if (interface_name == 0)
    {
      interface_name = (char *) this->local_name ();
    }

  // Buffer with name of the var class.
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_var",
                   interface_name);

  TAO_OutStream *ch = tao_cg->client_header ();

  // Generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *ch << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Note the private inheritance from TAO_Base_var to emphasize that
  // a given _var does not satisfy the IS-A relationship.  _var
  // classes are merely IMPLEMENTED-IN-TERMS-OF TAO_Base_var.  This
  // also invalidates assignments like:
  //    Foo_var v;
  //    TAO_Base_var * t = &v;
  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf
      << " : private TAO_Base_var" << be_nl;
  *ch << "{" << be_nl;
  *ch << "public:" << be_idt_nl;

  // Default constructor.
  *ch << namebuf << " (void); // default constructor" << be_nl;
  *ch << namebuf << " (" << interface_name
      << "_ptr p)"
      << " : ptr_ (p) {} " << be_nl;

  // Copy constructor.
  *ch << namebuf << " (const " << namebuf
      << " &); // copy constructor" << be_nl;

  // Destructor.
  *ch << "~" << namebuf
      << " (void); // destructor" << be_nl;
  *ch << be_nl;

  // Assignment operator from a pointer.
  *ch << namebuf << " &operator= (" << interface_name
      << "_ptr);" << be_nl;

  // Assignment from _var.
  *ch << namebuf << " &operator= (const " << namebuf
      << " &);" << be_nl;

  // Arrow operator
  *ch << interface_name << "_ptr operator-> (void) const;" << be_nl;
  *ch << be_nl;

  // Other extra types (cast operators, [] operator, and others).
  *ch << "operator const " << interface_name
      << "_ptr &() const;" << be_nl;
  *ch << "operator " << interface_name
      << "_ptr &();" << be_nl;

  *ch << "// in, inout, out, _retn " << be_nl;
  // The return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type.
  *ch << interface_name << "_ptr in (void) const;" << be_nl;
  *ch << interface_name << "_ptr &inout (void);" << be_nl;
  *ch << interface_name << "_ptr &out (void);" << be_nl;
  *ch << interface_name << "_ptr _retn (void);" << be_nl;

  // Generate an additional member function that returns the
  // underlying pointer.
  *ch << interface_name << "_ptr ptr (void) const;" << be_nl << be_nl;

  // Hooks for non-defined forward declared interfaces.
  *ch << "// Hooks used by template sequence and object manager classes"
      << be_nl
      << "// for non-defined forward declared interfaces." << be_nl
      << "static " << interface_name
      << "_ptr tao_duplicate ("
      << interface_name << "_ptr);" << be_nl
      << "static void tao_release (" << interface_name
      << "_ptr);" << be_nl
      << "static " << interface_name
      << "_ptr tao_nil (void);" << be_nl
      << "static " << interface_name
      << "_ptr tao_narrow (" << be_idt << be_idt_nl;

  if (this->is_abstract ())
    {
      *ch << "CORBA::AbstractBase *" << be_nl;
    }
  else if (this->node_type () == AST_Decl::NT_component)
    {
      *ch << "Components::CCMObject *" << be_nl;
    }
  else
    {
      *ch << "CORBA::Object *" << be_nl;
    }

  *ch << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt_nl;

  if (this->is_abstract ())
    {
      *ch << "static CORBA::AbstractBase *";
    }
  else if (this->node_type () == AST_Decl::NT_component)
    {
      *ch << "Components::CCMObject *";
    }
  else
    {
      *ch << "static CORBA::Object *";
    }

  *ch << " tao_upcast (void *);" << be_uidt_nl << be_nl;

  // Private.
  *ch << "private:" << be_idt_nl;
  *ch << interface_name << "_ptr ptr_;" << be_nl;
  *ch << "// Unimplemented - prevents widening assignment." << be_nl;
  *ch << interface_name << "_var (const TAO_Base_var & rhs);" << be_nl;
  *ch << interface_name << "_var & operator= (const TAO_Base_var & rhs);"
      << be_uidt_nl;

  *ch << "};";

  return 0;
}

// Implementation of the _var class. All of these get generated in the
// inline file.
// If the argument is 0, then use the name in <this>, otherwise use
// the name given. Just making the class more useful.
int
be_interface::gen_var_impl (char *interface_local_name,
                            char *interface_full_name)
{
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names

  // Decide on the names to use.
  // Even if one argument is 0, there is no point using the
  // arguments. Let us then use the name in this node.
  if (interface_local_name == 0 || interface_full_name == 0)
    {
      interface_local_name = (char *) this->local_name ();
      interface_full_name = (char *) this->full_name ();
    }

  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (fname,
                   "%s_var",
                   interface_full_name);

  ACE_OS::memset (lname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (lname,
                   "%s_var",
                   interface_local_name);

  TAO_OutStream *cs = tao_cg->client_stubs ();

  *cs << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;
  // Generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we
  // account for over here.

  *cs << be_nl << be_nl
      << "// *************************************************************"
      << be_nl
      << "// " << fname << be_nl
      << "// *************************************************************"
      << be_nl << be_nl;

  // Default constructor.
  *cs << fname << "::" << lname
      << " (void)" << be_idt_nl;
  *cs << ": ptr_ (" << interface_local_name
      << "::_nil ())" << be_uidt_nl;
  *cs << "{}" << be_nl << be_nl;

  // The additional ptr () member function. This member function must be
  // defined before the remaining member functions including the copy
  // constructor because this inline function is used elsewhere. Hence to make
  // inlining of this function possible, we must define it before its use.
  *cs << "::" << interface_full_name
      << "_ptr" << be_nl;
  *cs << fname << "::ptr (void) const" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Copy constructor.
  *cs << fname << "::" << lname
      << " (const ::" << interface_full_name
      << "_var &p)" << be_nl;
  *cs << "  : TAO_Base_var ()," << be_nl;
  *cs << "    ptr_ (" << interface_local_name
      << "::_duplicate (p.ptr ()))" << be_nl;
  *cs << "{}" << be_nl << be_nl;

  // Destructor.
  *cs << fname << "::~" << lname
      << " (void)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "CORBA::release (this->ptr_);" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Assignment operator.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (" << interface_local_name
      << "_ptr p)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "CORBA::release (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = p;" << be_nl;
  *cs << "return *this;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Assignment operator from _var.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const "
      << "::" << interface_full_name
      << "_var &p)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "if (this != &p)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "CORBA::release (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = ::" << interface_full_name
      << "::_duplicate (p.ptr ());" << be_uidt_nl;
  *cs << "}" << be_nl;
  *cs << "return *this;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Other extra methods - cast operator ().
  *cs << fname << "::operator const ::" << interface_full_name
      << "_ptr &() const // cast" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  *cs << fname << "::operator ::" << interface_full_name
      << "_ptr &() // cast " << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // operator->
  *cs << "::" << interface_full_name
      << "_ptr" << be_nl;
  *cs << fname << "::operator-> (void) const" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // in, inout, out, and _retn
  *cs << "::" << interface_full_name
      << "_ptr" << be_nl;
  *cs << fname << "::in (void) const" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  *cs << "::" << interface_full_name
      << "_ptr &" << be_nl;
  *cs << fname << "::inout (void)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  *cs << "::" << interface_full_name
      << "_ptr &" << be_nl;
  *cs << fname << "::out (void)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "CORBA::release (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = ::" << interface_full_name
      << "::_nil ();" << be_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  *cs << "::" << interface_full_name
      << "_ptr" << be_nl;
  *cs << fname << "::_retn (void)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "// yield ownership of managed obj reference" << be_nl;
  *cs << "::" << interface_full_name
      << "_ptr val = this->ptr_;" << be_nl;
  *cs << "this->ptr_ = ::" << interface_full_name
      << "::_nil ();" << be_nl;
  *cs << "return val;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Hooks for the flat name global functions used by references to
  // non-defined interfaces.
  *cs << "::" << interface_full_name
      << "_ptr" << be_nl
      << fname << "::tao_duplicate ("
      << interface_local_name << "_ptr p)" << be_nl
      << "{" << be_idt_nl
      << "return ::" << interface_full_name
      << "::_duplicate (p);"
      << be_uidt_nl
      << "}" << be_nl << be_nl;

  *cs << "void" << be_nl
      << fname << "::tao_release (" << interface_local_name
      << "_ptr p)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::release (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *cs << "::" << interface_full_name << "_ptr" << be_nl
      << fname << "::tao_nil (void)" << be_nl
      << "{" << be_idt_nl
      << "return ::" << interface_full_name
      << "::_nil ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *cs << "::" << interface_full_name << "_ptr" << be_nl
      << fname << "::tao_narrow (" << be_idt << be_idt_nl;

  if (this->is_abstract ())
    {
      *cs << "CORBA::AbstractBase *p" << be_nl;
    }
  else if (this->node_type () == AST_Decl::NT_component)
    {
      *cs << "Components::CCMObject *p" << be_nl;
    }
  else
    {
      *cs << "CORBA::Object *p" << be_nl;
    }

  *cs << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return ::" << interface_full_name
      << "::_narrow (p ACE_ENV_ARG_PARAMETER);"
      << be_uidt_nl
      << "}" << be_nl << be_nl;

  if (this->is_abstract ())
    {
      *cs << "CORBA::AbstractBase *" << be_nl;
    }
  else if (this->node_type () == AST_Decl::NT_component)
    {
      *cs << "Components::CCMObject *" << be_nl;
    }
  else
    {
      *cs << "CORBA::Object *" << be_nl;
    }

  *cs << fname << "::tao_upcast (void *src)" << be_nl
      << "{" << be_idt_nl
      << interface_local_name << " **tmp =" << be_idt_nl
      << "ACE_static_cast (" << interface_local_name
      << " **, src);"
      << be_uidt_nl
      << "return *tmp;" << be_uidt_nl
      << "}";

  return 0;
}

// Generate the out definition. If <interface_name> is not 0, generate
// the out defn for that name. Otherwise, do it for the interface you
// are visiting (this).
int
be_interface::gen_out_defn (char *interface_name)
{
  char namebuf [NAMEBUFSIZE];  // to hold the _out name

  // Decide which name to use.
  if (interface_name == 0)
    interface_name = (char *) this->local_name ();

  // Create the buffer with the name of the out class.
  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf,
                   "%s_out",
                   interface_name);

  TAO_OutStream *ch = tao_cg->client_header ();

  *ch << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the out definition (always in the client header)
  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf << be_nl;
  *ch << "{" << be_nl;
  *ch << "public:" << be_idt_nl;

  // No default constructor.

  // Constructor from a pointer.
  *ch << namebuf << " (" << interface_name
      << "_ptr &);" << be_nl;

  // Constructor from a _var &.
  *ch << namebuf << " (" << interface_name
      << "_var &);" << be_nl;

  // Constructor from a _out &.
  *ch << namebuf << " (const " << namebuf
      << " &);" << be_nl;

  // Assignment operator from a _out &
  *ch << namebuf << " &operator= (const " << namebuf
      << " &);" << be_nl;

  // Assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators.
  // Only interface allows assignment from var &.
  *ch << namebuf << " &operator= (const " << interface_name
      << "_var &);" << be_nl;
  *ch << namebuf << " &operator= (" << interface_name
      << "_ptr);" << be_nl;

  // Cast.
  *ch << "operator " << interface_name
      << "_ptr &();" << be_nl;

  // ptr fn
  *ch << interface_name << "_ptr &ptr (void);" << be_nl;

  // operator ->
  *ch << interface_name << "_ptr operator-> (void);" << be_uidt_nl << be_nl;

  *ch << "private:" << be_idt_nl;
  *ch << interface_name << "_ptr &ptr_;" << be_uidt_nl;

  *ch << "};";

  return 0;
}


// Generate the out class definition. If <interface_name> is not 0,
// generate the out defn for that name. Otherwise, do it for the
// interface you are visiting (this).
int
be_interface::gen_out_impl (char *interface_local_name,
                            char *interface_full_name)
{
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names

  // Even if one argument is 0, there is no point using the
  // arguments. Let us then use the name in this node.
  if (interface_local_name == 0 || interface_full_name == 0)
    {
      interface_local_name = (char *) local_name ();
      interface_full_name = (char *) this->full_name ();
    }

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_out", interface_full_name);

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_out", interface_local_name);

  TAO_OutStream *cs = tao_cg->client_stubs ();

  // Generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *cs << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *cs << be_nl << be_nl
      << "// *************************************************************"
      << be_nl
      << "// " << fname << be_nl
      << "// *************************************************************";

      // Constructor from a _ptr.
  *cs << be_nl << be_nl
      << fname << "::" << lname
      << " ("  << interface_local_name
      << "_ptr &p)" << be_nl;
  *cs << "  : ptr_ (p)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "this->ptr_ = ::" << interface_full_name
      << "::_nil ();" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Constructor from _var &.
  *cs << fname << "::" << lname
      << " (" << interface_local_name
      << "_var &p)" << be_nl;
  *cs << "  : ptr_ (p.out ())" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "CORBA::release (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = ::" << interface_full_name
      << "::_nil ();" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Copy constructor.
  *cs << fname << "::" << lname
      << " (const ::" << interface_full_name
      << "_out &p)" << be_nl;
  *cs << "  : ptr_ (ACE_const_cast (" << interface_local_name
      << "_out &, p).ptr_)" << be_nl;
  *cs << "{}" << be_nl << be_nl;

  // Assignment operator from _out &.
  *cs << "::" << fname
      << " &" << be_nl;
  *cs << fname << "::operator= (const ::" << interface_full_name
      << "_out &p)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "this->ptr_ = ACE_const_cast (" << interface_local_name
      << "_out&, p).ptr_;" << be_nl;
  *cs << "return *this;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Assignment operator from _var.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const ::" << interface_full_name
      << "_var &p)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "this->ptr_ = ::" << interface_full_name
      << "::_duplicate (p.ptr ());" << be_nl;
  *cs << "return *this;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Assignment operator from _ptr.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (" << interface_local_name
      << "_ptr p)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "this->ptr_ = p;" << be_nl;
  *cs << "return *this;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Other extra methods - cast operator ().
  *cs << fname << "::operator ::" << interface_full_name
      << "_ptr &() // cast" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // ptr function.
  *cs << "::" << interface_full_name
      << "_ptr &" << be_nl;
  *cs << fname << "::ptr (void)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // operator->
  *cs << "::" << interface_full_name
      << "_ptr" << be_nl;
  *cs << fname << "::operator-> (void)" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;
  *cs << "}";

  return 0;
}

// ****************************************************************

TAO_IDL_Inheritance_Hierarchy_Worker::~TAO_IDL_Inheritance_Hierarchy_Worker (
    void
  )
{
}

class TAO_IDL_Gen_OpTable_Worker : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  TAO_IDL_Gen_OpTable_Worker (const char *skeleton_name);

  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *os,
                    be_interface *base_interface);

private:
  const char *skeleton_name_;
};

TAO_IDL_Gen_OpTable_Worker::
TAO_IDL_Gen_OpTable_Worker (const char *skeleton_name)
  : skeleton_name_ (skeleton_name)
{
}

int
TAO_IDL_Gen_OpTable_Worker::emit (be_interface * /* derived_interface */,
                                  TAO_OutStream *os,
                                  be_interface *base_interface)
{
  // Generate entries for the derived class using the properties of its
  // ancestors.
  be_interface *bi = be_interface::narrow_from_decl (base_interface);
  return bi->gen_optable_entries (this->skeleton_name_, os);
}

int
be_interface::gen_operation_table (const char *flat_name,
                                   const char *skeleton_class_name)
{
  // Check out the op_lookup_strategy.
  switch (be_global->lookup_strategy ())
  {
    case BE_GlobalData::TAO_DYNAMIC_HASH:
      {
        // Init the outstream appropriately.
        TAO_OutStream *os = this->strategy_->get_out_stream ();

        // Start from current indentation level.
        os->indent ();

        // Start the table generation.
        *os << "static const TAO_operation_db_entry " << flat_name <<
          "_operations [] = {" << be_nl;

        os->incr_indent (0);

        // Make sure the queues are empty.
        this->insert_queue.reset ();
        this->del_queue.reset ();

        // Insert ourselves in the queue.
        if (insert_queue.enqueue_tail (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_interface::gen_operation_table - "
                               "error generating entries\n"),
                              -1);
          }

        // Traverse the graph.
        TAO_IDL_Gen_OpTable_Worker worker (skeleton_class_name);

        if (this->traverse_inheritance_graph (worker, os) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_interface::gen_operation_table - "
                               "inheritance graph traversal failed\n"), -1);
          }

        // Generate the skeleton for the is_a method.
        *os << "{\"_is_a\", &" << skeleton_class_name
            << "::_is_a_skel}," << be_nl;

        this->skel_count_++;

        *os << "{\"_non_existent\", &" << skeleton_class_name
            << "::_non_existent_skel}," << be_nl;

        this->skel_count_++;

        *os << "{\"_interface\", &" << skeleton_class_name
            << "::_interface_skel}" << be_uidt_nl;

        this->skel_count_++;

        *os << "};\n" << be_nl;
        *os << "static const CORBA::Long _tao_" << flat_name
            << "_optable_size = sizeof (ACE_Hash_Map_Entry<const char *,"
            << " TAO_Skeleton>) * (" << (3 * this->skel_count_)
            << ");" << be_nl;
        *os << "static char _tao_" << flat_name << "_optable_pool "
            << "[_tao_" << flat_name << "_optable_size];" << be_nl;
        *os << "static ACE_Static_Allocator_Base _tao_" << flat_name
            << "_allocator (_tao_" << flat_name << "_optable_pool, "
            << "_tao_" << flat_name << "_optable_size);" << be_nl;
        *os << "static TAO_Dynamic_Hash_OpTable tao_"
            << flat_name << "_optable " << "(" << be_idt << be_idt_nl
            << flat_name << "_operations," << be_nl
            << this->skel_count_ << "," << be_nl
            << 2 * this->skel_count_ << "," << be_nl
            << "&_tao_" << flat_name << "_allocator" << be_uidt_nl
            << ");" << be_uidt_nl;
      }
      break;

    case BE_GlobalData::TAO_LINEAR_SEARCH:
      // For generating linear search also, we are calling GPERF
      // only.
    case BE_GlobalData::TAO_BINARY_SEARCH:
      // For generating binary search also, we are calling GPERF
      // only.
    case BE_GlobalData::TAO_PERFECT_HASH:
      // For each interface in the IDL, have a new temp file to
      // collect the input for the gperf program.
      {
        // Temp file name.
        char *temp_file = 0;
        ACE_NEW_RETURN (temp_file,
                        char [ACE_OS::strlen (idl_global->temp_dir ())
                              + ACE_OS::strlen (flat_name)
                              + ACE_OS::strlen (".gperf")
                              + 1],
                        -1);

        ACE_OS::sprintf (temp_file,
                         "%s%s.gperf",
                         idl_global->temp_dir (),
                         flat_name);

        // QNX can't handle individual file names (path components)
        // longer than 48 characters.
#if defined(__QNX__)
        size_t temp_dir_len = ACE_OS::strlen (idl_global->temp_dir ());

        if (ACE_OS::strlen (temp_file) > temp_dir_len + 47)
          {
            temp_file[temp_dir_len + 47] = 0;
          }
#endif /* defined(__QNX__) */

        // Save this file name with the codegen singleton.
        tao_cg->gperf_input_filename (temp_file);

        // Make a new outstream to hold the gperf_temp_file for this
        // interface.

        // Retrieve the singleton instance to the outstream factory.
        TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

        // Get a new instance for the temp file.
        TAO_OutStream *os = factory->make_outstream ();

        if (os == 0)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "be_visitor_interface_ss"
                               "::"
                               "visit_interface-"
                               "make_outstream failed\n"),
                              -1);
          }

        // Store the outstream with the codegen singleton.
        tao_cg->gperf_input_stream (os);

        // Open the temp file.
        if (os->open (temp_file,
                      TAO_OutStream::TAO_GPERF_INPUT) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "be_visitor_interface_ss"
                               "::"
                               "visit_interface-"
                               "gperf_input.tmp file open failed\n"),
                              -1);
          }

        // Add the gperf input header.
        this->gen_gperf_input_header (os);

        // Make sure the queues are empty.
        this->insert_queue.reset ();
        this->del_queue.reset ();

        // Insert ourselves in the queue.
        if (insert_queue.enqueue_tail (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_interface::gen_operation_table - "
                               "error generating entries\n"),
                              -1);
          }

        // Traverse the graph.
        TAO_IDL_Gen_OpTable_Worker worker (skeleton_class_name);

        if (this->traverse_inheritance_graph (worker, os) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_interface::gen_operation_table - "
                               "inheritance graph traversal failed\n"),
                              -1);
          }

        // Generate the skeleton for the is_a method.
        os->indent ();

        *os << "_is_a, &"
            << skeleton_class_name
            << "::_is_a_skel" << be_nl;

        this->skel_count_++;

        *os << "_non_existent, &"
            << skeleton_class_name
            << "::_non_existent_skel" << be_nl;

        this->skel_count_++;

        *os << "_component, &"
            << skeleton_class_name
            << "::_component_skel" << be_nl;
        this->skel_count_++;

        *os << "_interface, &"
            << skeleton_class_name
            << "::_interface_skel" << be_nl;

        this->skel_count_++;

        // Input to the gperf is ready. Run gperf and get things
        // done. This method also unlinks the temp file that we used
        // for the gperf.
        this->gen_gperf_things (flat_name);
      }
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_interface"
                         "::"
                         "gen_operation_table"
                         "unknown op_lookup_strategy\n"),
                        -1);
  }

  return 0;
}

// Helper.
int
be_interface::gen_operation_table (void)
{
  const char *flat_name =
    this->flat_name ();
  const char *skeleton_class_name =
    this->full_skel_name ();

  return this->gen_operation_table (flat_name, skeleton_class_name);
}



// Output the header (type declaration and %%) to the gperf's input
// file.
void
be_interface::gen_gperf_input_header (TAO_OutStream *os)
{
  *os << "class TAO_operation_db_entry {\n"
      << "public:\n"
      << "\tchar *opname_;" << "\n"
      << "\tTAO_Skeleton skel_ptr_;" << "\n"
      << "};" << "\n"
      << "%%"
      << "\n";
}

// we separate the generation of operation table entries from the
// "gen_operation_table" method. This enables us to invoke generation of
// entries for interfaces from which we inherit without any additional
// code. The parameter "derived" is the one for which the entire operation
// table is being built.
int
be_interface::gen_optable_entries (const char *full_skeleton_name,
                                   TAO_OutStream *os)
{
  int lookup_strategy =
    be_global->lookup_strategy ();

  if (lookup_strategy == BE_GlobalData::TAO_DYNAMIC_HASH)
    {
      for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          // get the next AST decl node
          AST_Decl *d = si.item ();

          if (d->node_type () == AST_Decl::NT_op)
            {
              // Start from current indentation level.
              os->indent ();

              // We are an operation node.
              *os << "{\"" << d->original_local_name () << "\", &"
                  << full_skeleton_name << "::"
                  << d->local_name () << "_skel},\n";

              this->skel_count_++;
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr =
                AST_Attribute::narrow_from_decl (d);

              if (attr == 0)
                return -1;

              // Start from current indentation level.
              os->indent ();

              // Generate only the "get" entry if we are
              // readonly.
              *os << "{\"_get_" << d->original_local_name ()
                  << "\", &" << full_skeleton_name
                  << "::_get_" << d->local_name () << "_skel},\n";

              this->skel_count_++;

              if (!attr->readonly ())
                {
                  // The set method
                  os->indent (); // Start from current indentation level.
                  *os << "{\"_set_" << d->original_local_name ()
                      << "\", &" << full_skeleton_name
                      << "::_set_" << d->local_name () << "_skel},\n";

                  this->skel_count_++;
                }
            }
        }
    }
  else if (lookup_strategy == BE_GlobalData::TAO_LINEAR_SEARCH
           || lookup_strategy == BE_GlobalData::TAO_BINARY_SEARCH
           || lookup_strategy == BE_GlobalData::TAO_PERFECT_HASH)
    {
      // We call GPERF for all these three strategies.
      // Init the outstream.
      // @@ We probably do no need to do this, the "right" <os>
      //    argument is passed down!!
      os = tao_cg->gperf_input_stream ();

      for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          // get the next AST decl node
          AST_Decl *d = si.item ();

          if (d->node_type () == AST_Decl::NT_op)
            {
              // Generate operation name.

              // Start from current indentation level
              os->indent ();

              // We are an operation node. We use the original
              // operation name, not the one with _cxx_ in it.
              *os << d->original_local_name () << ",\t&"
                  << full_skeleton_name << "::"
                  << d->local_name () << "_skel" << "\n";

              this->skel_count_++;
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr =
                AST_Attribute::narrow_from_decl (d);

              if (attr == 0)
                return -1;

              os->indent ();

              // Generate only the "get" entry if we are readonly.
              *os << "_get_" << d->original_local_name () << ",\t&"
                  << full_skeleton_name << "::_get_"
                  << d->local_name () << "_skel\n";

              this->skel_count_++;

              if (!attr->readonly ())
                {
                  // The set method
                  os->indent ();
                  *os << "_set_" << d->original_local_name () << ",\t&"
                      << full_skeleton_name << "::_set_"
                      << d->local_name () << "_skel\n";

                  this->skel_count_++;
                }
            }
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_interface::gen_optable_entries - "
                         "unknown op_lookup_strategy\n"),
                        -1);
    }

  return 0;
}

void
be_interface::analyze_parentage (void)
{
  this->has_mixed_parentage_ = 0;

  for (long i = 0; i < this->pd_n_inherits; ++i)
    {
      if (this->pd_inherits[i]->is_abstract ())
        {
          this->has_mixed_parentage_ = 1;
          break;
        }
    }

  if (this->has_mixed_parentage_ == 1)
    {
      be_global->mixed_parentage_interfaces.enqueue_tail (this);
    }
}

// ****************************************************************

be_code_emitter_wrapper::
be_code_emitter_wrapper (be_interface::tao_code_emitter emitter)
  : emitter_ (emitter)
{
}

int
be_code_emitter_wrapper::emit (be_interface *derived_interface,
                               TAO_OutStream *output_stream,
                               be_interface *base_interface)
{
  return this->emitter_ (derived_interface,
                         base_interface,
                         output_stream);
}

// Template method that traverses the inheritance graph in a breadth-first
// style. The actual work on each element in the inheritance graph is carried
// out by the function passed as argument.
int
be_interface::traverse_inheritance_graph (be_interface::tao_code_emitter gen,
                                          TAO_OutStream *os,
                                          idl_bool abstract_paths_only)
{
  // Make sure the queues are empty.
  this->insert_queue.reset ();
  this->del_queue.reset ();

  // Insert ourselves in the queue.
  if (insert_queue.enqueue_tail (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::traverse_inheritance_graph - "
                         "error generating entries\n"),
                        -1);
    }

  be_code_emitter_wrapper wrapper (gen);

  return this->traverse_inheritance_graph (wrapper,
                                           os,
                                           abstract_paths_only);
}

int
be_interface::traverse_inheritance_graph (
    TAO_IDL_Inheritance_Hierarchy_Worker &worker,
    TAO_OutStream *os,
    idl_bool abstract_paths_only
  )
{
  // Do until queue is empty.
  while (!this->insert_queue.is_empty ())
    {
      AST_Interface *intf;  // element inside the queue

      // Use breadth-first strategy i.e., first generate entries for ourselves,
      // followed by nodes that we immediately inherit from, and so on. In the
      // process make sure that we do not generate code for the same node more
      // than once. Such a case may arise due to multiple inheritance forming
      // a diamond-like inheritance graph.

      // Dequeue the element at the head of the queue.
      if (this->insert_queue.dequeue_head (intf))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "dequeue_head failed\n"),
                            -1);
        }

      // Insert the dequeued element in the del_queue.
      if (this->del_queue.enqueue_tail (intf) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "enqueue_head failed\n"),
                            -1);
        }

      be_interface *bi = be_interface::narrow_from_decl (intf);

      // Use the helper method to generate code for ourself using the
      // properties of the element dequeued. For the first iteration, the
      // element dequeued and "this" will be the same i.e., ourselves.
      if (worker.emit (this, os, bi) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "helper code gen failed\n"),
                            -1);
        }

      // If we are doing a component, we check for a parent.
      if (bi->node_type () == AST_Decl::NT_component)
        {
          AST_Component *base =
            AST_Component::narrow_from_decl (bi)->base_component ();

          if (base != 0)
            {
              (void) this->insert_non_dup (base);

              long n_supports = base->n_supports ();
              AST_Interface **supports = base->supports ();

              for (long j = 0; j < n_supports; ++j)
                {
                  (void) this->insert_non_dup (supports[j]);
                }
            }
          else
            {
              (void) this->insert_non_dup (be_global->ccmobject ());
            }
        }

      // Now check if the dequeued element has any ancestors. If yes, insert
      // them inside the queue making sure that there are no duplicates.
      // If we are doing a component, the inheritance list is actually a
      // supports list.
      for (long i = 0; i < bi->n_inherits (); ++i)
        {
          // Retrieve the next parent from which the dequeued element inherits.
          AST_Interface *parent = bi->inherits ()[i];

          if (parent == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_interface::traverse_graph -"
                                 " bad inherited interface\n"),
                                -1);
            }

          if (abstract_paths_only && ! parent->is_abstract ())
            {
              continue;
            }

          (void) this->insert_non_dup (parent);
        } // end of for loop
    } // end of while queue not empty

  return 0;
}

// Run GPERF and get the correct lookup and other operations
// depending on which strategy we are using. Returns 0 on sucess, -1
// on error.
int
be_interface::gen_gperf_things (const char *flat_name)
{
  // GPERF can give Binary search, Linear search and Perfect Hash
  // methods. Generate the class defintion according to that.

  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the correct class definition for the operation lookup
  // strategy. Then, get the lookup method from GPERF. And then,
  // instantiate the correct class for the operation lookup strategy
  // we are following.
  switch (be_global->lookup_strategy ())
  {
    case BE_GlobalData::TAO_PERFECT_HASH:
      // Output a class definition deriving from
      // TAO_Perfect_Hash_OpTable.
      this->gen_perfect_hash_class_definition (flat_name);

      // Call GPERF and get the methods defined.
      if (this->gen_gperf_lookup_methods (flat_name) == -1)
        {
          return -1;
        }

      // Create an instance of the correct class corresponding the
      // operation lookup strategy we are following.
      this->gen_perfect_hash_instance (flat_name);

      break;

    case BE_GlobalData::TAO_BINARY_SEARCH:
      // Output a class definition deriving from
      // TAO_Binary_Search_OpTable.
      this->gen_binary_search_class_definition (flat_name);

      // Call GPERF and get the methods defined.
      if (gen_gperf_lookup_methods (flat_name) == -1)
        {
          return -1;
        }

      // Create an instance of the correct class corresponding the
      // operation lookup strategy we are following.
      this->gen_binary_search_instance (flat_name);

      break;

    case BE_GlobalData::TAO_LINEAR_SEARCH:
      // Output a class definition deriving from
      // TAO_Linear_Search_OpTable.
      this->gen_linear_search_class_definition (flat_name);

      // Call GPERF and get the methods defined.
      if (this->gen_gperf_lookup_methods (flat_name) == -1)
        {
          return -1;
        }

      // Create an instance of the correct class corresponding the
      // operation lookup strategy we are following.
      this->gen_linear_search_instance (flat_name);

      break;

    default:
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "tao_idl:ERROR:%N:%l:Unknown Operation Lookup Strategy\n"
        ),
        -1
      );
  }

  return 0;
}


// Outputs the class definition for the perfect hashing. This class
// will inherit from the TAO_Perfect_Hash_OpTable.
void
be_interface::gen_perfect_hash_class_definition (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << "class " << "TAO_" << flat_name << "_Perfect_Hash_OpTable"
      << " : public TAO_Perfect_Hash_OpTable"
      << be_nl
      << "{"
      << be_nl
      << "private:"
      << be_nl
      << "  unsigned int hash (const char *str, unsigned int len);"
      << be_nl
      << "public:"
      << be_nl
      << " const TAO_operation_db_entry * lookup (const char *str, unsigned int len);"
      << be_nl
      << "};\n\n";
}

// Outputs the class definition for the binary searching. This class
// will inherit from the TAO_Binary_Seach_OpTable.
void
be_interface::gen_binary_search_class_definition (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << "class " << "TAO_" << flat_name << "_Binary_Search_OpTable"
      << " : public TAO_Binary_Search_OpTable"
      << be_nl
      << "{"
      << be_nl
      << "public:"
      << be_nl
      << " const TAO_operation_db_entry * lookup (const char *str);"
      << be_nl
      << "};\n\n";
}

// Outputs the class definition for the linear search. This class
// will inherit from the TAO_Linear_Search_OpTable.
void
be_interface::gen_linear_search_class_definition (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *ss = this->strategy_->get_out_stream ();

  *ss << "class " << "TAO_" << flat_name << "_Linear_Search_OpTable"
      << " : public TAO_Linear_Search_OpTable"
      << be_nl
      << "{"
      << be_nl
      << "public:"
      << be_nl
      << " const TAO_operation_db_entry * lookup (const char *str);"
      << be_nl
      << "};\n\n";
}

// We have collected the input (Operations and the corresponding
// skeleton pointers) for the gperf program. Now let us execute gperf
// and get things done.
// GPERF reads from our temp file and write to the Server Skeleton
// file.
int
be_interface::gen_gperf_lookup_methods (const char *flat_name)
{
  // Using ACE_Process.
  ACE_Process process;
  ACE_Process_Options process_options;

  // Adjust the offset of the underlying file pointer.
  ACE_OS::rewind (tao_cg->gperf_input_stream ()->file ());

  // Set the stdin and stdout appropriately for the gperf program.

  // Stdin is our temp file. Close the temp file and open. We will use
  // <open_temp_file> to  open the file now, so that the file will get
  // deleted once when we close the file.

  // Close the file.
  if (ACE_OS::fclose (tao_cg->gperf_input_stream ()->file ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p:File close failed on temp gperf's input file\n",
                         "fclose"),
                        -1);
    }

  // Open the temp file.
  ACE_HANDLE input = ACE::open_temp_file (tao_cg->gperf_input_filename (),
                                          O_RDONLY);

  if (input == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p:File open failed on gperf's temp input file\n",
                         "open_temp_file"),
                        -1);
    }

  // Stdout is server skeleton.  Do *not* close the file, just open
  // again with <ACE_OS::open> with WRITE + APPEND option.. After
  // this, remember to update the file offset to the correct location.

  ACE_HANDLE output = ACE_OS::open (this->strategy_->get_out_stream_fname (),
                                    O_WRONLY | O_APPEND);

  if (output == ACE_INVALID_HANDLE)
    {
      ACE_OS::close (input);
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p:File open failed on server skeleton file\n",
                         "open"),
                        -1);
    }

  // Seek to the end of the output file.
  ACE_OS::lseek (output, 0, SEEK_END);

  // Set the handles now in the process options.
  process_options.set_handles (input, output);

  int result = 0;

  // Set the command line for the gperf program. Give the right
  // arguments for the operation lookup strategy that we are using.
  switch (be_global->lookup_strategy ())
  {
      // Perfect Hashing.
    case BE_GlobalData::TAO_PERFECT_HASH:
      process_options.command_line ("%s"
                                    " "
                                    "-m -M -J -c -C"
                                    " "
                                    "-D -E -T -f 0"
                                    " "
                                    "-F 0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname_ -L C++"
                                    " "
                                    "-Z TAO_%s_Perfect_Hash_OpTable"
                                    " "
                                    "-N lookup",
                                    idl_global->gperf_path (),
                                    flat_name);
      break;

      // Binary search methods from GPERF. Everythis and the -B flag.
    case BE_GlobalData::TAO_BINARY_SEARCH:
      process_options.command_line ("%s"
                                    " "
                                    "-B"
                                    "  "
                                    "-m -M -J -c -C"
                                    " "
                                    "-D -E -T -f 0"
                                    " "
                                    "-F 0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname_ -L C++"
                                    " "
                                    "-Z TAO_%s_Binary_Search_OpTable"
                                    " "
                                    "-N lookup",
                                    idl_global->gperf_path (),
                                    flat_name);
      break;

      // Linear search methods from GPERF. Everything and the -z flag.
    case BE_GlobalData::TAO_LINEAR_SEARCH:
      process_options.command_line ("%s"
                                    " "
                                    "-b"
                                    "  "
                                    "-m -M -J -c -C"
                                    " "
                                    "-D -E -T -f 0"
                                    " "
                                    "-F 0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname_ -L C++"
                                    " "
                                    "-Z TAO_%s_Linear_Search_OpTable"
                                    " "
                                    "-N lookup",
                                    idl_global->gperf_path (),
                                    flat_name);
      break;

    default:
      ACE_ERROR ((LM_ERROR,
                  "tao_idl:ERROR:%N:%l:Unknown Operation Lookup Strategy\n"));

      result = -1;
  }

  if (result != -1)
    {
      // Spawn a process for gperf.
      if (process.spawn (process_options) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error:%p:Couldnt spawn a process for gperf program\n",
                      "process.spawn"));

          result = -1;
        }

      // Wait for gperf to complete.
      else if (process.wait () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error:%p:Error on waiting for completion of gperf program.\n",
                      "process.wait"));

          result = -1;
        }

      // Adjust the file offset to the EOF for the server skeleton
      // file.
      ACE_OS::fseek (this->strategy_->get_out_stream()->file (),
                     0,
                     SEEK_END);
    }

  ACE_OS::close (output);
  ACE_OS::close (input);

  return result;
}

// Create an instance of this perfect hash table.
void
be_interface::gen_perfect_hash_instance (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << "static TAO_" << flat_name << "_Perfect_Hash_OpTable"
      << " "
      << "tao_" << flat_name << "_optable"
      << ";\n" << be_nl;
}

// Create an instance of the binary search optable.
void
be_interface::gen_binary_search_instance (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << "static TAO_" << flat_name << "_Binary_Search_OpTable"
      << " "
      << "tao_" << flat_name << "_optable"
      << ";\n" << be_nl;
}


// Create an instance of this perfect hash table.
void
be_interface::gen_linear_search_instance (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << "static TAO_" << flat_name << "_Linear_Search_OpTable"
      << " "
      << "tao_" << flat_name << "_optable"
      << ";\n" << be_nl;
}

int
be_interface::is_a_helper (be_interface * /*derived*/,
                           be_interface *bi,
                           TAO_OutStream *os)
{
  // Emit the comparison code.
  *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
      << "(char *)value," << be_nl
      << "\"" << bi->repoID () << "\"" << be_uidt_nl
      << ") ||" << be_uidt_nl;

  return 0;
}

int
be_interface::queryinterface_helper (be_interface *derived,
                                     be_interface *ancestor,
                                     TAO_OutStream *os)
{
  // Emit the comparison code.
  *os << "(type == ACE_reinterpret_cast ("
      << be_idt << be_idt <<be_idt << be_idt << be_idt << be_idt_nl
      << "ptrdiff_t," << be_nl;

  be_decl *scope =
    be_scope::narrow_from_scope (ancestor->defined_in ())->decl ();

  be_decl *derived_scope =
    be_scope::narrow_from_scope (derived->defined_in ())->decl ();

  // If the ancestor is in the root scope, we can use the local name.
  if (scope->node_type () == AST_Decl::NT_root)
    {
      *os << "&" << ancestor->local_name () << "::_tao_class_id)"
          << be_uidt_nl;
    }
  // Or, if it's defined in a scope different than the child's, the
  // ACE_NESTED_CLASS macro won't work - we use the scoped name.
  else if (scope != derived_scope)
    {
      *os << "&::" << ancestor->name () << "::_tao_class_id)"
          << be_uidt_nl;
    }
  // The ACE_NESTED_CLASS macro is necessary in this case.
  else
    {
      *os << "&ACE_NESTED_CLASS (::" << scope->name () << ", "
          << ancestor->local_name () << ")" << "::_tao_class_id)"
          << be_uidt_nl;
    }

  *os << ")" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_idt_nl;

  if (derived == ancestor)
    {
      *os << "retv = ACE_reinterpret_cast (void*, this);" << be_uidt_nl;
    }
  else
    {
      *os << "retv =" << be_idt_nl
          << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
          << "void *," << be_nl
          << "ACE_static_cast (" << be_idt << be_idt_nl
          << ancestor->full_name () << "_ptr," << be_nl
          << "this" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt << be_uidt << be_uidt_nl;
    }

  *os << "}" << be_uidt_nl
      << "else if ";

  return 0;
}



int
be_interface::downcast_helper (be_interface * /* derived */,
                               be_interface *base,
                               TAO_OutStream *os)
{
  // Abstract interfaces have no code generated on the skeleton side.
  if (base->is_abstract ())
    {
      return 0;
    }

  *os << "if (ACE_OS::strcmp (logical_type_id," << be_nl
      << "                    \""
      << base->repoID () << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return ACE_static_cast ("
      << base->full_skel_name () << "_ptr, this);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  return 0;
}


int
be_interface::gen_skel_helper (be_interface *derived,
                               be_interface *ancestor,
                               TAO_OutStream *os)
{
  // If derived and ancestor are same, skip it.
  if (derived == ancestor)
    {
      return 0;
    }

  // If an operation or an attribute is abstract (declared in an
  // abstract interface), we will either generate the full
  // definition (if there are no concrete interfaces between the
  // abstract ancestor and us) or, if there is a concrete ancestor
  // in between, we will catch its definition elsewhere in this
  // traversal.
  if (ancestor->is_abstract ())
    {
      return 0;
    }

  // Else generate code that does the cast to the appropriate type.

  if (ancestor->nmembers () > 0)
    {
      // If there are elements in ancestor scope i.e., any operations and
      // attributes defined by "ancestor", become methods on the derived class
      // which call the corresponding method of the base class by doing the
      // proper casting.

      for (UTL_ScopeActiveIterator si (ancestor, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          // Get the next AST decl node
          AST_Decl *d = si.item ();

          if (d->node_type () == AST_Decl::NT_op)
            {
              *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
                  << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // Generate the static method corresponding to this method.
                  *os << "static void " << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req, " << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
                      << ");" << be_uidt;
                }
              else
                { // Generate code in the inline file.
                  // Generate the static method corresponding to this method.
                  *os << "ACE_INLINE void "
                      << derived->full_skel_name () << "::"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL" << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl;
                  *os << ancestor->full_skel_name ()
                      << "_ptr impl = ("
                      << derived->full_skel_name ()
                      << "_ptr) obj;" << be_nl;
                  *os << ancestor->full_skel_name ()
                      << "::" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "req," << be_nl
                      << "(" << ancestor->full_skel_name ()
                      << "_ptr) impl," << be_nl
                      << "context" << be_nl
                      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
                      << ");" << be_uidt << be_uidt_nl
                      << "}";
                }
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);

              if (attr == 0)
                {
                  return -1;
                }

             // Start from current indentation level.
              os->indent ();
              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // Generate the static method corresponding to this method.
                  *os << "static void _get_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
                      << ");" << be_uidt;
                }
              else
                { // Generate code in the inline file.
                  // Generate the static method corresponding to this method.
                  *os << "ACE_INLINE void "
                      << derived->full_skel_name () << "::_get_"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL" << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl
                      << ancestor->full_skel_name ()
                      << "_ptr impl = ("
                      << derived->full_skel_name ()
                      << "_ptr) obj;" << be_nl;
                  *os << ancestor->full_skel_name ()
                      << "::_get_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "req," << be_nl
                      << "(" << ancestor->full_skel_name ()
                      << "_ptr) impl," << be_nl
                      << "context" << be_nl
                      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
                      << ");" << be_uidt << be_uidt_nl
                      << "}";
                }

              if (!attr->readonly ())
                {
                  *os << be_nl << be_nl;

                  if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                    {
                      // Generate the static method corresponding to
                      // this method.
                      *os << "static void _set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "TAO_ServerRequest &req," << be_nl
                          << "void *obj," << be_nl
                          << "void *context" << be_nl
                          << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
                          << ");" << be_uidt;
                    }
                  else
                    { // Generate code in the inline file.
                      // Generate the static method corresponding to
                      // this method.
                      *os << "ACE_INLINE void "
                          << derived->full_skel_name ()
                          << "::_set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "TAO_ServerRequest &req," << be_nl
                          << "void *obj," << be_nl
                          << "void *context" << be_nl
                          << "ACE_ENV_ARG_DECL" << be_uidt_nl
                          << ")" << be_uidt_nl
                          << "{" << be_idt_nl
                          << ancestor->full_skel_name ()
                          << "_ptr impl = ("
                          << derived->full_skel_name ()
                          << "_ptr) obj;" << be_nl;
                      *os << ancestor->full_skel_name ()
                          << "::_set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "req," << be_nl
                          << "(" << ancestor->full_skel_name ()
                          << "_ptr) impl," << be_nl
                          << "context" << be_nl
                          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
                          << ");" << be_uidt << be_uidt_nl
                          << "}";
                    }
                }
            }
        } // End of while.
    }

  return 0;
}

int
be_interface::copy_ctor_helper (be_interface *derived,
                                be_interface *base,
                                TAO_OutStream *os)
{
  // We can't call ourselves in a copy constructor, and
  // abstract interfaces don't exist on the skeleton side.
  if (derived == base || base->is_abstract ())
    {
      return 0;
    }

  if (base->is_nested ())
    {
      be_decl *scope;
      scope = be_scope::narrow_from_scope (base->defined_in ())->decl ();

      *os << "  ACE_NESTED_CLASS (POA_" << scope->name () << ","
          << base->local_name () << ") (rhs)," << be_nl;
    }
  else
    {
      *os << "  " << base->full_skel_name () << " (rhs)," << be_nl;
    }

  return 0;
}

int
be_interface::in_mult_inheritance_helper (be_interface *derived,
                                          be_interface *base,
                                          TAO_OutStream *)
{
  switch (derived->n_inherits ())
  {
    case 0:
      // No parent.
      derived->in_mult_inheritance (0);
      break;
    case 1:
      if (derived == base)
        {
          // Prevent indefinite recursion.
          derived->in_mult_inheritance (-1);
        }
      else
        {
          // One parent. We have the same characteristics as our base.
          derived->in_mult_inheritance (base->in_mult_inheritance ());
        }

      break;
    default:
      // Direct multiple inheritance.
      derived->in_mult_inheritance (1);
  }

  return 0;
}

int
be_interface::gen_abstract_init_helper (be_interface *node,
                                        be_interface *base,
                                        TAO_OutStream *os)
{
  if (node == base)
    {
      return 0;
    }

  *os << ",";

  if (base->is_nested ())
    {
      UTL_Scope *parent_scope = base->defined_in ();
      AST_Decl *parent_decl = ScopeAsDecl (parent_scope);

      *os << be_nl
          << "ACE_NESTED_CLASS ("
          << parent_decl->name () << ", "
          << base->local_name ()
          << ") (objref, _tao_collocated, servant)";
    }
  else
    {
      *os << be_nl
          << base->name ()
          << " (objref, _tao_collocated, servant)";
    }

  return 0;
}

void
be_interface::destroy (void)
{
  // Call the destroy methods of our base classes.
  this->AST_Interface::destroy ();
  this->be_scope::destroy ();
  this->be_type::destroy ();
}

int
be_interface::accept (be_visitor *visitor)
{
  return visitor->visit_interface (this);
}


TAO_CodeGen::CG_STATE
be_interface::next_state (TAO_CodeGen::CG_STATE current_state,
                          int is_extra_state)
{
  return this->strategy_->next_state (current_state,
                                      is_extra_state);
}

int
be_interface::has_extra_code_generation (TAO_CodeGen::CG_STATE current_state)
{
  return this->strategy_->has_extra_code_generation (current_state);
}

void
be_interface::original_interface (be_interface *original_interface)
{
  this->original_interface_ = original_interface;
}

be_interface *
be_interface::original_interface ()
{
  return this->original_interface_;
}

be_interface *
be_interface::replacement (void)
{
  return this->strategy_->replacement ();
}

int
be_interface::has_mixed_parentage (void)
{
  if (this->is_abstract_)
    {
      return 0;
    }

  if (this->has_mixed_parentage_ == -1)
    {
      this->analyze_parentage ();
    }

  return this->has_mixed_parentage_;
}

const char *
be_interface::base_proxy_impl_name (void)
{
  return this->strategy_->base_proxy_impl_name ();
}

const char *
be_interface::full_base_proxy_impl_name (void)
{
  return this->strategy_->full_base_proxy_impl_name ();
}

const char *
be_interface::remote_proxy_impl_name (void)
{
  return this->strategy_->remote_proxy_impl_name ();
}

const char *
be_interface::full_remote_proxy_impl_name (void)
{
  return this->strategy_->full_remote_proxy_impl_name ();
}

const char *
be_interface::thru_poa_proxy_impl_name (void)
{
  return this->strategy_->thru_poa_proxy_impl_name ();
}


const char *
be_interface::full_thru_poa_proxy_impl_name (void)
{
  return this->strategy_->full_thru_poa_proxy_impl_name ();
}


const char *
be_interface::direct_proxy_impl_name (void)
{
  return this->strategy_->direct_proxy_impl_name ();
}

const char *
be_interface::full_direct_proxy_impl_name (void)
{
  return this->strategy_->full_direct_proxy_impl_name ();
}


const char *
be_interface::base_proxy_broker_name (void)
{
  return this->strategy_->base_proxy_broker_name ();
}

const char *
be_interface::full_base_proxy_broker_name (void)
{
  return this->strategy_->full_base_proxy_broker_name ();
}


const char *
be_interface::remote_proxy_broker_name (void)
{
  return this->strategy_->remote_proxy_broker_name ();
}

const char *
be_interface::full_remote_proxy_broker_name (void)
{
  return this->strategy_->full_remote_proxy_broker_name ();
}


const char *
be_interface::strategized_proxy_broker_name (void)
{
  return this->strategy_->strategized_proxy_broker_name ();
}

const char *
be_interface::full_strategized_proxy_broker_name (void)
{
  return this->strategy_->full_strategized_proxy_broker_name ();
}

const char *
be_interface::client_enclosing_scope (void)
{
  return this->strategy_->client_scope ();
}

const char *
be_interface::flat_client_enclosing_scope (void)
{
  return this->strategy_->flat_client_scope ();
}

const char *
be_interface::server_enclosing_scope (void)
{
  return this->strategy_->server_scope ();
}


// Narrowing
IMPL_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_interface)
IMPL_NARROW_FROM_SCOPE (be_interface)
