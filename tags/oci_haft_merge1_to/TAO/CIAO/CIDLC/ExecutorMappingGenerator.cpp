// $Id$
#include "ExecutorMappingGenerator.hpp"

#include <map>
#include <ostream>
#include <fstream>

#include "CCF/CIDL/CIDL_SyntaxTree.hpp"
#include "CCF/CIDL/CIDL_Traversal.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IDLStream.hpp"

using std::cout;
using std::endl;
using std::string;
using std::make_pair;

using namespace CIDL;
using namespace CIDL::SyntaxTree;

namespace
{
  typedef
  std::map <CompositionPtr, ComponentDefPtr, CompositionOrderComparator>
  CompositionMap;

  // ProvidesCollector collects interfaces that appear in provides
  // declaration of a component in question. Used as a building block
  // by Collector.
  //
  class ProvidesCollector : public virtual Traversal::ComponentDef,
                            public virtual Traversal::ProvidesDecl
  {
  public:
    ProvidesCollector(UnconstrainedInterfaceDeclSet& interface_set)
        : interface_set_ (interface_set)
    {
    }

    virtual void
    visit_provides_decl (ProvidesDeclPtr const& p)
    {
      //@@ CCM issue: interface should be defined at this point
      //              and this should be ensured during semantic
      //              checking.

      UnconstrainedInterfaceDeclPtr decl (
        p->type ()->dynamic_type<UnconstrainedInterfaceDecl> ());

      // We are only interested in unconstrained interfaces.
      if (decl != 0)
      {
        ScopedName orig (decl->name ());
        ScopedName mapping (orig.scope (), "CCM_" + orig.simple ());

        // Check if mapping has already been provided.
        //@@ bad code: i->scope ()->table ()
        if (!decl->scope ()->table ().exist (mapping))
        {
          // Add to the list if it's not already there.
          interface_set_.insert (decl);
        }
      }
    }

  private:
    UnconstrainedInterfaceDeclSet& interface_set_;
  };


  // Collector populates lists of declarations for which local facet
  // executor mapping should be provided.
  //
  class Collector : public virtual Traversal::TranslationUnit,
                    public virtual Traversal::PrincipalTranslationRegion,
                    public virtual Traversal::FileScope,
                    public virtual Traversal::Module,
                    public virtual Traversal::Composition,
                    public virtual Traversal::HomeExecutor,
                    public virtual Traversal::ConcreteEventTypeDef
  {
  public:
    Collector (UnconstrainedInterfaceDeclSet& interface_set,
               ComponentDefSet& component_set,
               ConcreteEventTypeDefSet& event_type_set,
               HomeDefSet& home_set,
               CompositionMap& composition_map)
        : home_set_ (home_set),
          component_set_ (component_set),
          event_type_set_ (event_type_set),
          composition_map_ (composition_map),
          provides_collector_ (interface_set)
    {
    }

    virtual void
    visit_home_executor (HomeExecutorPtr const& he)
    {
      CompositionPtr composition (
        he->scope ()->dynamic_type<SyntaxTree::Composition> ());

      HomeDefPtr home (he->implements ());

      ComponentDefPtr component (home->manages ());

      composition_map_.insert (make_pair (composition, component));

      add_home (home);
      add_component (component);
    }

    virtual void
    visit_concrete_event_type_def (ConcreteEventTypeDefPtr const& et)
    {
      /*
      @@ CCM issue: spec says that I need to ganarate this but nobody
                    knows why.
      event_type_set_.insert (et);
      */
    }

  private:
    void
    add_home (HomeDefPtr const& h)
    {
      ScopedName n (h->name ());

      ScopedName main (n.scope (), "CCM_" + n.simple ());
      ScopedName expl (n.scope (), "CCM_" + n.simple () + "Explicit");
      ScopedName impl (n.scope (), "CCM_" + n.simple () + "Implicit");

      // Check if mapping has already been provided.
      if (h->table ().exist (main) ||
          h->table ().exist (expl) ||
          h->table ().exist (impl)) return;

      if(home_set_.insert (h).second)
      {
        // Note that I don't go after components that home manages
        // because it will be handled by component inheritance tree.
        //
        if (h->inherits ()) add_home (h->inherits ().resolve ());
      }
    }

    void
    add_component (ComponentDefPtr const& component)
    {
      ScopedName n (component->name ());

      ScopedName monolith (n.scope (), "CCM_" + n.simple ());
      ScopedName context (n.scope (), "CCM_" + n.simple () + "_Context");

      // Check if mapping has already been provided.
      if (component->table ().exist (context) ||
          component->table ().exist (monolith)) return;

      if(component_set_.insert (component).second)
      {
        component->accept (&provides_collector_);

        if (component->inherits ())
        {
          add_component (component->inherits ().resolve ());
        }
      }
    }

  private:
    HomeDefSet& home_set_;
    ComponentDefSet& component_set_;
    ConcreteEventTypeDefSet& event_type_set_;
    CompositionMap& composition_map_;

    ProvidesCollector provides_collector_;
  };

