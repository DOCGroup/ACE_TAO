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

TAO_IDL_BE_Export TAO_CodeGen *tao_cg = 0;

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
    visitor_factory_ (0)
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

  if (this->visitor_factory_ == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO_CodeGen::make_visitor - ")
                         ACE_TEXT ("No Visitor Factory\n\n")),
                        0);
    }

  return this->visitor_factory_->make_visitor (ctx);
}

// Change the string to all upper case.
const char *
TAO_CodeGen::upcase (const char *str)
{
  static char upcase_str [NAMEBUFSIZE];

  ACE_OS::memset (upcase_str, 
                  '\0', 
                  NAMEBUFSIZE);

  // Convert letters in str to upper case.
  for (unsigned int i=0; i < ACE_OS::strlen (str); i++)
    {
      if (isalpha (str [i]))
        {
          upcase_str[i] = (char) toupper (str [i]);
        }
      else
        {
          // Copy it as it is.
          upcase_str[i] = str[i];
        }
    }

  return upcase_str;
}

// Set the client header stream.
int
TAO_CodeGen::start_client_header (const char *fname)
{
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.
  idl_global->validate_included_idl_files ();

  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->client_header_ = factory->make_outstream ();

  if (!this->client_header_)
    {
      return -1;
    }

  if (this->client_header_->open (fname, TAO_OutStream::TAO_CLI_HDR) == -1)
    {
      return -1;
    }
  else
    {
      // Now generate the #if !defined clause.
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, 
                      '\0', 
                      NAMEBUFSIZE);

      const char *suffix = ACE_OS::strrchr (fname, '.');

      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            {
              // Bad file name.
              return -1;
            }
          else
            {
              suffix = fname;
            }
        }

      ACE_OS::sprintf (macro_name, "_TAO_IDL_");

      // Convert letters in fname to upper case.
      for (int i=0; i < (suffix - fname); i++)
        {
          if (isalpha (fname [i]))
            {
              macro_name[i+9] = (char) toupper (fname [i]);
            }
          else if (isdigit (fname [i]))
            {
              macro_name[i+9] = fname[i];
            }
          else
            {
              macro_name[i+9] = '_';
            }
        }

      ACE_OS::strcat (macro_name, "_H_");

      // Generate the #ifndef ... #define statements.
      this->client_header_->print ("#ifndef %s\n", 
                                   macro_name);
      this->client_header_->print ("#define %s\n\n", 
                                   macro_name);

      if (be_global->pre_include () != 0)
        {
          *this->client_header_ << "#include \""
                                << be_global->pre_include ()
                                << "\"\n";
        }

      // Including standard files.

      // Switch between changing or non-changing standard include files
      // include files, so that #include statements can be
      // generated with ""s or <>s respectively, for the standard include
      // files (e.g. tao/corba.h).
      *this->client_header_ << "#include ";

      if (be_global->changing_standard_include_files () == 1)
        {
          *this->client_header_ << "\"";
        }
      else
        {
          *this->client_header_ << "<";
        }

      *this->client_header_ << "tao/corba.h";

      if (be_global->changing_standard_include_files () == 1)
        {
          *this->client_header_ << "\"\n";
        }
      else
        {
          *this->client_header_ << ">\n";
        }

      // Some compilers don't optimize the #ifndef header include
      // protection, but do optimize based on #pragma once.
      *this->client_header_ << "\n#if !defined (ACE_LACKS_PRAGMA_ONCE)\n"
                            << "# pragma once\n"
                            << "#endif /* ACE_LACKS_PRAGMA_ONCE */\n\n";

      // Other include files.

      if (be_global->stub_export_include () != 0)
        {
          *this->client_header_ << "#include \""
                                << be_global->stub_export_include ()
                                << "\"\n";
        }

      // Include the Messaging files if AMI is enabled.
      if (be_global->ami_call_back () == I_TRUE)
        {
          // Include Messaging skeleton file.
          *this->client_header_ << "#include ";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->client_header_ << "\"";
            }
          else
            {
              *this->client_header_ << "<";
            }

          *this->client_header_ << "tao/MessagingC.h";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->client_header_ << "\"\n";
            }
          else
            {
              *this->client_header_ << ">\n";
            }

          // Including Asynch Invocation file.
          *this->client_header_ << "#include ";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->client_header_ << "\"";
            }
          else
            {
              *this->client_header_ << "<";
            }

          *this->client_header_ << "tao/Asynch_Invocation.h";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->client_header_ << "\"\n";
            }
          else
            {
              *this->client_header_ << ">\n";
            }
        }

      // Include the smart proxy base class if smart proxies are enabled.
      if (be_global->gen_smart_proxies () == I_TRUE)
        {
          *this->client_header_ << "#include ";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->client_header_ << "\"";
            }
          else
            {
              *this->client_header_ << "<";
            }

          *this->client_header_ << "tao/SmartProxies/Smart_Proxies.h";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->client_header_ << "\"\n";
            }
          else
            {
              *this->client_header_ << ">\n";
            }
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
          char* idl_name = idl_global->included_idl_files ()[j];

          // Make a String out of it.
          UTL_String idl_name_str = idl_name;

          // Make sure this file was actually got included, not
          // ignored by some #if defined compiler directive.


          // Get the clnt header from the IDL file name.
          const char* client_hdr =
            BE_GlobalData::be_get_client_hdr (&idl_name_str, 
                                              1);

          // Sanity check and then print.
          if (client_hdr != 0)
            {
              this->client_header_->print ("#include \"%s\"\n",
                                           client_hdr);
            }
          else
            {
              ACE_ERROR ((LM_WARNING,
                          ACE_TEXT ("WARNING, invalid file '%s' included\n"),
                          idl_name));
            }
        }
      *this->client_header_ << "\n";

      // Generate the TAO_EXPORT_MACRO macro.
      *this->client_header_ << "#if defined (TAO_EXPORT_MACRO)\n";
      *this->client_header_ << "#undef TAO_EXPORT_MACRO\n";
      *this->client_header_ << "#endif\n";
      *this->client_header_ << "#define TAO_EXPORT_MACRO "
                            << be_global->stub_export_macro ()
                            << be_nl << be_nl;

      // Generate export macro for nested classes.
      *this->client_header_ << "#if defined (TAO_EXPORT_NESTED_CLASSES)\n"
                            << "#  if defined (TAO_EXPORT_NESTED_MACRO)\n"
                            << "#    undef TAO_EXPORT_NESTED_MACRO\n"
                            << "#  endif /* defined "
                            << "(TAO_EXPORT_NESTED_MACRO) */\n"
                            << "#  define TAO_EXPORT_NESTED_MACRO "
                            << be_global->stub_export_macro ()
                            << be_nl
                            << "#endif /* TAO_EXPORT_NESTED_CLASSES */\n\n";

      *this->client_header_ << "#if defined(_MSC_VER)\n"
                            << "#if (_MSC_VER >= 1200)\n"
                            << "#pragma warning(push)\n"
                            << "#endif /* _MSC_VER >= 1200 */\n"
                            << "#pragma warning(disable:4250)\n";

      if (be_global->use_raw_throw ())
        {
          *this->client_header_ << "#pragma warning(disable:4290)\n";
        }

      *this->client_header_ << "#endif /* _MSC_VER */\n\n";

      return 0;
    }
}

