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
#include "be_exception.h"
#include "be_visitor.h"
#include "be_helper.h"
#include "be_stream_factory.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "ast_generator.h"
#include "ast_component.h"
#include "global_extern.h"
#include "idl_defines.h"
#include "nr_extern.h"

#include "ace/Process.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"

ACE_RCSID (be,
           be_interface,
           "$Id$")

// Default constructor.
be_interface::be_interface (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    AST_Interface (),
    be_scope (),
    be_type (),
    var_out_seq_decls_gen_ (0),
    skel_count_ (0),
    in_mult_inheritance_ (-1),
    strategy_ (0),
    original_interface_ (0),
    has_mixed_parentage_ (-1),
    session_component_child_ (-1)
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
                            bool local,
                            bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_interface,
              n),
    AST_Type (AST_Decl::NT_interface,
              n),
    UTL_Scope (AST_Decl::NT_interface),
    AST_Interface (n,
                   ih,
                   nih,
                   ih_flat,
                   nih_flat,
                   local,
                   abstract),
    be_scope (AST_Decl::NT_interface),
    be_type (AST_Decl::NT_interface,
             n),
    var_out_seq_decls_gen_ (0),
    skel_count_ (0),
    in_mult_inheritance_ (-1),
    original_interface_ (0),
    has_mixed_parentage_ (-1),
    session_component_child_ (-1)
{
  ACE_NEW (this->strategy_,
           be_interface_default_strategy (this));

  if (this->imported () || this->node_type () == AST_Decl::NT_valuetype)
    {
      return ;
    }

  if (this->is_defined ())
    {
      // Set the flag that says we have a interface in this IDL file.
      idl_global->interface_seen_ = true;

      if (abstract || this->has_mixed_parentage ())
        {
          // Set the flag for abstract interface seen in this IDL file.
          idl_global->abstract_iface_seen_ = true;
        }

      if (local)
        {
          // Set the flag for local interface seen in this IDL file.
          idl_global->local_iface_seen_ = true;
        }
      else
        {
          // Set the flag for non-local interface seen in this IDL file.
          idl_global->non_local_iface_seen_ = true;
        }
    }
  else
    {
      // Forward declared non-defined interface. Still gets a _var decl.
      idl_global->fwd_iface_seen_ = true;
    }
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
be_interface::flat_name (void)
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
      size_t namelen = ACE_OS::strlen (prefix);
      long first = true;
      long second = false;
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
              first = second = false;
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
                  first = false;
                }
              else
                {
                  second = true;
                }
            }
        }

      ACE_NEW (skelname,
               char [namelen+1]);
      skelname[0] = '\0';
      first = true;
      second = false;
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
              first = second = false;
            }

          // Print the identifier.
          item_name = j.item ()->get_string ();
          ACE_OS::strcat (skelname, item_name);

          if (first)
            {
              if (ACE_OS::strcmp (item_name, "") != 0)
                {
                  // Does not start with a "".
                  first = false;
                }
              else
                {
                  second = true;
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
int
be_interface::in_mult_inheritance (void)
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

void
be_interface::in_mult_inheritance (int mi)
{
  if (this->in_mult_inheritance_ == -1)
    {
      this->in_mult_inheritance_ = mi;
    }
}

void
be_interface::redefine (AST_Interface *from)
{
  be_interface *bi = be_interface::narrow_from_decl (from);
  this->var_out_seq_decls_gen_ = bi->var_out_seq_decls_gen_;
  this->has_mixed_parentage_ = bi->has_mixed_parentage_;

  if (bi->has_mixed_parentage_)
    {
      ACE_Unbounded_Queue<be_interface *> &q =
        be_global->mixed_parentage_interfaces;
      size_t slot = 0;
      be_interface **t = 0;

      // The queue of interfaces with mixed parentage must
      // replace each interface that has been forward
      // declared, since the pointer existing in the queue
      // will be deleted after redefine() returns.
      for (slot = 0; slot < q.size (); ++slot)
        {
          (void) q.get (t, slot);

          if (*t == bi)
            {
              (void) q.set (this, slot);
              break;
            }
        }
    }

  AST_Interface::redefine (from);
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
          << "::CORBA::Boolean _tao_collocated," << be_nl
          << "TAO_Abstract_ServantBase *servant," << be_nl
          << "TAO_ORB_Core *oc" << be_uidt_nl
          << ")" << be_nl
          << ": ";

      bool the_check =
        (this->has_mixed_parentage_
         && !this->is_abstract_
         && this->pd_n_inherits > 0
         && this->pd_inherits[0]->is_abstract ())
        || this->is_abstract_;

      if (this->has_mixed_parentage_)
        {
          *os << "::CORBA::"
              << (the_check ? "AbstractBase" : "Object")
              << " ("
              << be_idt << be_idt << be_idt_nl
              << "objref," << be_nl
              << "_tao_collocated," << be_nl
              << "servant"
              << (the_check ? "" : ", oc") << be_uidt_nl
              << ")" << be_uidt;

          if (!the_check)
            {
              *os << "," << be_nl
                  << "::CORBA::AbstractBase ("
                  << be_idt << be_idt_nl
                  << "objref," << be_nl
                  << "_tao_collocated," << be_nl
                  << "servant"
                  << (the_check ? ", oc" : "") << be_uidt_nl
                  << ")" << be_uidt;
            }

          int status =
            this->traverse_inheritance_graph (
                      be_interface::gen_abstract_init_helper,
                      os,
                      true
                    );

          if (status == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "be_interface::gen_stub_ctor - "
                          "inheritance graph traversal failed\n"));
            }
        }
      else
        {
          *os << be_idt;
        }

      if (the_check && !this->is_abstract_)
        {
          *os << "," << be_uidt_nl;
        }

      if (!this->is_abstract_
          && (!this->has_mixed_parentage_ || the_check))
        {
          *os << "::CORBA::Object (objref, _tao_collocated, servant, oc)"
        }

      *os << "," << be_nl
          << "the"<< this->base_proxy_broker_name () << "_ (0)"
          << be_uidt << be_uidt;

      *os << be_nl << "{" << be_idt_nl
          << "this->" << this->flat_name ()
          << "_setup_collocation ();";

      if (this->is_abstract ())
        {
          *os << be_nl
              << "ACE_UNUSED_ARG (oc);";
        }

      *os << be_uidt_nl
          << "}";
    }
}

