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

#include "be_codegen.h"
#include "be_helper.h"
#include "be_visitor_factory.h"
#include "be_stream_factory.h"
#include "be_extern.h"
#include "global_extern.h"
#include "utl_string.h"
#include "idl_defines.h"
#include "ace/OS_NS_ctype.h"

ACE_RCSID (be,
           be_codegen,
           "$Id$")

TAO_CodeGen * tao_cg = 0;

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
    anyop_header_ (0),
    anyop_source_ (0),
    gperf_input_stream_ (0),
    gperf_input_filename_ (0),
    curr_os_ (0),
    visitor_factory_ (0)
{
}

// destructor
TAO_CodeGen::~TAO_CodeGen (void)
{
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
  for (unsigned int i = 0; i < ACE_OS::strlen (str); ++i)
    {
      if (ACE_OS::ace_isalpha (str[i]))
        {
          upcase_str[i] = static_cast<char> (ACE_OS::ace_toupper (str[i]));
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
  if (0 == fname)
    {
      // Bad file name.
      return -1;
    }

  // @@ We are making use of "included_idl_files" that is in the
  // idl_global. We need to make sure the validity of those files.
  idl_global->validate_included_idl_files ();

  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Clean up between multiple files.
  delete this->client_header_;
  this->client_header_ = factory->make_outstream ();

  if (!this->client_header_)
    {
      return -1;
    }

  if (this->client_header_->open (fname, TAO_OutStream::TAO_CLI_HDR) == -1)
    {
      return -1;
    }

  *this->client_header_ << be_nl << "// TAO_IDL - Generated from" << be_nl
                        << "// " << __FILE__ << ":" << __LINE__
                        << be_nl << be_nl;

  // Generate the #ident string, if any.
  this->gen_ident_string (this->client_header_);

  ACE_CString pidl_checker (idl_global->filename ()->get_string ());
  bool const got_pidl =
    (pidl_checker.substr (pidl_checker.length () - 5) == ".pidl");

  if (be_global->gen_dcps_type_support_only ())
    {
      if (!got_pidl)
        {
          // Generate #ifndef string which is different from
          // no dcps type support.
          this->gen_ifndef_string (fname,
                                  this->client_header_,
                                  "_TAO_IDL_DDS_",
                                  "_H_");
        }
      else
        {
          // Generate the #ifndef clause.
          this->gen_ifndef_string (fname,
                                  this->client_header_,
                                  "_TAO_PIDL_DDS_",
                                  "_H_");
        }

      if (be_global->stub_export_include () != 0)
        {
          *this->client_header_ << "\n#include /**/ \""
                                << be_global->stub_export_include ()
                                << "\"";
        }

      // DDS/DCPS marshaling.
      this->gen_cond_file_include (
          true,
          "dds/DCPS/Serializer.h",
          this->client_header_
        );

      // Generate the includes of tao version sequence header.
      this->client_header_->print ("\n#include \"tao/%s\"",
                                          fname);

      // Add #if 0 to disable TAO specific code
      *this->client_header_ << be_nl << be_nl
        << "#if 0 // disable TAO specific code"
        << be_nl << be_nl;
    }
  else
    {
      if (!got_pidl)
        {
          // Generate the #ifndef clause.
          this->gen_ifndef_string (fname,
                                  this->client_header_,
                                  "_TAO_IDL_",
                                  "_H_");
        }
      else
        {
          // Generate the #ifndef clause.
          this->gen_ifndef_string (fname,
                                  this->client_header_,
                                  "_TAO_PIDL_",
                                  "_H_");
        }
    }

  if (be_global->pre_include () != 0)
    {
      *this->client_header_ << "#include /**/ \""
                            << be_global->pre_include ()
                            << "\"\n\n";
    }

  if (be_global->include_guard () != 0)
    {
      *this->client_header_ << "#ifndef "
                            << be_global->include_guard ()
                            << "\n";

      *this->client_header_ << "#error "
                            << "You should not include " << fname;

      if (be_global->safe_include () != 0)
        {
          *this->client_header_ << ", use " << be_global->safe_include ();
        }

      *this->client_header_ << "\n";

      *this->client_header_ << "#endif /* "
                            << be_global->include_guard ()
                            << " */\n";
    }

  // To get ACE_UNUSED_ARGS
  this->gen_standard_include (this->client_header_,
                              "ace/config-all.h",
                              true);

  // Some compilers don't optimize the #ifndef header include
  // protection, but do optimize based on #pragma once.
  *this->client_header_ << "\n\n#if !defined (ACE_LACKS_PRAGMA_ONCE)\n"
                        << "# pragma once\n"
                        << "#endif /* ACE_LACKS_PRAGMA_ONCE */";

  *this->client_header_ << be_nl;

  // So the ostream operator header will be visible in included ORB files.
  if (be_global->gen_ostream_operators ())
    {
      *this->client_header_ << "\n#if !defined (GEN_OSTREAM_OPS)\n"
                            << "#define GEN_OSTREAM_OPS\n"
                            << "#endif /* GEN_OSTREAM_OPS */";
    }

  *this->client_header_ << be_nl;

  // Other include files.

  if (be_global->stub_export_include () != 0)
    {
      *this->client_header_ << "\n#include /**/ \""
                            << be_global->stub_export_include ()
                            << "\"";
    }

  if (be_global->unique_include () != 0)
    {
      *this->client_header_ << "\n#include \""
                            << be_global->unique_include ()
                            << "\"";
    }
  else
    {
      this->gen_stub_hdr_includes ();

      size_t const nfiles = idl_global->n_included_idl_files ();

      if (nfiles > 0)
        {
          *this->client_header_ << "\n";
        }

      // We must include all the client headers corresponding to
      // IDL files included by the current IDL file.
      // We will use the included IDL file names as they appeared
      // in the original main IDL file, not the one which went
      // thru CC preprocessor.
      for (size_t j = 0; j < nfiles; ++j)
        {
          char* idl_name = idl_global->included_idl_files ()[j];

          if (be_global->gen_dcps_type_support ())
            {
              // When -Gdcps is enabled, if the "tao/*Seq.pidl" is
              // included in a DDS idl file then the generated code
              // will include the dds version idl generated code.
              if (ACE_OS::strstr (idl_name, "tao/") == idl_name
                && ACE_OS::strstr (idl_name, "Seq.pidl"))
                {
                  idl_name[0] = 'd';
                  idl_name[1] = 'd';
                  idl_name[2] = 's';
                }
            }

          // Make a String out of it.
          UTL_String idl_name_str = idl_name;

          // Make sure this file was actually got included, not
          // ignored by some #if defined compiler directive.

          // Get the clnt header from the IDL file name.
          const char* client_hdr =
            BE_GlobalData::be_get_client_hdr (&idl_name_str,
                                              1);

          idl_name_str.destroy ();

          // Sanity check and then print.
          if (client_hdr != 0)
            {
              this->client_header_->print ("\n#include \"%s\"",
                                           client_hdr);
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("\nERROR, invalid file '%s' included"),
                                 idl_name),
                                -1);
            }
        }
    }

  // Generate the TAO_EXPORT_MACRO macro.
  *this->client_header_ << "\n\n#if defined (TAO_EXPORT_MACRO)\n";
  *this->client_header_ << "#undef TAO_EXPORT_MACRO\n";
  *this->client_header_ << "#endif\n";
  *this->client_header_ << "#define TAO_EXPORT_MACRO "
                        << be_global->stub_export_macro ();

  // Begin versioned namespace support after initial headers have been
  // included, but before the inline file and post include
  // directives.
  *this->client_header_ << be_global->versioning_begin ();

  return 0;
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

  // Clean up between multiple files.
  delete this->client_stubs_;
  this->client_stubs_ = factory->make_outstream ();

  if (!this->client_stubs_)
    {
      return -1;
    }

  if (this->client_stubs_->open (fname, TAO_OutStream::TAO_CLI_IMPL) == -1)
    {
      return -1;
    }

  *this->client_stubs_ << be_nl << "// TAO_IDL - Generated from" << be_nl
                       << "// " << __FILE__ << ":" << __LINE__
                       << be_nl << be_nl;

  this->gen_stub_src_includes ();

  // Generate the ident string, if any.
  this->gen_ident_string (this->client_stubs_);

  // Only when we generate a client inline file generate the include
  if (be_global->gen_client_inline ())
    {
      // Generate the code that includes the inline file if not included in the
      // header file.
      *this->client_stubs_ << "\n\n#if !defined (__ACE_INLINE__)";
      *this->client_stubs_ << "\n#include \""
                           << be_global->be_get_client_inline_fname (1)
                           << "\"";
      *this->client_stubs_ << "\n#endif /* !defined INLINE */";
    }

  // Begin versioned namespace support after all headers have been
  // included, but before any code is generated.
  *this->client_stubs_ << be_global->versioning_begin ();

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

  // Clean up between multiple files.
  delete this->client_inline_;
  this->client_inline_ = factory->make_outstream ();

  if (!this->client_inline_)
    {
      return -1;
    }

  if (this->client_inline_->open (fname, TAO_OutStream::TAO_CLI_INL) == -1)
    {
      return -1;
    }

  // Generate the ident string, if any.
  this->gen_ident_string (this->client_inline_);

  // Begin versioned namespace support after initial headers, if any,
  // have been included.
  *this->client_inline_ << be_global->versioning_begin ();

  return 0;
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
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Clean up between multiple files.
  delete this->server_header_;
  this->server_header_ = factory->make_outstream ();

  if (!this->server_header_)
    {
      return -1;
    }

  if (this->server_header_->open (fname, TAO_OutStream::TAO_SVR_HDR) == -1)
    {
      return -1;
    }

  // If we are suppressing skel file generation, bail after generating the
  // copyright text and an informative message.
  if (!be_global->gen_skel_files ())
    {
      *this->server_header_ << be_nl
                            << "// Skeleton file generation suppressed with "
                            << "command line option -SS" << be_nl;

      return 0;
    }

  *this->server_header_ << be_nl
                        << "// TAO_IDL - Generated from" << be_nl
                        << "// " << __FILE__ << ":" << __LINE__
                        << be_nl << be_nl;

  // Generate the ident string, if any.
  this->gen_ident_string (this->server_header_);

  if (be_global->gen_dcps_type_support_only ())
    {
      // Generate the #ifndef clause.
      this->gen_ifndef_string (fname,
                               this->server_header_,
                               "_TAO_IDL_DDS_",
                               "_H_");
    }
  else
    {
      // Generate the #ifndef clause.
      this->gen_ifndef_string (fname,
                              this->server_header_,
                              "_TAO_IDL_",
                              "_H_");
    }

  if (be_global->pre_include () != 0)
    {
      *this->server_header_ << "#include /**/ \""
                            << be_global->pre_include ()
                            << "\"\n";
    }

  // The server header should include the client header.
  if (be_global->safe_include ())
    {
      // Generate the safe include if it is defined instead of the client header
      // need to put only the base names. Path info is not required.
      *this->server_header_ << "\n#include \""
                            << be_global->safe_include ()
                            << "\"";
    }
  else
    {
      *this->server_header_ << "\n#include \""
                            << be_global->be_get_client_hdr_fname (1)
                            << "\"";
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

      char const * const server_hdr =
        BE_GlobalData::be_get_server_hdr (&idl_name_str, 1);

      idl_name_str.destroy ();

      this->server_header_->print ("\n#include \"%s\"",
                                   server_hdr);
    }

  // Some compilers don't optimize the #ifndef header include
  // protection, but do optimize based on #pragma once.
  *this->server_header_ << "\n\n#if !defined (ACE_LACKS_PRAGMA_ONCE)\n"
                        << "# pragma once\n"
                        << "#endif /* ACE_LACKS_PRAGMA_ONCE */\n";

  // Include the definitions for the PortableServer namespace,
  // this forces the application to link the POA library, a good
  // thing, because we need the definitions there, it also
  // registers the POA factory with the Service_Configurator, so
  // the ORB can automatically find it.
  if (idl_global->non_local_iface_seen_)
    {
      // Include the Messaging files if AMI is enabled.
      if (be_global->ami_call_back () == true)
        {
          // Include Messaging skeleton file.
          this->gen_standard_include (this->server_header_,
                                      "tao/Messaging/MessagingS.h");
        }

      this->gen_standard_include (this->server_header_,
                                  "tao/Collocation_Proxy_Broker.h");
      this->gen_standard_include (this->server_header_,
                                  "tao/PortableServer/PortableServer.h");
      this->gen_standard_include (this->server_header_,
                                  "tao/PortableServer/Servant_Base.h");

      if (be_global->gen_amh_classes ())
        {
          this->gen_standard_include (this->server_header_,
                                      "tao/Messaging/AMH_Response_Handler.h");
        }
    }

  if (be_global->skel_export_include () != 0)
    {
      *this->server_header_ << "\n\n#include /**/ \""
                            << be_global->skel_export_include ()
                            << "\"";

      // Generate the TAO_EXPORT_MACRO macro.
      *this->server_header_ << "\n\n#if defined (TAO_EXPORT_MACRO)\n";
      *this->server_header_ << "#undef TAO_EXPORT_MACRO\n";
      *this->server_header_ << "#endif\n";
      *this->server_header_ << "#define TAO_EXPORT_MACRO "
                            << be_global->skel_export_macro ();
    }

  // Begin versioned namespace support after initial headers have been
  // included, but before the inline file and post include
  // directives.
  *this->server_header_ << be_global->versioning_begin ();

  return 0;
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

  // Clean up between multiple files.
  delete this->server_template_header_;
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

  *this->server_template_header_ << be_nl << "// TAO_IDL - Generated from"
                                 << be_nl
                                 << "// " << __FILE__ << ":" << __LINE__
                                 << be_nl << be_nl;

  // Generate the ident string, if any.
  this->gen_ident_string (this->server_template_header_);

  // Generate the #ifndef clause.
  this->gen_ifndef_string (fname,
                           this->server_template_header_,
                           "_TAO_IDL_",
                           "_H_");

  if (be_global->pre_include () != 0)
    {
      *this->server_template_header_ << "#include /**/ \""
                                     << be_global->pre_include ()
                                     << "\"";
    }

  // Begin versioned namespace support after initial headers have been
  // included, but before the inline file and post include
  // directives.
  *this->server_template_header_ << be_global->versioning_begin ();

  return 0;
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

  // Clean up between multiple files.
  delete this->server_skeletons_;
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

  *this->server_skeletons_ << be_nl << be_nl << "// TAO_IDL - Generated from "
                           << be_nl << "// " << __FILE__ << ":" << __LINE__
                           << be_nl << be_nl;

  // Generate the ident string, if any.
  this->gen_ident_string (this->server_skeletons_);

  // Generate the include statement for the precompiled header file.
  if (be_global->pch_include ())
    {
      *this->server_skeletons_ << "#include \""
                               << be_global->pch_include ()
                               << "\"\n";
    }

  // Generate the #ifndef clause.
  this->gen_ifndef_string (fname,
                           this->server_skeletons_,
                           "_TAO_IDL_",
                           "_CPP_");

  // Generate the include statement for the server header.
  *this->server_skeletons_
    << "\n#include \""
    << be_global->be_get_server_hdr_fname (1)
    << "\"";

  this->gen_skel_src_includes ();

  // Only when we generate a server inline file generate the include
  if (be_global->gen_server_inline ())
    {
      // Generate the code that includes the inline file if not included in the
      // header file.
      *this->server_skeletons_ << "\n\n#if !defined (__ACE_INLINE__)\n";
      *this->server_skeletons_ << "#include \""
                               << be_global->be_get_server_inline_fname (1)
                               << "\"\n";
      *this->server_skeletons_ << "#endif /* !defined INLINE */";
    }

  // Begin versioned namespace support after initial headers have been
  // included, but before the inline file and post include
  // directives.
  *this->server_skeletons_ << be_global->versioning_begin ();

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

  // Clean up between multiple files.
  delete this->server_template_skeletons_;
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

  *this->server_template_skeletons_ << be_nl << "// TAO_IDL - Generated from "
                                    << be_nl << "// "
                                    << __FILE__ << ":" << __LINE__
                                    << be_nl << be_nl;

  // Generate the ident string, if any.
  this->gen_ident_string (this->server_template_skeletons_);

  // Generate the #ifndef clause.
  this->gen_ifndef_string (fname,
                           this->server_template_skeletons_,
                           "_TAO_IDL_",
                           "_CPP_");

  // Generate the include statement for the server header.
  *this->server_template_skeletons_
      << "#include \""
      << be_global->be_get_server_template_hdr_fname (1)
      << "\"";

  // Begin versioned namespace support after initial headers have been
  // included, but before the inline file and post include
  // directives.
  *this->server_template_skeletons_ << be_global->versioning_begin ();

  return 0;
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

  // Clean up between multiple files.
  delete this->server_inline_;
  this->server_inline_ = factory->make_outstream ();

  if (!this->server_inline_)
    {
      return -1;
    }

  if (this->server_inline_->open (fname, TAO_OutStream::TAO_SVR_INL) == -1)
    {
      return -1;
    }

  // Generate the ident string, if any.
  this->gen_ident_string (this->server_inline_);

  // Begin versioned namespace support after initial headers, if any, have been
  // included.
  *this->server_inline_ << be_global->versioning_begin ();

  return 0;
}

