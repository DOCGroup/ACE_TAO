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
  : mt_ (be_sequence::MNG_UNKNOWN)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_sequence::be_sequence (AST_Expression *v, AST_Type *t)
  : AST_Sequence (v, t),
    AST_Decl (AST_Decl::NT_sequence,
              NULL,
              NULL),
    mt_ (be_sequence::MNG_UNKNOWN)
{
  // check if we are bounded or unbounded. An expression value of 0 means
  // unbounded
  if (v->ev ()->u.ulval == 0)
    {
      this->unbounded_ = I_TRUE;
    }
  else
    {
      this->unbounded_ = I_FALSE;
    }

  this->size_type (be_decl::VARIABLE); // a sequence data type is always
                                       // VARIABLE
}

// helper to create_name
const char *
be_sequence::gen_name (void)
{
  char namebuf [NAMEBUFSIZE];
  be_type *bt; // base type;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);  // reset the buffer
  // retrieve the base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_name - "
                         "bad base type\n"),
                        0);
    }
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      // our base type is an anonymous sequence
      be_sequence *seq;
      seq = be_sequence::narrow_from_decl (bt);
      if (!seq)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_name - "
                             "error converting base type to sequence\n"),
                            0);
        }
      seq->set_defined_in (this); // set ourselves as its parent
      this->fe_add_sequence (seq); // add the child to our scope
      ACE_OS::sprintf (namebuf, "_tao_seq_%s", seq->gen_name ());
    }
  else
    {
      ACE_OS::sprintf (namebuf, "_tao_seq_%s", bt->local_name ()->get_string ());
    }
  // append the size (if any)
  if (!this->unbounded_)
    {
      ACE_OS::sprintf (namebuf, "%s_%d", namebuf, this->max_size ()->ev
                       ()->u.ulval);
    }
  return namebuf;
}

// create a name for ourselves
int
be_sequence::create_name (void)
{
  static char namebuf [NAMEBUFSIZE];
  UTL_ScopedName *n = NULL;
  be_decl *scope; // scope in which we are defined

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);  // reset the buffer
  ACE_OS::strcpy (namebuf, this->gen_name ()); // generate a local name

  // now see if we have a fully scoped name and if so, generate one
  scope = be_scope::narrow_from_scope (this->defined_in ())->decl ();
  if (scope)
    {
      // make a copy of the enclosing scope's  name
      n = (UTL_ScopedName *)scope->name ()->copy () ;

      // add our local name as the last component
      n->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                    (namebuf), 1,
                                                    0, I_FALSE),
                                    NULL));
      // set the fully scoped name
      this->set_name (n);
    }
  else
    {
      // We better be not here because we must be inside some scope,
      // atleast the ROOT scope.
      return -1;
    }
  return 0;
}

// Does this sequence have a managed type sequence element?
be_sequence::MANAGED_TYPE
be_sequence::managed_type (void)
{
  if (this->mt_ == be_sequence::MNG_UNKNOWN) // not calculated yet
    {
      be_type  *bt, *prim_type; // base types

      bt = be_type::narrow_from_decl (this->base_type ());

      if (bt->node_type () == AST_Decl::NT_typedef)
        {
          // get the primitive base type of this typedef node
          be_typedef *t = be_typedef::narrow_from_decl (bt);
          prim_type = t->primitive_base_type ();
        }
      else
        prim_type = bt;

      // determine if we need a managed type and which one
      switch (prim_type->node_type ())
        {
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          this->mt_ = be_sequence::MNG_OBJREF;
          break;
        case AST_Decl::NT_string:
          this->mt_ = be_sequence::MNG_STRING;
          break;
        case AST_Decl::NT_pre_defined:
          {
            be_predefined_type *bpd = be_predefined_type::narrow_from_decl
              (prim_type);
            if (bpd->pt () == AST_PredefinedType::PT_pseudo)
              {
                this->mt_ = be_sequence::MNG_OBJREF;
              }
            else
              {
                this->mt_ = be_sequence::MNG_NONE;
              }
          }
          break;
        default:
          this->mt_ = be_sequence::MNG_NONE;
        } // end of switch
    }
  return this->mt_;
}