// Get the client header stream.
TAO_OutStream *
TAO_CodeGen::client_header (void)
{
  return this->client_header_;
}

// Set the client stub stream.
int
TAO_CodeGen::start_client_stubs (const char *fname)
{
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->client_stubs_ = factory->make_outstream ();

  if (!this->client_stubs_)
    {
      return -1;
    }

  if (this->client_stubs_->open (fname, TAO_OutStream::TAO_CLI_IMPL) == -1)
    {
      return -1;
    }

  // Generate the include statement for the precompiled header file.
  if (be_global->pch_include ())
    {
      *this->client_stubs_ << "#include \""
                           << be_global->pch_include ()
                           << "\"\n\n";
    }

  // Generate the include statement for the client header. We just
  // need to put only the base names. Path info is not required.
  *this->client_stubs_ << "#include \""
                       << be_global->be_get_client_hdr_fname (1)
                       << "\"\n\n";

  // Include the RequestInfo_Util utility header.  Used by the
  // PortableInterceptor::RequestInfo subclasses.
  *this->client_stubs_ << "#if TAO_HAS_INTERCEPTORS == 1\n"
                        << "#include ";

  if (be_global->changing_standard_include_files () == 1)
    {
      *this->client_stubs_ << "\"";
    }
  else
    {
      *this->client_stubs_ << "<";
    }

  *this->client_stubs_ << "tao/RequestInfo_Util.h";

  if (be_global->changing_standard_include_files () == 1)
    {
      *this->client_stubs_ << "\"\n";
    }
  else
    {
      *this->client_stubs_ << ">\n";
    }

  *this->client_stubs_ << "#endif  /* TAO_HAS_INTERCEPTORS == 1 */\n\n";

  // Generate the code that includes the inline file if not included in the
  // header file.
  *this->client_stubs_ << "#if !defined (__ACE_INLINE__)\n";
  *this->client_stubs_ << "#include \""
                       << be_global->be_get_client_inline_fname (1)
                       << "\"\n";
  *this->client_stubs_ << "#endif /* !defined INLINE */\n\n";

  return 0;
}