  //
  //
  //
  class ForcedCollector : public virtual Traversal::TranslationUnit,
                          public virtual Traversal::PrincipalTranslationRegion,
                          public virtual Traversal::FileScope,
                          public virtual Traversal::Module,
                          public virtual Traversal::ComponentDef,
                          public virtual Traversal::HomeDef,
                          public virtual Traversal::UnconstrainedInterfaceDef
  {
  public:
    ForcedCollector (UnconstrainedInterfaceDeclSet& interface_set,
                     ComponentDefSet& component_set,
                     ConcreteEventTypeDefSet& event_type_set,
                     HomeDefSet& home_set,
                     CompositionMap& composition_map)
        : home_set_ (home_set),
          component_set_ (component_set),
          event_type_set_ (event_type_set),
          composition_map_ (composition_map),
          interface_set_ (interface_set)
    {
    }

    virtual void
    visit_component_def (ComponentDefPtr const& c)
    {
      add_component (c);
    }

    virtual void
    visit_home_def (HomeDefPtr const& h)
    {
      add_home (h);
    }

    virtual void
    visit_unconstrained_interface_def (UnconstrainedInterfaceDefPtr const& i)
    {
      ScopedName orig (i->name ());
      ScopedName mapping (orig.scope (), "CCM_" + orig.simple ());

      // Check if mapping has already been provided.
      if (i->table ().exist (mapping)) return;

      // Add to the list if it's not already there.
      interface_set_.insert (i);
    }

    virtual void
    visit_concrete_event_type_def (ConcreteEventTypeDefPtr const& et)
    {
      /*
      @@ CCM issue: spec says that I need to ganarate this but nobody
                    knows why.
      event_type_set_.insert (et);
      */
    }

  private:
    void
    add_home (HomeDefPtr const& h)
    {
      ScopedName n (h->name ());

      ScopedName main (n.scope (), "CCM_" + n.simple ());
      ScopedName expl (n.scope (), "CCM_" + n.simple () + "Explicit");
      ScopedName impl (n.scope (), "CCM_" + n.simple () + "Implicit");

      // Check if mapping has already been provided.
      if (h->table ().exist (main) ||
          h->table ().exist (expl) ||
          h->table ().exist (impl)) return;

      if(home_set_.insert (h).second)
      {
        // Note that I don't go after components that home manages
        // because it will be handled by component inheritance tree.
        //
        if (h->inherits ()) add_home (h->inherits ().resolve ());
      }
    }

    void
    add_component (ComponentDefPtr const& component)
    {
      ScopedName n (component->name ());

      ScopedName monolith (n.scope (), "CCM_" + n.simple ());
      ScopedName context (n.scope (), "CCM_" + n.simple () + "_Context");

      // Check if mapping has already been provided.
      if (component->table ().exist (context) ||
          component->table ().exist (monolith)) return;

      if(component_set_.insert (component).second)
      {
        if (component->inherits ())
        {
          add_component (component->inherits ().resolve ());
        }
      }
    }

  private:
    HomeDefSet& home_set_;
    ComponentDefSet& component_set_;
    ConcreteEventTypeDefSet& event_type_set_;
    CompositionMap& composition_map_;
    UnconstrainedInterfaceDeclSet& interface_set_;
  };