// Generate the forward declarations and static methods used by the
// interface _var and _out template classes, as well as by the
// template sequence classes for object references.
void
be_interface:: gen_var_out_seq_decls (void)
{
  if (this->var_out_seq_decls_gen_ == 1)
    {
      return;
    }

  const char *lname = this->local_name ();
  TAO_OutStream *os = tao_cg->client_header ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the ifdefined macro for this interface.
  os->gen_ifdef_macro (this->flat_name (),
                       "var_out");

  *os << be_nl << be_nl
      << "class " << lname << ";" << be_nl
      << "typedef " << lname << " *" << lname << "_ptr;";

  *os << be_nl << be_nl
      << "typedef" << be_idt_nl
      << "TAO_Objref_Var_T<" << be_idt << be_idt_nl
      << lname << be_uidt_nl
      << ">" << be_uidt_nl
      << lname << "_var;" << be_uidt_nl << be_nl
      << "typedef" << be_idt_nl
      << "TAO_Objref_Out_T<" << be_idt << be_idt_nl
      << lname << be_uidt_nl
      << ">" << be_uidt_nl
      << lname << "_out;" << be_uidt;

  os->gen_endif ();

  this->var_out_seq_decls_gen_ = 1;
}

// ****************************************************************

TAO_IDL_Inheritance_Hierarchy_Worker::~TAO_IDL_Inheritance_Hierarchy_Worker (
    void
  )
{
}

// =================================================================

class TAO_IDL_Gen_OpTable_Worker
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  TAO_IDL_Gen_OpTable_Worker (const char *skeleton_name);

  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *os,
                    be_interface *base_interface);

private:
  const char *skeleton_name_;
};

TAO_IDL_Gen_OpTable_Worker::TAO_IDL_Gen_OpTable_Worker (
    const char *skeleton_name
  )
  : skeleton_name_ (skeleton_name)
{
}

