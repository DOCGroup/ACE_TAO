// $Id$
#include "ServantGenerator.hpp"
#include "ServantHeaderGenerator.hpp"
#include "ServantSourceGenerator.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IDLStream.hpp"

ProvidesCollector::ProvidesCollector (
  UnconstrainedInterfaceDefSet& interface_set)
    : interface_set_ (interface_set)
{
}

void
ProvidesCollector::visit_provides_decl (ProvidesDeclPtr const& p)
{
  //@@ CCM issue: interface should be defined at this point
  UnconstrainedInterfaceDefPtr def (
    p->type ()->dynamic_type<SyntaxTree::UnconstrainedInterfaceDef> ());

  if (def != 0)
  {
    // Add to the list if it's not already there.
    interface_set_.insert (def);
  }
}

Collector::Collector (UnconstrainedInterfaceDefSet& interface_set,
                      ComponentDefSet& component_set,
                      HomeDefSet& home_set)
    : component_set_ (component_set),
      home_set_ (home_set),
      provides_collector_ (interface_set)
{
}

void
Collector::visit_home_executor (HomeExecutorPtr const& he)
{
  HomeDefPtr h = he->implements ();
  ComponentDefPtr c = h->manages ();

  //@@unsupported: need to handle inherited components as well
  component_set_.insert (c);
  home_set_.insert (h);

  c->accept (&provides_collector_);
}

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

void
ServantGenerator::generate (CIDL::SyntaxTree::TranslationUnitPtr const& u)
{
  ComponentDefSet component_set_;
  HomeDefSet home_set_;
  UnconstrainedInterfaceDefSet interface_set;

  {
    Collector collector (interface_set,
                         component_set_,
                         home_set_);
    u->accept (&collector);
  }

  compute_export_macro (u->principal_translation_region ()->file_path ());

  {
    fs::ofstream hdr_ofs;
    ostream& hdr_os = configure_stream ("hdr-file-suffix",
                                        "_svnt.h",
                                        "hdr-file-regex",
                                        hdr_ofs);

    // Set auto-indentation for os
    IDLFormattingBuffer ifb (hdr_os.rdbuf ());
    hdr_os.rdbuf (&ifb);

    {
      ServantHeaderEmitter hdr_emitter (hdr_os,
                                        cl_,
                                        export_macro_,
                                        interface_set,
                                        component_set_,
                                        home_set_);
      u->accept (&hdr_emitter);
    }

    {
      ServantHeaderFinalizingEmitter hdr_finalizer (hdr_os,
                                                    cl_,
                                                    export_macro_);
      u->accept (&hdr_finalizer);
    }
  }

  {
    fs::ofstream src_ofs;
    ostream& src_os = configure_stream ("src-file-suffix",
                                        "_svnt.cpp",
                                        "src-file-regex",
                                        src_ofs);

    // Set auto-indentation for os
    IDLFormattingBuffer ifb (src_os.rdbuf ());
    src_os.rdbuf (&ifb);

    {
      ServantSourceEmitter src_emitter (src_os,
                                        cl_,
                                        interface_set,
                                        component_set_,
                                        home_set_);
      u->accept (&src_emitter);
    }

    {
      ServantSourceFinalizingEmitter src_finalizer (src_os,
                                                    export_macro_);
      u->accept (&src_finalizer);
    }
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
                     "/^(.*?)(\\.(IDL|CIDL))?$/$1_SVNT_Export/");

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
                     "/^(.*?)(\\.(idl|cidl))?$/$1" + file_suffix + "/");

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