// Get the client stubs stream.
TAO_OutStream *
TAO_CodeGen::client_stubs (void)
{
  return this->client_stubs_;
}

// Set the client inline stream.
int
TAO_CodeGen::start_client_inline (const char *fname)
{
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->client_inline_ = factory->make_outstream ();

  if (!this->client_inline_)
    {
      return -1;
    }

  return this->client_inline_->open (fname, TAO_OutStream::TAO_CLI_INL);
}

// Get the client inline stream.
TAO_OutStream *
TAO_CodeGen::client_inline (void)
{
  return this->client_inline_;
}

// Set the server header stream.
int
TAO_CodeGen::start_server_header (const char *fname)
{
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.
  idl_global->validate_included_idl_files ();

  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->server_header_ = factory->make_outstream ();

  if (!this->server_header_)
    {
      return -1;
    }

  if (this->server_header_->open (fname, TAO_OutStream::TAO_SVR_HDR) == -1)
    {
      return -1;
    }
  else
    {
      // Now generate the #if !defined clause.
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, '\0', NAMEBUFSIZE);
      const char *suffix = ACE_OS::strrchr (fname, '.');

      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            {
              // Bad file name.
              return -1;
            }
          else
            {
              suffix = fname;
            }
        }

      ACE_OS::sprintf (macro_name, "_TAO_IDL_");

      // Convert letters in fname to upper case.
      for (int i=0; i < (suffix - fname); i++)
        {
          if (isalpha (fname [i]))
            {
              macro_name[i+9] = (char) toupper (fname [i]);
            }
          else if (isdigit (fname [i]))
            {
              macro_name[i+9] = fname[i];
            }
          else
            {
              macro_name[i+9] = '_';
            }
        }

      ACE_OS::strcat (macro_name, "_H_");

      this->server_header_->print ("#ifndef %s\n", macro_name);
      this->server_header_->print ("#define %s\n\n", macro_name);

      if (be_global->pre_include () != 0)
        {
          *this->server_header_ << "#include \""
                                << be_global->pre_include ()
                                << "\"\n";
        }

      // Include the Messaging files if AMI is enabled.
      if (be_global->ami_call_back () == I_TRUE)
        {
          // Include Messaging skeleton file.
          *this->server_header_ << "#include ";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->server_header_ << "\"";
            }
          else
            {
              *this->server_header_ << "<";
            }

          *this->server_header_ << "tao/PortableServer/MessagingS.h";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->server_header_ << "\"\n";
            }
          else
            {
              *this->server_header_ << ">\n";
            }

          // Including Asynch Invocation file.
          *this->server_header_ << "#include ";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->server_header_ << "\"";
            }
          else
            {
              *this->server_header_ << "<";
            }

          *this->server_header_ << "tao/Asynch_Invocation.h";

          if (be_global->changing_standard_include_files () == 1)
            {
              *this->server_header_ << "\"\n";
            }
          else
            {
              *this->server_header_ << ">\n";
            }
        }

      // We must include all the skeleton headers corresponding to
      // IDL files included by the current IDL file.
      // We will use the included IDL file names as they appeared
      // in the original main IDL file, not the one  which went
      // thru CC preprocessor.
      for (size_t j = 0;
           j < idl_global->n_included_idl_files ();
           ++j)
        {
          char* idl_name = idl_global->included_idl_files ()[j];

          // String'ifying the name.
          UTL_String idl_name_str (idl_name);

          const char* server_hdr =
            BE_GlobalData::be_get_server_hdr (&idl_name_str, 1);

          this->server_header_->print ("#include \"%s\"\n",
                                       server_hdr);
        }

      // The server header should include the client header.
      *this->server_header_ << "#include \""
                            << be_global->be_get_client_hdr_fname (1)
                            << "\"\n\n";

      // Some compilers don't optimize the #ifndef header include
      // protection, but do optimize based on #pragma once.
      *this->server_header_ << "\n#if !defined (ACE_LACKS_PRAGMA_ONCE)\n"
                            << "# pragma once\n"
                            << "#endif /* ACE_LACKS_PRAGMA_ONCE */\n\n";

      // Include the definitions for the PortableServer namespace,
      // this forces the application to link the POA library, a good
      // thing, because we need the definitions there, it also
      // registers the POA factory with the Service_Configurator, so
      // the ORB can automatically find it.
      *this->server_header_
        << "#include \"tao/PortableServer/PortableServer.h\"\n"
        << "#include \"tao/PortableServer/Servant_Base.h\"\n"
        << "#include \"tao/PortableServer/Collocated_Object.h\"\n"
        << "#include \"tao/PortableServer/ThruPOA_Object_Proxy_Impl.h\"\n"
        << "#include \"tao/PortableServer/Direct_Object_Proxy_Impl.h\"\n"
        << "#include \"tao/PortableServer/ServerRequestInfo.h\"\n"
        << "\n";

      *this->server_header_ << "#if defined(_MSC_VER)\n"
                            << "#if (_MSC_VER >= 1200)\n"
                            << "#pragma warning(push)\n"
                            << "#endif /* _MSC_VER >= 1200 */\n"
                            << "#pragma warning(disable:4250)\n";

      if (be_global->use_raw_throw ())
        {
          *this->server_header_ << "#pragma warning(disable:4290)\n";
        }

      *this->server_header_ << "#endif /* _MSC_VER */\n\n";

      if (be_global->skel_export_include () != 0)
        {
          *this->server_header_ << "#include \""
                                << be_global->skel_export_include ()
                                << "\"\n";

          // Generate the TAO_EXPORT_MACRO macro.
          *this->server_header_ << "#if defined (TAO_EXPORT_MACRO)\n";
          *this->server_header_ << "#undef TAO_EXPORT_MACRO\n";
          *this->server_header_ << "#endif\n";
          *this->server_header_ << "#define TAO_EXPORT_MACRO "
                                << be_global->skel_export_macro ()
                                << be_nl;

          // Generate export macro for nested classes.
          *this->server_header_
            << "#if defined (TAO_EXPORT_NESTED_CLASSES)\n"
            << "#  if defined (TAO_EXPORT_NESTED_MACRO)\n"
            << "#    undef TAO_EXPORT_NESTED_MACRO\n"
            << "#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */\n"
            << "#  define TAO_EXPORT_NESTED_MACRO "
            << be_global->skel_export_macro ()
            << be_nl
            << "#endif /* TAO_EXPORT_NESTED_CLASSES */\n";
        }

      return 0;
    }
}

