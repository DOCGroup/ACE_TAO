// $Id$
#ifndef SERVANT_SOURCE_GENERATOR_HPP
#define SERVANT_SOURCE_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include "CCF/CIDL/CIDL_SyntaxTree.hpp"
#include "CCF/CIDL/CIDL_Traversal.hpp"

using std::ostream;

using namespace CIDL;
using namespace CIDL::SyntaxTree;

// SourceEmitterBase is a base class that holds the ostream member
// common to every other class in this file.
//
class SourceEmitterBase
{
protected:
  SourceEmitterBase (ostream&);

  ostream& os;
};

// Emitter generates the servant source mapping for declarations collected
// by Collector. Note that the original structure of modules is preserved.
//
class ServantSourceEmitter
  : public SourceEmitterBase,
    public virtual Traversal::TranslationUnit,
    public virtual Traversal::TranslationRegion,
    public virtual Traversal::FileScope,
    public virtual Traversal::Module,
    public virtual Traversal::UnconstrainedInterfaceDef,
    public virtual Traversal::ConcreteEventTypeDef,
    public virtual Traversal::ComponentDef,
    public virtual Traversal::HomeDef
{
public:
  ServantSourceEmitter (ostream& os_,
                        CommandLine const& cl,
                        UnconstrainedInterfaceDefSet const& interface_set,
                        ComponentDefSet const& component_set,
                        HomeDefSet const& home_set);

  bool
  contains_element (ModulePtr const& m) const;

  virtual void
  visit_translation_unit_pre (TranslationUnitPtr const&);

  virtual void
  visit_module_pre (ModulePtr const& m);

  virtual void
  visit_unconstrained_interface_def (
    UnconstrainedInterfaceDefPtr const& i);

  virtual void
  visit_component_def (ComponentDefPtr const& c);

  virtual void
  visit_home_def (HomeDefPtr const& h);

  virtual void
  visit_module_post (ModulePtr const& m);

private:
  CommandLine const& cl_;

  ComponentDefSet const& component_set_;
  HomeDefSet const& home_set_;
  UnconstrainedInterfaceDefSet const& interface_set_;
};

class ServantSourceFinalizingEmitter
  : public SourceEmitterBase,
    public virtual Traversal::TranslationUnit,
    public virtual Traversal::TranslationRegion,
    public virtual Traversal::FileScope,
    public virtual Traversal::Module,
    public virtual Traversal::HomeDef
{
public:
  ServantSourceFinalizingEmitter (ostream& os_,
                                  string export_macro);

  virtual void
  visit_home_def (HomeDefPtr const& h);

private:
  string export_macro_;
};

#endif // SERVANT_SOURCE_GENERATOR_HPP
