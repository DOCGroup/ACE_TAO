// $Id$
#ifndef SERVANT_GENERATOR_HPP
#define SERVANT_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include "CCF/CIDL/CIDL_SyntaxTree.hpp"
#include "CCF/CIDL/CIDL_Traversal.hpp"

using namespace std;

using namespace CIDL;
using namespace CIDL::SyntaxTree;

// ProvidesCollector collects interfaces that appear in provides
// declaration of a component in question. Used as a building block
// by Collector.
//
class ProvidesCollector : public virtual Traversal::ComponentDef,
                          public virtual Traversal::ProvidesDecl
{
public:
  ProvidesCollector (UnconstrainedInterfaceDefSet& interface_set);

  virtual void
  visit_provides_decl (ProvidesDeclPtr const& p);

private:
  UnconstrainedInterfaceDefSet& interface_set_;
};

// Collector populates lists of declarations for which local facet
// executor mapping should be provided.
//
class Collector : public virtual Traversal::TranslationUnit,
                  public virtual Traversal::TranslationRegion,
                  public virtual Traversal::FileScope,
                  public virtual Traversal::Module,
                  public virtual Traversal::Composition,
                  public virtual Traversal::HomeExecutor
{
public:
  Collector (UnconstrainedInterfaceDefSet& interface_set,
             ComponentDefSet& component_set,
             HomeDefSet& home_set);

  virtual void
  visit_home_executor (HomeExecutorPtr const& he);

private:
  ComponentDefSet& component_set_;
  HomeDefSet& home_set_;

  ProvidesCollector provides_collector_;
};

class ServantGenerator
{
public:
  ServantGenerator (CommandLine const& cl);

  void
  generate (TranslationUnitPtr const& u);

private:
  void
  compute_export_macro (const fs::path& file_path);

  ostream&
  configure_stream (string const& suffix_option,
                    string const& default_suffix,
                    string const& regex_option,
                    fs::ofstream& ofs);

private:
  CommandLine const& cl_;
  string file_name_;
  string export_macro_;
};

#endif // SERVANT_GENERATOR_HPP