  //
  //
  //
  class TypeNameEmitter : public virtual Traversal::BuiltInTypeDef,
                          public virtual Traversal::TypeDecl
  {
  public:
    TypeNameEmitter (std::ostream& os_)
        : os (os_)
    {
    }

    virtual void
    visit_built_in_type_def (BuiltInTypeDefPtr const& t)
    {
      os << t->name ().simple ();
    }

    virtual void
    visit_type_decl (TypeDeclPtr const& t)
    {
      os << t->name ();
    }

  private:
    std::ostream& os;
  };


  // MonolithEmitter generates what spec calls 'Monolithic Component
  // Executor'.
  //
  class MonolithEmitter : public virtual Traversal::ComponentDef,
                          public virtual Traversal::AttributeDecl,
                          public virtual Traversal::ProvidesDecl,
                          public virtual Traversal::ConsumesDecl
  {
  public:
    MonolithEmitter (std::ostream& os_,
                     ComponentDefSet const& component_set,
                     CCF::Traversal::Visitor* type_name_emitter)
        : AttributeDecl (type_name_emitter),
          os (os_),
          component_set_ (component_set)
    {
    }

    virtual void
    visit_component_def (ComponentDefPtr const& c)
    {
      if (component_set_.find (c) != component_set_.end ())
      {
        Traversal::ComponentDef::visit_component_def (c);
      }
    }

    virtual void
    visit_component_def_pre (ComponentDefPtr const& c)
    {
      os << "local interface CCM_" << c->name ().simple () << " : ";

      ComponentDefRef cr = c->inherits ();

      if (cr)
      {
        os << cr.name ().scope () << "::CCM_" << cr.name ().simple ();
      }
      else
      {
        os << "::Components::EnterpriseComponent";
      }

      for (SyntaxTree::ComponentDef::Iterator i = c->supports_begin ();
           i != c->supports_end ();
           i++)
      {
        os << ", " << i->name ();
      }

      os << endl
         << "{" << endl;
    }

    virtual void
    visit_attribute_pre (AttributeDeclPtr const& a)
    {
      os << "attribute ";
    }

    virtual void
    visit_attribute_post (AttributeDeclPtr const& a)
    {
      os << " " << a->name ().simple () << ";" << endl;
    }

    virtual void
    visit_provides_decl (ProvidesDeclPtr const& p)
    {
      ScopedName n = p->type ()->name ();

      os << n.scope () << "::CCM_" << n.simple ()
         << " get_" << p->name ().simple () << " ();" << endl;
    }

    virtual void
    visit_consumes_decl (ConsumesDeclPtr const& p)
    {
      os << "void push_" << p->name ().simple ()
         << " (in " << p->type()->name () << " ev);" << endl;
    }

    virtual void
    visit_component_def_post (ComponentDefPtr const& c)
    {
      os << "};" << endl
         << endl;
    }

  private:
    std::ostream& os;
    ComponentDefSet const& component_set_;
  };