// Get the server header stream.
TAO_OutStream *
TAO_CodeGen::server_header (void)
{
  return this->server_header_;
}

// Set the server header stream.
int
TAO_CodeGen::start_server_template_header (const char *fname)
{
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->server_template_header_ = factory->make_outstream ();

  if (!this->server_template_header_)
    {
      return -1;
    }

  if (this->server_template_header_->open (fname,
                                           TAO_OutStream::TAO_SVR_TMPL_HDR)
        == -1)
    {
      return -1;
    }
  else
    {
      // Now generate the #if !defined clause.
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, 
                      '\0', 
                      NAMEBUFSIZE);

      const char *suffix = ACE_OS::strrchr (fname, '.');

      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            {
              // Bad file name.
              return -1;
            }
          else
            {
              suffix = fname;
            }
        }

      ACE_OS::sprintf (macro_name, 
                       "_TAO_IDL_");

      // Convert letters in fname to upper case.
      for (int i = 0; i < (suffix - fname); ++i)
        {
          if (isalpha (fname [i]))
            {
              macro_name[i+9] = (char) toupper (fname [i]);
            }
          else if (isdigit (fname [i]))
            {
              macro_name[i+9] = fname[i];
            }
          else
            {
              macro_name[i+9] = '_';
            }
        }

      ACE_OS::strcat (macro_name, "_H_");

      this->server_template_header_->print ("#ifndef %s\n", macro_name);
      this->server_template_header_->print ("#define %s\n\n", macro_name);

      if (be_global->pre_include () != 0)
        {
          *this->server_template_header_ << "#include \""
                                         << be_global->pre_include ()
                                         << "\"\n";
        }

      *this->server_template_header_ << "#if defined(_MSC_VER)\n"
                                     << "#if (_MSC_VER >= 1200)\n"
                                     << "#pragma warning(push)\n"
                                     << "#endif /* _MSC_VER >= 1200 */\n"
                                     << "#pragma warning(disable:4250)\n";

      if (be_global->use_raw_throw ())
        {
          *this->server_template_header_ << "#pragma warning(disable:4290)\n";
        }

      *this->server_template_header_ << "#endif /* _MSC_VER */\n\n";

      return 0;
    }
}

