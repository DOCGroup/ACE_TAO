/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_codegen.h
//
// = DESCRIPTION
//
//   Code generation
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

static const int CHUNK = 100;

/* BE global Data */
TAO_CodeGen::TAO_CodeGen (void)
  : client_header_ (0),
    client_stubs_ (0),
    client_inline_ (0),
    server_header_ (0),
    server_skeletons_ (0),
    server_inline_ (0),
    curr_os_ (0),
    state_ (new TAO_CodeGen::CG_STATE [CHUNK]),
    top_ (0),
    size_ (CHUNK),
    visitor_factory_ (0)
{
  // set the current code generation state
  this->state_ [this->top_++] = TAO_CodeGen::TAO_INITIAL;
}

// destructor
TAO_CodeGen::~TAO_CodeGen (void)
{
  delete this->client_header_;
  delete this->server_header_;
  delete this->client_stubs_;
  delete this->server_skeletons_;
  delete this->client_inline_;
  delete this->server_inline_;
  this->curr_os_ = 0;
  delete [] this->state_;
  delete this->visitor_factory_;
}

// visitor factory method
be_visitor *
TAO_CodeGen::make_visitor (TAO_CodeGen::CG_STATE st)
{
  ACE_ASSERT (this->visitor_factory_ != 0);
  return this->visitor_factory_->make_visitor (st);
}

