// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_codegen.cpp
//
// = DESCRIPTION
//   Code generation
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_codegen, "$Id$")

TAO_CodeGen *tao_cg = 0;

/* BE global Data */
TAO_CodeGen::TAO_CodeGen (void)
  : client_header_ (0),
    client_stubs_ (0),
    client_inline_ (0),
    server_header_ (0),
    implementation_header_(0),
    implementation_skeleton_(0),
    server_template_header_ (0),
    server_skeletons_ (0),
    server_template_skeletons_ (0),
    server_inline_ (0),
    server_template_inline_ (0),
    gperf_input_stream_ (0),
    gperf_input_filename_ (0),
    curr_os_ (0),
    visitor_factory_ (0),
    strategy_ (TAO_PERFECT_HASH)
{
}

// destructor
TAO_CodeGen::~TAO_CodeGen (void)
{
  delete this->client_header_;
  delete this->server_header_;
  delete this->implementation_header_;
  delete this->implementation_skeleton_;
  delete this->server_template_header_;
  delete this->client_stubs_;
  delete this->server_skeletons_;
  delete this->server_template_skeletons_;
  delete this->client_inline_;
  delete this->server_inline_;
  delete this->server_template_inline_;
  this->curr_os_ = 0;
  //  delete this->visitor_factory_;
}

// visitor factory method
be_visitor *
TAO_CodeGen::make_visitor (be_visitor_context *ctx)
{

  if (!this->visitor_factory_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_CodeGen::make_visitor - "
                         "No Visitor Factory\n\n"),
                        0);
    }

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
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.
  idl_global->validate_included_idl_files ();

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
      const char *suffix = ACE_OS::strrchr (fname, '.');
      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            // bad file name
            return -1;
          else
            suffix = fname;
        }

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
      this->client_header_->print ("#ifndef %s\n", macro_name);
      this->client_header_->print ("#define %s\n\n", macro_name);

      // Including standard files

      // switch between changing or non-changing standard include files
      // include files, so that #include statements can be
      // generated with ""s or <>s respectively, for the standard include
      // files (e.g. tao/corba.h)
      *this->client_header_ << "#include ";

      if (idl_global->changing_standard_include_files () == 1)
        *this->client_header_ << "\"";
      else
        *this->client_header_ << "<";

      *this->client_header_ << "tao/corba.h";

      if (idl_global->changing_standard_include_files () == 1)
        *this->client_header_ << "\"\n";
      else
        *this->client_header_ << ">\n";

      // Other include files

      if (idl_global->export_include () != 0)
        {
          *this->client_header_ << "#include \""
                                    << idl_global->export_include ()
                                << "\"\n";
        }

      // We must include all the skeleton headers corresponding to
      // IDL files included by the current IDL file.
      // We will use the included IDL file names as they appeared
      // in the original main IDL file, not the one  which went
      // thru CC preprocessor.
      for (size_t j = 0;
           j < idl_global->n_included_idl_files ();
           j++)
        {
          char* idl_name =
                idl_global->included_idl_files ()[j];

          // Make a String out of it.
          String idl_name_str = idl_name;

          // Make sure this file was actually got included, not
          // ignored by some #if defined compiler directive.


          // Get the clnt header from the IDL file name.
          const char* client_hdr =
            IDL_GlobalData::be_get_client_hdr (&idl_name_str, 1);

          // Sanity check and then print.
          if (client_hdr != 0)
            {
              this->client_header_->print ("#include \"%s\"\n",
                                               client_hdr);
            }
          else
            {
              ACE_ERROR ((LM_WARNING,
                          "WARNING, invalid file '%s' included\n",
                          idl_name));
            }
        }
      *this->client_header_ << "\n";

      // generate the TAO_EXPORT_MACRO macro
      *this->client_header_ << "#if defined (TAO_EXPORT_MACRO)\n";
      *this->client_header_ << "#undef TAO_EXPORT_MACRO\n";
      *this->client_header_ << "#endif\n";
      *this->client_header_ << "#define TAO_EXPORT_MACRO "
                            << idl_global->export_macro ()
                            << be_nl;

      *this->client_header_ << "#if defined(_MSC_VER)\n"
                            << "#pragma warning(disable:4250)\n"
                            << "#endif /* _MSC_VER */\n\n";

      return 0;
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

  // generate the include statement for the client header. We just
  // need to put only the base names. Path info is not required.
  *this->client_stubs_ << "#include \"" <<
    idl_global->be_get_client_hdr_fname (1) << "\"\n\n";

  *this->client_stubs_ << "#include \"" <<
    idl_global->be_get_server_hdr_fname (1) << "\"\n\n";

  // generate the code that includes the inline file if not included in the
  // header file
  *this->client_stubs_ << "#if !defined (__ACE_INLINE__)\n";
  *this->client_stubs_ << "#include \"" <<
    idl_global->be_get_client_inline_fname (1) << "\"\n";
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

