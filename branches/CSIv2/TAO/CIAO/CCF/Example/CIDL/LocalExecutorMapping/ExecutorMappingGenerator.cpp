// file      : CCF/Example/CIDL/LocalExecutorMapping/ExecutorMappingGenerator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "ExecutorMappingGenerator.hpp"

#include <map>
#include <ostream>
#include <fstream>

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IndentationIDL.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

using std::cout;
using std::endl;
using std::string;
using std::make_pair;

using namespace CCF::CIDL;
using namespace SyntaxTree;

/*

@@ bad code: i->scope ()->table ()

*/

namespace
{
  class Declarations
  {
  public:
    bool
    add (HomeDefPtr const& h)
    {
      return homes_.insert (h).second;
    }

    bool
    add (ComponentDefPtr const& c)
    {
      return components_.insert (c).second;
    }

    bool
    add (CompositionPtr const& cs, ComponentDefPtr const& cn)
    {
      return compositions_.insert (make_pair(cs, cn)).second;
    }

    bool
    add (UnconstrainedInterfaceDeclPtr const& i)
    {
      return interfaces_.insert (i).second;
    }

  public:
    bool
    find (HomeDefPtr const& h) const
    {
      return homes_.find (h) != homes_.end ();
    }

    bool
    find (ComponentDefPtr const& c) const
    {
      return components_.find (c) != components_.end ();
    }

    bool
    find (CompositionPtr const& c) const
    {
      return compositions_.find (c) != compositions_.end ();
    }

    bool
    find (UnconstrainedInterfaceDeclPtr const& i) const
    {
      return interfaces_.find (i) != interfaces_.end ();
    }

  public:
    ComponentDefPtr
    resolve (CompositionPtr const& c) const
    {
      CompositionMap::const_iterator i = compositions_.find (c);
      if (i != compositions_.end ())
      {
        return i->second;
      }
      else
      {
        return ComponentDefPtr (0);
      }
    }

  public:
    bool
    contains_suborder (Order const& o) const
    {
      for (UnconstrainedInterfaceDeclSet::const_iterator i =
           interfaces_.begin ();
           i != interfaces_.end ();
           i++)
      {
        if (o.suborder ((*i)->order ())) return true;
      }

      for (ComponentDefSet::const_iterator i = components_.begin ();
           i != components_.end ();
           i++)
      {
        if (o.suborder ((*i)->order ())) return true;
      }

      for (HomeDefSet::const_iterator i = homes_.begin ();
           i != homes_.end ();
           i++)
      {
        if (o.suborder ((*i)->order ())) return true;
      }

      for (CompositionMap::const_iterator i = compositions_.begin ();
           i != compositions_.end ();
           i++)
      {
        if (o.suborder (i->first->order ())) return true;
      }

      return false;
    }

  private:
    typedef
    std::map <CompositionPtr, ComponentDefPtr, CompositionOrderComparator>
    CompositionMap;

    HomeDefSet homes_;
    ComponentDefSet components_;
    CompositionMap compositions_;
    UnconstrainedInterfaceDeclSet interfaces_;
  };


  //
  //
  //
  class HomeCollector : public Traversal::HomeDef
  {
  public:
    HomeCollector (Declarations& declarations)
        : declarations_ (declarations)
    {
    }

    virtual void
    traverse (HomeDefPtr const& h)
    {
      ScopedName n (h->name ());

      ScopedName main (n.scope (), "CCM_" + n.simple ());
      ScopedName expl (n.scope (), "CCM_" + n.simple () + "Explicit");
      ScopedName impl (n.scope (), "CCM_" + n.simple () + "Implicit");

      // Check if mapping has already been provided.
      if (h->table ().exist (main) ||
          h->table ().exist (expl) ||
          h->table ().exist (impl)) return;

      if(declarations_.add (h))
      {
        // Note that I don't go after components that inherited home manages
        // because it will be handled by component inheritance tree.
        //
        if (h->inherits ()) traverse (h->inherits ().resolve ());
      }
    }

  private:
    Declarations& declarations_;
  };

  //
  //
  //
  class ComponentCollector : public Traversal::ComponentDef
  {
  public:
    ComponentCollector (Declarations& declarations)
        : declarations_ (declarations)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
      ScopedName n (c->name ());