// factory method
be_state *
TAO_CodeGen::make_state (void)
{
  switch (this->state ())
    {
    case TAO_STRUCT_CH:
    case TAO_STRUCT_CS:
    case TAO_STRUCT_CI:
      return TAO_BE_STATE_STRUCT::instance ();
    case TAO_UNION_DISCTYPEDEFN_CH:
      return TAO_BE_STATE_UNION_DISCTYPEDEFN_CH::instance ();
    case TAO_UNION_DISCTYPEDEFN_CI:
      return TAO_BE_STATE_UNION_DISCTYPEDEFN_CI::instance ();
    case TAO_UNION_PUBLIC_CH:
      return TAO_BE_STATE_UNION_PUBLIC_CH::instance ();
    case TAO_UNION_PUBLIC_CI:
      return TAO_BE_STATE_UNION_PUBLIC_CI::instance ();
    case TAO_UNION_PUBLIC_CS:
    case TAO_UNION_PUBLIC_ASSIGN_CS:
      return TAO_BE_STATE_UNION_PUBLIC_CS::instance ();
    case TAO_UNION_PRIVATE_CH:
      return TAO_BE_STATE_UNION_PRIVATE_CH::instance ();
    case TAO_OPERATION_CH:
    case TAO_OPERATION_RETURN_TYPE_CS:
    case TAO_OPERATION_RETVAL_DECL_CS:
    case TAO_OPERATION_RETVAL_EXCEPTION_CS:
    case TAO_OPERATION_RETVAL_RETURN_CS:
    case TAO_OPERATION_SH:
    case TAO_OPERATION_RETVAL_DECL_SS:
    case TAO_OPERATION_RETVAL_ASSIGN_SS:
    case TAO_OPERATION_RESULT_SS:
      return TAO_BE_STATE_OPERATION::instance ();
    case TAO_ARGUMENT_CH:
    case TAO_ARGUMENT_CS:
    case TAO_ARGUMENT_PRE_DOCALL_CS:
    case TAO_ARGUMENT_DOCALL_CS:
    case TAO_ARGUMENT_POST_DOCALL_CS:
    case TAO_ARGUMENT_SH:
    case TAO_ARGUMENT_SS:
    case TAO_ARGUMENT_VARDECL_SS:
    case TAO_ARGUMENT_PRE_UPCALL_SS:
    case TAO_ARGUMENT_UPCALL_SS:
    case TAO_ARGUMENT_POST_UPCALL_SS:
      return TAO_BE_STATE_ARGUMENT::instance ();
    case TAO_TYPEDEF_CH:
    case TAO_TYPEDEF_CS:
    case TAO_TYPEDEF_CI:
      return TAO_BE_STATE_TYPEDEF::instance ();
    case TAO_ARRAY_DEFN_CH:
    case TAO_ARRAY_OTHER_CH:
    case TAO_ARRAY_DEFN_CI:
      return TAO_BE_STATE_ARRAY::instance ();
    case TAO_SEQUENCE_BASE_CH:
    case TAO_SEQUENCE_BASE_CS:
    case TAO_SEQUENCE_BASE_CI:
    case TAO_SEQUENCE_BODY_CH:
    case TAO_SEQUENCE_BODY_CS:
    case TAO_SEQUENCE_BODY_CI:
    case TAO_SEQELEM_RETTYPE_CH:
    case TAO_SEQELEM_RETTYPE_CI:
    case TAO_SEQELEM_RETTYPE_CS:
      return TAO_BE_STATE_SEQUENCE::instance ();
    case TAO_ATTRIBUTE_RETURN_TYPE_CH:
    case TAO_ATTRIBUTE_INPARAM_TYPE_CH:
    case TAO_ATTRIBUTE_RETURN_TYPE_CS:
    case TAO_ATTRIBUTE_RETVAL_DECL_CS:
    case TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
    case TAO_ATTRIBUTE_RETVAL_RETURN_CS:
    case TAO_ATTRIBUTE_INPARAM_TYPE_CS:
    case TAO_ATTRIBUTE_PRE_DOCALL_CS:
    case TAO_ATTRIBUTE_DOCALL_CS:
    case TAO_ATTRIBUTE_POST_DOCALL_CS:
    case TAO_ATTRIBUTE_RETURN_TYPE_SH:
    case TAO_ATTRIBUTE_INPARAM_TYPE_SH:
    case TAO_ATTRIBUTE_RETVAL_DECL_SS:
    case TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
    case TAO_ATTRIBUTE_RESULT_SS:
    case TAO_ATTRIBUTE_INPARAM_TYPE_SS:
    case TAO_ATTRIBUTE_PRE_UPCALL_SS:
    case TAO_ATTRIBUTE_UPCALL_SS:
    case TAO_ATTRIBUTE_POST_UPCALL_SS:
      return TAO_BE_STATE_ATTRIBUTE::instance ();
    case TAO_EXCEPTION_CH:
    case TAO_EXCEPTION_CTOR_CH:
    case TAO_EXCEPTION_CS:
    case TAO_EXCEPTION_CTOR_CS:
    case TAO_EXCEPTION_CTOR_ASSIGN_CS:
    case TAO_EXCEPTION_CI:
      return TAO_BE_STATE_EXCEPTION::instance ();
    default:
      return 0;
    }
}

// change the string to all upcase
const char *
TAO_CodeGen::upcase (const char *str)
{
  static char upcase_str [NAMEBUFSIZE];

  ACE_OS::memset (upcase_str, '\0', NAMEBUFSIZE);
  // convert letters in str to upcase
  for (unsigned int i=0; i < ACE_OS::strlen (str); i++)
    {
      if (isalpha (str [i]))
        {
          upcase_str[i] = toupper (str [i]);
        }
      else
        {
          // copy it as it is
          upcase_str[i] = str[i];
        }
    }
  return upcase_str;
}