// Get the server header stream.
TAO_OutStream *
TAO_CodeGen::server_template_header (void)
{
  return this->server_template_header_;
}

// Set the server skeletons stream.
int
TAO_CodeGen::start_server_skeletons (const char *fname)
{
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->server_skeletons_ = factory->make_outstream ();

  if (!this->server_skeletons_)
    {
      return -1;
    }

  if (this->server_skeletons_->open (fname, 
                                     TAO_OutStream::TAO_SVR_IMPL) 
        == -1)
    {
      return -1;
    }

  // Generate the include statement for the precompiled header file.
  if (be_global->pch_include ())
    {
      *this->server_skeletons_ << "#include \""
                               << be_global->pch_include ()
                               << "\"\n\n";
    }

  // Generate the include statement for the server header.
  *this->server_skeletons_
    << "#include \""
    << be_global->be_get_server_hdr_fname (1)
    << "\"\n"
    << "#include \"tao/PortableServer/Object_Adapter.h\"\n"
    << "#include \"tao/PortableServer/Operation_Table.h\"\n"
    << "\n";

  // Include the RequestInfo_Util utility header.  Used by the
  // PortableInterceptor::RequestInfo subclasses.
  *this->server_skeletons_ << "#if TAO_HAS_INTERCEPTORS == 1\n"
                        << "#include ";

  if (be_global->changing_standard_include_files () == 1)
    {
      *this->server_skeletons_ << "\"";
    }
  else
    {
      *this->server_skeletons_ << "<";
    }

  *this->server_skeletons_ << "tao/RequestInfo_Util.h";

  if (be_global->changing_standard_include_files () == 1)
    {
      *this->server_skeletons_ << "\"\n";
    }
  else
    {
      *this->server_skeletons_ << ">\n";
    }

  *this->server_skeletons_ << "#endif  /* TAO_HAS_INTERCEPTORS == 1 */\n\n";


  // Generate the code that includes the inline file if not included in the
  // header file.
  *this->server_skeletons_ << "#if !defined (__ACE_INLINE__)\n";
  *this->server_skeletons_ << "#include \""
                           << be_global->be_get_server_inline_fname (1)
                           << "\"\n";
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
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->server_template_skeletons_ = factory->make_outstream ();

  if (!this->server_template_skeletons_)
    {
      return -1;
    }

  if (this->server_template_skeletons_->open (fname,
                                              TAO_OutStream::TAO_SVR_TMPL_IMPL)
        == -1)
    {
      return -1;
    }
  else
    {
      // Now generate the #if !defined clause.
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, 
                      '\0', 
                      NAMEBUFSIZE);

      const char *suffix = ACE_OS::strrchr (fname, '.');

      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            {
              // Bad file name.
              return -1;
            }
          else
            {
              suffix = fname;
            }
        }

      ACE_OS::sprintf (macro_name, "_TAO_IDL_");

      // Convert letters in fname to upper case.
      for (int i=0; i < (suffix - fname); i++)
        {
          if (isalpha (fname [i]))
            {
              macro_name[i+9] = (char) toupper (fname [i]);
            }
          else if (isdigit (fname [i]))
            {
              macro_name[i+9] = fname[i];
            }
          else
            {
              macro_name[i+9] = '_';
            }
        }

      ACE_OS::strcat (macro_name, "_CPP_");

      this->server_template_skeletons_->print ("#ifndef %s\n", macro_name);
      this->server_template_skeletons_->print ("#define %s\n\n", macro_name);

      // Generate the include statement for the server header.
      *this->server_template_skeletons_
          << "#include \""
          << be_global->be_get_server_template_hdr_fname (1)
          << "\"\n\n";

      // Generate the code that includes the inline file if not included in the
      // header file.
      *this->server_template_skeletons_ << "#if !defined (__ACE_INLINE__)\n";
      *this->server_template_skeletons_
          << "#include \""
          << be_global->be_get_server_template_inline_fname (1)
          << "\"\n";
      *this->server_template_skeletons_ << "#endif /* !defined INLINE */\n\n";

      return 0;
    }
}