  // ContextEmitter generates component context interface.
  //
  //
  class ContextEmitter : public virtual Traversal::ComponentDef,
                         public virtual Traversal::UsesDecl,
                         public virtual Traversal::PublishesDecl,
                         public virtual Traversal::EmitsDecl
  {
  public:
    ContextEmitter (std::ostream& os_,
                    ComponentDefSet const& component_set)
        : os (os_),
          component_set_ (component_set)
    {
    }

    virtual void
    visit_component_def (ComponentDefPtr const& c)
    {
      if (component_set_.find (c) != component_set_.end ())
      {
        Traversal::ComponentDef::visit_component_def (c);
      }
    }


    virtual void
    visit_component_def_pre (ComponentDefPtr const& c)
    {
      os << "local interface "
         << "CCM_" << c->name ().simple () << "_Context "
         << " : ";

      ComponentDefRef cr = c->inherits ();

      if (cr)
      {
        ScopedName name (cr.name ());
        os << name.scope () << "::CCM_" << name.simple () << "_Context";
      }
      else
      {
        os << "::Components::SessionContext";
      }

      os << endl
         << "{" << endl;
    }

    virtual void
    visit_uses_decl (UsesDeclPtr const& d)
    {
      os << d->type ()->name ()
         << " get_connection_" << d->name ().simple ()
         << " ();" << endl;
    }

    virtual void
    visit_publishes_decl (PublishesDeclPtr const& d)
    {
      os << "void push_" << d->name ().simple ()
         << " (in " << d->type ()->name () << " ev);" << endl;
    }

    virtual void
    visit_emits_decl (EmitsDeclPtr const& d)
    {
      os << "void push_" << d->name ().simple ()
         << " (in " << d->type ()->name () << " ev);" << endl;
    }

    virtual void
    visit_component_def_post (ComponentDefPtr const& c)
    {
      os << "};" << endl
         << endl;
    }

  private:
    std::ostream& os;
    ComponentDefSet const& component_set_;
  };

  // HomeExplicitEmitter generates home explicit interface
  //
  //
  class HomeExplicitEmitter : public virtual Traversal::OperationParameter,
                              public virtual Traversal::Comma,
                              public virtual Traversal::HomeFactoryDecl,
                              public virtual Traversal::OperationDecl,
                              public virtual Traversal::AttributeDecl,
                              public virtual Traversal::HomeDef
  {
  public:
    HomeExplicitEmitter (std::ostream& os_,
                         CCF::Traversal::Visitor* type_name_emitter)
        : OperationParameter (type_name_emitter,
                              type_name_emitter,
                              type_name_emitter),

          OperationDecl (type_name_emitter),
          AttributeDecl (type_name_emitter),
          os (os_)
    {
    }

    virtual void
    visit_home_def_pre (HomeDefPtr const& h)
    {
      os << "local interface "
         << "CCM_" << h->name ().simple () << "Explicit"
         << " : ";

      HomeDefRef hr = h->inherits ();

      if (hr)
      {
        ScopedName name (hr.name ());
        os << name.scope () << "::CCM_" << name.simple () << "Explicit";
      }
      else
      {
        os << "::Components::HomeExecutorBase";
      }

      for (SyntaxTree::HomeDef::Iterator i = h->supports_begin ();
           i != h->supports_end ();
           i++)
      {
        os << ", " << i->name ();
      }

      os << endl
         << "{" << endl;

    }

    //
    // OperationParameter
    //

    virtual void
    visit_operation_parameter_post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    visit_comma (CommaPtr const& s)
    {
      os << ", ";
    }

    //
    // HomeFactory
    //

    virtual void
    visit_home_factory_decl_type (HomeFactoryDeclPtr const& d)
    {
      os << "::Components::EnterpriseComponent ";
    }

    virtual void
    visit_home_factory_decl_name (HomeFactoryDeclPtr const& d)
    {
      os << d->name ().simple () << " (";
    }

    virtual void
    visit_home_factory_decl_post (HomeFactoryDeclPtr const& d)
    {
      os << ");" << endl;
    }

    //
    // Operation
    //

    virtual void
    visit_operation_decl_name (OperationDeclPtr const& d)
    {
      os << " " << d->name ().simple () << " (";
    }

    virtual void
    visit_operation_parameter_pre (OperationParameterPtr const& op)
    {
      os << op->direction () << " ";
    }

    virtual void
    visit_operation_decl_post (OperationDeclPtr const& d)
    {
      os << ");" << endl;
    }

    //
    // Attribute
    //

    virtual void
    visit_attribute_pre (AttributeDeclPtr const& a)
    {
      os << "attribute ";
    }

    virtual void
    visit_attribute_post (AttributeDeclPtr const& a)
    {
      os << " " << a->name ().simple () << ";" << endl;
    }

    virtual void
    visit_home_def_post (HomeDefPtr const& h)
    {
      os << "};" << endl
         << endl;
    }

  private:
    std::ostream& os;
  };