int
TAO_IDL_Gen_OpTable_Worker::emit (be_interface *derived_interface,
                                  TAO_OutStream *os,
                                  be_interface *base_interface)
{
  // Generate entries for the derived class using the properties of its
  // ancestors.
  be_interface *bi = be_interface::narrow_from_decl (base_interface);
  return bi->gen_optable_entries (derived_interface,
                                  this->skeleton_name_,
                                  os);
}

// =================================================================

class Pure_Virtual_Regenerator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Pure_Virtual_Regenerator (be_visitor *visitor);

  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *os,
                    be_interface *base_interface);

private:
  be_visitor *visitor_;
};

Pure_Virtual_Regenerator::Pure_Virtual_Regenerator (
    be_visitor *visitor
  )
  : visitor_ (visitor)
{
}

// We don't use the output stream in the signature but instead
// pass the visitor member (which is always be_visitor_operation_ch)
// to each iterator item, and (almost, see below) everything else
// is automatic.
int
Pure_Virtual_Regenerator::emit (be_interface *derived_interface,
                                TAO_OutStream *,
                                be_interface *base_interface)
{
  if (derived_interface == base_interface)
    {
      return 0;
    }

  // If the parent is local, it will already have its operations declared
  // as pure virtual, and if it's abstract, its operations will already
  // be generated as pure virtual for the derived local interface.
  if (base_interface->is_local () || base_interface->is_abstract ())
    {
      return 0;
    }

  be_decl *d = 0;

  for (UTL_ScopeActiveIterator si (base_interface, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = be_decl::narrow_from_decl (si.item ());

      if (d->node_type () == AST_Decl::NT_op)
        {
          // Hack to force the generation of the pure virtual ' = 0'
          // at the end of the operation declaration.
          d->set_local (true);

          if (d->accept (this->visitor_) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                "(%N:%l) Pure_Virtual_Regenerator::emit - "
                                "visit base interface operation failed\n"),
                                -1);
            }

          d->set_local (false);
        }
    }

  return 0;
}

// =================================================================