// Get the server inline stream.
TAO_OutStream *
TAO_CodeGen::server_inline (void)
{
  return this->server_inline_;
}

int
TAO_CodeGen::start_anyop_header (const char *fname)
{
  // We may want to generate the full file or generate an empty
  // one, but this is the only condition under which we want to
  // skip it completely.
  if (!be_global->gen_anyop_files () && !be_global->gen_empty_anyop_header ())
    {
      return 0;
    }

  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Clean up between multiple files.
  delete this->anyop_header_;
  this->anyop_header_ = factory->make_outstream ();

  if (0 == this->anyop_header_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_CodeGen::start_anyop_header - "
                         "Error creating file stream\n"),
                        -1);
    }

  if (this->anyop_header_->open (fname,
                                 TAO_OutStream::TAO_CLI_HDR)
       == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_CodeGen::start_anyop_header - "
                         "Error opening file\n"),
                        -1);
    }

  // We want the empty file not only with -GX
  // but also when -GA appears with -Sa or -St.
  bool gen_empty_file = be_global->gen_empty_anyop_header ()
                        || be_global->gen_anyop_files ()
                           && !be_global->any_support ();

  if (gen_empty_file)
    {
      *this->anyop_header_ << be_nl
                           << "// Generated empty file" << be_nl
                           << be_nl;
      return 0;
    }

  *this->anyop_header_ << be_nl
                       << "// TAO_IDL - Generated from" << be_nl
                       << "// " << __FILE__ << ":" << __LINE__
                       << be_nl << be_nl;

  // Generate the #ident string, if any.
  this->gen_ident_string (this->anyop_header_);

  // Generate the #ifndef clause.
  this->gen_ifndef_string (fname,
                           this->anyop_header_,
                           "_TAO_IDL_",
                           "_H_");

  if (be_global->pre_include () != 0)
    {
      *this->anyop_header_ << "#include /**/ \""
                           << be_global->pre_include ()
                           << "\"\n";
    }

  // If anyop macro hasn't been set, default to stub macro.
  if (be_global->anyop_export_include () != 0)
    {
      *this->anyop_header_ << "\n#include /**/ \""
                           << be_global->anyop_export_include ()
                           << "\"";
    }
  else if (be_global->stub_export_include () != 0)
    {
      *this->anyop_header_ << "\n#include /**/ \""
                           << be_global->stub_export_include ()
                           << "\"";
    }

  const char *tao_prefix = "";
  ACE_CString pidl_checker (idl_global->filename ()->get_string ());
  bool const got_tao_pidl =
    (pidl_checker.substr (pidl_checker.length () - 5) == ".pidl");

  // If we're here and we have a .pidl file, we need to generate
  // the *C.h include from the tao library.
  if (got_tao_pidl)
    {
      tao_prefix = "tao/";
    }

  // Generate the include statement for the client header. We just
  // need to put only the base names. Path info is not required.
  if (be_global->safe_include ())
    {
      // Generate the safe include if it is defined instead of the client header
      // need to put only the base names. Path info is not required.
      *this->anyop_header_ << "\n#include \""
                           << be_global->safe_include ()
                           << "\"";
    }
  else
    {
      *this->anyop_header_ << "\n#include \"" << tao_prefix
                           << be_global->be_get_client_hdr_fname ()
                           << "\"";
    }

  // If we have not suppressed Any operator generation and also
  // are not generating the operators in a separate file, we
  // need to include the *A.h file from all .pidl files here.
  if (be_global->any_support ())
    {
      for (size_t j = 0; j < idl_global->n_included_idl_files (); ++j)
        {
          char* idl_name = idl_global->included_idl_files ()[j];

          ACE_CString pidl_checker (idl_name);
          bool const got_pidl =
            (pidl_checker.substr (pidl_checker.length () - 5) == ".pidl");

          // If we're here and we have a .pidl file, we need to generate
          // the *A.h include, if it is not a .pidl file we don't generate
          // a thing because the *C.h include is already generated in the
          // C.h file
          if (got_pidl)
            {
              // Make a String out of it.
              UTL_String idl_name_str = idl_name;

              const char *anyop_hdr =
                BE_GlobalData::be_get_anyop_header (&idl_name_str, 1);

              idl_name_str.destroy ();

              // Stripped off any scope in the name and add the
              // AnyTypeCode prefix.
              ACE_CString work_hdr (anyop_hdr);
              ACE_CString final_hdr = "tao/AnyTypeCode/";
              ACE_CString::size_type pos = work_hdr.rfind ('/');

              if (pos != ACE_CString::npos)
                {
                  ACE_CString scope (work_hdr.substr (0, pos - 1));

                  // If we find a '/' in the containing scope name, it
                  // means we are including a .pidl file from a
                  // subdirectory of $TAO_ROOT/tao, and so we should
                  // include the anyop_hdr string as is, and not strip
                  // off the scope name and prepend "tao/AnyTypeCode/".
                  // Only .pidl files in $TAO_ROOT/tao itself have
                  // their generated *A.* files moved to the AnyTypeCode
                  // library.
                  if (scope.find ('/') == ACE_CString::npos)
                    {
                      work_hdr = work_hdr.substr (pos + 1);
                      final_hdr += work_hdr;
                    }
                  else
                    {
                      final_hdr = work_hdr;
                    }
                }

              this->anyop_header_->print ("\n#include \"%s\"",
                                          final_hdr.c_str ());
            }
        }
    }
  *this->anyop_header_ << "\n";

  // Begin versioned namespace support after initial headers have been
  // included, but before the inline file and post include
  // directives.
  *this->anyop_header_ << be_global->versioning_begin ();

  return 0;
}

