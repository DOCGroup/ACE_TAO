// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_sequence.cpp
//
// = DESCRIPTION
//    Extension of class AST_Sequence that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Sequence
 */
be_sequence::be_sequence (void)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_sequence::be_sequence (AST_Expression *v, AST_Type *t)
  : AST_Sequence (v, t),
    AST_Decl (AST_Decl::NT_sequence,
              NULL,
              NULL),
    seq_node_ (NULL)
{
  static char namebuf [200];
  UTL_ScopedName *n = NULL;
  AST_Decl *d;

  ACE_OS::memset (namebuf, '\0', 200);

  // we generate a name for ourselves. Start by generating a local name

  // check if we are bounded or unbounded. An expression value of 0 means
  // unbounded 
  if (v->ev ()->u.ulval == 0)
    {
      unbounded_ = I_TRUE;
      ACE_OS::sprintf (namebuf, "_seq_%s", t->local_name ()->get_string ());
    }
  else
    {
      unbounded_ = I_FALSE;
      ACE_OS::sprintf (namebuf, "_seq_%s_%d", t->local_name ()->get_string (),
                       v->ev ()->u.ulval); 
    }

  // now see if we have a fully scoped name.
  d = ScopeAsDecl (this->defined_in ());
  if (d != NULL)
    {
      n = (UTL_ScopedName *)d->name ()->copy ();
    }

  if (n)
    {
      n->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                    (namebuf), 1,
                                                    0, I_FALSE),
                                    NULL));
      this->set_name (n);
    }
  else // we are freely defined in the outermost scope OR we may be an
       // anonymous sequence base type of another sequence. This case is dealt
       // with below.
    this->set_name (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                        (namebuf), 1,
                                                        0, I_FALSE),
                                        NULL));

  if (t->node_type () == AST_Decl::NT_sequence)
    {
      // our element type is a sequence. Let it save us as its parent i.e., as
      // if we are its enclosing scope
      be_sequence::narrow_from_decl (t)->seq_node_ = this;
      
      // this also calls for assigning a fully scoped name to our base type
      // node which happens to be an anonymous sequence.
      be_sequence::narrow_from_decl (t)->compute_scoped_name ();
    }
  this->size_type (be_decl::VARIABLE); // a sequence data type is always
                                       // VARIABLE 
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

void 
be_sequence::compute_scoped_name (void)
{
  UTL_ScopedName *n = (UTL_ScopedName *)this->seq_node_->name ()->copy ();
  n->nconc (this->name ());
  this->set_name (n);
}