  // HomeImplicitEmitter generates home implicit interface
  //
  //
  class HomeImplicitEmitter : public virtual Traversal::HomeDef
  {
  public:
    HomeImplicitEmitter (std::ostream& os_) : os (os_) {}

    virtual void
    visit_home_def_pre (HomeDefPtr const& h)
    {
      os << "local interface "
         << "CCM_" << h->name ().simple () << "Implicit" << endl
         << "{" << endl
         << "::Components::EnterpriseComponent "
         << "create () raises (::Components::CCMException);"
         << endl;

    }

    virtual void
    visit_home_def_post (HomeDefPtr const& h)
    {
      os << "};" << endl
         << endl;
    }

  private:
    std::ostream& os;
  };

  // HomeMainEmitter generates home main interface
  //
  //
  class HomeMainEmitter : public virtual Traversal::HomeDef
  {
  public:
    HomeMainEmitter (std::ostream& os_) : os (os_) {}

    virtual void
    visit_home_def_pre (HomeDefPtr const& h)
    {
      SimpleName name = h->name ().simple ();

      os << "local interface "
         << "CCM_" << name
         << " : "
         << "CCM_" << name << "Explicit, "
         << "CCM_" << name << "Implicit" << endl
         << "{" << endl;
    }

    virtual void
    visit_home_def_post (HomeDefPtr const& h)
    {
      os << "};" << endl
         << endl;
    }

  private:
    std::ostream& os;
  };

  class IncludeEmitter :
    public virtual Traversal::TranslationUnit,
    public virtual Traversal::ImpliedIncludeTranslationRegion,
    public virtual Traversal::PrincipalTranslationRegion,
    public virtual Traversal::IncludeTranslationRegion,
    public virtual Traversal::SysIncludeTranslationRegion
  {
  public:
    IncludeEmitter (std::ostream& os_) : os (os_) {}

    virtual void
    visit_implied_include_translation_region (
      ImpliedIncludeTranslationRegionPtr const& r)
    {
      os << "#include <" << r->file_path ().string () << ">" << endl;
    }

    virtual void
    visit_include_translation_region (IncludeTranslationRegionPtr const& r)
    {
      os << "#include \"" << r->file_path ().string () << "\"" << endl;
    }

    virtual void
    visit_sys_include_translation_region (
      SysIncludeTranslationRegionPtr const& r)
    {
      os << "#include <" << r->file_path ().string () << ">" << endl;
    }
  private:
    std::ostream& os;
  };