// set the server header stream.
int
TAO_CodeGen::start_server_header (const char *fname)
{
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.
  idl_global->validate_included_idl_files ();

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
      const char *suffix = ACE_OS::strrchr (fname, '.');
      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            // bad file name
            return -1;
          else
            suffix = fname;
        }

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

      this->server_header_->print ("#ifndef %s\n", macro_name);
      this->server_header_->print ("#define %s\n\n", macro_name);

      // We must include all the skeleton headers corresponding to
      // IDL files included by the current IDL file.
      // We will use the included IDL file names as they appeared
      // in the original main IDL file, not the one  which went
      // thru CC preprocessor.
      for (size_t j = 0;
           j < idl_global->n_included_idl_files ();
           ++j)
            {
              char* idl_name =
                idl_global->included_idl_files ()[j];

              // String'ifying the name.
              String idl_name_str (idl_name);

              const char* server_hdr =
                IDL_GlobalData::be_get_server_hdr (&idl_name_str, 1);

              this->server_header_->print ("#include \"%s\"\n",
                                           server_hdr);
            }
      // the server header should include the client header
      *this->server_header_ << "#include \"" <<
        idl_global->be_get_client_hdr_fname (1) << "\"\n\n";

      *this->server_header_ << "#if defined(_MSC_VER)\n"
                            << "#pragma warning(disable:4250)\n"
                            << "#endif /* _MSC_VER */\n\n";

      return 0;
    }
}

// get the server header stream
TAO_OutStream *
TAO_CodeGen::server_header (void)
{
  return this->server_header_;
}

// set the server header stream
int
TAO_CodeGen::start_server_template_header (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->server_template_header_ = factory->make_outstream ();
  if (!this->server_template_header_)
    return -1;

  if (this->server_template_header_->open (fname,
                                           TAO_OutStream::TAO_SVR_TMPL_HDR)
      == -1)
    return -1;
  else
    {
      // now generate the #if !defined clause
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
      const char *suffix = ACE_OS::strrchr (fname, '.');
      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            // bad file name
            return -1;
          else
            suffix = fname;
        }
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

      this->server_template_header_->print ("#ifndef %s\n", macro_name);
      this->server_template_header_->print ("#define %s\n\n", macro_name);

      *this->server_template_header_ << "#if defined(_MSC_VER)\n"
                                     << "#pragma warning(disable:4250)\n"
                                     << "#endif /* _MSC_VER */\n\n";

      return 0;
    }
}

// get the server header stream
TAO_OutStream *
TAO_CodeGen::server_template_header (void)
{
  return this->server_template_header_;
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
    idl_global->be_get_server_hdr_fname (1) << "\"\n\n";

  // generate the code that includes the inline file if not included in the
  // header file
  *this->server_skeletons_ << "#if !defined (__ACE_INLINE__)\n";
  *this->server_skeletons_ << "#include \"" <<
    idl_global->be_get_server_inline_fname (1) << "\"\n";
  *this->server_skeletons_ << "#endif /* !defined INLINE */\n\n";
  return 0;
}