// set the client header stream
int
TAO_CodeGen::client_header (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->client_header_ = factory->make_outstream ();
  if (!this->client_header_)
    {
      return -1;
    }

  if (this->client_header_->open (fname, TAO_OutStream::TAO_CLI_HDR) == -1)
    return -1;
  else
    {
      // now generate the #if !defined clause
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
      const char *suffix = ACE_OS::strstr (fname, ".h");
      if (suffix == 0)
        return -1; // bad file name
      else
        {
          ACE_OS::sprintf (macro_name, "_TAO_IDL_");
          // convert letters in fname to upcase
          for (int i=0; i < (suffix - fname); i++)
            if (isalpha (fname [i]))
              macro_name[i+9] = toupper (fname [i]);
            else
              macro_name[i+9] = fname[i];

          ACE_OS::strcat (macro_name, "_H_");

          this->client_header_->print ("#if !defined (%s)\n", macro_name);
          this->client_header_->print ("#define %s\n\n", macro_name);
          *this->client_header_ << "#include \"tao/corba.h\"\n";

	  // We must include all the skeleton headers corresponding to
	  // IDL files included by the current IDL file.
	  for (size_t j = 0;
	       j < idl_global->n_include_file_names ();
	       ++j)
	    {
	      String* idl_name =
		idl_global->include_file_names()[j];

	      const char* client_hdr =
		IDL_GlobalData::be_get_client_hdr (idl_name);

	      this->client_header_->print ("#include \"%s\"\n",
					   client_hdr);
	    }
	  *this->client_header_ << "\n";
          return 0;
        }
    }
}

// get the client header stream
TAO_OutStream *
TAO_CodeGen::client_header (void)
{
  return this->client_header_;
}

// set the client stub stream
int
TAO_CodeGen::client_stubs (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->client_stubs_ = factory->make_outstream ();
  if (!this->client_stubs_)
    {
      return -1;
    }

  if (this->client_stubs_->open (fname, TAO_OutStream::TAO_CLI_IMPL) == -1)
    {
      return -1;
    }
  // generate the include statement for the client header
  *this->client_stubs_ << "#include \"" <<
    idl_global->be_get_client_hdr_fname () << "\"\n\n";

  *this->client_stubs_ << "#include \"" <<
    idl_global->be_get_server_hdr_fname () << "\"\n\n";

  // generate the code that includes the inline file if not included in the
  // header file
  *this->client_stubs_ << "#if !defined (__ACE_INLINE__)\n";
  *this->client_stubs_ << "#include \"" <<
    idl_global->be_get_client_inline_fname () << "\"\n";
  *this->client_stubs_ << "#endif // !defined INLINE\n\n";
  return 0;
}

// get the client stubs stream
TAO_OutStream *
TAO_CodeGen::client_stubs (void)
{
  return this->client_stubs_;
}

// set the client inline stream
int
TAO_CodeGen::client_inline (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->client_inline_ = factory->make_outstream ();
  if (!this->client_inline_)
    {
      return -1;
    }

  return this->client_inline_->open (fname, TAO_OutStream::TAO_CLI_INL);
}

// get the client inline stream
TAO_OutStream *
TAO_CodeGen::client_inline (void)
{
  return this->client_inline_;
}

// set the server header stream
int
TAO_CodeGen::server_header (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->server_header_ = factory->make_outstream ();
  if (!this->server_header_)
    {
      return -1;
    }

  if (this->server_header_->open (fname, TAO_OutStream::TAO_SVR_HDR) == -1)
    return -1;
  else
    {
      // now generate the #if !defined clause
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
      const char *suffix = ACE_OS::strstr (fname, ".h");
      if (suffix == 0)
	return -1; // bad file name
      else
	{
          ACE_OS::sprintf (macro_name, "_TAO_IDL_");
          // convert letters in fname to upcase
          for (int i=0; i < (suffix - fname); i++)
            {
              if (isalpha (fname [i]))
                {
                  macro_name[i+9] = toupper (fname [i]);
                }
            }
          ACE_OS::strcat (macro_name, "_H_");

          this->server_header_->print ("#if !defined (%s)\n", macro_name);
          this->server_header_->print ("#define %s\n\n", macro_name);

	  // We must include all the skeleton headers corresponding to
	  // IDL files included by the current IDL file.
	  for (size_t j = 0;
	       j < idl_global->n_include_file_names ();
	       ++j)
	    {
	      String* idl_name =
		idl_global->include_file_names()[j];

	      const char* server_hdr =
		IDL_GlobalData::be_get_server_hdr (idl_name);

	      this->server_header_->print ("#include \"%s\"\n",
					   server_hdr);
	    }
          // the server header should include the client header
          *this->server_header_ << "#include \"" <<
            idl_global->be_get_client_hdr_fname () << "\"\n\n";

          return 0;
        }
    }
}