int
be_sequence::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_state *s;       // state based code gen object

  if (!this->cli_hdr_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      // first create a name for ourselves. We defer name creation for
      // ourselves to this point since named sequences should get the name
      // of the typedef node, else some other technique of name generation
      // should be used.
      if (this->create_name () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_sequence::gen_client_header - name creation failed\n"),
                            -1);
        }

      ch = cg->client_header ();

      // generate the ifdefined macro for the sequence type
      ch->gen_ifdef_macro (this->flatname ());

      ch->indent (); // start with the current indentation level

      *ch << "// *************************************************************"
          << nl;
      *ch << "// class " << this->local_name () << nl;
      *ch << "// *************************************************************"
              << nl << nl;

      *ch << "class " << this->local_name () << nl;
      *ch << "{" << nl;
      *ch << "public:\n";
      ch->incr_indent (0);

      // retrieve the base type since we may need to do some code
      // generation for the base type.
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence.cpp - "
                             "Bad base type\n"),
                            -1);
        }

      cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE_CH); // set current code gen
      // state
      s = cg->make_state ();
      if (!s || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence.cpp - "
                             "base type codegen failed\n"),
                            -1);
        }
      cg->pop ();

      // now generate the sequence body
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CH);
      s = cg->make_state ();
      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_header - "
                             "bad state\n"),
                            -1);
        }

      // generate constructors
      ch->indent ();
      *ch << this->local_name () << " (void); // default constructor" << nl;
      // check whether we are bounded or not. Depending on that the mapping is
      // slightly different as shown below
      if (this->unbounded_)
        {
          *ch << local_name () << " (CORBA::ULong max);" << nl;
          *ch << local_name () << " (CORBA::ULong max, CORBA::ULong length, " << nl;
        }
      else
        {
          // bounded seq does not take the "max" argument
          *ch << local_name () << " (CORBA::ULong length, " << nl;
        }

      *ch << "\t";
      // generate the type info for the element type
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }

      *ch << " *value, CORBA::Boolean release=CORBA::B_FALSE);" << nl;
      *ch << local_name () << "(const " << local_name () <<
        " &); // copy constructor" << nl;
      *ch << "~" << this->local_name () << " (void);" << nl;
      *ch << this->local_name () << " &operator= (const " <<
        this->local_name () << " &);" << nl;
      *ch << "CORBA::ULong maximum (void) const;" << nl;
      *ch << "void length (CORBA::ULong);" << nl;
      *ch << "CORBA::ULong length (void) const;" << nl;

      // we need a new state here because the [] operator for strings and obj
      // references returns a managed type as opposed to the normal type
      cg->push (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH);
      s = cg->make_state ();
      if (!s || s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }
      *ch << " &operator[] (CORBA::ULong index);" << nl;
      *ch << "const ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }
      *ch << " &operator[] (CORBA::ULong index) const;" << nl;
      cg->pop (); // back to the previous state

      s = cg->make_state ();
      // generate the static allocbuf and freebuf methods
      *ch << "static ";
      if (!s || s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }
      *ch << " *allocbuf (CORBA::ULong nelems);" << nl;
      *ch << "static void freebuf (";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }
      *ch << " *);\n" ;
      ch->decr_indent ();
      *ch << "private:\n";
      ch->incr_indent ();
      *ch << "CORBA::ULong maximum_;" << nl;
      *ch << "CORBA::ULong length_;" << nl;
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }

      *ch << " *buffer_;" << nl;
      *ch << "CORBA::Boolean release_;\n";
      // any managed type ?
      switch (this->managed_type ())
        {
        case be_sequence::MNG_OBJREF:
          ch->indent ();
          *ch << "TAO_ObjRefMngType *mgr_;\n";
          break;
        case be_sequence::MNG_STRING:
          ch->indent ();
          *ch << "TAO_StrMngType *mgr_;\n";
          break;
        }
      ch->decr_indent ();
      *ch << "};\n";
      ch->indent ();
      *ch << "typedef " << this->local_name () << "* "
	  << this->local_name () << "_ptr;\n";

      // Generate the typecode decl
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

      ch->gen_endif (); // endif macro

      // generate the ifdefined macro for the var type
      ch->gen_ifdef_macro (this->flatname (), "_var");

      // generate the var and out types
      if (this->gen_var_defn () == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_sequence::"
                           "gen_client_header - "
                           "codegen for _var failed\n"),
                          -1);
      }

      ch->gen_endif ();

      // generate the ifdefined macro for the var type
      ch->gen_ifdef_macro (this->flatname (), "_out");

      if (this->gen_out_defn () == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_sequence::"
                           "gen_client_header - "
                           "codegen for _out failed\n"),
                          -1);
      }
      ch->gen_endif ();

      cg->pop ();

      this->cli_hdr_gen_ = I_TRUE;
    } // if (cli_hdr_gen_)
  return 0;
}