int
TAO_CodeGen::start_anyop_source (const char *fname)
{
  if (!be_global->gen_anyop_files () || !be_global->any_support ())
    {
      return 0;
    }

  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Clean up between multiple files.
  delete this->anyop_source_;
  this->anyop_source_ = factory->make_outstream ();

  if (0 == this->anyop_source_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_CodeGen::start_anyop_source - "
                         "Error creating file stream\n"),
                        -1);
    }

  if (this->anyop_source_->open (fname, TAO_OutStream::TAO_CLI_IMPL) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_CodeGen::start_anyop_source - "
                         "Error opening file\n"),
                        -1);
    }

  // Generate the include statement for the precompiled header file.
  if (be_global->pch_include ())
    {
      *this->anyop_source_ << "#include \""
                           << be_global->pch_include ()
                           << "\"";
    }

  // Generate the include statement for the client header. We just
  // need to put only the base names. Path info is not required.
  *this->anyop_source_ << "\n#include \""
                       << be_global->be_get_anyop_header_fname (1)
                       << "\"";

  this->gen_typecode_includes (this->anyop_source_);

  this->gen_any_file_includes (this->anyop_source_);

  // Begin versioned namespace support after initial headers have been
  // included, but before the inline file and post include
  // directives.
  *this->anyop_source_ << be_global->versioning_begin ();

  return 0;
}