      ScopedName monolith (n.scope (), "CCM_" + n.simple ());
      ScopedName context (n.scope (), "CCM_" + n.simple () + "_Context");

      // Check if mapping has already been provided.
      if (c->table ().exist (context) ||
          c->table ().exist (monolith)) return;

      if(declarations_.add (c))
      {
        if (c->inherits ())
        {
          traverse (c->inherits ().resolve ());
        }
      }
    }

  private:
    Declarations& declarations_;
  };

  //
  //
  //
  class InterfaceCollector : public Traversal::UnconstrainedInterfaceDecl
  {
  public:
    InterfaceCollector (Declarations& declarations)
        : declarations_ (declarations)
    {
    }

    virtual void
    traverse (UnconstrainedInterfaceDeclPtr const& i)
    {
      ScopedName orig (i->name ());
      ScopedName mapping (orig.scope (), "CCM_" + orig.simple ());

      // Check if mapping has already been provided.
      if (i->scope ()->table ().exist (mapping)) return;

      // Add to the list if it's not already there.
      declarations_.add (i);
    }

  private:
    Declarations& declarations_;
  };


  //
  //
  //
  class HomeExecutorCollector : public Traversal::HomeExecutor
  {
  public:
    HomeExecutorCollector (Declarations& declarations,
                           Traversal::Dispatcher* home_collector,
                           Traversal::Dispatcher* component_collector)
        : declarations_ (declarations),
          home_collector_ (home_collector),
          component_collector_ (component_collector)
    {
    }

    virtual void
    traverse (HomeExecutorPtr const& he)
    {
      HomeDefPtr home (he->implements ());
      home_collector_->dispatch (home);

      ComponentDefPtr component (home->manages ());
      component_collector_->dispatch (component);

      CompositionPtr composition (
        he->scope ()->dynamic_type<SyntaxTree::Composition> ());
      declarations_.add (composition, component);
    }

  private:
    Declarations& declarations_;

    Traversal::Dispatcher* home_collector_;
    Traversal::Dispatcher* component_collector_;
  };


  //
  //
  //
  class TypeNameEmitter : public Traversal::BuiltInTypeDef,
                          public Traversal::TypeDecl
  {
  public:
    TypeNameEmitter (std::ostream& os_)
        : os (os_)
    {
    }

    virtual void
    traverse (BuiltInTypeDefPtr const& t)
    {
      os << t->name ().simple ();
    }

    virtual void
    traverse (TypeDeclPtr const& t)
    {
      os << t->name ();
    }

  private:
    std::ostream& os;
  };


  //
  //
  //
  class ComponentEmitter : public Traversal::ComponentDef
  {
  public:
    ComponentEmitter (Declarations const& declarations)
        : declarations_ (declarations)
    {
    }

    virtual void
    traverse (ComponentDefPtr const& c)
    {
      if (declarations_.find (c))
      {
        Traversal::ComponentDef::traverse (c);
      }
    }

  private:
    Declarations const& declarations_;
  };


  // MonolithEmitter generates what spec calls 'Monolithic Component
  // Executor'.
  //
  class MonolithEmitter : public Traversal::ComponentDef,
                          public Traversal::AttributeDecl,
                          public Traversal::ProvidesDecl,
                          public Traversal::ConsumesDecl
  {
  public:
    MonolithEmitter (std::ostream& os_,
                     Traversal::Dispatcher* type_name_emitter)
        : AttributeDecl (type_name_emitter),
          os (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
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

      os << "{";
    }

    virtual void
    pre (AttributeDeclPtr const& a)
    {
      os << "attribute ";
    }

    virtual void
    post (AttributeDeclPtr const& a)
    {
      os << " " << a->name ().simple () << ";";
    }

    virtual void
    traverse (ProvidesDeclPtr const& p)
    {
      ScopedName n = p->type ()->name ();

      os << n.scope () << "::CCM_" << n.simple ()
         << " get_" << p->name ().simple () << " ();";
    }

    virtual void
    traverse (ConsumesDeclPtr const& p)
    {
      os << "void push_" << p->name ().simple () << " ("
         << "in " << p->type()->name () << " ev);";
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "};" << endl;
    }

  private:
    std::ostream& os;
  };

  // ContextEmitter generates component context interface.
  //
  //
  class ContextEmitter : public Traversal::ComponentDef,
                         public Traversal::UsesDecl,
                         public Traversal::PublishesDecl,
                         public Traversal::EmitsDecl
  {
  public:
    ContextEmitter (std::ostream& os_) : os (os_) {}

    virtual void
    pre (ComponentDefPtr const& c)
    {
      os << "local interface CCM_" << c->name ().simple () << "_Context : ";

      ComponentDefRef cr = c->inherits ();

      if (cr)
      {
        ScopedName name (cr.name ());
        os << name.scope () << "::CCM_" << name.simple () << "_Context";
      }
      else
      {
        //@@ (diego) Not clear to me which to use...  I use this one to
        // make examples work...
        //os << "::Components::CCMContext";
        os << "::Components::SessionContext";
      }

      os << "{";
    }

    virtual void
    traverse (UsesDeclPtr const& d)
    {
      os << d->type ()->name ()
         << " get_connection_" << d->name ().simple () << " ();";
    }

    virtual void
    traverse (PublishesDeclPtr const& d)
    {
      os << "void push_" << d->name ().simple () << " ("
         << "in " << d->type ()->name () << " ev);";
    }

    virtual void
    traverse (EmitsDeclPtr const& d)
    {
      os << "void push_" << d->name ().simple () << " ("
         << "in " << d->type ()->name () << " ev);";
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "};" << endl;
    }

  private:
    std::ostream& os;
  };


  //
  //
  //
  class HomeEmitter : public Traversal::HomeDef
  {
  public:
    HomeEmitter (Declarations const& declarations)
        : declarations_ (declarations)
    {
    }

    virtual void
    traverse (HomeDefPtr const& h)
    {
      if (declarations_.find (h))
      {
        Traversal::HomeDef::traverse (h);
      }
    }

  private:
    Declarations const& declarations_;
  };

  // HomeExplicitEmitter generates home explicit interface
  //
  //
  class HomeExplicitEmitter : public Traversal::OperationParameter,
                              public Traversal::Comma,
                              public Traversal::HomeFactoryDecl,
                              public Traversal::OperationDecl,
                              public Traversal::AttributeDecl,
                              public Traversal::HomeDef
  {
  public:
    HomeExplicitEmitter (std::ostream& os_,
                         Traversal::Dispatcher* type_name_emitter)
        : OperationParameter (type_name_emitter,
                              type_name_emitter,
                              type_name_emitter),

          OperationDecl (type_name_emitter),
          AttributeDecl (type_name_emitter),
          os (os_)
    {
    }

    virtual void
    pre (HomeDefPtr const& h)
    {
      os << "local interface CCM_" << h->name ().simple () << "Explicit : ";

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

      os << "{";

    }

    virtual void
    post (HomeDefPtr const& h)
    {
      os << "};" << endl;
    }

    //
    // OperationParameter
    //

    virtual void
    post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    traverse (CommaPtr const& s)
    {
      os << ", ";
    }

    //
    // HomeFactory
    //

    virtual void
    type (HomeFactoryDeclPtr const& d)
    {
      os << "::Components::EnterpriseComponent ";
    }

    virtual void
    name (HomeFactoryDeclPtr const& d)
    {
      os << d->name ().simple () << " (";
    }

    virtual void
    post (HomeFactoryDeclPtr const& d)
    {
      os << ");";
    }

    //
    // Operation
    //

    virtual void
    name (OperationDeclPtr const& d)
    {
      os << " " << d->name ().simple () << " (";
    }

    virtual void
    pre (OperationParameterPtr const& op)
    {
      os << op->direction () << " ";
    }

    virtual void
    post (OperationDeclPtr const& d)
    {
      os << ");";
    }

    //
    // Attribute
    //

    virtual void
    pre (AttributeDeclPtr const& a)
    {
      os << "attribute ";
    }

    virtual void
    post (AttributeDeclPtr const& a)
    {
      os << " " << a->name ().simple () << ";";
    }

  private:
    std::ostream& os;
  };


  // HomeImplicitEmitter generates home implicit interface
  //
  //
  class HomeImplicitEmitter : public Traversal::HomeDef
  {
  public:
    HomeImplicitEmitter (std::ostream& os_) : os (os_) {}

    virtual void
    pre (HomeDefPtr const& h)
    {
      os << "local interface " << "CCM_" << h->name ().simple () << "Implicit"
         << "{"
         << "::Components::EnterpriseComponent "
         << "create () raises (::Components::CCMException);";
    }

    virtual void
    post (HomeDefPtr const& h)
    {
      os << "};" << endl;
    }

  private:
    std::ostream& os;
  };


  // HomeMainEmitter generates home main interface
  //
  //
  class HomeMainEmitter : public Traversal::HomeDef
  {
  public:
    HomeMainEmitter (std::ostream& os_) : os (os_) {}

    virtual void
    traverse (HomeDefPtr const& h)
    {
      SimpleName name = h->name ().simple ();

      os << "local interface CCM_" << name << " : "
         << "CCM_" << name << "Explicit, "
         << "CCM_" << name << "Implicit"
         << "{"
         << "};" << endl;
    }

  private:
    std::ostream& os;
  };

  //
  //
  //
  class IncludeEmitter : public Traversal::PrincipalTranslationRegion,
                         public Traversal::UserIncludeTranslationRegion,
                         public Traversal::SysIncludeTranslationRegion,
                         public Traversal::ImpliedIncludeTranslationRegion
  {
  public:
    IncludeEmitter (std::ostream& os_) : os (os_) {}

    virtual void
    traverse (ImpliedIncludeTranslationRegionPtr const& r)
    {
      os << "#include <" << r->file_path ().string () << ">" << endl;
    }

    virtual void
    traverse (UserIncludeTranslationRegionPtr const& r)
    {
      os << "#include \"" << r->file_path ().string () << "\"" << endl;
    }

    virtual void
    traverse (SysIncludeTranslationRegionPtr const& r)
    {
      os << "#include <" << r->file_path ().string () << ">" << endl;
    }

  private:
    std::ostream& os;
  };

  //
  //
  //
  class ModuleEmitter : public Traversal::Module
  {
  public:
    ModuleEmitter (std::ostream& os_,
                   Declarations const& declarations)
        : os (os_),
          declarations_ (declarations)
    {
    }

  public:

    virtual void
    traverse (ModulePtr const& m)
    {
      if (declarations_.contains_suborder (m->order ()))
      {
        Traversal::Module::traverse (m);
      }
    }

    virtual void
    pre (ModulePtr const& m)
    {
      os << "module " << m->name ().simple () << "{";
    }

    virtual void
    post (ModulePtr const& m)
    {
      os << "};" << endl;
    }

  private:
    std::ostream& os;
    Declarations const& declarations_;
  };

  //
  //
  //
  class InterfaceEmitter : public Traversal::UnconstrainedInterfaceDecl
  {
  public:
    InterfaceEmitter (std::ostream& os_,
                      Declarations const& declarations)
        : os (os_),
          declarations_ (declarations)
    {
    }

  public:
    virtual void
    traverse (UnconstrainedInterfaceDeclPtr const& i)
    {
      if (declarations_.find (i))
      {
        os << "local interface CCM_" << i->name ().simple ()
           << " : " << i->name ().simple ()
           << "{};" << endl;
      }
    }

  private:
    std::ostream& os;
    Declarations const& declarations_;
  };

  //
  //
  //
  class CompositionEmitter : public Traversal::Composition
  {
  public:
    CompositionEmitter (std::ostream& os_,
                        Declarations const& declarations)
        : os (os_),
          declarations_ (declarations)
    {
    }

  public:

    virtual void
    traverse (CompositionPtr const& c)
    {
      if (declarations_.find (c))
      {
        Traversal::Composition::traverse (c);
      }
    }

    virtual void
    pre (CompositionPtr const& c)
    {
      ComponentDefPtr component (declarations_.resolve (c));
      ScopedName name (component->name ());

      SyntaxTree::Composition::Category::Value category = c->category ();

      os << "module " << c->name ().simple ()
         << "{";

      os << "local interface " << name.simple () << "Context : "
         << name.scope () << "::CCM_" << name.simple () << "_Context, ";

      switch (category)
      {
      case SyntaxTree::Composition::Category::ENTITY:
        {
          os << "::Components::EntityContext";
          break;
        }
      default:
        {
          os << "::Components::SessionContext";
          break;
        }
      }

      os << "{};" << endl;
    }

    virtual void
    post (CompositionPtr const& c)
    {
      os << "};" << endl;
    }

  private:
    std::ostream& os;
    Declarations const& declarations_;
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
      "/(\\.(idl|cidl))?$/" + suffix + "/");

    string lem_file_name = regex::perl_s (file_name, expr);

    fs::path lem_file_path (lem_file_name);

    ofs.open (lem_file_path);

    if (!ofs.is_open ())
    {
      cerr << lem_file_name << ": error: unable to open in write mode"
           << endl;
      return;
    }
  }

  std::ostream& os = ofs.is_open () ? ofs : std::cout;

  // Set auto-indentation for os
  Indentation::Implanter<Indentation::IDL> guard (os);

  Declarations declarations;

  if (cl.get_value ("lem-force-all", false))
  {
    InterfaceCollector interface (declarations);
    Traversal::ProvidesDecl provides (&interface);

    ComponentCollector component (declarations);
    component.add_scope_delegate (&provides);

    HomeCollector home (declarations);

    // Note the trick. interface is of type InterfaceDecl but I only
    // want to traverse InterfaceDef's. So I use original InterfaceDef but
    // delegate to InterfaceDecl.
    //
    Traversal::UnconstrainedInterfaceDef interface_def;
    interface_def.add_delegate (&interface);

    Traversal::Module module;
    module.add_scope_delegate (&home);
    module.add_scope_delegate (&component);
    module.add_scope_delegate (&interface_def);

    Traversal::FileScope file_scope;
    file_scope.add_scope_delegate (&module);
    file_scope.add_scope_delegate (&home);
    file_scope.add_scope_delegate (&component);
    file_scope.add_scope_delegate (&interface_def);

    Traversal::PrincipalTranslationRegion region (&file_scope);

    Traversal::TranslationUnit unit;
    unit.add_content_delegate (&region);

    unit.dispatch (u);
  }
  else
  {
    InterfaceCollector interface (declarations);
    Traversal::ProvidesDecl provides (&interface);

    ComponentCollector component (declarations);
    component.add_scope_delegate (&provides);

    HomeCollector home (declarations);

    HomeExecutorCollector home_executor (declarations, &home, &component);

    Traversal::Composition composition;
    composition.add_scope_delegate (&home_executor);

    Traversal::Scope scope;
    scope.add_scope_delegate (&composition);
    scope.add_scope_delegate (&scope);

    Traversal::Module module;
    module.add_scope_delegate (&composition);

    Traversal::FileScope file_scope;
    file_scope.add_scope_delegate (&module);
    file_scope.add_scope_delegate (&composition);

    Traversal::PrincipalTranslationRegion region (&file_scope);

    Traversal::TranslationUnit unit;
    unit.add_content_delegate (&region);

    unit.dispatch (u);
  }

  {
    TypeNameEmitter type_name (os);

    MonolithEmitter monolith (os, &type_name);
    ContextEmitter context (os);

    ComponentEmitter component (declarations);
    component.add_delegate (&monolith);
    component.add_delegate (&context);

    HomeImplicitEmitter home_implicit (os);
    HomeExplicitEmitter home_explicit (os, &type_name);
    HomeMainEmitter home_main (os);

    HomeEmitter home (declarations);
    home.add_delegate (&home_implicit);
    home.add_delegate (&home_explicit);
    home.add_delegate (&home_main);

    InterfaceEmitter interface (os, declarations);
    CompositionEmitter composition (os, declarations);

    ModuleEmitter module (os, declarations);

    module.add_scope_delegate (&interface);
    module.add_scope_delegate (&component);
    module.add_scope_delegate (&home);
    module.add_scope_delegate (&composition);
    module.add_scope_delegate (&module);

    Traversal::FileScope file_scope;
    file_scope.add_scope_delegate (&module);
    file_scope.add_scope_delegate (&interface);
    file_scope.add_scope_delegate (&component);
    file_scope.add_scope_delegate (&home);
    file_scope.add_scope_delegate (&composition);


    Traversal::TranslationRegion region (&file_scope);

    IncludeEmitter include (os);

    Traversal::TranslationUnit unit;
    unit.add_content_delegate (&include);
    unit.add_content_delegate (&region);

    unit.dispatch (u);
  }
}