int
be_sequence::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  be_type *bt; // base type
  be_state *s; //state object

  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      cs = cg->client_stubs (); // retrieve the client stubs stream

      // retrieve base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_stubs - "
                             "bad type\n"),
                            -1);
        }

      cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
      s = cg->make_state ();

      // generate stubs for our base type if it itself is a sequence  or we
      // need to generate code for our managed types
      if (!s || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_stubs - "
                             "state based codegen failed\n"),
                            -1);
        }

      cg->pop ();

      // generate the methods of the sequence C++ mapping
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CS);
      s = cg->make_state ();
      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_stubs - "
                             "bad state\n"),
                            -1);
        }


      *cs << "// *************************************************************"
          << nl;
      *cs << "// class " << this->name () << nl;
      *cs << "// *************************************************************\n\n";

      // copy constructor
      cs->indent ();
      *cs << "// copy constructor" << nl;
      *cs << this->name () << "::" << this->local_name () <<
        " (const " << this->name () << " &seq)" << nl;
      *cs << "\t: maximum_ (seq.maximum_)," << nl;
      *cs << "\t  length_ (seq.length_)," << nl;
      *cs << "\t  buffer_ (" << this->name () << "::allocbuf (seq.maximum_)),"
          << nl;
      *cs << "\t  release_ (1) // we always own it" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // copy each element
      switch (this->managed_type ())
        {
        case be_sequence::MNG_OBJREF:
          {
            *cs << "this->mgr_ = new " << this->name () <<
              "::TAO_ObjRefMngType [seq.maximum_];" << nl;
            *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
            *cs << "{" << nl;
            *cs << "\tthis->buffer_[i] = " << bt->name () << "::_duplicate ("
                << "seq.buffer_[i]);" << nl;
            *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
            *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
            *cs << "}\n";
          }
          break;
        case be_sequence::MNG_STRING:
          {
            *cs << "this->mgr_ = new " << this->name () <<
              "::TAO_StrMngType [seq.maximum_];" << nl;
            *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
            *cs << "{" << nl;
            *cs << "\tthis->buffer_[i] = CORBA::string_dup (" <<
              "seq.buffer_[i]);" << nl;
            *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
            *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
            *cs << "}\n";
          }
          break;
        default: // all other types are self managed, just assign them.
          *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
          *cs << "\tthis->buffer_[i] = seq.buffer_[i];\n";
        }
      cs->decr_indent ();
      *cs << "}\n\n";

      // constructor only for unbounded seq. This takes in "max length"
      if (this->unbounded_)
        {
          cs->indent ();
          *cs << "// constructor for unbounded seq" << nl;
          *cs << this->name () << "::" << this->local_name () <<
            "(CORBA::ULong max )" << nl;
          *cs << "\t: maximum_ (max)," << nl;
          *cs << "\t  length_ (0)," << nl;
          *cs << "\t  buffer_ (" << this->name () << "::allocbuf (max))," << nl;
          *cs << "\t  release_ (1) // owns" << nl;
          *cs << "{\n";
          cs->incr_indent ();
          // copy each element
          switch (this->managed_type ())
            {
            case be_sequence::MNG_OBJREF:
              {
                *cs << "this->mgr_ = new " << this->name () <<
                  "::TAO_ObjRefMngType [this->maximum_];" << nl;
                *cs << "for (CORBA::ULong i=0; i < max; i++)" << nl;
                *cs << "{" << nl;
                *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
                *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
                *cs << "}" << nl;
              }
              break;
            case be_sequence::MNG_STRING:
              {
                *cs << "this->mgr_ = new " << this->name () <<
                  "::TAO_StrMngType [this->maximum_];" << nl;
                *cs << "for (CORBA::ULong i=0; i < max; i++)" << nl;
                *cs << "{" << nl;
                *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
                *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
                *cs << "}" << nl;
              }
              break;
            }
          *cs << "\n";
          cs->decr_indent ();
          *cs << "}\n\n";
        }

      // constructor that takes in the data buffer
      cs->indent ();
      *cs << "// constructor from data buffer" << nl;
      *cs << this->name () << "::" << this->local_name ();
      // depending on whether we are bounded or not, the constructor has
      // different sets of parameters
      if (this->unbounded_)
        {
          *cs << " (CORBA::ULong max, CORBA::ULong length, " << nl;
        }
      else
        {
          // bounded seq does not take the "max" argument
          *cs << " (CORBA::ULong length, " << nl;
        }
      *cs << "\t";
      if (s->gen_code (bt, this) == -1)
        return -1;
      *cs << " *value, CORBA::Boolean release)" << nl;
      // for unbounded we have the additional max parameter
      if (this->unbounded_)
        {
          *cs << "\t: maximum_ (max)," << nl;
        }
      else
        {
          *cs << "\t: maximum_ (" << this->max_size () << ")," << nl;
        }
      *cs << "\t  length_ (length)," << nl;
      *cs << "\t  buffer_ (value)," << nl;
      *cs << "\t  release_ (release) // ownership depends on release" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // copy each element
      switch (this->managed_type ())
        {
        case be_sequence::MNG_OBJREF:
          {
            *cs << "this->mgr_ = new " << this->name () <<
              "::TAO_ObjRefMngType [this->maximum_];" << nl;
            *cs << "for (CORBA::ULong i=0; i < this->maximum_; i++)" << nl;
            *cs << "{" << nl;
            *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
            *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
            *cs << "}" << nl;
          }
          break;
        case be_sequence::MNG_STRING:
          {
            *cs << "this->mgr_ = new " << this->name () <<
              "::TAO_StrMngType [this->maximum_];" << nl;
            *cs << "for (CORBA::ULong i=0; i < this->maximum_; i++)" << nl;
            *cs << "{" << nl;
            *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
            *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
            *cs << "}" << nl;
          }
          break;
        }
      *cs << "\n";
      cs->decr_indent ();
      *cs << "}\n\n";

      // destructor
      cs->indent ();
      *cs << "// destructor" << nl;
      *cs << this->name () << "::~" << this->local_name () << " (void)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      *cs << "if (this->release_) // we own the buffer" << nl;
      *cs << "{\n";
      cs->incr_indent ();

      // only for obj references and strings, we need to free each individual
      // element
      if (this->managed_type () != be_sequence::MNG_NONE)
        {
          *cs << "delete []this->mgr_; // ensures each element is freed" << nl;
        }
      // call freebuf on the buffer
      *cs << this->name () << "::freebuf (this->buffer_);\n";
      cs->decr_indent ();
      *cs << "}\n";
      cs->decr_indent ();
      *cs << "}\n\n";

      // assignment operator
      cs->indent ();
      *cs << "// assignment operator" << nl;
      *cs << this->name () << "& " << nl;
      *cs << this->name () << "::operator=" <<
        " (const " << this->name () << " &seq)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // check for equality
      *cs << "if (this == &seq) return *this;" << nl;
      // otherwise, if release flag, free the buffer
      *cs << "if (this->release_)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // only for obj references and strings, we need to free each individual
      // element
      if (this->managed_type () != be_sequence::MNG_NONE)
        {
          *cs << "delete []this->mgr_; // ensures each element is freed" << nl;
        }
      *cs << this->name () << "::freebuf (this->buffer_);\n";
      cs->decr_indent ();
      *cs << "}" << nl;

      *cs << "this->length_ = seq.length_;" << nl;
      *cs << "this->maximum_ = seq.maximum_;" << nl;
      *cs << "this->buffer_ = " << this->name () << "::allocbuf (seq.maximum_),"
          << nl;
      *cs << "this->release_ = 1; // we always own it" << nl;
      // copy each element
      switch (this->managed_type ())
        {
        case be_sequence::MNG_OBJREF:
          {
            *cs << "this->mgr_ = new " << this->name () <<
              "::TAO_ObjRefMngType [seq.maximum_];" << nl;
            *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
            *cs << "{" << nl;
            *cs << "\tthis->buffer_[i] = " << bt->name () << "::_duplicate ("
                << "seq.buffer_[i]);" << nl;
            *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
            *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
            *cs << "}" << nl;
          }
          break;
        case be_sequence::MNG_STRING:
          {
            *cs << "this->mgr_ = new " << this->name () <<
              "::TAO_StrMngType [seq.maximum_];" << nl;
            *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
            *cs << "{" << nl;
            *cs << "\tthis->buffer_[i] = CORBA::string_dup (" <<
              "seq.buffer_[i]);" << nl;
            *cs << "\tthis->mgr_[i].ptr_ = &this->buffer_[i];" << nl;
            *cs << "\tthis->mgr_[i].release_ = this->release_;" << nl;
            *cs << "}" << nl;
          }
          break;
        default: // all other types are self managed, just assign them.
          *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
          *cs << "\tthis->buffer_[i] = seq.buffer_[i];" << nl;
        }
      *cs << "return *this;\n";
      cs->decr_indent ();
      *cs << "}\n\n";

      // the length method
      cs->indent ();
      *cs << "void" << nl;
      *cs << this->name () << "::length (CORBA::ULong length)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      if (this->max_size () != 0) // bounded sequence - we cannot increase
                                  // length more than its max => no
                                  // reallocation necessary
        {
          // The sequence has a maximum length, check that the new
          // length is valid before changing anything.
          *cs << "if (length > this->maximum_)" << nl;
          *cs << "{\n";
          cs->incr_indent ();
          *cs << "// @@ throw something?" << nl;
          *cs << "return;" << nl;
          cs->decr_indent ();
          *cs << "}" << nl;
          *cs << "this->length_ = length;\n";
        }
      else // unbounded buffer - may need reallocation
        {
          // Reallocate the buffer.
          *cs << "if (length > this->maximum_) // need realloc" << nl;
          *cs << "{\n";
          cs->incr_indent ();
          if (s->gen_code (bt, this) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_sequence::"
                                 "gen_client_stubs - "
                                 "bad state\n"),
                                -1);
            }

          *cs << " *tmp = " << this->name ()
              << "::allocbuf (length);" << nl;
          *cs << "if (!tmp)" << nl;
          *cs << "\treturn;" << nl;

          // copy each element. Allocate a new manager and initialize it.
          switch (this->managed_type ())
            {
            case be_sequence::MNG_OBJREF:
              {
                *cs << this->name () << "::TAO_ObjRefMngType *tmp_mgr" <<
                  " = new " << this->name () <<
                  "::TAO_ObjRefMngType [length];" << nl;
                *cs << "for (CORBA::ULong i=0; i < length; i++)" << nl;
                *cs << "{" << nl;
                *cs << "\ttmp[i] = " << bt->name () << "::_duplicate ("
                    << "this->buffer_[i]);" << nl;
                *cs << "\ttmp_mgr[i].ptr_ = &tmp[i];" << nl;
                *cs << "\ttmp_mgr[i].release_ = 1; // always" << nl;
                *cs << "}" << nl;
              }
              break;
            case be_sequence::MNG_STRING:
              {
                *cs << this->name () << "::TAO_StrMngType *tmp_mgr_" <<
                  " = new " << this->name () <<
                  "::TAO_StrMngType [length];" << nl;
                *cs << "for (CORBA::ULong i=0; i < length; i++)" << nl;
                *cs << "{" << nl;
                *cs << "\ttmp[i] = CORBA::string_dup (" <<
                  "this->buffer_[i]);" << nl;
                *cs << "\ttmp_mgr[i]->ptr_ = &tmp[i];" << nl;
                *cs << "\ttmp_mgr[i]->release_ = 1;" << nl;
                *cs << "}" << nl;
              }
              break;
            default: // all other types are self managed, just assign them.
              *cs << "\ttmp[i] = this->buffer_[i];" << nl;
            }
          // if release is set, we must free the previous buffer
          *cs << "if (this->release_) // free old one if we own it" << nl;
          *cs << "{\n";
          cs->incr_indent ();
          // only for obj references and strings, we need to free each individual
          // element. Others are self managed.
          if (this->managed_type () != be_sequence::MNG_NONE)
            {
              *cs << "delete []this->mgr_; // ensures each element is freed" <<
                nl;
            }
          *cs << this->name () << "::freebuf (this->buffer_);\n";
          cs->decr_indent ();
          *cs << "}" << nl;

          *cs << "//assign the newly reallocated buffer" << nl;
          *cs << "this->buffer_ = tmp;" << nl;
          *cs << "this->release_ = 1; //after reallocation, we own it" << nl;
          if (this->managed_type () != be_sequence::MNG_NONE)
            {
              *cs << "this->mgr_ = tmp_mgr;" << nl;
            }
          *cs << "this->maximum_ = length;\n";
          cs->decr_indent ();
          *cs << "}" << nl;;
          *cs << "this->length_ = length;\n";
        }
      cs->decr_indent ();
      *cs << "}\n\n";

      // the allocbuf method
      cs->indent ();
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_stubs - "
                             "state based codegen failed\n"),
                            -1);
        }

      *cs << " *" << nl;
      *cs << this->name () << "::allocbuf (CORBA::ULong nelems)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_stubs - "
                             "state based codegen failed\n"),
                            -1);
        }

      *cs << " *buf = new ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_stubs - "
                             "state based codegen failed\n"),
                            -1);
        }
      *cs << "[nelems]; // allocate from heap" << nl;
      // the managed types must be initilaized
      switch (this->managed_type ())
        {
        case be_sequence::MNG_OBJREF:
          {
            *cs << "for (CORBA::ULong i=0; i < nelems; i++)" << nl;
            *cs << "\tbuf[i] = " << bt->name () << "::_nil ();" << nl;
          }
          break;
        case be_sequence::MNG_STRING:
          {
            *cs << "for (CORBA::ULong i=0; i < nelems; i++)" << nl;
            *cs << "\tbuf[i] = (char *)0;" << nl;
          }
          break;
        }
      *cs << "return buf;\n";

      cs->decr_indent ();
      *cs << "}\n\n";

      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl;
      *cs << "{\n";
      cs->incr_indent (0);
      if (this->gen_encapsulation () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_stubs - "
                             "codegen for encapsulation failed\n"),
                            -1);
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
      this->cli_stub_gen_ = I_TRUE;
    }
  return 0;
}

