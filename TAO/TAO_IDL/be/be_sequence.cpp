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
  be_state *s;       // state based code gen object

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

      // see if we have already generated a defn for ourselves
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

          cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE_CH); // set current code gen
                                                        // state
          cg->node (this);

          // if the base is itself a sequence, first generate its
          // definition. We will have to do this even for a sequence of
          // "strings".
          if (bt->node_type () == AST_Decl::NT_sequence)
            {
              if (bt->gen_client_header () == -1)
                {
                  ACE_ERROR ((LM_ERROR, "be_sequence: seq base type client hdr gen failed\n"));
                  return -1;
                }
            }
          cg->pop ();

          // now generate the sequence body
          cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CH);
          s = cg->make_state ();

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
          if (s->gen_code (bt, this) == -1)
            return -1;
          *ch << " value, CORBA::Boolean release=CORBA::B_FALSE);" << nl;
          *ch << local_name () << "(const " << local_name () << 
            " &); // copy constructor" << nl;
          *ch << "~" << this->local_name () << " (void);" << nl;
          *ch << this->local_name () << " &operator= (const " <<
            this->local_name () << " &);" << nl;
          *ch << "CORBA::ULong maximum (void) const;" << nl;
          *ch << "void length (CORBA::ULong);" << nl;
          *ch << "CORBA::ULong length (void) const;" << nl;
          if (s->gen_code (bt, this) == -1)
            return -1;
          *ch << " &operator[] (CORBA::ULong index);" << nl;
          *ch << "const ";
          if (s->gen_code (bt, this) == -1)
            return -1;
          *ch << " &operator[] (CORBA::ULong index) const;" << nl; 

          // generate the static allocbuf and freebuf methods
          *ch << "static ";
          if (s->gen_code (bt, this) == -1)
            return -1;
          *ch << " *allocbuf (CORBA::ULong nelems);" << nl;
          *ch << "static void freebuf (";
          if (s->gen_code (bt, this) == -1)
            return -1;
          *ch << " *);\n" ;
          ch->decr_indent ();
          *ch << "private:\n";
          ch->incr_indent ();
          *ch << "CORBA::ULong maximum_;" << nl;
          *ch << "CORBA::ULong length_;" << nl;
          if (s->gen_code (bt, this) == -1)
            return -1;
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
      if (this->is_nested ())
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
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line


  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CS); // set current code gen state

      cs = cg->client_stubs ();
      // pass info
      cg->outstream (cs);
      cg->node (this);
      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl; 
      *cs << "{\n";
      cs->incr_indent (0);
      if (this->gen_encapsulation () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_sequence:Error generating encapsulation\n\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "};" << nl;

      *cs << "static CORBA::TypeCode _tc__tc_" << this->flatname () << 
        " (CORBA::tk_sequence, sizeof (_oc_" <<  this->flatname () << 
        "), (unsigned char *) &_oc_" << this->flatname () << 
        ", CORBA::B_FALSE);" << nl;
      *cs << "CORBA::TypeCode_ptr " << this->tc_name () << " = &_tc__tc_" <<
        this->flatname () << ";\n\n";

      cg->pop ();
      this->cli_stub_gen_ = 1;

    }
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
          if (bt->gen_client_inline () == -1)
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
// generate typecode.
// Typecode for sequences comprises the enumerated value followed by the
// encapsulation of the parameters

int
be_sequence::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_sequence, // typecode kind" << nl;
  *cs << this->tc_size () << ", // encapsulation length\n";
  // now emit the encapsulation
  return this->gen_encapsulation ();
}

// generate encapsulation
// An encapsulation for ourselves will be necessary when we are part of some
// other IDL type and a typecode for that other type is being generated. This
// will comprise our typecode kind. IDL types with parameters will additionally
// have the encapsulation length and the entire typecode description

int
be_sequence::gen_encapsulation (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_type *bt; // base type

  os = cg->client_stubs ();
  os->indent (); // start from the current indentation level
  
  // XXXASG - byte order must be based on what m/c we are generating code -
  // TODO 
  *os << "0, // byte order" << nl; 

  // emit typecode of element type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt || (bt->gen_typecode () == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_sequence::gen_typecode - bad base type\n"));
      return -1;
    }

  //  emit the length
  os->indent ();
  *os << this->max_size () << ",\n";
  return 0;
}

// compute typecode size
long
be_sequence::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation length
  return 4 + 4 + this->tc_encap_len ();
}

long
be_sequence::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      be_type *bt; // base type

      this->encap_len_ = 4;  // holds the byte order flag
      // add the encapsulation length of our base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR ((LM_ERROR, 
                      "be_sequence::tc_encap_len - bad base type\n")); 
          return 0;
        }
      this->encap_len_ += bt->tc_encap_len ();
      this->encap_len_ += 4; // to hold the max size 

    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_sequence, AST_Sequence, be_type)
IMPL_NARROW_FROM_DECL (be_sequence)

  