int
be_interface::gen_operation_table (const char *flat_name,
                                   const char *skeleton_class_name)
{
  // Check out the op_lookup_strategy.
  switch (be_global->lookup_strategy ())
  {
    case BE_GlobalData::TAO_DYNAMIC_HASH:
      {
        this->skel_count_ = 0;
        // Init the outstream appropriately.
        TAO_OutStream *os = this->strategy_->get_out_stream ();

        // Start from current indentation level.
        os->indent ();

        // Start the table generation.
        *os << be_nl << be_nl
            << "static const TAO_operation_db_entry " << flat_name
            << "_operations [] = {" << be_idt_nl;

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
            << "::_is_a_skel, 0}," << be_nl;

        this->skel_count_++;

        *os << "{\"_non_existent\", &" << skeleton_class_name
            << "::_non_existent_skel, 0}," << be_nl;

        this->skel_count_++;

        *os << "{\"_component\", &" << skeleton_class_name
            << "::_component_skel, 0}," << be_nl;

        this->skel_count_++;

        *os << "{\"_interface\", &" << skeleton_class_name
            << "::_interface_skel, 0}," << be_nl;

        this->skel_count_++;

        *os << "{\"_repository_id\", &" << skeleton_class_name
            << "::_repository_id_skel, 0}" << be_uidt_nl;

        this->skel_count_++;

        *os << "};" << be_nl << be_nl;
        *os << "static const ::CORBA::Long _tao_" << flat_name
            << "_optable_size = sizeof (ACE_Hash_Map_Entry<const char *,"
            << " TAO::Operation_Skeletons>) * (" << (3 * this->skel_count_)
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
        // We must randomize this a bit in order to avoid problems with
        // processing more than one idl file (in separate processes) with
        // the same name (in different directories).
        char *temp_file = 0;
        ACE_NEW_RETURN (temp_file,
                        char [ACE_OS::strlen (idl_global->temp_dir ())
                              + 11 // The number of possible digits in
                                   // a 32-bit number plus a dot
                              + ACE_OS::strlen (flat_name)
                              + ACE_OS::strlen (".gperf")
                              + 1],
                        -1);

        // This degree of randomness is necessary because there was an
        // obscure chance of even this arriving at colliding filenames
        // on multiprocessor machines when the IDL compiler was run at
        // exactly the same time.
        ACE_RANDR_TYPE seed =
          (static_cast<ACE_RANDR_TYPE> (ACE_OS::time())
           + static_cast<ACE_RANDR_TYPE> (ACE_OS::getpid ()));
        ACE_OS::sprintf (temp_file,
                         "%s%d.%s.gperf",
                         idl_global->temp_dir (),
                         ACE_OS::rand_r (seed),
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

        *os << "_is_a,&"
            << skeleton_class_name
            << "::_is_a_skel, 0" << be_nl;

        this->skel_count_++;

        *os << "_non_existent,&"
            << skeleton_class_name
            << "::_non_existent_skel, 0" << be_nl;

        this->skel_count_++;

        *os << "_component,&"
            << skeleton_class_name
            << "::_component_skel, 0" << be_nl;
        this->skel_count_++;

        *os << "_interface,&"
            << skeleton_class_name
            << "::_interface_skel, 0" << be_nl;

        this->skel_count_++;

        *os << "_repository_id,&"
            << skeleton_class_name
            << "::_repository_id_skel, 0" << be_nl;
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

int
be_interface::convert_parent_ops (be_visitor *visitor)
{
  // Make sure the queues are empty.
  this->insert_queue.reset ();
  this->del_queue.reset ();

  // Insert ourselves in the queue.
  if (insert_queue.enqueue_tail (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_interface::convert_parent_ops - "
                          "error generating entries\n"),
                        -1);
    }

  // Traverse the graph.
  Pure_Virtual_Regenerator worker (visitor);

  if (this->traverse_inheritance_graph (worker, 0) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_interface::"
                          "convert_parent_ops - "
                          "codegen for base class operations failed\n"),
                        -1);
    }

  return 0;
}

// Output the header (type declaration and %%) to the gperf's input
// file.
void
be_interface::gen_gperf_input_header (TAO_OutStream *os)
{
  *os << "TAO_operation_db_entry {\n"
      << "\tchar * opname;" << "\n"
      << "\tTAO_Skeleton skel_ptr;" << "\n"
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
be_interface::gen_optable_entries (be_interface *derived_interface,
                                   const char *full_skeleton_name,
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
              // We are an operation node.
              *os << "{\"" << d->original_local_name () << "\", &"
                  << full_skeleton_name << "::"
                  << d->local_name () << "_skel,";

              if (be_global->gen_direct_collocation ())
                {
                  *os << " &"
                      << derived_interface->full_direct_proxy_impl_name ()
                      << "::" << d->local_name ();
                }
              else
                {
                  *os << " 0";
                }

              *os << "}," << be_nl;

              derived_interface->skel_count_++;
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr =
                AST_Attribute::narrow_from_decl (d);

              if (attr == 0)
                return -1;

              // Generate only the "get" entry if we are
              // readonly.
              *os << "{\"_get_" << d->original_local_name ()
                  << "\", &" << full_skeleton_name
                  << "::_get_" << d->local_name () << "_skel,";

              if (be_global->gen_direct_collocation ())
                {
                  *os << " &"
                      << derived_interface->full_direct_proxy_impl_name ()
                      << "::_get_" << d->local_name ();
                }
              else
                {
                  *os << " 0";
                }

              *os << "}," << be_nl;

              derived_interface->skel_count_++;

              if (!attr->readonly ())
                {
                  // The set method
                  *os << "{\"_set_" << d->original_local_name ()
                      << "\", &" << full_skeleton_name
                      << "::_set_" << d->local_name () << "_skel,";

                  if (be_global->gen_direct_collocation ())
                    {
                      *os << " &"
                          << derived_interface->full_direct_proxy_impl_name ()
                          << "::_set_" << d->local_name ();
                    }
                  else
                    {
                      *os << " 0";
                    }

                  *os << "}," << be_nl;

                  derived_interface->skel_count_++;
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

              // We are an operation node. We use the original
              // operation name, not the one with _cxx_ in it.
              *os << d->original_local_name () << ",&"
                  << full_skeleton_name << "::"
                  << d->local_name () << "_skel,";

              if (be_global->gen_direct_collocation ())
                {
                  *os << " &"
                      << derived_interface->full_direct_proxy_impl_name ();
                  *os << "::" << d->local_name ();
                }
              else
                {
                  *os << " 0";
                }

              *os << "\n";

              this->skel_count_++;
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr =
                AST_Attribute::narrow_from_decl (d);

              if (attr == 0)
                {
                  return -1;
                }

              // Generate only the "get" entry if we are readonly.
              *os << "_get_" << d->original_local_name () << ",&"
                  << full_skeleton_name << "::_get_"
                  << d->local_name () << "_skel,";

              if (be_global->gen_direct_collocation ())
                {
                  *os << " &"
                      << derived_interface->full_direct_proxy_impl_name ()
                      << "::_get_" << d->local_name ();
                }
              else
                {
                  *os << " 0";
                }

              *os << "\n";

              this->skel_count_++;

              if (!attr->readonly ())
                {
                  // The set method
                  *os << "_set_" << d->original_local_name () << ",&"
                      << full_skeleton_name << "::_set_"
                      << d->local_name () << "_skel,";

                  if (be_global->gen_direct_collocation ())
                    {
                      *os << " &"
                          << derived_interface->full_direct_proxy_impl_name ()
                          << "::_set_" << d->local_name ();
                    }
                  else
                    {
                      *os << " 0";
                    }

                  *os << "\n";

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
be_interface::gen_collocated_skel_body (be_interface *derived,
                                        be_interface *ancestor,
                                        AST_Decl *d,
                                        const char *prefix,
                                        bool /* direct */,
                                        UTL_ExceptList *list,
                                        TAO_OutStream *os)
{
  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the static method corresponding to this method.
  *os << be_nl << be_nl
      << "ACE_INLINE void" << be_nl
      << derived->full_direct_proxy_impl_name ()
      << "::" << prefix << d->local_name () << " (" << be_idt << be_idt_nl
      << "TAO_Abstract_ServantBase *servant," << be_nl
      << "TAO::Argument ** args," << be_nl
      << "int num_args" << env_decl << be_uidt_nl
      << ")";

  be_interface::gen_throw_spec (list, os);

  *os << be_uidt_nl
      << "{" << be_idt_nl
      << ancestor->full_direct_proxy_impl_name ()
      << "::" << prefix << d->local_name () << " (" << be_idt << be_idt_nl
      << "servant," << be_nl
      << "args," << be_nl
      << "num_args" << env_arg << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}"<< be_nl;


}

void
be_interface::analyze_parentage (void)
{
  if (this->has_mixed_parentage_ != -1)
    {
      return;
    }

  this->has_mixed_parentage_ = 0;

  for (long i = 0; i < this->pd_n_inherits; ++i)
    {
      be_interface *parent =
        be_interface::narrow_from_decl (this->pd_inherits[i]);

      if (parent->is_abstract () || parent->has_mixed_parentage ())
        {
          this->has_mixed_parentage_ = 1;
          break;
        }
    }

  AST_Decl::NodeType nt = this->node_type ();
  bool can_be_mixed = nt == AST_Decl::NT_interface
                          || nt == AST_Decl::NT_component
                          || nt == AST_Decl::NT_home;

  if (this->has_mixed_parentage_ == 1 && can_be_mixed && this->is_defined ())
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
                                          bool abstract_paths_only)
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
    bool abstract_paths_only
  )
{
  AST_Interface *intf = 0;  // element inside the queue

  if (!this->insert_queue.is_empty ())
    {
      // Dequeue the element at the head of the queue.
      if (this->insert_queue.dequeue_head (intf))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "dequeue_head failed\n"),
                            -1);
        }

      // If we are doing a component, we check for a parent.
      if (intf->node_type () == AST_Decl::NT_component)
        {
          (void) this->insert_non_dup (be_global->ccmobject ());

          AST_Component *base =
            AST_Component::narrow_from_decl (intf)->base_component ();

          if (base != 0)
            {
              (void) this->insert_non_dup (base);

              long n_supports = base->n_supports ();
              AST_Interface **supports = base->supports ();

              for (long j = 0; j < n_supports; ++j)
                {
                  (void) this->insert_non_dup (supports[j],
                                               abstract_paths_only);
                }
            }
        }

      (void) this->insert_non_dup (intf, abstract_paths_only);
    }

  // Do until queue is empty.
  while (!this->insert_queue.is_empty ())
    {
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
      << be_idt_nl
      << ": public TAO_Perfect_Hash_OpTable" << be_uidt_nl
      << "{" << be_nl
      << "private:" << be_idt_nl
      << "unsigned int hash (const char *str, unsigned int len);"
      << be_uidt_nl << be_nl
      << "public:" << be_idt_nl
      << "const TAO_operation_db_entry * lookup "
      << "(const char *str, unsigned int len);"
      << be_uidt_nl
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
      << be_idt_nl
      << ": public TAO_Binary_Search_OpTable" << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "const TAO_operation_db_entry * lookup (const char *str);"
      << be_uidt_nl
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
      << be_idt_nl
      << ": public TAO_Linear_Search_OpTable" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "const TAO_operation_db_entry * lookup (const char *str);"
      << be_uidt_nl
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
                                    "-F 0,0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname -L C++"
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
                                    "-F 0,0,0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname -L C++"
                                    " "
                                    "-Z TAO_%s_Binary_Search_OpTable"
                                    " "
                                    "-N lookup",
                                    idl_global->gperf_path (),
                                    flat_name);
      break;

      // Linear search methods from GPERF. Everything and the -Z flag.
    case BE_GlobalData::TAO_LINEAR_SEARCH:
      process_options.command_line ("%s"
                                    " "
                                    "-b"
                                    "  "
                                    "-m -M -J -c -C"
                                    " "
                                    "-D -E -T -f 0"
                                    " "
                                    "-F 0,0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname -L C++"
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

  *os << be_nl
      << "static TAO_" << flat_name << "_Perfect_Hash_OpTable"
      << " "
      << "tao_" << flat_name << "_optable;";
}

// Create an instance of the binary search optable.
void
be_interface::gen_binary_search_instance (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << be_nl
      << "static TAO_" << flat_name << "_Binary_Search_OpTable"
      << " "
      << "tao_" << flat_name << "_optable;";
}


// Create an instance of this perfect hash table.
void
be_interface::gen_linear_search_instance (const char *flat_name)
{
  // Outstream.
  TAO_OutStream *os = this->strategy_->get_out_stream ();

  *os << be_nl
      << "static TAO_" << flat_name << "_Linear_Search_OpTable"
      << " "
      << "tao_" << flat_name << "_optable;";
}

int
be_interface::is_a_helper (be_interface * /*derived*/,
                           be_interface *bi,
                           TAO_OutStream *os)
{
  // Emit the comparison code.
  *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
      << "value," << be_nl
      << "\"" << bi->repoID () << "\"" << be_uidt_nl
      << ") ||" << be_uidt_nl;

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
                  << "// " << __FILE__ << ":" << __LINE__;

              *os << be_nl << be_nl;

              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // Generate the static method corresponding to this method.
                  *os << "static void" << be_nl
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest & server_request, " << be_nl
                      << "void * servant_upcall," << be_nl
                      << "void * servant" << env_dflts << be_uidt_nl
                      << ");" << be_uidt;
                }
              else
                { // Generate code in the inline file.
                  // Generate the static method corresponding to this method.
                  *os << "ACE_INLINE" << be_nl
                      << "void" << be_nl
                      << derived->full_skel_name () << "::"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest & server_request," << be_nl
                      << "void * servant_upcall," << be_nl
                      << "void * servant" << env_decl << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl;

                  *os << ancestor->full_skel_name ()
                      << " * const impl = static_cast<"
                      << derived->full_skel_name ()
                      << " *> (servant);" << be_nl;

                  *os << ancestor->full_skel_name ()
                      << "::" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "server_request," << be_nl
                      << "servant_upcall," << be_nl
                      << "impl" << env_arg << be_uidt_nl
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

              *os << be_nl << be_nl;

              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // Generate the static method corresponding to this method.
                  *os << "static void" << be_nl
                      << "_get_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest & server_request," << be_nl
                      << "void * servant_upcall," << be_nl
                      << "void * servant" << env_dflts << be_uidt_nl
                      << ");" << be_uidt;
                }
              else
                { // Generate code in the inline file.
                  // Generate the static method corresponding to this method.
                  *os << "ACE_INLINE" << be_nl
                      << "void" << be_nl
                      << derived->full_skel_name () << "::_get_"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest & server_request," << be_nl
                      << "void * servant_upcall," << be_nl
                      << "void * servant" << env_decl << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl;

                  *os << ancestor->full_skel_name ()
                      << " * const impl = static_cast<"
                      << derived->full_skel_name ()
                      << " *> (servant);" << be_nl;

                  *os << ancestor->full_skel_name ()
                      << "::_get_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "server_request," << be_nl
                      << "servant_upcall," << be_nl
                      << "impl" << env_arg << be_uidt_nl
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
                      *os << "static void" << be_nl
                          << "_set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "TAO_ServerRequest & server_request," << be_nl
                          << "void * servant_upcall," << be_nl
                          << "void * servant" << env_dflts << be_uidt_nl
                          << ");" << be_uidt;
                    }
                  else
                    { // Generate code in the inline file.
                      // Generate the static method corresponding to
                      // this method.
                      *os << "ACE_INLINE" << be_nl
                          << "void" << be_nl
                          << derived->full_skel_name ()
                          << "::_set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "TAO_ServerRequest & server_request," << be_nl
                          << "void * servant_upcall," << be_nl
                          << "void * servant" << env_decl << be_uidt_nl
                          << ")" << be_uidt_nl
                          << "{" << be_idt_nl;

                      *os << ancestor->full_skel_name ()
                          << " * const impl = static_cast<"
                          << derived->full_skel_name ()
                          << " *> (servant);" << be_nl;

                      *os << ancestor->full_skel_name ()
                          << "::_set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "server_request," << be_nl
                          << "servant_upcall," << be_nl
                          << "impl" << env_arg << be_uidt_nl
                          << ");" << be_uidt << be_uidt_nl
                          << "}";
                    }
                }
            }
        } // End of FOR.
    }

  return 0;
}