// Get the server template skeletons stream.
TAO_OutStream *
TAO_CodeGen::server_template_skeletons (void)
{
  return this->server_template_skeletons_;
}

// Set the server inline stream.
int
TAO_CodeGen::start_server_inline (const char *fname)
{
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->server_inline_ = factory->make_outstream ();

  if (!this->server_inline_)
    {
      return -1;
    }

  return this->server_inline_->open (fname, TAO_OutStream::TAO_SVR_INL);
}

// Get the server inline stream.
TAO_OutStream *
TAO_CodeGen::server_inline (void)
{
  return this->server_inline_;
}

// Set the server template inline stream.
int
TAO_CodeGen::start_server_template_inline (const char *fname)
{
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->server_template_inline_ = factory->make_outstream ();

  if (!this->server_template_inline_)
    {
      return -1;
    }

  return this->server_template_inline_->open (fname, 
                                              TAO_OutStream::TAO_SVR_INL);
}

// Get the server template inline stream.
TAO_OutStream *
TAO_CodeGen::server_template_inline (void)
{
  return this->server_template_inline_;
}


// Set the server header stream.
int
TAO_CodeGen::start_implementation_header (const char *fname)
{
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.


  idl_global->validate_included_idl_files ();

  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->implementation_header_ = factory->make_outstream ();

  if (!this->implementation_header_)
    {
      return -1;
    }

  if (this->implementation_header_->open (fname,
                                          TAO_OutStream::TAO_IMPL_HDR)
        == -1)
    {
      return -1;
    }
  else
    {
      // Now generate the #ifndef clause.
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, 
                      '\0', 
                      NAMEBUFSIZE);

      const char *suffix = ACE_OS::strrchr (fname, '.');

      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            {
              // Bad file name.
              return -1;
            }
          else
            {
              suffix = fname;
            }
        }

      for (int i = 0; i < (suffix - fname); ++i)
        {
          if (isalpha (fname [i]))
            {
              macro_name[i] = (char) toupper (fname [i]);
            }
          else if (isdigit (fname [i]))
            {
              macro_name[i] = fname[i];
            }
          else
            {
              macro_name[i] = '_';
            }
        }

      ACE_OS::strcat (macro_name, "_H_");

      this->implementation_header_->print ("#ifndef %s\n", macro_name);
      this->implementation_header_->print ("#define %s\n\n", macro_name);

      const char* server_hdr = BE_GlobalData::be_get_server_hdr_fname (1);

      *this->implementation_header_<< "#include \"" << server_hdr <<"\"\n\n";

      *this->implementation_header_
        << "#if !defined (ACE_LACKS_PRAGMA_ONCE)\n"
        << "#pragma once\n"
        << "#endif /* ACE_LACKS_PRAGMA_ONCE */\n\n";

      return 0;
    }
}