// Generates the client-side inline information
int
be_sequence::gen_client_inline (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  be_state *s;       // code gen state
  be_type *bt;  // base type

  if (!this->cli_inline_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      ci = cg->client_inline ();

      // retrieve base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "bad type\n"),
                            -1);
        }

      cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE_CI);
      s = cg->make_state ();

      // generate inline methods for our base type if it itself is a sequence
      if (!s || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "state based codegen failed\n"),
                            -1);
        }
      cg->pop ();

      // generate the methods of the sequence C++ mapping
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CI);
      s = cg->make_state ();
      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "bad state\n"),
                            -1);
        }

      // generate the ifdefined macro for type
      ci->gen_ifdef_macro (this->flatname ());

      // freebuf method
      ci->indent ();
      *ci << "ACE_INLINE void" << nl;
      *ci << this->name () << "::freebuf (";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "state based codegen failed\n"),
                            -1);
        }

      *ci << " *seq)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "delete []seq;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // default constructor
      ci->indent ();
      *ci << "//default constructor" << nl;
      *ci << "ACE_INLINE " << nl;
      *ci << this->name () << "::" << this->local_name () << " (void)" << nl;
      // for bounded and unbounded, initialize the data members differently
      if (this->unbounded_)
        {
          *ci << "\t: maximum_ (0)," << nl;
          *ci << "\t  length_ (0)," << nl;
          *ci << "\t  buffer_ (0)," << nl;
          *ci << "\t  release_ (0) // does not own" << nl;
        }
      else
        {
          *ci << "\t: maximum_ (" << this->max_size () << ")," << nl;
          *ci << "\t  length_ (0)," << nl;
          *ci << "\t  buffer_ (" << this->name () << "::allocbuf (" <<
            this->max_size () << "))," << nl;
          *ci << "\t  release_ (1) // owns" << nl;
        }
      if (this->managed_type () != be_sequence::MNG_NONE)
        {
          *ci << "\t, mgr_ (0)" << nl;
        }
      *ci << "{}\n\n";

      // the maximum method
      ci->indent ();
      *ci << "ACE_INLINE CORBA::ULong" << nl;
      *ci << this->name () << "::maximum (void) const" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->maximum_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // the length method
      ci->indent ();
      *ci << "ACE_INLINE CORBA::ULong" << nl;
      *ci << this->name () << "::length  (void) const" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->length_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      cg->pop ();

      // subscript operators (1) read-only, (2) read/write
      cg->push (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI);
      s = cg->make_state ();
      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "bad state\n"),
                            -1);
        }

      ci->indent ();
      *ci << "ACE_INLINE ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "state based codegen failed\n"),
                            -1);
        }

      *ci << " &" << nl;
      *ci << this->name () << "::operator[] (CORBA::ULong index) // read/write"
         << nl;
      *ci << "{\n";
      ci->incr_indent ();
      if (this->managed_type () != be_sequence::MNG_NONE)
        *ci << "return this->mgr_[index];\n";
      else
        *ci << "return this->buffer_[index];\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE const ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "state based codegen failed\n"),
                            -1);
        }

      *ci << " &" << nl;
      *ci << this->name () << "::operator[] (CORBA::ULong index) const // read"
         << nl;
      *ci << "{\n";
      ci->incr_indent ();
      if (this->managed_type () != be_sequence::MNG_NONE)
        *ci << "return this->mgr_[index];\n";
      else
        *ci << "return this->buffer_[index];\n";
      ci->decr_indent ();
      *ci << "}\n\n";
      cg->pop ();
      ci->gen_endif (); // endif macro

      // generate the ifdefined macro for the var type
      ci->gen_ifdef_macro (this->flatname (), "_var");

      // generate the implementations for the _var and _impl classes
      if (this->gen_var_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "var implementation failed\n"),
                            -1);
        }
      ci->gen_endif ();

      // generate the ifdefined macro for the var type
      ci->gen_ifdef_macro (this->flatname (), "_out");

      if (this->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_client_inline - "
                             "out impl failed\n"),
                            -1);
        }
      ci->gen_endif (); // endif macro

      this->cli_inline_gen_ = I_TRUE;
    }
  return 0;
}