int
be_interface::gen_colloc_op_decl_helper (be_interface *derived,
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
  if (ancestor->is_abstract () || ancestor->nmembers () == 0)
    {
      return 0;
    }

  UTL_ExceptList *list = 0;

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

          // Generate the static method corresponding to this method.
          *os << "static void" << be_nl
              << d->local_name () << " (" << be_idt << be_idt_nl
              << "TAO_Abstract_ServantBase *servant, " << be_nl
              << "TAO::Argument ** args," << be_nl
              << "int num_args" << env_dflts << be_uidt_nl
              << ")";

          list = be_operation::narrow_from_decl (d)->exceptions ();
          be_interface::gen_throw_spec (list, os);

          *os << ";";
        }
      else if (d->node_type () == AST_Decl::NT_attr)
        {
          AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);

          if (attr == 0)
            {
              return -1;
            }

          // Generate the static method corresponding to this method.
          *os << "static void" << be_nl
              << "_get_" << d->local_name () << " (" << be_idt << be_idt_nl
              << "TAO_Abstract_ServantBase *servant, " << be_nl
              << "TAO::Argument ** args," << be_nl
              << "int num_args" << env_dflts << be_uidt_nl
              << ")";

          list = attr->get_get_exceptions ();
          be_interface::gen_throw_spec (list, os);

          *os << ";";

          if (!attr->readonly ())
            {
              *os << be_nl << be_nl;

              // Generate the static method corresponding to
              // this method.
              *os << "static void" << be_nl
                  << "_set_" << d->local_name () << " (" << be_idt << be_idt_nl
                  << "TAO_Abstract_ServantBase *servant, " << be_nl
                  << "TAO::Argument ** args," << be_nl
                  << "int num_args" << env_dflts << be_uidt_nl
                  << ")";

              list = attr->get_set_exceptions ();
              be_interface::gen_throw_spec (list, os);

              *os << ";";
            }
        }
    }

  return 0;
}

