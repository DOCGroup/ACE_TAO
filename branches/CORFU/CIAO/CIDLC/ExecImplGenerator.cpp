// file      : CIDLC/ExecImplGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ExecImplGenerator.hpp"
#include "ExecImplHeaderGenerator.hpp"
#include "ExecImplSourceGenerator.hpp"
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

ExecImplGenerator::ExecImplGenerator (CommandLine const& cl)
    : cl_ (cl),
      file_name_ (""),
      export_macro_ ("")
{
}

void ExecImplGenerator::options (CL::Description& d)
{
  d.add_option (CL::OptionDescription (
                  "exec-hdr-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'_exec.h\' "
                  "when constructing name of executor implementation file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "exec-hdr-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of executor implementation file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "exec-src-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'_exec.cpp\' "
                  "when constructing name of executor implementation file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "exec-src-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of executor implementation file.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "exec-export-macro",
                  "macro",
                  "Replace default executor DLL export macro "
                  "with provided macro.",
                  CL::OptionType::value));

  d.add_option (CL::OptionDescription (
                  "exec-export-include",
                  "file",
                  "Replace default executor export include file "
                  "with provided file.",
                  CL::OptionType::value));
}


void ExecImplGenerator::generate (SemanticGraph::TranslationUnit& u,
                                  fs::path const& file)
{
  // Generate files
  compute_export_macro (file);

  {
    fs::ofstream hdr_ofs;
    ostream& hdr_os = configure_stream ("exec-hdr-file-suffix",
                                        "_exec.h",
                                        "exec-hdr-file-regex",
                                        hdr_ofs);

    Indentation::Implanter<Indentation::Cxx> header_guard (hdr_os);

    // Set C++ name printer for os.
    //
    CxxNamePrinter name_printer;
    hdr_os.pword (name_printer_index) = &name_printer;


    ExecImplHeaderEmitter hdr_emitter (hdr_os,
                                       cl_,
                                      export_macro_,
                                      file);
    hdr_emitter.generate (u);
  }

  {
    fs::ofstream src_ofs;
    ostream& src_os = configure_stream ("exec-src-file-suffix",
                                        "_exec.cpp",
                                        "exec-src-file-regex",
                                        src_ofs);

    Indentation::Implanter<Indentation::Cxx> header_guard (src_os);

    // Set C++ name printer for os.
    //
    CxxNamePrinter name_printer;
    src_os.pword (name_printer_index) = &name_printer;

    ExecImplSourceEmitter src_emitter (src_os,
                                       cl_,
                                       export_macro_,
                                       file);
    src_emitter.generate (u);
  }
}

void
ExecImplGenerator::compute_export_macro (const fs::path& file_path)
{
  if (!file_path.empty ())
  {
    file_name_ = file_path.leaf ();
  }

  export_macro_ = cl_.get_value ("exec-export-macro", "");

  if (export_macro_.empty () && !file_name_.empty ())
  {
    // Modify a copy of the filename string.
    export_macro_ = file_name_;
    str_upcase (export_macro_);

    // Replace the suffix.
    export_macro_ =
      regex::perl_s (export_macro_,
                     "/^(.+?)(\\.(IDL|CIDL|CDL))?$/$1_EXEC_Export/");

    // Replace any remaining '.' in the string with '_'.
    export_macro_ = regex::perl_s (export_macro_,
                                   "/\\./_/");
  }
}

ostream&
ExecImplGenerator::configure_stream (string const& suffix_option,
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

