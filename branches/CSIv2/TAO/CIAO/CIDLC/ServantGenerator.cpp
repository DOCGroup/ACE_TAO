// $Id$
#include "ServantGenerator.hpp"
#include "ServantHeaderGenerator.hpp"
#include "ServantSourceGenerator.hpp"

#include "Collectors.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IndentationIDL.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

using namespace CCF;
using namespace CIDL;
using namespace SyntaxTree;
using namespace Traversal;

namespace
{
  // On some platforms toupper can be something else than a
  // function.
  int
  upcase (int c)
  {
    return toupper (c);
  }
}

ServantGenerator::ServantGenerator (CommandLine const& cl)
    : cl_ (cl),
      file_name_ (""),
      export_macro_ ("")
{
}

void ServantGenerator::
options (CL::Description& d)
{
  d.add_option (CL::OptionDescription (
                  "svnt-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'_svnt\' "
                  "when constructing name of servant file.",
                  true));

  d.add_option (CL::OptionDescription (
                  "svnt-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of servant file.",
                  true));
  /*
  d.add_option (CL::OptionDescription (
                  "lem-force-all",
                  "Force generation of local executor mapping for all IDL "
                  "types including those not used (directly or inderectly) "
                  "by compositions. This option is useful for generating a "
                  "common portion of local executor mapping used by more "
                  "than one component or composition.",
                  true));
  */
}


void ServantGenerator::generate (TranslationUnitPtr const& u)
{
  Declarations declarations;


  ProvidedInterfaceCollector provides (declarations);
  //  Traversal::InterfaceDecl provides (&i);

  ComponentCollector component (declarations);
  component.add_scope_delegate (&provides);

  HomeCollector home (declarations);

  HomeExecutorCollector home_executor (declarations, &home, &component);

  Traversal::Composition composition;
  composition.add_scope_delegate (&home_executor);

  Traversal::Scope scope;
  scope.add_scope_delegate (&composition);
  scope.add_scope_delegate (&scope);

  Traversal::PrincipalTranslationRegion region (&scope);

  Traversal::TranslationUnit collector;
  collector.add_content_delegate (&region);

  collector.dispatch (u);

  // Generate files
  compute_export_macro (u->principal_translation_region ()->file_path ());

  {
    fs::ofstream hdr_ofs;
    ostream& hdr_os = configure_stream ("hdr-file-suffix",
                                        "_svnt.h",
                                        "hdr-file-regex",
                                        hdr_ofs);

    Indentation::Implanter<Indentation::IDL> header_guard (hdr_os);


    ServantHeaderEmitter hdr_emitter (hdr_os,
                                      cl_,
                                      export_macro_,
                                      declarations);
    hdr_emitter.generate (u);
  }

  {
    fs::ofstream src_ofs;
    ostream& src_os = configure_stream ("src-file-suffix",
                                        "_svnt.cpp",
                                        "src-file-regex",
                                        src_ofs);

    Indentation::Implanter<Indentation::IDL> header_guard (src_os);

    ServantSourceEmitter src_emitter (src_os,
                                      export_macro_,
                                      cl_,
                                      declarations);
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

  export_macro_ = cl_.get_value ("export-macro", "");

  if (export_macro_.empty () && !file_name_.empty ())
  {
    // Modify a copy of the filename string.
    export_macro_ = file_name_;

    // Convert filename string to upper case.
    transform (export_macro_.begin (),
               export_macro_.end (),
               export_macro_.begin (),
               upcase);

    // Replace the suffix.
    export_macro_ =
      regex::perl_s (export_macro_,
                     "/(\\.(IDL|CIDL))?$/_SVNT_Export/");

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
                     "/(\\.(idl|cidl))?$/" + file_suffix + "/");

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
    : static_cast<ostream&> (cout);
}


/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