int
be_sequence::gen_server_header (void)
{
  // nothing to be done
  return 0;
}

int
be_sequence::gen_server_skeletons (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int
be_sequence::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// generate the _var definition for ourself
int
be_sequence::gen_var_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // names
  be_state *s;       // code gen state
  be_type *bt;  // base type


  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_var", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

  *ch << "// *************************************************************"
      << nl;
  *ch << "// class " << this->name () << "_var" << nl;
  *ch << "// *************************************************************\n\n";

  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CH);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_var_defn - "
                         "state based codegen failed\n"),
                        -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ch->indent (); // start with whatever was our current indent level
  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();
  // default constr
  *ch << namebuf << " (void); // default constructor" << nl;
  // constr
  *ch << namebuf << " (" << this->local_name () << " *);" << nl;
  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << this->local_name () << " *);" << nl;
  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf <<
    " &);" << nl;

  // arrow operator
  *ch << this->local_name () << " *operator-> (void);" << nl;
  *ch << "const " << this->local_name () << " *operator-> (void) const;" << nl;
  *ch << nl;

  // other extra types (cast operators, [] operator, and others)

  // cast operator
  *ch << "operator const " << this->local_name () << " &() const;" << nl;
  *ch << "operator " << this->local_name () << " &();" << nl;
  *ch << "operator " << this->local_name () << " &() const;" << nl;

  // overloaded [] operator. The const version is not required for sequences

  switch (this->managed_type ())
    {
    case be_sequence::MNG_STRING:
      *ch << "ACE_NESTED_CLASS (" << this->name () << ", TAO_StrMngType)";
      break;
    case be_sequence::MNG_OBJREF:
      *ch << "ACE_NESTED_CLASS (" << this->name () << ", TAO_ObjRefMngType)";
      break;
    default:
      // gen code for base return type
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_var_defn - "
                             "state based codegen failed\n"),
                            -1);
        }
    }

  *ch << " &operator[] (CORBA::ULong index);" << nl;

  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  *ch << "const " << this->local_name () << " &in (void) const;" << nl;
  *ch << this->local_name () << " &inout (void);" << nl;
  *ch << this->local_name () << " *&out (void);" << nl;
  *ch << this->local_name () << " *_retn (void);" << nl;

  // generate an additional member function that returns the underlying pointer
  *ch << this->local_name () << " *ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // generate the private section
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << this->local_name () << " *ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  cg->pop ();

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_sequence::gen_var_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names
  be_state *s;       // code gen state
  be_type *bt;  // base type


  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_var", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_var", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CI);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_var_impl - invalid state obj\n"), -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the var implementation in the inline file
  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // default constr
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname <<
    " (void) // default constructor" << nl;
  *ci << "\t" << ": ptr_ (0)" << nl;
  *ci << "{}\n\n";

  // constr from a _ptr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_ptr p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (p.ptr_)" << nl;
  *ci << "\tthis->ptr_ = new " << this->name () << "(*p.ptr_);" << nl;
  *ci << "else" << nl;
  *ci << "\tthis->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // destructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator from a pointer
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << name () <<
    " *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator from _var
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << fname <<
    " &p) // deep copy" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (this != &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = new " << this->name () << " (*p.ptr_);\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // two arrow operators
  ci->indent ();
  *ci << "ACE_INLINE const " << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - 3 cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << name () <<
    " &() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << " &() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << " &() const// cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator []
  ci->indent ();
  *ci << "ACE_INLINE ";
  switch (this->managed_type ())
    {
    case be_sequence::MNG_STRING:
      *ci << this->name () << "::TAO_StrMngType";
      break;
    case be_sequence::MNG_OBJREF:
      *ci << this->name () << "::TAO_ObjRefMngType";
      break;
    default:
      // gen code for base return type
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_var_impl - "
                             "state based codegen failed\n"),
                            -1);
        }
    }

  *ci << "&" << nl;
  *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_->operator[] (index);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE const " << name () << " &" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << " &" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "// mapping for variable size " << nl;
  *ci << "ACE_INLINE " << name () << " *&" << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;" << nl;
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << " *" << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << this->name () << " *tmp = this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;" << nl;
  *ci << "return tmp;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // the additional ptr () member function
  ci->indent ();
  *ci << "ACE_INLINE " << name () << " *" << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  cg->pop ();

  return 0;
}