// Get the implementation header stream.
TAO_OutStream *
TAO_CodeGen::implementation_header (void)
{
  return this->implementation_header_;
}


// Set the implementation skeleton stream.
int
TAO_CodeGen::start_implementation_skeleton (const char *fname)
{
  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.
  idl_global->validate_included_idl_files ();

  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Retrieve a specialized instance.
  this->implementation_skeleton_ = factory->make_outstream ();

  if (!this->implementation_skeleton_)
    {
      return -1;
    }

  if (this->implementation_skeleton_->open (fname,
                                            TAO_OutStream::TAO_IMPL_SKEL)
        == -1)
    {
      return -1;
    }
  else
    {
      static char macro_name [NAMEBUFSIZE];

      ACE_OS::memset (macro_name, 
                      '\0', 
                      NAMEBUFSIZE);

      const char *suffix = ACE_OS::strrchr (fname, '.');


      if (suffix == 0)
        {
          // File seems to have no extension, so let us take the name
          // as it is.
          if (fname == 0)
            {
              // Bad file name.
              return -1;
            }
          else
            {
              suffix = fname;
            }
        }

      // Convert letters in fname to upper case.
      for (int i = 0; i < (suffix - fname); ++i)
        {
          if (isalpha (fname [i]))
            {
              macro_name[i] = fname [i];
            }
          else
            {
              macro_name[i] = fname[i];
            }
        }

      const char* impl_hdr =
        BE_GlobalData::be_get_implementation_hdr_fname ();

      this->implementation_skeleton_->print ("#include \"%s\"\n\n", 
                                             impl_hdr);

      return 0;
    }
}


// Get the implementation header stream.
TAO_OutStream *
TAO_CodeGen::implementation_skeleton (void)
{
  return this->implementation_skeleton_;
}

// Put the last #endif in the client and server headers.
int
TAO_CodeGen::end_client_header (void)
{
  // Generate the <<= and >>= operators here.

  // Insert the code to include the inline file.
  *this->client_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->client_header_ << "#include \""
                        << be_global->be_get_client_inline_fname (1)
                        << "\"\n";
  *this->client_header_ << "#endif /* defined INLINE */\n\n";

  *this->client_header_ << "#if defined(_MSC_VER) && (_MSC_VER >= 1200)\n"
                        << "#pragma warning(pop)\n"
                        << "#endif /* _MSC_VER */\n";

  // Code to put the last #endif.
  *this->client_header_ << "\n";

  if (be_global->post_include () != 0)
    {
      *this->client_header_ << "#include \""
                            << be_global->post_include ()
                            << "\"\n";
    }

  *this->client_header_ << "#endif /* ifndef */\n";
  return 0;
}