int
be_interface::gen_colloc_op_defn_helper (be_interface *derived,
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
  if (ancestor->is_abstract () || ancestor->nmembers () == 0)
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_operation *op = 0;

  for (UTL_ScopeActiveIterator si (ancestor, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      // Get the next AST decl node
      d = si.item ();

      if (d->node_type () == AST_Decl::NT_op)
        {
          op = be_operation::narrow_from_decl (d);

          if (be_global->gen_direct_collocation ())
            {
              be_interface::gen_collocated_skel_body (derived,
                                                      ancestor,
                                                      d,
                                                      "",
                                                      true,
                                                      op->exceptions (),
                                                      os);
            }
        }
      else if (d->node_type () == AST_Decl::NT_attr)
        {
          AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);

          if (attr == 0)
            {
              return -1;
            }

          if (be_global->gen_direct_collocation ())
            {
              be_interface::gen_collocated_skel_body (
                  derived,
                  ancestor,
                  d,
                  "_get_",
                  true,
                  attr->get_get_exceptions (),
                  os
                );
            }

          if (!attr->readonly ())
            {
              if (be_global->gen_direct_collocation ())
                {
                  be_interface::gen_collocated_skel_body (
                      derived,
                      ancestor,
                      d,
                      "_set_",
                      true,
                      attr->get_set_exceptions (),
                      os
                    );
                }
            }
        }
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

  *os << "," << be_idt_nl;

  bool is_rh_base =
    (ACE_OS::strcmp (base->flat_name (), "Messaging_ReplyHandler") == 0);

  if (is_rh_base)
    {
      *os << "::POA_Messaging::ReplyHandler (rhs)";
    }
  else if (base->is_nested ())
    {
      be_decl *scope;
      scope = be_scope::narrow_from_scope (base->defined_in ())->decl ();

      *os << "POA_" << scope->name () << "::"
          << base->local_name () << " (rhs)";
    }
  else
    {
      *os << base->full_skel_name () << " (rhs)";
    }

  *os << be_uidt;

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

  *os << "," << be_nl;

  if (base->is_nested ())
    {
      UTL_Scope *parent_scope = base->defined_in ();
      AST_Decl *parent_decl = ScopeAsDecl (parent_scope);

      *os << ""
          << parent_decl->name () << "::"
          << base->local_name ()<< " (" << be_idt << be_idt_nl;
    }
  else
    {
      *os << base->name () << " (" << be_idt << be_idt_nl;
    }

  *os << "objref," << be_nl
      << "_tao_collocated," << be_nl
      << "servant" << be_uidt_nl
      << ")" << be_uidt;


  return 0;
}

void
be_interface::gen_throw_spec (UTL_ExceptList *list,
                              TAO_OutStream *os)
{
  const char *throw_spec_open = "throw (";
  const char *throw_spec_close = ")";

  if (!be_global->use_raw_throw ())
    {
      throw_spec_open = "ACE_THROW_SPEC ((";
      throw_spec_close = "))";
    }

  *os << be_nl << throw_spec_open;
  *os << be_idt_nl << "::CORBA::SystemException";

  // Initialize an iterator to iterate thru the exception list.
  for (UTL_ExceptlistActiveIterator ei (list);
       !ei.is_done ();
       ei.next ())
    {
      *os << "," << be_nl
          << "::" << ei.item ()->name ();
    }

  *os << be_uidt_nl
      << throw_spec_close << be_uidt;
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

int
be_interface::session_component_child (void)
{
  if (this->session_component_child_ == -1)
    {
      // We are looking only for executor interfaces.
      if (!this->is_local_)
        {
          this->session_component_child_ = 0;
          return this->session_component_child_;
        }

      Identifier tail_id ("SessionComponent");
      UTL_ScopedName tail (&tail_id, 0);
      Identifier head_id ("Components");
      UTL_ScopedName sn (&head_id, &tail);

      AST_Decl *session_component =
        const_cast<be_interface*> (this)->scope ()->lookup_by_name (&sn,
                                                                    true);

      tail_id.destroy ();
      head_id.destroy ();

      // If Components::SessionComponent is not in the AST, we are
      // barking up the wrong tree.
      if (session_component == 0)
        {
          this->session_component_child_ = 0;
          return this->session_component_child_;
        }

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          AST_Decl *tmp = this->pd_inherits[i];

          if (tmp == session_component)
            {
              this->session_component_child_ = 1;
              return this->session_component_child_;
            }
        }

      this->session_component_child_ = 0;
    }

  return this->session_component_child_;
}

bool
be_interface::is_event_consumer (void)
{
  return
    this->pd_n_inherits == 1
    && ACE_OS::strcmp (this->pd_inherits[0]->full_name (),
                       "Components::EventConsumerBase") == 0;
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
