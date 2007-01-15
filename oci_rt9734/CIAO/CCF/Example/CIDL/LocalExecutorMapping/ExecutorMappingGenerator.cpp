// file      : CCF/Example/CIDL/LocalExecutorMapping/ExecutorMappingGenerator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "ExecutorMappingGenerator.hpp"

#include <set>
#include <ostream>
#include <fstream>

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IndentationIDL.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

using std::string;
using std::ostream;
using std::endl;

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

namespace
{
  class Context
  {
  public:
    Context (TranslationUnit& tu)
        : tu_ (tu)
    {
    }

    TranslationUnit&
    tu () const
    {
      return tu_;
    }

  public:
    bool
    add (Home& h)
    {
      return homes_.insert (&h).second;
    }

    bool
    add (Component& c)
    {
      return components_.insert (&c).second;
    }

    bool
    add (UnconstrainedInterface& i)
    {
      return interfaces_.insert (&i).second;
    }

    bool
    add (Composition& c)
    {
      return compositions_.insert (&c).second;
    }

  public:
    bool
    find (Home& h) const
    {
      return homes_.find (&h) != homes_.end ();
    }

    bool
    find (Component& c) const
    {
      return components_.find (&c) != components_.end ();
    }

    bool
    find (UnconstrainedInterface& i) const
    {
      return interfaces_.find (&i) != interfaces_.end ();
    }

    bool
    find (Composition& c) const
    {
      return compositions_.find (&c) != compositions_.end ();
    }

  private:
    typedef std::set<Home*> Homes;
    typedef std::set<Component*> Components;
    typedef std::set<UnconstrainedInterface*> Interfaces;
    typedef std::set<Composition*> Compositions;

    TranslationUnit& tu_;

    Homes homes_;
    Components components_;
    Interfaces interfaces_;
    Compositions compositions_;
  };

  class Traverser
  {
  protected:
    Traverser (Context& c)
        : ctx (c)
    {
    }

    Context& ctx;
  };

  struct Collector : Traverser
  {
  protected:
    Collector (Context& c)
        : Traverser (c)
    {
    }

    bool
    exist (ScopedName const& name)
    {
      return !ctx.tu ().lookup (name).empty ();
    }
  };

  //
  //
  //
  struct ComponentCollector : Traversal::Component, Collector
  {
    ComponentCollector (Context& c)
        : Collector (c)
    {
    }

    virtual void
    traverse (Type& c)
    {
      SimpleName name (c.name ());
      ScopedName scope (c.scoped_name ().scope_name ());

      ScopedName monolith (scope, "CCM_" + name);
      ScopedName context (scope, "CCM_" + name + "_Context");

      // Check if mapping has already been provided.
      //
      if (exist (context) || exist (monolith)) return;

      if(ctx.add (c))
      {
        // Collect inherited components and provides interfaces.
        //
        Traversal::Component::traverse (c);
      }
    }
  };


  //
  //
  //
  struct HomeCollector : Traversal::Home, Collector
  {
    HomeCollector (Context& c)
        : Collector (c)
    {
    }

    virtual void
    traverse (Type& h)
    {
      SimpleName name (h.name ());
      ScopedName scope (h.scoped_name ().scope_name ());

      ScopedName main (scope, "CCM_" + name);
      ScopedName expl (scope, "CCM_" + name + "Explicit");
      ScopedName impl (scope, "CCM_" + name + "Implicit");

      // Check if mapping has already been provided.
      //
      if (exist (main) || exist (expl) || exist (impl)) return;

      if(ctx.add (h))
      {
        // Note that I don't go after components that inherited home manages
        // because it will be handled by component inheritance tree.
        //
        Traversal::Home::traverse (h);
      }
    }
  };


  //
  //
  //
  struct InterfaceCollector : Traversal::UnconstrainedInterface, Collector
  {
    InterfaceCollector (Context& c)
        : Collector (c)
    {
    }