int
TAO_CodeGen::end_server_header (void)
{
  // Insert the template header.
  if (be_global->gen_tie_classes ())
    {
      *this->server_header_ << "#include \""
                            << be_global->be_get_server_template_hdr_fname (1)
                            << "\"\n";
    }

  // Insert the code to include the inline file.
  *this->server_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->server_header_ << "#include \""
                        << be_global->be_get_server_inline_fname (1)
                        << "\"\n";
  *this->server_header_ << "#endif /* defined INLINE */\n\n";

  *this->server_header_ << "#if defined(_MSC_VER) && (_MSC_VER >= 1200)\n"
                        << "#pragma warning(pop)\n"
                        << "#endif /* _MSC_VER */\n";

  // Code to put the last #endif.
  *this->server_header_ << "\n";

  if (be_global->post_include () != 0)
    {
      *this->server_header_ << "#include \""
                            << be_global->post_include ()
                            << "\"\n";
    }

  *this->server_header_ << "#endif /* ifndef */\n";
  return 0;
}

int
TAO_CodeGen::end_implementation_header (const char *fname)
{
  static char macro_name [NAMEBUFSIZE];

  ACE_OS::memset (macro_name, 
                  '\0', 
                  NAMEBUFSIZE);

  const char *suffix = ACE_OS::strrchr (fname, '.');

  if (suffix == 0)
    {
      // File seems to have no extension, so let us take the name
      // as it is.
      if (fname == 0)
        {
          // Bad file name.
          return -1;
        }
      else
        {
          suffix = fname;
        }
    }

  // Convert letters in fname to upper case.
  for (int i = 0; i < (suffix - fname); ++i)
    {
      if (isalpha (fname [i]))
        {
          macro_name[i] = (char) toupper (fname [i]);
        }
      else if (isdigit (fname [i]))
        {
          macro_name[i] = fname[i];
        }
      else
        {
          macro_name[i] = '_';
        }
    }

  ACE_OS::strcat (macro_name, 
                  "_H_");

  // Code to put the last #endif.
  this->implementation_header_->print ("\n#endif /* %s  */\n", 
                                       macro_name);
  return 0;
}

int
TAO_CodeGen::end_server_template_header (void)
{
  // Insert the code to include the inline file.
  *this->server_template_header_ << "\n#if defined (__ACE_INLINE__)\n";
  *this->server_template_header_
      << "#include \""
      << be_global->be_get_server_template_inline_fname (1)
      << "\"\n";
  *this->server_template_header_ << "#endif /* defined INLINE */\n\n";

  // Insert the code to include the template source file.
  *this->server_template_header_
      << "\n#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)\n";
  *this->server_template_header_
      << "#include \""
      << be_global->be_get_server_template_skeleton_fname (1)
      << "\"\n";
  *this->server_template_header_ << "#endif /* defined REQUIRED SOURCE */\n\n";

  // Insert the code to include the template pragma.
  *this->server_template_header_
      << "\n#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)\n";
  *this->server_template_header_
      << "#pragma implementation (\""
      << be_global->be_get_server_template_skeleton_fname (1)
      << "\")\n";
  *this->server_template_header_ << "#endif /* defined REQUIRED PRAGMA */\n\n";

  *this->server_template_header_ << "#if defined(_MSC_VER) && (_MSC_VER >= 1200)\n"
                                 << "#pragma warning(pop)\n"
                                 << "#endif /* _MSC_VER */\n";

  // Code to put the last #endif.
  *this->server_template_header_ << "\n";

  if (be_global->post_include () != 0)
    {
      *this->server_template_header_ << "#include \""
                                     << be_global->post_include ()
                                     << "\"\n";
    }

  *this->server_template_header_ << "#endif /* ifndef */\n";
  return 0;
}

int
TAO_CodeGen::end_server_template_skeletons (void)
{
  // Code to put the last #endif.
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
  delete  [] this->gperf_input_filename_;
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
  // We have removed interpreted marshaling from TAO, so
  // TAO_INTERPRETIVE_VISITOR_FACTORY is no more.
  this->visitor_factory_ = TAO_COMPILED_VISITOR_FACTORY::instance ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