// generate the _out definition
int
be_sequence::gen_out_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // to hold the _out name
  be_state *s;       // code gen state
  be_type *bt;  // base type

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_out", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();
  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CH);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_out_defn - invalid state obj\n"), -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the out definition (always in the client header)
  ch->indent (); // start with whatever was our current indent level

  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor

  // constructor from a pointer
  *ch << namebuf << " (" << this->local_name () << " *&);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << this->local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (" << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (" << namebuf << " &);" << nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  // assignment
  *ch << namebuf << " &operator= (" << this->local_name () << " *);" << nl;
  // operator ()
  *ch << "operator " << this->local_name () << " *&();" << nl;
  // ptr fn
  *ch << this->local_name () << " *&ptr (void);" << nl;
  // operator ->
  *ch << this->local_name () << " *operator-> (void);" << nl;

  // overloaded [] operator only for sequence. The const version is not
  // required

  switch (this->managed_type ())
    {
    case be_sequence::MNG_STRING:
      *ch << "ACE_NESTED_CLASS (" << this->name () << ", TAO_StrMngType)";
      break;
    case be_sequence::MNG_OBJREF:
      *ch << "ACE_NESTED_CLASS (" << this->name () << ", TAO_ObjRefMngType)";
      break;
    default:
      // gen code for base return type
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_out_defn - "
                             "state based codegen failed\n"),
                            -1);
        }
    }

  *ch << " &operator[] (CORBA::ULong index);" << nl;
  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();

  *ch << this->local_name () << " *&ptr_;" << nl;
  *ch << "// assignment from T_var not allowed" << nl;
  *ch << "void operator= (const " << this->local_name () << "_var &);\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  cg->pop ();
  return 0;
}

int
be_sequence::gen_out_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names
  be_state *s;       // code gen state
  be_type *bt;  // base type


  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_out", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_out", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CI);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_out_impl - invalid state obj\n"), -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the out implementation in the inline file

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // constr from a pointer
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << " *&p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // constructor from _var &
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << this->name () <<
    "_var &p) // constructor from _var" << nl;
  *ci << "\t: ptr_ (p.out ())" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << fname <<
    " &p) // copy constructor" << nl;
  *ci << "\t: ptr_ (p.ptr_)" << nl;
  *ci << "{}\n\n";

  // assignment operator from _out &
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = p.ptr_;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment from _var is not allowed by a private declaration

  // assignment operator from pointer
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    " *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << this->name () <<
    " *&() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << " *&" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator ->
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // sequence has an additional method
  ci->indent ();
  *ci << "ACE_INLINE ";
  switch (this->managed_type ())
    {
    case be_sequence::MNG_STRING:
      *ci << this->name () << "::TAO_StrMngType";
      break;
    case be_sequence::MNG_OBJREF:
      *ci << this->name () << "::TAO_ObjRefMngType";
      break;
    default:
      // gen code for base return type
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_out_impl - "
                             "state based codegen failed\n"),
                            -1);
        }
    }

  *ci << "& " << nl;
  *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_->operator[] (index);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  cg->pop ();
  return 0;
}