// Get the server skeletons stream.
TAO_OutStream *
TAO_CodeGen::server_skeletons (void)
{
  return this->server_skeletons_;
}

// Start the server template skeleton stream.
int
TAO_CodeGen::start_server_template_skeletons (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->server_template_skeletons_ = factory->make_outstream ();
  if (!this->server_template_skeletons_)
    {
      return -1;
    }

  if (this->server_template_skeletons_->open (fname,
                                              TAO_OutStream::TAO_SVR_TMPL_IMPL)
      == -1)
    return -1;
  else
    {
      // now generate the #if !defined clause
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
      const char *suffix = ACE_OS::strrchr (fname, '.');
      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            // bad file name
            return -1;
          else
            suffix = fname;
        }

      ACE_OS::sprintf (macro_name, "_TAO_IDL_");
      // convert letters in fname to upcase
      for (int i=0; i < (suffix - fname); i++)
        if (isalpha (fname [i]))
          macro_name[i+9] = toupper (fname [i]);
        else if (isdigit (fname [i]))
          macro_name[i+9] = fname[i];
        else
          macro_name[i+9] = '_';

      ACE_OS::strcat (macro_name, "_CPP_");

      this->server_template_skeletons_->print ("#ifndef %s\n", macro_name);
      this->server_template_skeletons_->print ("#define %s\n\n", macro_name);

      *this->server_template_skeletons_ << "#if defined(_MSC_VER)\n"
                                        << "#pragma warning(disable:4250)\n"
                                        << "#endif /* _MSC_VER */\n\n";

      // generate the include statement for the server header
      *this->server_template_skeletons_ << "#include \"" <<
        idl_global->be_get_server_template_hdr_fname (1) << "\"\n\n";

      // generate the code that includes the inline file if not included in the
      // header file
      *this->server_template_skeletons_ << "#if !defined (__ACE_INLINE__)\n";
      *this->server_template_skeletons_ << "#include \"" <<
        idl_global->be_get_server_template_inline_fname (1) << "\"\n";
      *this->server_template_skeletons_ << "#endif /* !defined INLINE */\n\n";
      return 0;

    }
}

// get the server template skeletons stream
TAO_OutStream *
TAO_CodeGen::server_template_skeletons (void)
{
  return this->server_template_skeletons_;
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

// set the server template inline stream
int
TAO_CodeGen::start_server_template_inline (const char *fname)
{
  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->server_template_inline_ = factory->make_outstream ();
  if (!this->server_template_inline_)
    {
      return -1;
    }

  return this->server_template_inline_->open (fname, TAO_OutStream::TAO_SVR_INL);
}

// get the server template inline stream
TAO_OutStream *
TAO_CodeGen::server_template_inline (void)
{
  return this->server_template_inline_;
}


// set the server header stream.
int
TAO_CodeGen::start_implementation_header (const char *fname)
{
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.


  idl_global->validate_included_idl_files ();

  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->implementation_header_ = factory->make_outstream ();
  if (!this->implementation_header_)
    {
      return -1;
    }

  if (this->implementation_header_->open (fname, TAO_OutStream::TAO_IMPL_HDR) == -1)
    return -1;
  else
    {
      // now generate the #ifndef clause
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
      const char *suffix = ACE_OS::strrchr (fname, '.');
      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            // bad file name
            return -1;
          else
            suffix = fname;
        }

      for (int i=0; i < (suffix - fname); i++)
        if (isalpha (fname [i]))
          macro_name[i] = toupper (fname [i]);
        else if (isdigit (fname [i]))
          macro_name[i] = fname[i];
      else
      macro_name[i] = '_';

      ACE_OS::strcat (macro_name, "_H_");

      this->implementation_header_->print ("#ifndef %s\n", macro_name);
      this->implementation_header_->print ("#define %s\n\n", macro_name);

      // We must include all the skeleton headers corresponding to
      // IDL files included by the current IDL file.
      // We will use the included IDL file names as they appeared
      // in the original main IDL file, not the one  which went
      // thru CC preprocessor.
      for (size_t j = 0;
           j < idl_global->n_included_idl_files ();
           ++j)
            {
              char* idl_name =
                idl_global->included_idl_files ()[j];

              // Stringifying the name.
              String idl_name_str (idl_name);

              const char* implementation_hdr =
                IDL_GlobalData::be_get_implementation_hdr (&idl_name_str, 1);

              this->implementation_header_->print ("#include \"%s\"\n",
                                           implementation_hdr);
            }

      *this->implementation_header_ << "#if !defined (ACE_LACKS_PRAGMA_ONCE)\n"
                            << "#pragma once\n"
                            << "#endif /* ACE_LACKS_PRAGMA_ONCE */\n\n";

      const char* server_hdr =
        IDL_GlobalData::be_get_server_hdr_fname (1);

      *this->implementation_header_<< "#include \""<<server_hdr<<"\"\n\n";

      return 0;
    }
}