TAO_OutStream *
TAO_CodeGen::anyop_header (void)
{
  return this->anyop_header_;
}

TAO_OutStream *
TAO_CodeGen::anyop_source (void)
{
  return this->anyop_source_;
}

// Set the server header stream.
int
TAO_CodeGen::start_implementation_header (const char *fname)
{
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Clean up between multiple files.
  delete this->implementation_header_;
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

  *this->implementation_header_ << be_nl << "// TAO_IDL - Generated from "
                                << be_nl << "// "
                                << __FILE__ << ":" << __LINE__
                                << be_nl << be_nl;

  // Generate the ident string, if any.
  this->gen_ident_string (this->implementation_header_);

  // Generate the #ifndef clause.
  this->gen_ifndef_string (fname,
                           this->implementation_header_,
                           "",
                           "_H_");

  const char* server_hdr = BE_GlobalData::be_get_server_hdr_fname (1);

  *this->implementation_header_<< "#include \"" << server_hdr <<"\"";

  *this->implementation_header_
    << "\n\n#if !defined (ACE_LACKS_PRAGMA_ONCE)\n"
    << "#pragma once\n"
    << "#endif /* ACE_LACKS_PRAGMA_ONCE */\n\n";

  if (idl_global->local_iface_seen_)
    {
      *this->implementation_header_ << "#include \"tao/LocalObject.h\"\n\n";
    }

  return 0;
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
  // Retrieve the singleton instance to the outstream factory.
  TAO_OutStream_Factory *factory = TAO_OUTSTREAM_FACTORY::instance ();

  // Clean up between multiple files.
  delete this->implementation_skeleton_;
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

  *this->implementation_skeleton_ << be_nl << "// TAO_IDL - Generated from "
                                  << be_nl << "// "
                                  << __FILE__ << ":" << __LINE__
                                  << be_nl << be_nl;

  // Generate the ident string, if any.
  this->gen_ident_string (this->implementation_skeleton_);

  const char* impl_hdr =
    BE_GlobalData::be_get_implementation_hdr_fname ();

  this->implementation_skeleton_->print ("#include \"%s\"\n\n",
                                         impl_hdr);

  return 0;
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

  *this->client_header_ << be_nl << be_nl << "// TAO_IDL - Generated from"
                        << be_nl << "// " << __FILE__ << ":" <<
    __LINE__ << be_nl;

  // End versioned namespace support before remaining include
  // directives at end of file.
  *this->client_header_ << be_global->versioning_end ();

  // Only when we generate a client inline file generate the include
  if (be_global->gen_client_inline ())
    {
      // Insert the code to include the inline file.
      *this->client_header_ << "#if defined (__ACE_INLINE__)\n";
      *this->client_header_ << "#include \""
                            << be_global->be_get_client_inline_fname (1)
                            << "\"\n";
      *this->client_header_ << "#endif /* defined INLINE */\n\n";
    }

  // Code to put the last #endif.

  if (be_global->post_include () != 0)
    {
      *this->client_header_ << "#include /**/ \""
                            << be_global->post_include ()
                            << "\"\n\n";
    }

  if (be_global->gen_dcps_type_support_only ())
    {
      *this->client_header_ << be_nl << be_nl
        << "#endif /* end of disabling TAO specific code */"
        << be_nl << be_nl;
    }

  *this->client_header_ << "#endif /* ifndef */" << be_nl << be_nl;

  return 0;
}

void
TAO_CodeGen::end_client_inline (void)
{
  *this->client_inline_ << "\n";

  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->client_inline_ << be_global->versioning_end ();
}

void
TAO_CodeGen::end_client_stubs (void)
{
  *this->client_stubs_ << "\n";

  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->client_stubs_ << be_global->versioning_end ();
}

int
TAO_CodeGen::end_server_header (void)
{
  *this->server_header_ << be_nl << be_nl << "// TAO_IDL - Generated from "
                        << be_nl << "// " << __FILE__ << ":" << __LINE__
                        << be_nl << be_nl;

  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->server_header_ << be_global->versioning_end ();

  // Insert the template header.
  if (be_global->gen_tie_classes ())
    {
      *this->server_header_ << "#include \""
                            << be_global->be_get_server_template_hdr_fname (1)
                            << "\"\n";
    }

  // Only when we generate a server inline file generate the include
  if (be_global->gen_server_inline ())
    {
      // Insert the code to include the inline file.
      *this->server_header_ << "\n#if defined (__ACE_INLINE__)\n";
      *this->server_header_ << "#include \""
                            << be_global->be_get_server_inline_fname (1)
                            << "\"\n";
      *this->server_header_ << "#endif /* defined INLINE */";
    }

  // Code to put the last #endif.
  *this->server_header_ << "\n\n";

  if (be_global->post_include () != 0)
    {
      *this->server_header_ << "#include /**/ \""
                            << be_global->post_include ()
                            << "\"\n";
    }

  *this->server_header_ << "#endif /* ifndef */\n";
  return 0;
}

void
TAO_CodeGen::end_server_inline (void)
{
  *this->server_inline_ << "\n";

  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->server_inline_ << be_global->versioning_end ();
}