// generate the managed type
int
be_sequence::gen_managed_type_ch (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // names
  char typebuf [NAMEBUFSIZE];  // generated type
  be_type *bt;  // base type

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_gen_managed_type_ch - "
                         "state based codegen failed\n"),
                        -1);
    }

  // set the name of the generated class
  // the name depends on whether we are interface  type or a string type
  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (typebuf, '\0', NAMEBUFSIZE);
  switch (this->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      {
        ACE_OS::sprintf (namebuf, "%s", "TAO_ObjRefMngType");
        ACE_OS::sprintf (typebuf, "%s", bt->nested_type_name (this, "_ptr"));
      }
      break;
    case be_sequence::MNG_STRING:
      {
        ACE_OS::sprintf (namebuf, "%s", "TAO_StrMngType");
        ACE_OS::sprintf (typebuf, "%s", "char *");
      }
      break;
    default:
      // cannot be here
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_sequence::"
                           "gen_gen_managed_type_ch - "
                           "bad managed type\n"),
                          -1);
      }
    }

  ch->indent (); // start with whatever was our current indent level
  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "protected:\n";
  ch->incr_indent ();
  // generate the friend instruction
  *ch << "friend " << this->local_name () << ";" << nl;
  // default constr is protected as this managed type is not available
  // outside. Only this sequence can play with it.
  *ch << namebuf << " (void); // default constructor\n";
  ch->decr_indent ();
  *ch << "public:\n";
  ch->incr_indent ();
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from another managed type
  *ch << namebuf << " &operator= (const " << namebuf << "&);" << nl;
  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << typebuf << ");" << nl;

  // others
  if (this->managed_type () == be_sequence::MNG_STRING)
    {
      // assignment operator from a pointer to const
      *ch << namebuf << " &operator= (const " << typebuf << ");" << nl;
      // cast op
      *ch << "operator const char*() const;" << nl;
      *ch << "operator char *();" << nl;
      // overloaded [] operator - only for strings
      *ch << "char &operator[] (CORBA::ULong index);" << nl;
      *ch << "const char &operator[] (CORBA::ULong index) const;" << nl;
      *ch << "// in, inout, out, _retn " << nl;
      // the return types of in, out, inout, and _retn are based on the parameter
      // passing rules and the base type
      *ch << "const char *in (void) const;" << nl;
      *ch << "char *&inout (void);" << nl;
      *ch << "char *&out (void);" << nl;
      *ch << "char *_retn (void);" << nl;
    }
  else
    {
      *ch << "operator const " << bt->nested_type_name (this, "_ptr") <<
        " &() const;" << nl;
      *ch << "operator " << bt->nested_type_name (this, "_ptr") <<
        " &();" << nl;
      *ch << "// in, inout, out, _retn " << nl;
      // the return types of in, out, inout, and _retn are based on the parameter
      // passing rules and the base type
      *ch << "const " << bt->nested_type_name (this, "_ptr") <<
        " in (void) const;" << nl;
      *ch << bt->nested_type_name (this, "_ptr") << " &inout (void);" << nl;
      *ch << bt->nested_type_name (this, "_ptr") << " &out (void);" << nl;
      *ch << bt->nested_type_name (this, "_ptr") << " _retn (void);" << nl;
    }

  *ch << "\n";
  ch->decr_indent ();

  // generate the private section
  *ch << "private:\n";
  ch->incr_indent ();
  // copy constructor not allowed
  *ch << namebuf << " (const " << namebuf << " &);" <<
    "// copy constructor not allowed" << nl;
  *ch << typebuf << " *ptr_;" << nl;
  *ch << "CORBA::Boolean release_;\n";
  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// implementation of the managed type class. All of these get generated in the
