/* -*- c++ -*- */
// ============================================================================
//
// $Id$
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_codegen.cpp
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

TAO_CodeGen *tao_cg = TAO_CODEGEN::instance ();

/* BE global Data */
TAO_CodeGen::TAO_CodeGen (void)
  : client_header_ (0),
    client_stubs_ (0),
    client_inline_ (0),
    server_header_ (0),
    server_skeletons_ (0),
    server_inline_ (0),
    curr_os_ (0),
    visitor_factory_ (0)
{
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
  delete this->visitor_factory_;
}

// visitor factory method
be_visitor *
TAO_CodeGen::make_visitor (be_visitor_context *ctx)
{
  ACE_ASSERT (this->visitor_factory_ != 0);
  return this->visitor_factory_->make_visitor (ctx);
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
TAO_CodeGen::start_client_header (const char *fname)
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
	    {
	      if (isalpha (fname [i]))
		macro_name[i+9] = toupper (fname [i]);
	      else if (isdigit (fname [i]))
		macro_name[i+9] = fname[i];
	      else
		macro_name[i+9] = '_';
	    }

          ACE_OS::strcat (macro_name, "_H_");

          // generate the #ifndef ... #define statements
          this->client_header_->print ("#if !defined (%s)\n", macro_name);
          this->client_header_->print ("#define %s\n\n", macro_name);

          // generate the TAO_EXPORT_MACRO macro
          *this->client_header_ << "#if !defined (TAO_EXPORT_MACRO)\n";
          *this->client_header_ << "#define TAO_EXPORT_MACRO "
                                << idl_global->export_macro () << be_nl;
          *this->client_header_ << "#endif\n";

          *this->client_header_ << "#include \"tao/corba.h\"\n";

	  if (idl_global->export_include () != 0)
	    {
	      *this->client_header_ << "#include \""
				    << idl_global->export_include ()
				    << "\"\n";
	    }

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

	      if (client_hdr != 0)
		{
		  this->client_header_->print ("#include \"%s\"\n",
					       client_hdr);
		}
	      else
		{
		  ACE_ERROR ((LM_WARNING,
			      "WARNING, invalid file '%s' included\n",
			      idl_name->get_string ()));
		}
	    }
	  *this->client_header_ << "\n";

	  *this->client_header_ << "#if defined(_MSC_VER)\n"
				<< "#pragma warning(disable:4250)\n"
				<< "#endif /* _MSC_VER */\n\n";

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
TAO_CodeGen::start_client_stubs (const char *fname)
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
  *this->client_stubs_ << "#endif /* !defined INLINE */\n\n";
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
TAO_CodeGen::start_client_inline (const char *fname)
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
TAO_CodeGen::start_server_header (const char *fname)
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
            if (isalpha (fname [i]))
              macro_name[i+9] = toupper (fname [i]);
            else if (isdigit (fname [i]))
              macro_name[i+9] = fname[i];
            else
              macro_name[i+9] = '_';

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

	  *this->server_header_ << "#if defined(_MSC_VER)\n"
				<< "#pragma warning(disable:4250)\n"
				<< "#endif /* _MSC_VER */\n\n";

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
TAO_CodeGen::start_server_skeletons (const char *fname)
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
  *this->server_skeletons_ << "#endif /* !defined INLINE */\n\n";
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
TAO_CodeGen::start_server_inline (const char *fname)
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
  // generate the <<= and >>= operators here

  // insert the code to include the inline file
  *this->client_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->client_header_ << "#include \"" <<
    idl_global->be_get_client_inline_fname () << "\"\n";
  *this->client_header_ << "#endif /* defined INLINE */\n\n";

  *this->client_header_ << "#if defined(_MSC_VER)\n"
			<< "#pragma warning(default:4250)\n"
			<< "#endif /* _MSC_VER */\n";

  // code to put the last #endif
  *this->client_header_ << "\n#endif /* if !defined */\n";
  return 0;
}

int
TAO_CodeGen::end_server_header (void)
{
  // insert the code to include the inline file
  *this->server_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->server_header_ << "#include \"" <<
    idl_global->be_get_server_inline_fname () << "\"\n";
  *this->server_header_ << "#endif /* defined INLINE */\n\n";

  *this->server_header_ << "#if defined(_MSC_VER)\n"
			<< "#pragma warning(default:4250)\n"
			<< "#endif /* _MSC_VER */\n";

  // code to put the last #endif
  *this->server_header_ << "\n#endif /* if !defined */\n";
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


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
