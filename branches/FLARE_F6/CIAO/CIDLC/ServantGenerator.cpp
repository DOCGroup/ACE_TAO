// file      : CIDLC/ServantGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ServantGenerator.hpp"
#include "ServantHeaderGenerator.hpp"
#include "ServantSourceGenerator.hpp"
#include "CxxNamePrinter.hpp"
#include "Upcase.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IndentationCxx.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

using namespace CCF;
using namespace CIDL;
using namespace SemanticGraph;
using namespace Traversal;

using std::string;
using std::ostream;

ServantGenerator::ServantGenerator (CommandLine const& cl)
    : cl_ (cl),
      file_name_ (""),
      export_macro_ ("")
{
}

void ServantGenerator::options (CL::Description& d)
{
  d.add_option (CL::OptionDescription (
                  "svnt-hdr-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'_svnt.h\' "
                  "when constructing name of servant file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "svnt-hdr-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of servant file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "svnt-src-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'_svnt.cpp\' "
                  "when constructing name of servant file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "svnt-src-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of servant file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "svnt-export-macro",
                  "macro",
                  "Replace default servant DLL export macro "
                  "with provided macro.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "svnt-export-include",
                  "file",
                  "Replace default servant export include file "
                  "with provided file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "suppress-register-factory",
                  "Suppress generation of code to register eventtype "
                  "factories for event sinks.",
                  CL::OptionType::flag));

  d.add_option (CL::OptionDescription (
                  "custom-container",
                  "type",
                  "Generate code for custom container of the provided type.",
                  CL::OptionType::value));
                  
  d.add_option (CL::OptionDescription (
                  "static-config",
                  "No dynamic configuration, no locks on state access.",
                  CL::OptionType::flag));
}


void ServantGenerator::generate (SemanticGraph::TranslationUnit& u,
                                 fs::path const& file)
{
  // Generate files
  compute_export_macro (file);

  {
    fs::ofstream hdr_ofs;
    ostream& hdr_os = configure_stream ("svnt-hdr-file-suffix",
                                        "_svnt.h",
                                        "svnt-hdr-file-regex",
                                        hdr_ofs);

    Indentation::Implanter<Indentation::Cxx> header_guard (hdr_os);

    // Set C++ name printer for os.
    //
    CxxNamePrinter name_printer;
    hdr_os.pword (name_printer_index) = &name_printer;

    ServantHeaderEmitter hdr_emitter (hdr_os,
                                      cl_,
                                      export_macro_,
                                      file);
    hdr_emitter.generate (u);
  }

  {
    fs::ofstream src_ofs;
    ostream& src_os = configure_stream ("svnt-src-file-suffix",
                                        "_svnt.cpp",
                                        "svnt-src-file-regex",
                                        src_ofs);

    Indentation::Implanter<Indentation::Cxx> header_guard (src_os);

    // Set C++ name printer for os.
    //
    CxxNamePrinter name_printer;
    src_os.pword (name_printer_index) = &name_printer;

    ServantSourceEmitter src_emitter (src_os,
                                      cl_,
                                      export_macro_,
                                      file);
    src_emitter.generate (u);
  }
}

void
ServantGenerator::compute_export_macro (const fs::path& file_path)
{
  if (!file_path.empty ())
  {
    file_name_ = file_path.leaf ();
  }

  export_macro_ = cl_.get_value ("svnt-export-macro", "");

  if (export_macro_.empty () && !file_name_.empty ())
  {
    // Modify a copy of the filename string.
    export_macro_ = file_name_;
    str_upcase (export_macro_);

    // Replace the suffix.
    export_macro_ =
      regex::perl_s (export_macro_,
                     "/^(.+?)(\\.(IDL|CIDL|CDL))?$/$1_SVNT_Export/");

    // Replace any remaining '.' in the string with '_'.
    export_macro_ = regex::perl_s (export_macro_,
                                   "/\\./_/");
  }
}



ostream&
ServantGenerator::configure_stream (string const& suffix_option,
                                    string const& default_suffix,
                                    string const& regex_option,
                                    fs::ofstream& ofs)
{
  if (! file_name_.empty ())
  {
    string file_suffix = cl_.get_value (suffix_option,
                                        default_suffix);
    string file_expr =
      cl_.get_value (regex_option,
                     "/^(.+?)(\\.(idl|cidl|cdl))?$/$1" + file_suffix + "/");

    string file_name = regex::perl_s (file_name_, file_expr);

    fs::path file_path (file_name);

    ofs.open (file_path, std::ios_base::out);

    if (!ofs.is_open ())
    {
      cerr << file_name
          << ": error: unable to open file in write mode"
          << endl;
    }
  }

  return ofs.is_open ()
    ? static_cast<ostream&> (ofs)
    : static_cast<ostream&> (std::cout);
}


/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