// inline file
int
be_sequence::gen_managed_type_ci (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;  // base type
  char fnamebuf [NAMEBUFSIZE];  // full name
  char lnamebuf [NAMEBUFSIZE];  // local name
  char typebuf [NAMEBUFSIZE];  // type name


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // get the inline stream
  ci = cg->client_inline ();

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence.cpp::"
                         "gen_managed_types_ci - "
                         "Bad base type\n"),
                        -1);
    }

  // set the names (this is for easier code generation)
  ACE_OS::memset (fnamebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (lnamebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (typebuf, '\0', NAMEBUFSIZE);
  switch (this->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      {
        ACE_OS::sprintf (fnamebuf, "%s::%s", this->fullname (), "TAO_ObjRefMngType");
        ACE_OS::sprintf (lnamebuf, "%s", "TAO_ObjRefMngType");
        ACE_OS::sprintf (typebuf, "%s_ptr", bt->fullname ());
      }
      break;
    case be_sequence::MNG_STRING:
      {
        ACE_OS::sprintf (fnamebuf, "%s::%s", this->fullname (), "TAO_StrMngType");
        ACE_OS::sprintf (lnamebuf, "%s", "TAO_StrMngType");
        ACE_OS::sprintf (typebuf, "%s", "char *");
      }
      break;
    default:
      // cannot be here
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_sequence::"
                           "gen_managed_types_ci - "
                           "Bad managed type\n"),
                          -1);
      }
    }

  // generate the managed type implementation in the inline file
  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fnamebuf << nl;
  *ci << "// *************************************************************\n\n";

  // default constr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fnamebuf << "::" << lnamebuf <<
    " (void) // default constructor" << nl;
  *ci << "\t: ptr_ (0)," << nl;
  *ci << "\t  release_ (0)" << nl;
  *ci << "{}\n\n";

  // copy constructor not allowed

  // operators
  if (this->managed_type () == be_sequence::MNG_STRING)
    {
      // other extra methods - cast operator ()
      ci->indent ();
      *ci << "ACE_INLINE " << nl;
      *ci << fnamebuf << "::operator const " << typebuf <<
        " () const // cast" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE " << nl;
      *ci << fnamebuf << "::operator " << typebuf << " () // cast " << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // operator []
      ci->indent ();
      *ci << "ACE_INLINE char&" << nl;
      *ci << fnamebuf << "::operator[] (CORBA::ULong index)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_[index];\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // operator []
      ci->indent ();
      *ci << "ACE_INLINE const char&" << nl;
      *ci << fnamebuf << "::operator[] (CORBA::ULong index) const" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_[index];\n";
      ci->decr_indent ();
      *ci << "}\n\n";

    }
  else if (this->managed_type () == be_sequence::MNG_OBJREF)
    {
      // other extra methods - cast operator ()
      ci->indent ();
      *ci << "ACE_INLINE " << nl;
      *ci << fnamebuf << "::operator const " << typebuf <<
        " &() const // cast" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE " << nl;
      *ci << fnamebuf << "::operator " << typebuf << " &() // cast " << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_managed_types_ci - "
                         "Bad managed type\n"),
                        -1);
    }

  // in, inout
  ci->indent ();
  *ci << "ACE_INLINE const " << typebuf << nl;
  *ci << fnamebuf << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << typebuf << "&" << nl;
  *ci << fnamebuf << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_sequence::gen_managed_type_cs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;  // base type
  char fnamebuf [NAMEBUFSIZE];  // full name
  char lnamebuf [NAMEBUFSIZE];  // local name
  char typebuf [NAMEBUFSIZE];  // type name


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // get the stubs stream
  cs = cg->client_stubs ();

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence.cpp::"
                         "gen_managed_types_cs - "
                         "Bad base type\n"),
                        -1);
    }

  // set the names (this is for easier code generation)
  ACE_OS::memset (fnamebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (lnamebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (typebuf, '\0', NAMEBUFSIZE);
  switch (this->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      {
        ACE_OS::sprintf (fnamebuf, "%s::%s", this->fullname (), "TAO_ObjRefMngType");
        ACE_OS::sprintf (lnamebuf, "%s", "TAO_ObjRefMngType");
        ACE_OS::sprintf (typebuf, "%s_ptr", bt->fullname ());
      }
      break;
    case be_sequence::MNG_STRING:
      {
        ACE_OS::sprintf (fnamebuf, "%s::%s", this->fullname (), "TAO_StrMngType");
        ACE_OS::sprintf (lnamebuf, "%s", "TAO_StrMngType");
        ACE_OS::sprintf (typebuf, "%s", "char *");
      }
      break;
    default:
      // cannot be here
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_sequence.cpp::"
                           "gen_managed_types_cs - "
                           "Bad managed type\n"),
                          -1);
      }
    }

  switch (this->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      {
        // destructor
        cs->indent ();
        *cs << fnamebuf << "::~" << lnamebuf << " (void) // destructor" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "if (this->release_)" << nl;
        *cs << "  CORBA::release (*this->ptr_);\n";
        cs->decr_indent ();
        *cs << "}\n\n";

        // assignment operator from a pointer
        cs->indent ();
        *cs << fnamebuf << " &" << nl;
        *cs << fnamebuf << "::operator= (" << typebuf << " p)" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "if (this->release_) // need to free old one" << nl;
        *cs << "  CORBA::release (*this->ptr_);" << nl;
        *cs << "*this->ptr_ = p; // no copy" << nl;
        *cs << "return *this;\n";
        cs->decr_indent ();
        *cs << "}\n\n";

        // out method and _retn
        cs->indent ();
        *cs << "// mapping for variable size " << nl;
        *cs << bt->name () << "_ptr &" << nl;
        *cs << fnamebuf << "::out (void)" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "if (this->release_)" << nl;
        *cs << "  CORBA::release (*this->ptr_);" << nl;
        *cs << "*this->ptr_ = " << bt->name () << "::_nil ();" << nl;
        *cs << "return *this->ptr_;\n";
        cs->decr_indent ();
        *cs << "}\n\n";

        cs->indent ();
        *cs << bt->name () << "_ptr" << nl;
        *cs << fnamebuf << "::_retn (void)" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << bt->name () << "_ptr tmp = *this->ptr_;" << nl;
        *cs << "*this->ptr_ = " << bt->name () << "::_nil ();" << nl;
        *cs << "return tmp;\n";
        cs->decr_indent ();
        *cs << "}\n\n";
      }
      break;
    case be_sequence::MNG_STRING:
      {
        // destructor
        cs->indent ();
        *cs << fnamebuf << "::~" << lnamebuf << " (void) // destructor" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "if (this->release_)" << nl;
        *cs << "  CORBA::string_free (*this->ptr_);\n";
        cs->decr_indent ();
        *cs << "}\n\n";

        // assignment operator from a pointer
        cs->indent ();
        *cs << fnamebuf << " &" << nl;
        *cs << fnamebuf << "::operator= (" << typebuf << " p)" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "if (this->release_) // need to free old one" << nl;
        *cs << "  CORBA::string_free (*this->ptr_);" << nl;
        *cs << "*this->ptr_ = p; // no copy" << nl;
        *cs << "return *this;\n";
        cs->decr_indent ();
        *cs << "}\n\n";

        // assignment operator from a pointer to const
        cs->indent ();
        *cs << fnamebuf << " &" << nl;
        *cs << fnamebuf << "::operator= (const " << typebuf << " p)" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "if (this->release_) // need to free old one" << nl;
        *cs << "{" << nl;
        *cs << "  CORBA::string_free (*this->ptr_);" << nl;
        *cs << "  *this->ptr_ = CORBA::string_dup (p);// make copy" << nl;
        *cs << "}" << nl;
        *cs << "else // make no copy as we don't own it" << nl;
        *cs << "  *this->ptr_ = (char *)p; // no copy" << nl;
        *cs << "return *this;\n";
        cs->decr_indent ();
        *cs << "}\n\n";

        // out method and _retn
        cs->indent ();
        *cs << "// mapping for variable size " << nl;
        *cs << "char *&" << nl;
        *cs << fnamebuf << "::out (void)" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "if (this->release_)" << nl;
        *cs << "  CORBA::string_free (*this->ptr_);" << nl;
        *cs << "*this->ptr_ = 0;" << nl;
        *cs << "return *this->ptr_;\n";
        cs->decr_indent ();
        *cs << "}\n\n";

        cs->indent ();
        *cs << "char *" << nl;
        *cs << fnamebuf << "::_retn (void)" << nl;
        *cs << "{\n";
        cs->incr_indent ();
        *cs << "char *tmp = *this->ptr_;" << nl;
        *cs << "*this->ptr_ = 0;" << nl;
        *cs << "return tmp;\n";
        cs->decr_indent ();
        *cs << "}\n\n";
      }
      break;
    }

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

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << nl;

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
      this->encap_len_ += bt->tc_size ();
      this->encap_len_ += 4; // to hold the max size

    }
  return this->encap_len_;
}

/*
 * Add this be_sequence to the locally defined types in this scope
 */
be_sequence *
be_sequence::fe_add_sequence (be_sequence *t)
{
  if (t == NULL)
    return NULL;

  this->add_to_local_types(t);
  return t;
}

// overridden method
be_decl *
be_sequence::decl (void)
{
  return this;
}

int be_sequence::write_as_return (TAO_OutStream *stream,
				  be_type *type)
{
  *stream << type->name () << " *";
  return 0;
}

// Visiting methods
int be_sequence::accept (be_visitor *visitor)
{
  return visitor->visit_sequence (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_sequence)