// get the implementation header stream
TAO_OutStream *
TAO_CodeGen::implementation_header (void)
{
  return this->implementation_header_;
}


// set the implementation skeleton stream.
int
TAO_CodeGen::start_implementation_skeleton (const char *fname)
{
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.
  idl_global->validate_included_idl_files ();

  // retrieve the singleton instance to the outstream factory
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // retrieve a specialized instance
  this->implementation_skeleton_ = factory->make_outstream ();
  if (!this->implementation_skeleton_)
    {
      return -1;
    }

  if (this->implementation_skeleton_->open (fname, TAO_OutStream::TAO_IMPL_SKEL) == -1)
    return -1;
  else
    {



      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
      const char *suffix = ACE_OS::strrchr (fname, '.');


      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            // bad file name
            return -1;
          else
            suffix = fname;
        }

      // convert letters in fname to upcase
      for (int i=0; i < (suffix - fname); i++)
        if (isalpha (fname [i]))
          macro_name[i] = fname [i];
        else
          macro_name[i] = fname[i];


      const char* impl_hdr =
        IDL_GlobalData::be_get_implementation_hdr_fname ();

      this->implementation_skeleton_->print ("#include \"%s\"\n\n", impl_hdr);

      return 0;
    }
}


// get the implementation header stream
TAO_OutStream *
TAO_CodeGen::implementation_skeleton (void)
{
  return this->implementation_skeleton_;
}

// put the last #endif in the client and server headers
int
TAO_CodeGen::end_client_header (void)
{
  // generate the <<= and >>= operators here

  // insert the code to include the inline file
  *this->client_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->client_header_ << "#include \"" <<
    idl_global->be_get_client_inline_fname (1) << "\"\n";
  *this->client_header_ << "#endif /* defined INLINE */\n\n";

  *this->client_header_ << "#if defined(_MSC_VER)\n"
                        << "#pragma warning(default:4250)\n"
                        << "#endif /* _MSC_VER */\n";

  // code to put the last #endif
  *this->client_header_ << "\n#endif /* ifndef */\n";
  return 0;
}

int
TAO_CodeGen::end_server_header (void)
{
  // insert the template header
  *this->server_header_ << "#include \"" <<
    idl_global->be_get_server_template_hdr_fname (1) << "\"\n";

  // insert the code to include the inline file
  *this->server_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->server_header_ << "#include \"" <<
    idl_global->be_get_server_inline_fname (1) << "\"\n";
  *this->server_header_ << "#endif /* defined INLINE */\n\n";

  *this->server_header_ << "#if defined(_MSC_VER)\n"
                        << "#pragma warning(default:4250)\n"
                        << "#endif /* _MSC_VER */\n";

  // code to put the last #endif
  *this->server_header_ << "\n#endif /* ifndef */\n";
  return 0;
}