  // Emitter generates local executor mapping for declarations collected
  // by Collector. Note that the original structire of modules is preserved.
  //
  class Emitter : public virtual Traversal::TranslationUnit,
                  public virtual Traversal::TranslationRegion,
                  public virtual Traversal::FileScope,
                  public virtual Traversal::Module,
                  public virtual Traversal::UnconstrainedInterfaceDecl,
                  public virtual Traversal::ConcreteEventTypeDef,
                  public virtual Traversal::ComponentDef,
                  public virtual Traversal::HomeDef,
                  public virtual Traversal::Composition
  {
  public:
    Emitter (std::ostream& os_,
             UnconstrainedInterfaceDeclSet const& interface_set,
             ComponentDefSet const& component_set,
             ConcreteEventTypeDefSet const& event_type_set,
             HomeDefSet const& home_set,
             CompositionMap const& composition_map)
        : os(os_),
          home_set_ (home_set),
          component_set_ (component_set),
          event_type_set_ (event_type_set),
          interface_set_ (interface_set),
          composition_map_ (composition_map),

          type_name_emitter_ (os_),

          context_emitter_ (os, component_set),
          include_emitter_ (os),
          monolith_emitter_ (os, component_set, &type_name_emitter_),
          home_main_emitter_ (os),
          home_explicit_emitter_ (os, &type_name_emitter_),
          home_implicit_emitter_ (os)
    {
    }

    bool
    contains_element (ModulePtr const& m) const
    {
      for (UnconstrainedInterfaceDeclSet::const_iterator i =
             interface_set_.begin ();
           i != interface_set_.end ();
           i++)
      {
        if (m->order ().suborder ((*i)->order ())) return true;
      }

      for (ComponentDefSet::const_iterator i = component_set_.begin ();
           i != component_set_.end ();
           i++)
      {
        if (m->order ().suborder ((*i)->order ())) return true;
      }

      for (ConcreteEventTypeDefSet::const_iterator i =
             event_type_set_.begin ();
           i != event_type_set_.end ();
           i++)
      {
        if (m->order ().suborder ((*i)->order ())) return true;
      }

      for (HomeDefSet::const_iterator i = home_set_.begin ();
           i != home_set_.end ();
           i++)
      {
        if (m->order ().suborder ((*i)->order ())) return true;
      }

      for (CompositionMap::const_iterator i = composition_map_.begin ();
           i != composition_map_.end ();
           i++)
      {
        if (m->order ().suborder (i->first->order ())) return true;
      }

      return false;
    }

    virtual void
    visit_translation_unit_pre (TranslationUnitPtr const& u)
    {
      u->accept(&include_emitter_);
      os << endl;
    }

    virtual void
    visit_module_pre (ModulePtr const& m)
    {
      if (contains_element (m))
      {
        os << "module " << m->name ().simple () << endl
           << "{" << endl;
      }
    }

    virtual void
    visit_module_post (ModulePtr const& m)
    {
      if (contains_element (m))
      {
        os << "};" << endl
           << endl;
      }
    }

    virtual void
    visit_unconstrained_interface_decl_pre (
      UnconstrainedInterfaceDeclPtr const& i)
    {
      if (interface_set_.find (i) != interface_set_.end ())
      {
        os << "local interface CCM_" << i->name ().simple ()
           << " : " << i->name ().simple () << endl
           << "{"   << endl
           << "};"  << endl
           << endl;
      }
    }

    virtual void
    visit_concrete_event_type_def_pre (ConcreteEventTypeDefPtr const& et)
    {
      /*
      @@ CCM issue: spec says that I need to ganarate this but nobody
                    knows why.

      os << "local interface "
         << "CCM_" << et->name ().simple () << "Consumer" << endl
         << "{"  << endl
         << "void push (in " << et->name ().simple () << " ev);" << endl
         << "};"  << endl
         << endl;
      */
    }

    virtual void
    visit_component_def (ComponentDefPtr const& c)
    {
      c->accept (&monolith_emitter_);
      c->accept (&context_emitter_);
    }

    virtual void
    visit_home_def (HomeDefPtr const& h)
    {
      if (home_set_.find (h) != home_set_.end ())
      {
        h->accept (&home_explicit_emitter_);
        h->accept (&home_implicit_emitter_);
        h->accept (&home_main_emitter_);
      }
    }

    virtual void
    visit_composition (CompositionPtr const& c)
    {
      if (composition_map_.find (c) != composition_map_.end ())
      {
        Traversal::Composition::visit_composition (c);
      }
    }

    virtual void
    visit_composition_pre (CompositionPtr const& c)
    {
      ComponentDefPtr component (composition_map_.find (c)->second);
      SyntaxTree::Composition::Category::Value category = c->category ();

      os << "module " << c->name ().simple () << endl
         << "{" << endl;

      os << "local interface _Context : "
         << component->name ().scope () << "::CCM_"
         << component->name ().simple () << "_Context, ";

      if (category == SyntaxTree::Composition::Category::ENTITY)
      {
        os << "::Components::EntityContext" << endl;
      }
      else
      {
        os << "::Components::SessionContext" << endl;
      }

      os << "{" << endl
         << "};" << endl
         << endl;
    }


    virtual void
    visit_composition_post (CompositionPtr const& c)
    {
      os << "};" << endl
         << endl;
    }


  private:

    std::ostream& os;

    HomeDefSet const& home_set_;
    ComponentDefSet const& component_set_;
    ConcreteEventTypeDefSet const& event_type_set_;
    UnconstrainedInterfaceDeclSet const& interface_set_;
    CompositionMap const& composition_map_;

    TypeNameEmitter type_name_emitter_;

    ContextEmitter context_emitter_;
    IncludeEmitter include_emitter_;
    MonolithEmitter monolith_emitter_;
    HomeMainEmitter home_main_emitter_;
    HomeExplicitEmitter home_explicit_emitter_;
    HomeImplicitEmitter home_implicit_emitter_;
  };
}