// get the server header stream
TAO_OutStream *
TAO_CodeGen::server_header (void)
{
  return this->server_header_;
}

// set the server skeletons stream
int
TAO_CodeGen::server_skeletons (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->server_skeletons_ = factory->make_outstream ();
  if (!this->server_skeletons_)
    {
      return -1;
    }

  if (this->server_skeletons_->open (fname, TAO_OutStream::TAO_SVR_IMPL) == -1)
    {
      return -1;
    }

  // generate the include statement for the server header
  *this->server_skeletons_ << "#include \"" <<
    idl_global->be_get_server_hdr_fname () << "\"\n\n";

  // generate the code that includes the inline file if not included in the
  // header file
  *this->server_skeletons_ << "#if !defined (__ACE_INLINE__)\n";
  *this->server_skeletons_ << "#include \"" <<
    idl_global->be_get_server_inline_fname () << "\"\n";
  *this->server_skeletons_ << "#endif // !defined INLINE\n\n";
  return 0;
}

// get the server skeletons stream
TAO_OutStream *
TAO_CodeGen::server_skeletons (void)
{
  return this->server_skeletons_;
}

// set the server inline stream
int
TAO_CodeGen::server_inline (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->server_inline_ = factory->make_outstream ();
  if (!this->server_inline_)
    {
      return -1;
    }

  return this->server_inline_->open (fname, TAO_OutStream::TAO_SVR_INL);
}

// get the server inline stream
TAO_OutStream *
TAO_CodeGen::server_inline (void)
{
  return this->server_inline_;
}

// put the last #endif in the client and server headers
int
TAO_CodeGen::end_client_header (void)
{
  // insert the code to include the inline file
  *this->client_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->client_header_ << "#include \"" <<
    idl_global->be_get_client_inline_fname () << "\"\n";
  *this->client_header_ << "#endif // defined INLINE\n\n";

  // code to put the last #endif
  *this->client_header_ << "\n#endif // if !defined\n";
  return 0;
}

int
TAO_CodeGen::end_server_header (void)
{
  // insert the code to include the inline file
  *this->server_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->server_header_ << "#include \"" <<
    idl_global->be_get_server_inline_fname () << "\"\n";
  *this->server_header_ << "#endif // defined INLINE\n\n";

  // code to put the last #endif
  *this->server_header_ << "\n#endif // if !defined\n";
  return 0;
}

// We use the following helper functions to pass information. This class is the
// best place to pass such information rather than passing information through
// global variables spread everywhere. This class is a singleton and is
// effectively a global.

void
TAO_CodeGen::outstream (TAO_OutStream *os)
{
  this->curr_os_ = os;
}

TAO_OutStream *
TAO_CodeGen::outstream (void)
{
  return this->curr_os_;
}

void
TAO_CodeGen::push (TAO_CodeGen::CG_STATE s)
{
  if (this->top_ == this->size_)
    {
      TAO_CodeGen::CG_STATE *temp = this->state_;
      this->size_ += CHUNK;
      this->state_ = new TAO_CodeGen::CG_STATE [this->size_];
      for (int i=0; i < this->top_; i++)
        this->state_ [i] = temp [i];
      delete []temp;
    }
  this->state_[this->top_++] = s;
}

void
TAO_CodeGen::pop (void)
{
  this->top_--;
}

TAO_CodeGen::CG_STATE
TAO_CodeGen::state (void)
{
  return this->state_[this->top_ - 1]; // top points to the next free slot
}

void
TAO_CodeGen::reset (void)
{
  this->top_ = 1; // the 0th posn is always the INITIAL state
}

void
TAO_CodeGen::node (be_decl *n)
{
  this->node_ = n;
}

be_decl *
TAO_CodeGen::node (void)
{
  return this->node_;
}

void
TAO_CodeGen::visitor_factory (TAO_Visitor_Factory *f)
{
  this->visitor_factory_ = f;
}