int
be_sequence::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_decl *d;        // temporary

  if (!this->cli_hdr_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      ch = cg->client_header ();

      ch->indent (); // start with the current indentation level

      // before generating the class definition, make sure that it is not
      // already done so in the same scope. For this, we check in the enclosing
      // scope if such a definition was generated
      if (this->defined_in () == NULL)
        {
          // we are a sequence generated inside of another sequence. Retrieve
          // our enclosing parent
          d = this->seq_node_;
        }
      else
        {
          // retrieve our enclosing scope node
          d = be_decl::narrow_from_decl (ScopeAsDecl (this->defined_in ()));
        }

      if (!d->lookup_seq_name (this->local_name ()))
        {
          d->add_seq_name (this->local_name ());

          *ch << "class " << this->local_name () << nl;
          *ch << "{" << nl;
          *ch << "public:\n";
          ch->incr_indent (0);

          // retrieve the base type
          bt = be_type::narrow_from_decl (this->base_type ());
          if (bt == NULL)
            {
              ACE_ERROR ((LM_ERROR, "be_sequence: base type NULL\n"));
              return -1;
            }

          cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE); // set current code gen state
          // if the base is itself a sequence, first generate its
          // definition. We will have to do this even for a sequence of
          // "strings".
          if (bt->be_type::gen_client_header () == -1)
            {
              ACE_ERROR ((LM_ERROR, "be_sequence: seq base type client hdr gen failed\n"));
              return -1;
            }
          cg->pop ();

          // now generate the sequence body
          cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY);
          // generate constructors
          ch->indent ();
          *ch << this->local_name () << " (void); // default constructor" << nl;
          // check whether we are bounded or not. Depending on that the mapping is
          // slightly different as shown below
          if (this->unbounded_)
            {
              *ch << local_name () << " (const CORBA::ULong max);" << nl;
              *ch << local_name () << " (CORBA::ULong max, CORBA::ULong length, " << nl;
            }
          else
            {
              // bounded seq does not take the "max" argument
              *ch << local_name () << " (CORBA::ULong length, " << nl;
            }

          *ch << "\t";
          bt->be_type::gen_client_header (); // get the type
          *ch << " *value, CORBA::Boolean release=CORBA::B_FALSE);" << nl;
          *ch << local_name () << "(const " << local_name () << 
            " &); // copy constructor" << nl;
          *ch << "~" << this->local_name () << " (void);" << nl;
          *ch << this->local_name () << " &operator= (const " <<
            this->local_name () << " &);" << nl;
          *ch << "CORBA::ULong maximum (void) const;" << nl;
          *ch << "void length (CORBA::ULong);" << nl;
          *ch << "CORBA::ULong length (void) const;" << nl;
          bt->be_type::gen_client_header ();
          *ch << " &operator[] (CORBA::ULong index);" << nl;
          *ch << "const ";
          bt->be_type::gen_client_header ();
          *ch << " &operator[] (CORBA::ULong index) const;" << nl; 

          // generate the static allocbuf and freebuf methods
          *ch << "static ";
          if (bt->node_type () == AST_Decl::NT_string)
            *ch << "char *";
          else
            bt->be_type::gen_client_header ();
          *ch << " *allocbuf (CORBA::ULong nelems);" << nl;
          *ch << "static void freebuf (";
          if (bt->node_type () == AST_Decl::NT_string)
            *ch << "char *";
          else
            bt->be_type::gen_client_header ();
          *ch << " *);\n" ;
          ch->decr_indent ();
          *ch << "private:\n";
          ch->incr_indent ();
          *ch << "CORBA::ULong maximum_;" << nl;
          *ch << "CORBA::ULong length_;" << nl;
          if (bt->node_type () == AST_Decl::NT_string)
            *ch << "char *";
          else
            bt->be_type::gen_client_header ();
          *ch << " *buffer_;" << nl;
          *ch << "CORBA::Boolean release_;\n";
          ch->decr_indent ();
          *ch << "};\n";

          // generate the var and out types
          this->gen_var_defn ();
          this->gen_out_defn ();
          cg->pop ();
        } // end of if (d->lookup)

      // Generate the typecode decl
      // All names in the root scope have length 2 (for the root and
      // ourself). The children have length greater than 2. Thus, if our name
      // length is 2 or less, we are outermost and our typecode decl must be
      // extern, else we are defined static inside the enclosing scope.
      if (this->name ()->length () > 2)
        {
          // we have a scoped name
          ch->indent ();
          *ch << "static CORBA::TypeCode_ptr " << this->tc_name
            ()->last_component () << ";\n\n";
        }
      else
        {
          // we are in the ROOT scope
          ch->indent ();
          *ch << "extern CORBA::TypeCode_ptr " << this->tc_name
            ()->last_component () << ";\n\n";
        }
      this->cli_hdr_gen_ = I_TRUE;
    } // if (cli_hdr_gen_)
  return 0;
}

int
be_sequence::gen_client_stubs (void)
{
  return 0;
}

int
be_sequence::gen_server_header (void)
{
  return 0;
}

int
be_sequence::gen_server_skeletons (void)
{
  return 0;
}

// Generates the client-side inline information
int 
be_sequence::gen_client_inline (void)
{
  if (!this->cli_inline_gen_)
    {
      be_type *bt;

      if (this->gen_var_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_sequence: _var impl code gen failed\n"));
          return -1;
        }
      if (this->gen_out_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_sequence: _out impl code gen failed\n"));
          return -1;
        }

      // if our base type is an anonymous sequence, we generate its client
      // inline methods
      bt = be_type::narrow_from_decl (this->base_type ());
      if (bt->node_type () == AST_Decl::NT_sequence)
        {
          if (bt->be_type::gen_client_inline () == -1)
            {
              ACE_ERROR ((LM_ERROR, 
                "be_sequence: inline code gen for base type seq failed\n"));
              return -1;
            }
        }
      this->cli_inline_gen_ = I_TRUE;
    }
  return 0;
}

// Generates the server-side inline
int 
be_sequence::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// generates the typecode
int
be_sequence::gen_typecode (void)
{
  return 0;
}

long
be_sequence::tc_encap_len (void)
{
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_sequence, AST_Sequence, be_type)
IMPL_NARROW_FROM_DECL (be_sequence)

  