int
TAO_CodeGen::end_implementation_header (const char *fname)
{
  if (fname == 0)
    {
      // Bad file name.
      return -1;
    }

  const char *suffix = ACE_OS::strrchr (fname, '.');

  if (suffix == 0)
    {
      // File seems to have no extension, so let us take the name
      // as it is.
      suffix = fname;
    }

  char macro_name [NAMEBUFSIZE] = { 0 };

  // Convert letters in fname to upper case.
  for (int i = 0; i < (suffix - fname); ++i)
    {
      if (ACE_OS::ace_isalpha (fname [i]))
        {
          macro_name[i] = static_cast<char> (ACE_OS::ace_toupper (fname [i]));
        }
      else if (ACE_OS::ace_isdigit (fname [i]))
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
  *this->server_template_header_ << be_nl << be_nl << "// TAO_IDL - Generated from "
                                 << be_nl << "// "
                                 << __FILE__ << ":" << __LINE__ << "\n";

  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->server_template_header_ << be_global->versioning_end ();

  // Insert the code to include the template source file.
  *this->server_template_header_
      << "\n\n#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)";
  *this->server_template_header_
      << "\n#include \""
      << be_global->be_get_server_template_skeleton_fname (1)
      << "\"";
  *this->server_template_header_ << "\n#endif /* defined REQUIRED SOURCE */";

  // Insert the code to include the template pragma.
  *this->server_template_header_
      << "\n\n#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)";
  *this->server_template_header_
      << "\n#pragma implementation (\""
      << be_global->be_get_server_template_skeleton_fname (1)
      << "\")";
  *this->server_template_header_ << "\n#endif /* defined REQUIRED PRAGMA */";

  // Code to put the last #endif.
  *this->server_template_header_ << "\n\n";

  if (be_global->post_include () != 0)
    {
      *this->server_template_header_ << "#include /**/ \""
                                     << be_global->post_include ()
                                     << "\"\n";
    }

  *this->server_template_header_ << "#endif /* ifndef */\n";
  return 0;
}

int
TAO_CodeGen::end_server_template_skeletons (void)
{
  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->server_template_skeletons_ << be_global->versioning_end ();

  // Code to put the last #endif.
  *this->server_template_skeletons_ << "\n#endif /* ifndef */\n";

  return 0;
}

int
TAO_CodeGen::end_server_skeletons (void)
{
  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->server_skeletons_ << be_global->versioning_end ();

  // Code to put the last #endif.
  *this->server_skeletons_ << "\n\n#endif /* ifndef */\n";

  return 0;
}

int
TAO_CodeGen::end_anyop_header (void)
{
  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->anyop_header_ << be_global->versioning_end ();


  if (be_global->post_include () != 0)
    {
      *this->anyop_header_ << "\n\n#include /**/ \""
                           << be_global->post_include ()
                           << "\"";
    }

  // Code to put the last #endif.
  *this->anyop_header_ << "\n\n#endif /* ifndef */\n";

  return 0;
}

int
TAO_CodeGen::end_anyop_source (void)
{
  // End versioned namespace support.  Do not place include directives
  // before this.
  *this->anyop_source_ << be_global->versioning_end ();

  *this->anyop_source_ << "\n";

  return 0;
}

// We use the following helper functions to pass information. This class is the
// best place to pass such information rather than passing information through
// global variables spread everywhere. This class is a singleton and is
// effectively a global.

void
TAO_CodeGen::gperf_input_stream (TAO_OutStream *os)
{
#if !defined (linux) && !defined (__QNX__) && !defined (__GLIBC__)
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
  delete [] this->gperf_input_filename_;
  this->gperf_input_filename_ = filename;
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
  this->visitor_factory_ = TAO_VISITOR_FACTORY::instance ();
}

void
TAO_CodeGen::gen_ident_string (TAO_OutStream *stream) const
{
  const char *str = idl_global->ident_string ();

  if (str != 0)
    {
      *stream << "#" << str << be_nl << be_nl;
    }
}

void
TAO_CodeGen::gen_ifndef_string (const char *fname,
                                TAO_OutStream *stream,
                                const char *prefix,
                                const char *suffix)
{
  char macro_name [NAMEBUFSIZE] = { 0 };

  const char *extension = ACE_OS::strrchr (fname, '.');

  if (extension == 0)
    {
      // File seems to have no extension, so let us take the name
      // as it is.
      extension = fname;
    }

  ACE_OS::sprintf (macro_name, prefix);

  size_t offset = ACE_OS::strlen (prefix);

  // Convert letters in fname to upper case.
  for (int i = 0; i < (extension - fname); i++)
    {
      if (ACE_OS::ace_isalpha (fname [i]))
        {
          macro_name[i + offset] = static_cast<char> (ACE_OS::ace_toupper (fname [i]));
        }
      else if (ACE_OS::ace_isdigit (fname [i]))
        {
          macro_name[i + offset] = fname[i];
        }
      else
        {
          macro_name[i + offset] = '_';
        }
    }

  ACE_OS::strcat (macro_name, suffix);

  // Generate the #ifndef ... #define statements.
  stream->print ("#ifndef %s\n",
                 macro_name);
  stream->print ("#define %s\n\n",
                 macro_name);
}

void
TAO_CodeGen::gen_standard_include (TAO_OutStream *stream,
                                   const char *included_file,
                                   bool add_comment)
{
  // Switch between changing or non-changing standard include files
  // include files, so that #include statements can be
  // generated with ""s or <>s respectively, for the standard include
  // files (e.g. tao/corba.h).

  const char *start_delimiter = "\"";
  const char *end_delimiter = "\"";

  if (be_global->changing_standard_include_files () == 0)
    {
      start_delimiter = "<";
      end_delimiter = ">";
    }

  *stream << "\n#include ";

  if (add_comment)
    {
      *stream << "/**/ ";
    }

  *stream << start_delimiter
          << included_file
          << end_delimiter;
}

void
TAO_CodeGen::gen_stub_hdr_includes (void)
{
  // Include valuetype and Any/TypeCode headers before ORB core
  // headers to make sure some things are parsed before some templates
  // (e.g. TAO_Pseudo_{Var,Out}_T).  Addresses issues with compilers
  // that require all necessary non-dependent names be parsed prior to
  // parsing templates that may use them (e.g. GNU g++ 3.4.x).

  this->gen_cond_file_include (be_global->tc_support ()
                               && !be_global->gen_anyop_files (),
                               "tao/AnyTypeCode/AnyTypeCode_methods.h",
                               this->client_header_);

  if (idl_global->abstract_iface_seen_ || idl_global->abstractbase_seen_)
    {
      // Include the AbstractBase file from the Valuetype library.
      this->gen_standard_include (this->client_header_,
                                  "tao/Valuetype/AbstractBase.h");

      if (idl_global->abstract_iface_seen_)
        {
          // Turn on generation of the rest of the Valuetype library includes.
          idl_global->valuetype_seen_ = true;
        }
    }

  if (idl_global->valuebase_seen_)
    {
      // Include files from the Valuetype library.
      this->gen_standard_include (this->client_header_,
                                  "tao/Valuetype/ValueBase.h");

      // Valuebox needs CDR for _tao_marshal_v code in .inl file
      this->gen_standard_include (this->client_header_,
                                  "tao/CDR.h");
    }

  if (idl_global->valuetype_seen_)
    {
      // Don't want to generate this twice.
      if (!idl_global->valuebase_seen_)
        {
          this->gen_standard_include (this->client_header_,
                                      "tao/Valuetype/ValueBase.h");
        }

      this->gen_standard_include (this->client_header_,
                                  "tao/Valuetype/Valuetype_Adapter_Factory_Impl.h");

      // Check for setting this bit performed in y.tab.cpp, actual checking
      // code is in be_valuetype.cpp.
      this->gen_cond_file_include (
          idl_global->valuefactory_seen_,
          "tao/Valuetype/ValueFactory.h",
          this->client_header_
        );
    }

  // @note This header should not go first.  See the discussion above
  //       regarding non-dependent template names.
  this->gen_cond_file_include (
      be_global->gen_orb_h_include (),
      "tao/ORB.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->operation_seen_ || idl_global->valuefactory_seen_
         || idl_global->valuebase_seen_,
      "tao/SystemException.h",
      this->client_header_
    );

  // For IDL exception, we need full knowledge of CORBA::UserException.
  this->gen_cond_file_include (
      idl_global->exception_seen_,
      "tao/UserException.h",
      this->client_header_
    );

  this->gen_standard_include (this->client_header_,
                              "tao/Basic_Types.h");

  // May need ORB_Constants if users check SystemException minor
  // codes.
  this->gen_cond_file_include (
      idl_global->operation_seen_ || idl_global->valuefactory_seen_
         || idl_global->valuebase_seen_,
      "tao/ORB_Constants.h",
      this->client_header_
    );

  // Conditionally included.

  // DDS/DCPS marshaling.
  this->gen_cond_file_include (
      be_global->gen_dcps_type_support (),
      "dds/DCPS/Serializer.h",
      this->client_header_
    );

  // DDS/DCPS zero-copy read sequence type support.
  if (idl_global->dcps_support_zero_copy_read ())
    {
      // include needed for sample info template instantiation.
      this->gen_cond_file_include (
          true, // for Infrastructure.idl as well as Foo types
          "dds/DCPS/ZeroCopyInfoSeq_T.h",
          this->client_header_
        );
      // include needed for type specific sample sequence template instantitation.
      this->gen_cond_file_include (
          idl_global->dcps_gen_zero_copy_read (),
          "dds/DCPS/ZeroCopySeq_T.h",
          this->client_header_
        );
    }

  // Non-abstract interface or keyword 'Object'.
  this->gen_cond_file_include (
      idl_global->non_local_iface_seen_
      | idl_global->local_iface_seen_
      | idl_global->base_object_seen_,
      "tao/Object.h",
      this->client_header_
    );

  // This is true if we have a typecode or TCKind in the IDL file.
  // If not included here, it will appear in *C.cpp, if TCs not suppressed.
  this->gen_cond_file_include (
      idl_global->typecode_seen_
      | idl_global->any_seen_,
      "tao/AnyTypeCode/TypeCode.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->any_seen_
      | idl_global->typecode_seen_,
      "tao/AnyTypeCode/TypeCode_Constants.h",
      this->client_header_);

  // This is true if we have an 'any' in the IDL file.
  // If not included here, it will appear in *C.cpp, if Anys not suppressed.
  this->gen_cond_file_include (
      idl_global->any_seen_,
      "tao/AnyTypeCode/Any.h",
      this->client_header_
    );

  // Generated if (w)string member of struct/union/exception/array/valuetype
  // has been seen.
  this->gen_cond_file_include (idl_global->string_member_seen_,
                               "tao/String_Manager_T.h",
                               this->client_header_);

  // Include the Messaging library entry point, if AMI is enabled.
  if (be_global->ami_call_back () == true)
    {
      // Include Messaging skeleton file.
      this->gen_standard_include (this->client_header_,
                                  "tao/Messaging/Messaging.h");
    }

  // Include the smart proxy base class if smart proxies are enabled.
  if (be_global->gen_smart_proxies () == true)
    {
      this->gen_standard_include (this->client_header_,
                                  "tao/SmartProxies/Smart_Proxies.h");
    }

  // If we have not suppressed Any operator generation and also
  // are not generating the operators in a separate file, we
  // need to include the *A.h file from all .pidl files here.
  if (be_global->any_support () && !be_global->gen_anyop_files ())
    {
      for (size_t j = 0; j < idl_global->n_included_idl_files (); ++j)
        {
          char * const idl_name = idl_global->included_idl_files ()[j];

          ACE_CString pidl_checker (idl_name);
          bool const got_pidl =
            (pidl_checker.substr (pidl_checker.length () - 5) == ".pidl");

          // If we're here and we have a .pidl file, we need to generate
          // the *A.h include from the AnyTypeCode library.
          if (got_pidl)
            {
              // Make a String out of it.
              UTL_String idl_name_str = idl_name;

              char const * const anyop_hdr =
                BE_GlobalData::be_get_anyop_header (&idl_name_str, 1);

              idl_name_str.destroy ();

              // Stripped off any scope in the name and add the
              // AnyTypeCode prefix.
              ACE_CString work_hdr (anyop_hdr);
              ACE_CString final_hdr = "tao/AnyTypeCode/";
              ACE_CString::size_type const pos = work_hdr.rfind ('/');

              if (pos != ACE_CString::npos)
                {
                  ACE_CString scope (work_hdr.substr (0, pos));

                  // If we find a '/' in the containing scope name, it
                  // means we are including a .pidl file from a
                  // subdirectory of $TAO_ROOT/tao, and so we should
                  // include the anyop_hdr string as is, and not strip
                  // off the scope name and prepend "tao/AnyTypeCode/".
                  // Only .pidl files in $TAO_ROOT/tao itself have
                  // their generated *A.* files moved to the AnyTypeCode
                  // library.
                  if (scope.find ('/') == ACE_CString::npos)
                    {
                      work_hdr = work_hdr.substr (pos + 1);
                      final_hdr += work_hdr;
                    }
                  else
                    {
                      final_hdr = work_hdr;
                    }
                }

              this->client_header_->print ("\n#include \"%s\"",
                                           final_hdr.c_str ());
            }
        }
    }


  // Must have knowledge of the base class.
  this->gen_seq_file_includes ();

  // _vars and _outs are typedefs of template class instantiations.
  this->gen_var_file_includes ();

  // Versioned namespace support.
  this->gen_standard_include (this->client_header_,
                              "tao/Versioned_Namespace.h",
                              true);

  // On some platforms, this include isn't needed if certain command
  // line options are present. Rather than try to sort that all out,
  // and to keep cross-compiling robust, we always generate this
  // include if gen_ostream_operators_ is true.
  if (be_global->gen_ostream_operators ())
    {
      this->gen_standard_include (this->client_header_,
                                  "ace/streams.h");
    }
}

void
TAO_CodeGen::gen_stub_src_includes (void)
{
  // Generate the include statement for the precompiled header file.
  if (be_global->pch_include ())
    {
      *this->client_stubs_ << "#include \""
                           << be_global->pch_include ()
                           << "\"";
    }

  if (be_global->safe_include ())
    {
      // Generate the safe include if it is defined instead of the client header
      // need to put only the base names. Path info is not required.
      *this->client_stubs_ << "\n#include \""
                           << be_global->safe_include ()
                           << "\"";
    }
  else
    {
      // Generate the include statement for the client header. We just
      // need to put only the base names. Path info is not required.
      *this->client_stubs_ << "\n#include \""
                           << be_global->be_get_client_hdr_fname (1)
                           << "\"";
    }

  if (be_global->tc_support ()
      && !be_global->gen_anyop_files ())
    {
      this->gen_typecode_includes (this->client_stubs_);
    }

  if (be_global->gen_dcps_type_support_only ())
    {
      *this->client_stubs_ << "\n\n#if 0 // disable TAO specific code "
                           << be_nl << be_nl;
    }

  // Always generated.
  this->gen_standard_include (this->client_stubs_,
                              "tao/CDR.h");

  // Conditional includes.

   if (idl_global->non_local_op_seen_)
    {
      this->gen_standard_include (this->client_stubs_,
                                  "tao/Exception_Data.h");
    }

  // Operations for local interfaces are pure virtual.
  if (idl_global->non_local_op_seen_)
    {
      this->gen_standard_include (this->client_stubs_,
                                  "tao/Invocation_Adapter.h");
    }

  // Any abstract interface present will probably have an operation.
  if (idl_global->abstract_iface_seen_)
    {
      this->gen_standard_include (
          this->client_stubs_,
          "tao/Valuetype/AbstractBase_Invocation_Adapter.h"
        );

      this->gen_standard_include (
          this->client_stubs_,
          "tao/Valuetype/AbstractBase_T.h"
        );
    }

  if (be_global->ami_call_back () == true)
    {
      this->gen_standard_include (this->client_stubs_,
                                  "tao/Messaging/Asynch_Invocation_Adapter.h");

      this->gen_standard_include (this->client_stubs_,
                                  "tao/Messaging/ExceptionHolder_i.h");
    }

  // If valuefactory_seen_ was set, this was generated in the stub header file,
  // otherwise it needs to go here - used in _tao_unmarshal().
  if (idl_global->valuetype_seen_
      && !idl_global->valuefactory_seen_)
    {
      this->gen_standard_include (this->client_stubs_,
                                  "tao/Valuetype/ValueFactory.h");
    }

  if (idl_global->non_local_iface_seen_)
    {
      // Needed for _narrow(), which is now template-based.
      this->gen_standard_include (this->client_stubs_,
                                  "tao/Object_T.h");
    }

  if (idl_global->octet_seq_seen_)
    {
      // Needed for the TAO_NO_COPY_OCTET_SEQUENCES optimization. Note that
      // it is preferable to just refer to CORBA::OctetSeq in the IDL file.
      this->gen_standard_include (this->client_stubs_,
                                  "tao/ORB_Core.h");
    }

  // The UserException::_tao_{en,de}code() methods can throw a
  // CORBA::MARSHAL exception so make sure that system exception is
  // fully declared/defined by including "tao/SystemException.h".
  if (idl_global->exception_seen_)
    {
      this->gen_standard_include (this->client_stubs_,
                                  "tao/SystemException.h");
    }

  // Includes whatever Any template classes that may be needed.
  if (!be_global->gen_anyop_files ())
    {
      this->gen_any_file_includes (this->client_stubs_);
    }

  // Includes whatever arg helper template classes that may be needed.
  this->gen_stub_arg_file_includes (this->client_stubs_);

  // strlen() for DCPS marshaling or
  // strcmp() is used with interfaces and exceptions.
  if ((be_global->gen_dcps_type_support ()
          && (idl_global->string_seen_
              || idl_global->string_seq_seen_
                  || idl_global->wstring_seq_seen_) )
          || idl_global->interface_seen_
      || idl_global->exception_seen_
      || idl_global->union_seen_)
    {
      // Needed for _narrow(), which is now template-based.
      this->gen_standard_include (this->client_stubs_,
                                  "ace/OS_NS_string.h");
    }

  if (be_global->gen_amh_classes ())
    {
      // Necessary for the AIX compiler.
      this->gen_standard_include (this->client_stubs_,
                                  "ace/Auto_Ptr.h");
    }
}

void
TAO_CodeGen::gen_skel_src_includes (void)
{
  // Only non-local interfaces generate anything in the skeleton.
  if (!(idl_global->non_local_iface_seen_
        || idl_global->need_skeleton_includes_))
    {
      return;
    }

  switch (be_global->lookup_strategy ())
    {
      case BE_GlobalData::TAO_DYNAMIC_HASH:
        {
          this->gen_standard_include (this->server_skeletons_,
                                      "tao/PortableServer/Operation_Table_Dynamic_Hash.h");
        }
        break;
      case BE_GlobalData::TAO_LINEAR_SEARCH:
        {
          this->gen_standard_include (this->server_skeletons_,
                                      "tao/PortableServer/Operation_Table_Linear_Search.h");
        }
        break;
      case BE_GlobalData::TAO_BINARY_SEARCH:
        {
          this->gen_standard_include (this->server_skeletons_,
                                      "tao/PortableServer/Operation_Table_Binary_Search.h");
        }
        break;
      case BE_GlobalData::TAO_PERFECT_HASH:
        {
          this->gen_standard_include (this->server_skeletons_,
                                      "tao/PortableServer/Operation_Table_Perfect_Hash.h");
        }
        break;
    }

  if (be_global->gen_direct_collocation ())
    {
      this->gen_standard_include (
          this->server_skeletons_,
          "tao/PortableServer/Direct_Collocation_Upcall_Wrapper.h"
        );
    }

  if (be_global->ami_call_back () == true)
    {
      this->gen_standard_include (this->server_skeletons_,
                                  "tao/Exception_Data.h");
      this->gen_standard_include (this->server_skeletons_,
                                  "tao/Messaging/ExceptionHolder_i.h");
    }

  this->gen_standard_include (this->server_skeletons_,
                              "tao/PortableServer/Upcall_Command.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/PortableServer/Upcall_Wrapper.h");

  this->gen_standard_include (this->server_skeletons_,
                              "tao/TAO_Server_Request.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/ORB_Core.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/Profile.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/Stub.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/IFR_Client_Adapter.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/Object_T.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/AnyTypeCode/TypeCode.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/AnyTypeCode/DynamicC.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/CDR.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/operation_details.h");
  this->gen_standard_include (this->server_skeletons_,
                              "tao/PortableInterceptor.h");

  this->gen_skel_arg_file_includes (this->server_skeletons_);

  if (be_global->gen_thru_poa_collocation ()
      || be_global->gen_direct_collocation ())
    {
      // Collocation skeleton code doesn't use "SArg" variants.
      this->gen_stub_arg_file_includes (this->server_skeletons_);
    }

  // The following header must always be included.
  if (be_global->gen_amh_classes ())
    {
      this->gen_standard_include (this->server_skeletons_,
                                  "tao/Thread_Lane_Resources.h");
      this->gen_standard_include (this->server_skeletons_,
                                  "tao/Buffer_Allocator_T.h");
      this->gen_standard_include (this->server_skeletons_,
                                  "ace/Auto_Functor.h");
    }

  this->gen_standard_include (this->server_skeletons_,
                              "ace/Dynamic_Service.h");

  // For Static_Allocator_Base
  this->gen_standard_include (this->server_skeletons_,
                              "ace/Malloc_Allocator.h");
}

void
TAO_CodeGen::gen_seq_file_includes (void)
{
  // @@@ (JP) These can get more specialized, after the TAO seq template
  // files have been split up.

  this->gen_cond_file_include (
      idl_global->vt_seq_seen_,
      "tao/Valuetype/Sequence_T.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->seq_seen_,
      "tao/Sequence_T.h",
      this->client_header_
    );
}

void
TAO_CodeGen::gen_any_file_includes (TAO_OutStream * stream)
{
  if (be_global->any_support ())
    {
      this->gen_standard_include (stream,
                                  "tao/CDR.h");

      // Any_Impl_T.cpp needs the full CORBA::Any type.
      this->gen_cond_file_include (
          idl_global->interface_seen_
          | idl_global->valuetype_seen_,
          "tao/AnyTypeCode/Any.h",
          stream
        );

      this->gen_cond_file_include (
          idl_global->interface_seen_
          | idl_global->valuetype_seen_,
          "tao/AnyTypeCode/Any_Impl_T.h",
          stream
        );

      this->gen_cond_file_include (
          idl_global->aggregate_seen_
          | idl_global->seq_seen_
          | idl_global->exception_seen_,
          "tao/AnyTypeCode/Any_Dual_Impl_T.h",
          stream
        );

      this->gen_cond_file_include (
          idl_global->array_seen_,
          "tao/AnyTypeCode/Any_Array_Impl_T.h",
          stream
        );

      this->gen_cond_file_include (
          idl_global->enum_seen_,
          "tao/AnyTypeCode/Any_Basic_Impl_T.h",
          stream
        );
    }
}

void
TAO_CodeGen::gen_var_file_includes (void)
{
  this->gen_cond_file_include (
      idl_global->valuetype_seen_
      | idl_global->fwd_valuetype_seen_,
      "tao/Valuetype/Value_VarOut_T.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->interface_seen_
      | idl_global->fwd_iface_seen_,
      "tao/Objref_VarOut_T.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->seq_seen_,
      "tao/Seq_Var_T.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->seq_seen_,
      "tao/Seq_Out_T.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->aggregate_seen_,
      "tao/VarOut_T.h",
      this->client_header_
    );

  this->gen_cond_file_include (
      idl_global->array_seen_,
      "tao/Array_VarOut_T.h",
      this->client_header_
    );
}

void
TAO_CodeGen::gen_stub_arg_file_includes (TAO_OutStream * stream)
{
  this->gen_cond_file_include (
      idl_global->basic_arg_seen_,
      "tao/Basic_Arguments.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->bd_string_arg_seen_,
      "tao/BD_String_Argument_T.h",
      stream
    );

  // If we have a bound string and we have any generation enabled we must
  // include Any.h to get the <<= operator for BD_String
  this->gen_cond_file_include (
      idl_global->bd_string_arg_seen_ && be_global->any_support (),
      "tao/AnyTypeCode/Any.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->fixed_array_arg_seen_,
      "tao/Fixed_Array_Argument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->fixed_size_arg_seen_,
      "tao/Fixed_Size_Argument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->object_arg_seen_,
      "tao/Object_Argument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->special_basic_arg_seen_,
      "tao/Special_Basic_Arguments.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->ub_string_arg_seen_,
      "tao/UB_String_Arguments.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->var_array_arg_seen_,
      "tao/Var_Array_Argument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->var_size_arg_seen_,
      "tao/Var_Size_Argument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->any_arg_seen_,
      "tao/AnyTypeCode/Any_Arg_Traits.h",
      stream
    );
}


void
TAO_CodeGen::gen_skel_arg_file_includes (TAO_OutStream * stream)
{
  this->gen_cond_file_include (
      idl_global->basic_arg_seen_,
      "tao/PortableServer/Basic_SArguments.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->bd_string_arg_seen_,
      "tao/PortableServer/BD_String_SArgument_T.h",
      stream
    );

  // If we have a bound string and we have any generation enabled we must
  // include Any.h to get the <<= operator for BD_String
  this->gen_cond_file_include (
      idl_global->bd_string_arg_seen_ && be_global->any_support (),
      "tao/AnyTypeCode/Any.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->fixed_array_arg_seen_,
      "tao/PortableServer/Fixed_Array_SArgument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->fixed_size_arg_seen_,
      "tao/PortableServer/Fixed_Size_SArgument_T.h",
      stream
    );

  // Always needed for CORBA::Object handling in _component() skeleton
  // code when an unconstrained (non-local) IDL interface is defined.
  this->gen_cond_file_include (
      idl_global->non_local_iface_seen_
      || idl_global->object_arg_seen_,
      "tao/PortableServer/Object_SArgument_T.h",
      stream
    );

  // Always needed for CORBA::Boolean handling in _is_a() skeleton
  // code when an unconstrained (non-local) IDL interface is defined.
  this->gen_cond_file_include (
      idl_global->non_local_iface_seen_
      || idl_global->special_basic_arg_seen_,
      "tao/PortableServer/Special_Basic_SArguments.h",
      stream
    );

  // Always needed for string argument handling in _is_a() skeleton
  // code when an unconstrained (non-local) IDL interface is defined.
  this->gen_cond_file_include (
      idl_global->non_local_iface_seen_
      || idl_global->ub_string_arg_seen_,
      "tao/PortableServer/UB_String_SArguments.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->var_array_arg_seen_,
      "tao/PortableServer/Var_Array_SArgument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->var_size_arg_seen_,
      "tao/PortableServer/Var_Size_SArgument_T.h",
      stream
    );

  this->gen_cond_file_include (
      idl_global->any_arg_seen_,
      "tao/PortableServer/Any_SArg_Traits.h",
      stream
    );

  this->gen_standard_include (stream,
                              "tao/PortableServer/TypeCode_SArg_Traits.h");
  this->gen_standard_include (stream,
                              "tao/PortableServer/Object_SArg_Traits.h");

  if (be_global->gen_thru_poa_collocation ())
    {
      // Thru-POA/skeleton argument selection function templates.
      this->gen_cond_file_include (idl_global->non_local_iface_seen_,
                                   "tao/PortableServer/get_arg.h",
                                   stream);

      // We need the stub side argument templates when thru-POA
      // collocation is enabled for type resolution.
      // this->gen_stub_arg_file_includes (stream);

      // Always needed for CORBA::Boolean handling in _is_a() skeleton
      // code when an unconstrained (non-local) IDL interface is defined.
      this->gen_cond_file_include (idl_global->non_local_iface_seen_,
                                   "tao/Special_Basic_Arguments.h",
                                   stream);

      // Always needed for string argument handling in _is_a() skeleton
      // code when an unconstrained (non-local) IDL interface is defined.
      this->gen_cond_file_include (idl_global->non_local_iface_seen_,
                                   "tao/UB_String_Arguments.h",
                                   stream);
    }
}

void
TAO_CodeGen::gen_cond_file_include (bool condition_green,
                                    const char *filepath,
                                    TAO_OutStream *stream)
{
  if (condition_green)
    {
      this->gen_standard_include (stream,
                                  filepath);
    }
}

void
TAO_CodeGen::gen_typecode_includes (TAO_OutStream * stream)
{
  this->gen_standard_include (stream,
                              "tao/AnyTypeCode/Null_RefCount_Policy.h");

  this->gen_standard_include (stream,
                              "tao/AnyTypeCode/TypeCode_Constants.h");

  // Just assume we're going to need alias TypeCodes since there is
  // currently no alias_seen_ or typedef_seen_ flag in idl_global.
  this->gen_standard_include (stream,
                              "tao/AnyTypeCode/Alias_TypeCode_Static.h");

  this->gen_cond_file_include (idl_global->enum_seen_,
                               "tao/AnyTypeCode/Enum_TypeCode_Static.h",
                               stream);

  this->gen_cond_file_include (idl_global->interface_seen_,
                               "tao/AnyTypeCode/Objref_TypeCode_Static.h",
                               stream);

  this->gen_cond_file_include (idl_global->seq_seen_
                               | idl_global->array_seen_,
                               "tao/AnyTypeCode/Sequence_TypeCode_Static.h",
                               stream);

  this->gen_cond_file_include (idl_global->string_seen_,
                               "tao/AnyTypeCode/String_TypeCode_Static.h",
                               stream);

  this->gen_cond_file_include (
      idl_global->exception_seen_
      | idl_global->aggregate_seen_,
      "tao/AnyTypeCode/Struct_TypeCode_Static.h",
      stream);

  this->gen_cond_file_include (
      idl_global->exception_seen_
      | idl_global->aggregate_seen_,
      "tao/AnyTypeCode/TypeCode_Struct_Field.h",
      stream);

  this->gen_cond_file_include (idl_global->union_seen_,
                               "tao/AnyTypeCode/TypeCode_Case_T.h",
                               stream);

  this->gen_cond_file_include (idl_global->union_seen_,
                               "tao/AnyTypeCode/Union_TypeCode_Static.h",
                               stream);

  this->gen_cond_file_include (idl_global->valuetype_seen_,
                               "tao/AnyTypeCode/Value_TypeCode_Static.h",
                               stream);

  this->gen_cond_file_include (idl_global->valuetype_seen_,
                               "tao/AnyTypeCode/TypeCode_Value_Field.h",
                               stream);

  this->gen_cond_file_include (idl_global->recursive_type_seen_,
                               "tao/AnyTypeCode/Recursive_Type_TypeCode.h",
                               stream);
}

void
TAO_CodeGen::destroy (void)
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
  delete this->anyop_source_;
  delete this->anyop_header_;
#if !defined (linux) && !defined (__QNX__) && !defined (__GLIBC__)
  // This causes a seg fault on Linux RH 5.1.  Let it leak . . .
  delete this->gperf_input_stream_;
#endif /* ! linux */
  delete [] this->gperf_input_filename_;
  this->curr_os_ = 0;
}