int
TAO_CodeGen::end_implementation_header (const char *fname)
{
  static char macro_name [NAMEBUFSIZE];

  ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
  const char *suffix = ACE_OS::strrchr (fname, '.');
  if (suffix == 0)
    {
      // File seems to have no extension, so let us take the name
      // as it is.
      if (fname == 0)
        // bad file name
        return -1;
      else
        suffix = fname;
    }


  // convert letters in fname to upcase
  for (int i=0; i < (suffix - fname); i++)
    if (isalpha (fname [i]))
      macro_name[i] = toupper (fname [i]);
    else if (isdigit (fname [i]))
      macro_name[i] = fname[i];
    else
      macro_name[i] = '_';

  ACE_OS::strcat (macro_name, "_H_");


  // code to put the last #endif
  //*this->implementation_header_ << "\n#endif /* %s  */\n";
  this->implementation_header_->print ("\n#endif /* %s  */\n", macro_name);
  return 0;
}

int
TAO_CodeGen::end_server_template_header (void)
{
  // insert the code to include the inline file
  *this->server_template_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->server_template_header_ << "#include \"" <<
    idl_global->be_get_server_template_inline_fname (1) << "\"\n";
  *this->server_template_header_ << "#endif /* defined INLINE */\n\n";

  // insert the code to include the template source file
  *this->server_template_header_
    << "\n#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)\n";
  *this->server_template_header_ << "#include \"" <<
    idl_global->be_get_server_template_skeleton_fname (1) << "\"\n";
  *this->server_template_header_ << "#endif /* defined REQUIRED SOURCE */\n\n";

  // insert the code to include the template pragma
  *this->server_template_header_
    << "\n#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)\n";
  *this->server_template_header_ << "#pragma implementation (\"" <<
    idl_global->be_get_server_template_skeleton_fname (1) << "\")\n";
  *this->server_template_header_ << "#endif /* defined REQUIRED PRAGMA */\n\n";

  *this->server_template_header_ << "#if defined(_MSC_VER)\n"
                                 << "#pragma warning(default:4250)\n"
                                 << "#endif /* _MSC_VER */\n";

  // code to put the last #endif
  *this->server_template_header_ << "\n#endif /* ifndef */\n";
  return 0;
}

int
TAO_CodeGen::end_server_template_skeletons (void)
{
  // code to put the last #endif
  *this->server_template_skeletons_ << "\n#endif /* ifndef */\n";
  return 0;
}

// We use the following helper functions to pass information. This class is the
// best place to pass such information rather than passing information through
// global variables spread everywhere. This class is a singleton and is
// effectively a global.

void
TAO_CodeGen::gperf_input_stream (TAO_OutStream *os)
{
#if !defined (linux)
  // This causes a seg fault on Linux RH 5.1.  Let it leak . . .
  delete this->gperf_input_stream_;
#endif /* ! linux */
  this->gperf_input_stream_ = os;
}

TAO_OutStream *
TAO_CodeGen::gperf_input_stream (void)
{
  return this->gperf_input_stream_;
}

void
TAO_CodeGen::gperf_input_filename (char *filename)
{
  delete[] this->gperf_input_filename_;
  this->gperf_input_filename_ = ACE::strnew (filename);
}

char *
TAO_CodeGen::gperf_input_filename (void)
{
  return this->gperf_input_filename_;
}

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
TAO_CodeGen::config_visitor_factory (void)
{
  // What strategy are we interested in? Interpreted or Compiled Marshaling?
  // This is the top level distinction we make and strategize our visitor
  // factory object accordingly
  if (idl_global->compiled_marshaling ())
    this->visitor_factory_ = TAO_COMPILED_VISITOR_FACTORY::instance ();
  else
    this->visitor_factory_ = TAO_INTERPRETIVE_VISITOR_FACTORY::instance ();
}

void
TAO_CodeGen::lookup_strategy (LOOKUP_STRATEGY s)
{
  this->strategy_ = s;
}

TAO_CodeGen::LOOKUP_STRATEGY
TAO_CodeGen::lookup_strategy (void) const
{
  return this->strategy_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