void ExecutorMappingGenerator::
options (CL::Description& d)
{
  d.add_option (CL::OptionDescription (
                  "lem-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'_exec\' "
                  "when constructing name of local executor mapping file.",
                  true));

  d.add_option (CL::OptionDescription (
                  "lem-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of local executor mapping file.",
                  true));

  d.add_option (CL::OptionDescription (
                  "lem-force-all",
                  "Force generation of local executor mapping for all IDL "
                  "types including those not used (directly or inderectly) "
                  "by compositions. This option is useful for generating a "
                  "common portion of local executor mapping used by more "
                  "than one component or composition.",
                  true));
}


void ExecutorMappingGenerator::
generate (CommandLine const& cl,
          TranslationUnitPtr const& u)
{
  fs::path file_path = u->principal_translation_region ()->file_path ();

  fs::ofstream ofs;

  if (!file_path.empty ())
  {
    string file_name = file_path.leaf ();

    string suffix = cl.get_value ("lem-file-suffix", "_exec.idl");
    string expr = cl.get_value (
      "lem-file-regex",
      "/^(.*?)(\\.(idl|cidl))?$/$1" + suffix + "/");

    string lem_file_name = regex::perl_s (file_name, expr);

    fs::path lem_file_path (lem_file_name);

    ofs.open (lem_file_path, std::ios_base::out);

    if (!ofs.is_open ())
    {
      cerr << lem_file_name << ": error: unable to open in write mode"
           << endl;
      return;
    }
  }

  std::ostream& os =
    ofs.is_open ()
    ? static_cast<std::ostream&> (ofs)
    : static_cast<std::ostream&> (std::cout);

  //@@ need to apply RAII here

  // Set auto-indentation for os
  IDLFormattingBuffer ifb (os.rdbuf ());
  os.rdbuf (&ifb);

  HomeDefSet home_set;
  ComponentDefSet component_set;
  CompositionMap composition_map;
  ConcreteEventTypeDefSet event_type_set;
  UnconstrainedInterfaceDeclSet interface_set;

  if (cl.get_value ("lem-force-all", false))
  {
    ForcedCollector collector (interface_set,
                               component_set,
                               event_type_set,
                               home_set,
                               composition_map);
    u->accept (&collector);
  }
  else
  {
    Collector collector (interface_set,
                         component_set,
                         event_type_set,
                         home_set,
                         composition_map);
    u->accept (&collector);
  }

  {
    Emitter emitter (os,
                     interface_set,
                     component_set,
                     event_type_set,
                     home_set,
                     composition_map);
    u->accept (&emitter);
  }

  // Reset auto-indentation for os
  os.rdbuf (ifb.next ());
}