    virtual void
    traverse (Type& i)
    {
      SimpleName name (i.name ());
      ScopedName scope (i.scoped_name ().scope_name ());

      ScopedName mapping (scope, "CCM_" + name);

      // Check if mapping has already been provided.
      //
      if (exist (mapping)) return;

      // Add to the list if it's not already there.
      //
      ctx.add (i);
    }
  };

  struct CompositionCollector : Traversal::Composition, Collector
  {
    CompositionCollector (Context& c)
        : Collector (c)
    {
    }

    virtual void
    traverse (Type& c)
    {
      // Add to the list if it's not already there.
      //
      if (ctx.add (c))
      {
        Traversal::Composition::traverse (c);
      }
    }
  };

  struct Emitter : Traverser
  {
  protected:
    Emitter (Context& c, ostream& os_)
        : Traverser (c), os (os_)
    {
    }

  protected:
    ostream& os;
  };


  //
  //
  //
  struct TypeNameEmitter : Traversal::FundamentalType,
                           Traversal::Type,
                           Emitter
  {
    TypeNameEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    traverse (SemanticGraph::FundamentalType& t)
    {
      os << t.name ();
    }

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << t.scoped_name ();
    }
  };


  struct NameMangler : Traversal::Nameable, Emitter
  {
    NameMangler (Context& c,
                 ostream& os,
                 string const& prefix,
                 string const& suffix = "")
        : Emitter (c, os), prefix_ (prefix), suffix_ (suffix)
    {
    }

    virtual void
    traverse (Type& t)
    {
      ScopedName n (t.scoped_name ());
      os << n.scope_name () << "::" << prefix_ << n.simple_name () << suffix_;
    }

  private:
    string prefix_, suffix_;
  };


  //
  //
  //
  struct ComponentEmitter : Traversal::Component, Emitter
  {
  protected:
    ComponentEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    traverse (Type& c)
    {
      if (ctx.find (c))
      {
        Component::traverse (c);
      }
    }
  };

  struct AttributeEmitter : Traversal::ReadAttribute,
                            Traversal::ReadWriteAttribute,
                            Emitter
  {
    AttributeEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    // ReadAttribute
    //
    virtual void
    pre (SemanticGraph::ReadAttribute& )
    {
      os << "readonly attribute ";
    }

    virtual void
    name (SemanticGraph::ReadAttribute& a)
    {
      os << " " << a.name ();
    }

    virtual void
    post (SemanticGraph::ReadAttribute&)
    {
      os << ";";
    }

    // ReadWriteAttribute
    //
    virtual void
    pre (SemanticGraph::ReadWriteAttribute& )
    {
      os << "attribute ";
    }

    virtual void
    name (SemanticGraph::ReadWriteAttribute& a)
    {
      os << " " << a.name ();
    }

    virtual void
    post (SemanticGraph::ReadWriteAttribute&)
    {
      os << ";";
    }
  };


  // MonolithEmitter generates what spec calls 'Monolithic Component
  // Executor'.
  //
  struct MonolithEmitter : ComponentEmitter
  {
    MonolithEmitter (Context& c, ostream& os)
        : ComponentEmitter (c, os),
          monolith_name_emitter (c, os, "CCM_"),
          attribute (c, os),
          consumer (c, os),
          provider (c, os),
          type_name_emitter (c, os)
    {
      edge_traverser (inherits);
      edge_traverser (defines);

      inherits.node_traverser (monolith_name_emitter);

      defines.node_traverser (attribute);
      defines.node_traverser (consumer);
      defines.node_traverser (provider);

      attribute.edge_traverser (belongs);
      consumer.edge_traverser (belongs);
      provider.edge_traverser (provider_belongs);

      belongs.node_traverser (type_name_emitter);
      provider_belongs.node_traverser (monolith_name_emitter);
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& c)
    {
      os << "CCM_" << c.name ();
    }

    virtual void
    inherits_pre (Type&)
    {
      os << " : ";
    }

    virtual void
    inherits_none (Type&)
    {
      os << " : ::Components::EnterpriseComponent";
    }

    virtual void
    supports_pre (Type&)
    {
      os << ", ";
    }

    virtual void
    names_pre (Type&)
    {
      os << "{";
    }

    virtual void
    names_post (Type&)
    {
      os << "}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }

    virtual void
    comma (Type&)
    {
      os << ", ";
    }

  private:
    struct Consumer : Traversal::ConsumerSet, Emitter
    {
      Consumer (Context& c, ostream& os)
          : Emitter (c, os)
      {
      }

      virtual void
      returns (Type&)
      {
        os << "void";
      }

      virtual void
      name (Type& c)
      {
        os << " push_" << c.name ();
      }

      virtual void
      receives_pre (Type&)
      {
        os << " (in ";
      }

      virtual void
      receives_post (Type&)
      {
        os << " e)";
      }

      virtual void
      post (Type&)
      {
        os << ";";
      }
    };


    struct Provider : Traversal::ProviderGet, Emitter
    {
      Provider (Context& c, ostream& os)
          : Emitter (c, os)
      {
      }

      virtual void
      name (Type& c)
      {
        os << " get_" << c.name ();
      }

      virtual void
      receives_pre (Type&)
      {
        os << " (";
      }

      virtual void
      receives_post (Type&)
      {
        os << ")";
      }

      virtual void
      post (Type&)
      {
        os << ";";
      }
    };

    Traversal::Inherits inherits;
    Traversal::Defines defines;

    NameMangler monolith_name_emitter;

    AttributeEmitter attribute;
    Consumer consumer;
    Provider provider;

    Traversal::Belongs belongs;
    Traversal::Belongs provider_belongs;

    TypeNameEmitter type_name_emitter;
  };


  // ContextEmitter generates component context interface.
  //
  //
  struct ContextPortEmitter : Traversal::UserGet,
                              Traversal::PublisherSet,
                              Traversal::EmitterSet,
                              Emitter
  {
    ContextPortEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }


    // User.
    //
    virtual void
    name (SemanticGraph::User& u)
    {
      os << " get_connection_" << u.name ();
    }

    virtual void
    receives_pre (SemanticGraph::User&)
    {
      os << " (";
    }

    virtual void
    receives_post (SemanticGraph::User&)
    {
      os << ")";
    }

    virtual void
    post (SemanticGraph::User&)
    {
      os << ";";
    }


    // Publisher.
    //
    virtual void
    returns (SemanticGraph::Publisher&)
    {
      os << "void";
    }

    virtual void
    name (SemanticGraph::Publisher& p)
    {
      os << " push_" << p.name ();
    }

    virtual void
    receives_pre (SemanticGraph::Publisher&)
    {
      os << " (in ";
    }

    virtual void
    receives_post (SemanticGraph::Publisher&)
    {
      os << " e)";
    }

    virtual void
    post (SemanticGraph::Publisher&)
    {
      os << ";";
    }


    // Emitter.
    //
    virtual void
    returns (SemanticGraph::Emitter&)
    {
      os << "void";
    }

    virtual void
    name (SemanticGraph::Emitter& e)
    {
      os << " push_" << e.name ();
    }

    virtual void
    receives_pre (SemanticGraph::Emitter&)
    {
      os << " (in ";
    }

    virtual void
    receives_post (SemanticGraph::Emitter&)
    {
      os << " e)";
    }

    virtual void
    post (SemanticGraph::Emitter&)
    {
      os << ";";
    }
  };


  struct ContextEmitter : ComponentEmitter
  {
    ContextEmitter (Context& c, ostream& os)
        : ComponentEmitter (c, os), name_emitter (c, os, "CCM_", "_Context")
    {
      edge_traverser (inherits);
      inherits.node_traverser (name_emitter);
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& c)
    {
      os << "CCM_" << c.name () << "_Context";
    }

    virtual void
    inherits_pre (Type&)
    {
      os << " : ";
    }

    virtual void
    inherits_none (Type&)
    {
      os << " : ::Components::CCMContext";
    }

    virtual void
    names_pre (Type&)
    {
      os << "{";
    }

    virtual void
    names_post (Type&)
    {
      os << "}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }

  private:
    Traversal::Inherits inherits;
    NameMangler name_emitter;
  };


  //
  //
  //
  struct HomeEmitter : Traversal::Home, Emitter
  {
    HomeEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    traverse (Type& h)
    {
      if (ctx.find (h))
      {
        Home::traverse (h);
      }
    }
  };


  // HomeExplicitEmitter generates home explicit interface
  //
  //
  struct ExplicitPortEmitter : AttributeEmitter,
                               Traversal::Operation,
                               Traversal::HomeFactory,
                               Traversal::HomeFinder
  {
    ExplicitPortEmitter (Context& c, ostream& os)
        : AttributeEmitter (c, os)
    {
    }

    // Operation.
    //

    virtual void
    name (SemanticGraph::Operation& o)
    {
      os << " " << o.name ();
    }

    virtual void
    receives_pre (SemanticGraph::Operation&)
    {
      os << " (";
    }

    virtual void
    receives_post (SemanticGraph::Operation&)
    {
      os << ")";
    }

    virtual void
    raises_pre (SemanticGraph::Operation&)
    {
      os << " raises (";
    }

    virtual void
    raises_post (SemanticGraph::Operation&)
    {
      os << ")";
    }

    virtual void
    post (SemanticGraph::Operation&)
    {
      os << ";";
    }

    virtual void
    comma (SemanticGraph::Operation&)
    {
      os << ", ";
    }


    // HomeFactory.
    //

    virtual void
    returns (SemanticGraph::HomeFactory&)
    {
      os << "::Components::EnterpriseComponent ";
    }

    virtual void
    name (SemanticGraph::HomeFactory& hf)
    {
      os << " " << hf.name ();
    }

    virtual void
    receives_pre (SemanticGraph::HomeFactory&)
    {
      os << " (";
    }

    virtual void
    receives_post (SemanticGraph::HomeFactory&)
    {
      os << ")";
    }

    virtual void
    raises_pre (SemanticGraph::HomeFactory&)
    {
      os << " raises (";
    }

    virtual void
    raises_post (SemanticGraph::HomeFactory&)
    {
      os << ")";
    }

    virtual void
    post (SemanticGraph::HomeFactory&)
    {
      os << ";";
    }

    virtual void
    comma (SemanticGraph::HomeFactory&)
    {
      os << ", ";
    }

    // HomeFinder.
    //

    virtual void
    returns (SemanticGraph::HomeFinder&)
    {
      os << "::Components::EnterpriseComponent ";
    }

    virtual void
    name (SemanticGraph::HomeFinder& hf)
    {
      os << " " << hf.name ();
    }

    virtual void
    receives_pre (SemanticGraph::HomeFinder&)
    {
      os << " (";
    }

    virtual void
    receives_post (SemanticGraph::HomeFinder&)
    {
      os << ")";
    }

    virtual void
    raises_pre (SemanticGraph::HomeFinder&)
    {
      os << " raises (";
    }

    virtual void
    raises_post (SemanticGraph::HomeFinder&)
    {
      os << ")";
    }

    virtual void
    post (SemanticGraph::HomeFinder&)
    {
      os << ";";
    }

    virtual void
    comma (SemanticGraph::HomeFinder&)
    {
      os << ", ";
    }
  };

  struct ParameterEmitter : Traversal::InParameter,
                            Traversal::OutParameter,
                            Traversal::InOutParameter,
                            public Emitter
  {
    ParameterEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    pre (InParameter& p)
    {
      os << "in ";
    }

    virtual void
    pre (OutParameter& p)
    {
      os << "out ";
    }

    virtual void
    pre (InOutParameter& p)
    {
      os << "inout ";
    }

    virtual void
    name (InParameter& p)
    {
      os << " " << p.name ();
    }

    virtual void
    name (OutParameter& p)
    {
      os << " " << p.name ();
    }

    virtual void
    name (InOutParameter& p)
    {
      os << " " << p.name ();
    }
  };

  struct HomeExplicitEmitter : HomeEmitter
  {
    HomeExplicitEmitter (Context& c, ostream& os)
        : HomeEmitter (c, os), name_emitter (c, os, "CCM_", "Explicit")
    {
      edge_traverser (inherits);
      inherits.node_traverser (name_emitter);
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& h)
    {
      os << "CCM_" << h.name () << "Explicit";
    }

    virtual void
    inherits_pre (Type&)
    {
      os << " : ";
    }

    virtual void
    inherits_none (Type&)
    {
      os << " : ::Components::HomeExecutorBase";
    }

    virtual void
    supports_pre (Type&)
    {
      os << ", ";
    }

    virtual void
    names_pre (Type&)
    {
      os << "{";
    }

    virtual void
    names_post (Type&)
    {
      os << "}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }

    virtual void
    comma (Type&)
    {
      os << ", ";
    }

  private:
    Traversal::Inherits inherits;
    NameMangler name_emitter;
  };


  // HomeImplicitEmitter generates home implicit interface
  //
  //
  struct HomeImplicitEmitter : HomeEmitter
  {
    HomeImplicitEmitter (Context& c, ostream& os)
        : HomeEmitter (c, os)
    {
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& h)
    {
      os << "CCM_" << h.name () << "Implicit";
    }

    virtual void
    names (Type& h)
    {
      os<< "{"
        << "::Components::EnterpriseComponent "
        << "create () raises (::Components::CCMException);"
        << "}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }
  };


  // HomeMainEmitter generates home main interface
  //
  //
  struct HomeMainEmitter : HomeEmitter
  {
    HomeMainEmitter (Context& c, ostream& os)
        : HomeEmitter (c, os)
    {
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& h)
    {
      os << "CCM_" << h.name ();
    }

    virtual void
    inherits (Type& h)
    {
      os << " : "
         << "CCM_" << h.name () << "Explicit, "
         << "CCM_" << h.name () << "Implicit";
    }

    virtual void
    names (Type&)
    {
      os << "{}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }
  };

  //
  //
  //
  struct ModuleEmitter : Traversal::Module, Emitter
  {
    ModuleEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    traverse (Type& m)
    {
      if (has_elements (m))
      {
        Traversal::Module::traverse (m);
      }
    }

    virtual void
    pre (Type&)
    {
      os << "module ";
    }

    virtual void
    name (Type& m)
    {
      os << m.name ();
    }

    virtual void
    names_pre (Type&)
    {
      os << "{";
    }

    virtual void
    names_post (Type&)
    {
      os << "}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }

  private:

    template <typename T>
    struct Finder : T, Traverser
    {
      Finder (Context& c, bool& r)
          : Traverser (c), r_ (r)
      {
      }

      virtual void
      traverse (typename T::Type& t)
      {
        if (ctx.find (t)) r_ = true;
      }

    private:
      bool& r_;
    };

    bool
    has_elements (Type& m)
    {
      bool r (false);

      Traversal::Module module;
      Traversal::Defines defines;

      module.edge_traverser (defines);

      //@@ MSVC bug: interface is considered to be an alias for a struct.
      //
      Finder<Traversal::Composition> composition (ctx, r);
      Finder<Traversal::UnconstrainedInterface> interface_ (ctx, r);
      Finder<Traversal::Component> component (ctx, r);
      Finder<Traversal::Home> home (ctx, r);

      defines.node_traverser (module);
      defines.node_traverser (composition);
      defines.node_traverser (interface_);
      defines.node_traverser (component);
      defines.node_traverser (home);

      module.traverse (m);

      return r;
    }
  };

  //
  //
  //
  struct InterfaceEmitter : Traversal::UnconstrainedInterface, Emitter
  {
    InterfaceEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    traverse (Type& i)
    {
      if (ctx.find (i))
      {
        Traversal::UnconstrainedInterface::traverse (i);
      }
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& i)
    {
      os << "CCM_" << i.name ();
    }

    virtual void
    inherits (Type& i)
    {
      os << " : " << i.name ();
    }

    virtual void
    names_pre (Type&)
    {
      os << "{";
    }

    virtual void
    names_post (Type&)
    {
      os << "}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }
  };


  //
  //
  //
  struct CompositionEmitter : Traversal::Composition, Emitter
  {
    CompositionEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    traverse (Type& c)
    {
      if (ctx.find (c))
      {
        Traversal::Composition::traverse (c);
      }
    }

    virtual void
    pre (Type&)
    {
      os << "module ";
    }

    virtual void
    name (Type& m)
    {
      os << m.name ();
    }

    virtual void
    names_pre (Type&)
    {
      os << "{";
    }

    virtual void
    names_post (Type&)
    {
      os << "}";
    }

    virtual void
    post (Type&)
    {
      os << ";";
    }
  };


  struct ComponentContextEmitter : Traversal::ComponentExecutor, Emitter
  {
    ComponentContextEmitter (Context& c, ostream& os)
        : Emitter (c, os),
          name_emitter_ (c, os, "CCM_", "_Context")
    {
      implements_traverser_.node_traverser (name_emitter_);
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& i)
    {
      //@@ need to check if spec prescribes this name.
      //
      os << i.name () << "_Context";
    }

    virtual void
    implements (Type& i)
    {
      os << " : ";

      Traversal::ComponentExecutor::implements (i, implements_traverser_);

      os << ", "
         << "::Components::SessionContext";
    }

    virtual void
    post (Type&)
    {
      os << "{};";
    }

  private:
    NameMangler name_emitter_;
    Traversal::Implements implements_traverser_;
  };


  struct ComponentExecutorEmitter : Traversal::ComponentExecutor, Emitter
  {
    ComponentExecutorEmitter (Context& c, ostream& os)
        : Emitter (c, os),
          name_emitter_ (c, os, "CCM_")
    {
      implements_traverser_.node_traverser (name_emitter_);
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& i)
    {
      os << i.name ();
    }

    virtual void
    implements (Type& i)
    {
      os << " : ";

      Traversal::ComponentExecutor::implements (i, implements_traverser_);

      os << ", "
         << "::Components::SessionComponent";
    }

    virtual void
    post (Type&)
    {
      os << "{};";
    }

  private:
    NameMangler name_emitter_;
    Traversal::Implements implements_traverser_;
  };


  struct HomeExecutorEmitter : Traversal::HomeExecutor, Emitter
  {
    HomeExecutorEmitter (Context& c, ostream& os)
        : Emitter (c, os),
          name_emitter_ (c, os, "CCM_")
    {
      implements_traverser_.node_traverser (name_emitter_);
    }

    virtual void
    pre (Type&)
    {
      os << "local interface ";
    }

    virtual void
    name (Type& i)
    {
      os << i.name ();
    }

    virtual void
    implements (Type& i)
    {
      os << " : ";

      Traversal::HomeExecutor::implements (i, implements_traverser_);
    }

    virtual void
    post (Type&)
    {
      os << "{};";
    }

  private:
    NameMangler name_emitter_;
    Traversal::Implements implements_traverser_;
  };

  //
  //
  //
  struct IncludesEmitter : Traversal::QuoteIncludes,
                           Traversal::BracketIncludes,
                           Emitter
  {
    IncludesEmitter (Context& c, ostream& os)
        : Emitter (c, os)
    {
    }

    virtual void
    traverse (SemanticGraph::QuoteIncludes& qi)
    {
      os << "#include \"" << qi.file ().string () << "\"" << endl;
    }

    virtual void
    traverse (SemanticGraph::BracketIncludes& bi)
    {
      os << "#include <" << bi.file ().string () << ">" << endl;
    }
  };
}

void ExecutorMappingGenerator::
options (CL::Description& d)
{
  d.add_option (CL::OptionDescription (
                  "lem-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'E\' "
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
          TranslationUnit& tu,
          fs::path const& file_path)
{
  fs::ofstream ofs;

  if (!file_path.empty ())
  {
    string file_name (file_path.leaf ());

    string suffix (cl.get_value ("lem-file-suffix", "E.idl"));

    string expr (cl.get_value (
                   "lem-file-regex", "/^(.+?)(\\.(idl|cidl))?$/$1" + suffix + "/"));

    string lem_file_name (regex::perl_s (file_name, expr));

    fs::path lem_file_path (lem_file_name);

    ofs.open (lem_file_path, std::ios_base::out);

    if (!ofs.is_open ())
    {
      cerr << lem_file_name << ": error: unable to open in write mode"
           << endl;
      return;
    }
  }

  ostream& os = ofs.is_open ()
    ? static_cast<ostream&> (ofs)
    : static_cast<ostream&> (std::cout);

  // Set auto-indentation for os.
  //
  Indentation::Implanter<Indentation::IDL> guard (os);

  Context ctx (tu);

  if (cl.get_value ("lem-force-all", false))
  {
    Traversal::TranslationUnit unit;

    // Layer 1
    //
    Traversal::ContainsPrincipal contains_principal;

    unit.edge_traverser (contains_principal);

    //--
    Traversal::TranslationRegion region;

    contains_principal.node_traverser (region);


    // Layer 2
    //
    Traversal::ContainsRoot contains_root;
    region.edge_traverser (contains_root);

    //--
    Traversal::Root root;
    contains_root.node_traverser (root);


    // Layer 3
    //
    Traversal::Defines defines;
    root.edge_traverser (defines);

    //--
    Traversal::Module module;
    HomeCollector home (ctx);
    ComponentCollector component (ctx);
    InterfaceCollector interface_ (ctx);

    defines.node_traverser (module);
    defines.node_traverser (home);
    defines.node_traverser (component);
    defines.node_traverser (interface_);

    // Layer 4
    //
    Traversal::Defines component_defines;
    Traversal::Inherits component_inherits;
    Traversal::Inherits home_inherits;

    module.edge_traverser (defines);

    home.edge_traverser (home_inherits);
    component.edge_traverser (component_defines);
    component.edge_traverser (component_inherits);

    //--

    Traversal::Provider provider;

    component_defines.node_traverser (provider);
    component_inherits.node_traverser (component);
    home_inherits.node_traverser (home);


    // Layer 5
    //
    Traversal::Belongs provider_belongs;
    provider.edge_traverser (provider_belongs);

    //
    provider_belongs.node_traverser (interface_);

    // end

    unit.traverse (tu);
  }
  else
  {
    Traversal::TranslationUnit unit;

    // Layer 1
    //
    Traversal::ContainsPrincipal contains_principal;

    unit.edge_traverser (contains_principal);

    //--
    Traversal::TranslationRegion region;

    contains_principal.node_traverser (region);


    // Layer 2
    //
    Traversal::ContainsRoot contains_root;
    Traversal::Includes includes;

    region.edge_traverser (contains_root);
    region.edge_traverser (includes);

    //--
    Traversal::Root root;

    contains_root.node_traverser (root);
    includes.node_traverser (region);


    // Layer 3
    //
    Traversal::Defines defines;
    root.edge_traverser (defines);

    //--
    Traversal::Module module;
    CompositionCollector composition (ctx);

    defines.node_traverser (module);
    defines.node_traverser (composition);


    // Layer 4
    //
    Traversal::Defines composition_defines;

    module.edge_traverser (defines);
    composition.edge_traverser (composition_defines);

    //--
    Traversal::ComponentExecutor component_executor;
    Traversal::HomeExecutor home_executor;

    composition_defines.node_traverser (component_executor);
    composition_defines.node_traverser (home_executor);

    // Layer 5
    //
    Traversal::Implements component_executor_implements;
    Traversal::Implements home_executor_implements;

    component_executor.edge_traverser (component_executor_implements);
    home_executor.edge_traverser (home_executor_implements);

    //--
    ComponentCollector component (ctx);
    HomeCollector home (ctx);

    component_executor_implements.node_traverser (component);
    home_executor_implements.node_traverser (home);


    // Layer 6
    //
    Traversal::Defines component_defines;
    Traversal::Inherits component_inherits;
    Traversal::Inherits home_inherits;

    component.edge_traverser (component_defines);
    component.edge_traverser (component_inherits);
    home.edge_traverser (home_inherits);

    //--

    Traversal::Provider provider;

    component_defines.node_traverser (provider);
    component_inherits.node_traverser (component);
    home_inherits.node_traverser (home);


    // Layer 7
    //
    Traversal::Belongs provider_belongs;
    provider.edge_traverser (provider_belongs);

    //
    InterfaceCollector interface_ (ctx);

    provider_belongs.node_traverser (interface_);


    // end

    unit.traverse (tu);
  }

  {
    Traversal::TranslationUnit unit;

    // Layer 1
    //
    Traversal::ContainsPrincipal contains_principal;

    unit.edge_traverser (contains_principal);

    //--
    Traversal::TranslationRegion principal_region;

    contains_principal.node_traverser (principal_region);


    // Layer 2
    //
    Traversal::TranslationRegion included_region;

    // Inclusion handling is somewhat tricky because we want
    // to print only top-level #include's.
    //

    Traversal::ContainsRoot contains_root;
    Traversal::QuoteIncludes quote_includes;
    Traversal::BracketIncludes bracket_includes;
    IncludesEmitter includes_emitter (ctx, os);


    principal_region.edge_traverser (includes_emitter);
    principal_region.edge_traverser (quote_includes);
    principal_region.edge_traverser (bracket_includes);
    principal_region.edge_traverser (contains_root);

    included_region.edge_traverser (quote_includes);
    included_region.edge_traverser (bracket_includes);
    included_region.edge_traverser (contains_root);


    //--
    Traversal::Root root;

    contains_root.node_traverser (root);
    quote_includes.node_traverser (included_region);
    bracket_includes.node_traverser (included_region);


    // Layer 3
    //
    Traversal::Defines defines;
    root.edge_traverser (defines);

    //--
    ModuleEmitter module (ctx, os);

    CompositionEmitter composition (ctx, os);

    InterfaceEmitter interface_ (ctx, os);

    MonolithEmitter component_monolith (ctx, os);
    ContextEmitter component_context (ctx, os);

    HomeImplicitEmitter home_implicit (ctx, os);
    HomeExplicitEmitter home_explicit (ctx, os);
    HomeMainEmitter home_main (ctx, os);

    defines.node_traverser (module);

    defines.node_traverser (composition);

    defines.node_traverser (interface_);

    defines.node_traverser (component_monolith);
    defines.node_traverser (component_context);

    defines.node_traverser (home_implicit);
    defines.node_traverser (home_explicit);
    defines.node_traverser (home_main);

    // Layer 4
    //

    Traversal::Supports supports;

    Traversal::Defines composition_defines;

    Traversal::Defines component_context_defines;

    Traversal::Defines home_explicit_defines;

    module.edge_traverser (defines);

    composition.edge_traverser (composition_defines);

    component_monolith.edge_traverser (supports);
    component_context.edge_traverser (component_context_defines);

    home_explicit.edge_traverser (supports);
    home_explicit.edge_traverser (home_explicit_defines);

    //--
    TypeNameEmitter type (ctx, os);

    ComponentContextEmitter session_component_context (ctx, os);
    ComponentExecutorEmitter session_component_executor (ctx, os);
    HomeExecutorEmitter session_home_executor (ctx, os);

    ContextPortEmitter port_context (ctx, os);
    ExplicitPortEmitter port_explicit (ctx, os);

    supports.node_traverser (type);

    composition_defines.node_traverser (session_component_context);
    composition_defines.node_traverser (session_component_executor);
    composition_defines.node_traverser (session_home_executor);

    component_context_defines.node_traverser (port_context);

    home_explicit_defines.node_traverser (port_explicit);


    // Layer 5
    //
    Traversal::Belongs belongs;
    Traversal::Receives receives;
    Traversal::Raises raises;

    port_context.edge_traverser (belongs);
    port_explicit.edge_traverser (belongs);
    port_explicit.edge_traverser (raises);

    port_explicit.edge_traverser (receives);

    //--
    ParameterEmitter parameter (ctx, os);

    belongs.node_traverser (type);
    receives.node_traverser (parameter);
    raises.node_traverser (type);

    // Layer 6
    //
    parameter.edge_traverser (belongs);


    // end

    unit.traverse (tu);
  }
}
