// $Id$

#include "DescriptorGenerator.hpp"
#include "Literals.hpp"

#include <ostream>

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IndentationXML.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

using std::cout;
using std::endl;
using std::string;

using namespace StringLiterals;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

namespace
{
  string
  name_to_string (Name const& n)
  {
    std::ostringstream os;
    os << n;
    return os.str ();
  }

  Nameable* defined_in (Nameable& n)
  {
    for (Nameable::NamedIterator i (n.named_begin ()), e (n.named_end ());
        i != e;
        ++i)
    {
      if (Defines* d = dynamic_cast<Defines*> (*i))
      {
        return &d->scope ();
      }
    }

    return 0;
  }

  string
  compute_repo_id (Nameable& d)
  {
    if (d.context ().count (STRS[REPO_ID]))
    {
      return d.context ().get<string> (STRS[REPO_ID]);
    }

    string prefix ("");
    TypePrefix *tp = 0;

    if (d.context ().count (STRS[TYPE_PREFIX]))
    {
      tp = d.context ().get<TypePrefix*> (STRS[TYPE_PREFIX]);
      prefix = tp->prefix ().literal ();
    }
    else
    {
      Nameable* parent = defined_in (d);

      while (parent != 0)
      {
        if (parent->context ().count (STRS[TYPE_PREFIX]))
        {
          tp =
            parent->context ().get<TypePrefix*> (STRS[TYPE_PREFIX]);

          prefix = tp->prefix ().literal ();
          break;
        }

        parent = defined_in (*parent);
      }
    }

    if (prefix != "") prefix += "/";

    ScopedName scoped (d.scoped_name ());
    Name stripped (scoped.begin () + 1, scoped.end ());

    string scope_name = regex::perl_s (name_to_string (stripped), "%::%/%");

    string repo_id = "IDL:" + prefix + scope_name + ":1.0";

    // Store the repo id for possible future reference.
    d.context ().set<string> (STRS[REPO_ID], repo_id);
    return repo_id;
  }

  class EmitterBase
  {
  public:
    EmitterBase (fs::ofstream& ofs)
      : os (ofs)
    {}

  protected:
    fs::ofstream& os;
  };

  class CompositionEmitter : public EmitterBase,
                             public Traversal::Composition
  {
  // Nested classes for emitters created and dispatched inside
  // ComponsitionEmitter::traverse().
  private:
    struct ComponentIdEmitter : Traversal::Component, EmitterBase
    {
      ComponentIdEmitter (fs::ofstream& ofs)
        : EmitterBase (ofs)
      {}

      virtual void
      traverse (Type& c)
      {
        os << "<componentrepid repid=\"" << compute_repo_id (c) << "\"/>"
           << endl;
      }
    };

    struct HomeIdEmitter : Traversal::Home, EmitterBase
    {
      HomeIdEmitter (fs::ofstream& ofs)
        : EmitterBase (ofs)
      {}

      virtual void
      traverse (Type& h)
      {
        os << "<homerepid repid=\"" << compute_repo_id (h) << "\"/>"
           << endl;
      }
    };

    struct HomeFeaturesEmitter : Traversal::Home, EmitterBase
    {
      HomeFeaturesEmitter (fs::ofstream& ofs)
        : EmitterBase (ofs)
      {}

      virtual void
      traverse (Type& h)
      {
        os << "<homefeatures" << endl
           << "name=\"" << h.name () << "\"" << endl
           << "repid=\"" << compute_repo_id (h)
           << "\">" << endl;

        Traversal::Inherits home_inherits;
        HomeInheritanceEmitter emitter (os);
        home_inherits.node_traverser (emitter);

        inherits (h, home_inherits);

        os << "</homefeatures>" << endl << endl;

        // Go after inherited homes.
        //
        Traversal::Home::traverse (h);
      }

      struct HomeInheritanceEmitter : Traversal::Home, EmitterBase
      {
        HomeInheritanceEmitter (fs::ofstream& ofs)
          : EmitterBase (ofs)
        {}

        virtual void
        traverse (Type& h)
        {
          os << "<inheritshome repid=\"" << compute_repo_id (h) << "\"/>"
             << endl;
        }
      };
    };

    struct ComponentFeaturesEmitter : Traversal::Component, EmitterBase
    {
      ComponentFeaturesEmitter (fs::ofstream& ofs)
        : EmitterBase (ofs)
      {}

      virtual void
      traverse (Type& h)
      {
        os << "<componentfeatures" << endl
           << "name=\"" << h.name () << "\"" << endl
           << "repid=\"" << compute_repo_id (h)
           << "\">" << endl;

        Traversal::Inherits component_inherits;
        Traversal::Supports component_supports;
        ComponentInheritanceEmitter i_emitter (os);
        ComponentSupportsEmitter s_emitter (os);
        component_inherits.node_traverser (i_emitter);
        component_supports.node_traverser (s_emitter);

        inherits (h, component_inherits);
        supports (h, component_supports);

        os << "<ports>" << endl;

        Traversal::Defines defines;
        PortsEmitter ports_emitter (os);
        defines.node_traverser (ports_emitter);

        names (h, defines);

        os << "</ports>" << endl;

        os << "</componentfeatures>" << endl << endl;

        // Go after inherited components.
        //
        Traversal::Component::traverse (h);
      }

      struct ComponentInheritanceEmitter : Traversal::Component, EmitterBase
      {
        ComponentInheritanceEmitter (fs::ofstream& ofs)
          : EmitterBase (ofs)
        {}

        virtual void
        traverse (Type& h)
        {
          os << "<inheritscomponent repid=\"" << compute_repo_id (h) << "\"/>"
             << endl;
        }
      };

      struct ComponentSupportsEmitter : Traversal::Interface, EmitterBase
      {
        ComponentSupportsEmitter (fs::ofstream& ofs)
          : EmitterBase (ofs)
        {}

        virtual void
        traverse (Type& h)
        {
          os << "<supportsinterface repid=\"" << compute_repo_id (h) << "\"/>"
             << endl;
        }
      };

      struct PortsEmitter : Traversal::EmitterData,
                            Traversal::UserData,
                            Traversal::ProviderData,
                            Traversal::ConsumerData,
                            Traversal::PublisherData,
                            EmitterBase
      {
        PortsEmitter (fs::ofstream& ofs)
          : EmitterBase (ofs),
            type_name_emitter_ (ofs),
            facettag_ (1U)
        {
          belongs_.node_traverser (type_name_emitter_);
        }

        virtual void
        traverse (SemanticGraph::Emitter& e)
        {
          os << "<emits" << endl
             << "emitsname=\"" << e.name () << "\"" << endl
             << "eventtype=";

          Traversal::EmitterData::belongs (e, belongs_);

          os << ">" << endl
             << "<eventpolicy policy=\"normal\"/>" << endl
             << "</emits>" << endl;
        }

        virtual void
        traverse (SemanticGraph::User& u)
        {
          os << "<uses" << endl
             << "usesname=\"" << u.name () << "\"" << endl
             << "repid=";

          Traversal::UserData::belongs (u, belongs_);

          os << ">" << endl
             << "</uses>" << endl;
        }

        virtual void
        traverse (SemanticGraph::Provider& p)
        {
          os << "<provides" << endl
             << "providesname=\"" << p.name () << "\"" << endl
             << "repid=";

          Traversal::ProviderData::belongs (p, belongs_);

          os << endl
             << "facettag=\"" << facettag_ << "\">" << endl
             << "</provides>" << endl;

          ++facettag_;
        }

        virtual void
        traverse (SemanticGraph::Consumer& c)
        {
          os << "<consumes" << endl
             << "consumesname=\"" << c.name () << "\"" << endl
             << "eventtype=";

          Traversal::ConsumerData::belongs (c, belongs_);

          os << ">" << endl
             << "<eventpolicy policy=\"normal\"/>" << endl
             << "</consumes>" << endl;
        }

        virtual void
        traverse (SemanticGraph::Publisher& p)
        {
          os << "<publishes" << endl
            << "publishesname=\"" << p.name ()
            << "\"" << endl
            << "eventtype=";

          Traversal::PublisherData::belongs (p, belongs_);

          os << ">" << endl
             << "<eventpolicy policy=\"normal\"/>" << endl
             << "</publishes>" << endl;
        }

        struct TypeNameEmitter : Traversal::Type, EmitterBase
        {
          TypeNameEmitter (fs::ofstream& ofs)
            : EmitterBase (ofs)
          {}

          virtual void
          traverse (SemanticGraph::Type& t)
          {
            os << '\"' << compute_repo_id (t) << '\"';
          }
        };

      private:
        TypeNameEmitter type_name_emitter_;
        unsigned long facettag_;
        Traversal::Belongs belongs_;
      };
    };

    struct InterfaceEmitter : Traversal::UnconstrainedInterface,
                              EmitterBase
    {
      InterfaceEmitter (fs::ofstream& ofs)
        : EmitterBase (ofs)
      {}

      bool
      add (UnconstrainedInterface& i)
      {
        return interfaces_.insert (&i).second;
      }

      virtual void
      traverse (UnconstrainedInterface& i)
      {
        if (add (i))
        {
          os << "<interface" << endl
              << "name=\"" << i.name ()
              << "\"" << endl
              << "repid=\"" << compute_repo_id (i)
              << "\">" << endl;

          Traversal::Inherits interface_inherits;
          InterfaceInheritanceEmitter i_emitter (os);
          interface_inherits.node_traverser (i_emitter);

          inherits (i, interface_inherits);

          os << "</interface>" << endl << endl;

          // Go after inherited interfaces.
          Traversal::UnconstrainedInterface::traverse (i);
        }
      }

    struct InterfaceInheritanceEmitter : Traversal::Interface, EmitterBase
    {
      InterfaceInheritanceEmitter (fs::ofstream& ofs)
        : EmitterBase (ofs)
      {}

      virtual void
      traverse (Type& i)
      {
        os << "<inheritsinterface repid=\"" << compute_repo_id (i)
            << "\"/>" << endl;
      }
    };

    private:
      std::set<UnconstrainedInterface*> interfaces_;
    };

  public:
    CompositionEmitter (fs::ofstream& ofs,
                        CommandLine const& cl)
      : EmitterBase (ofs),
        cl_ (cl)
    {}

    virtual void
    traverse (Composition& c)
    {
      ScopedName scoped (c.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());
      string name = name_to_string (stripped);
      configure_stream (name);

      os << "<?xml version=\"1.0\"?>" << endl
         << "<!DOCTYPE corbacomponent SYSTEM \"corbacomponent.dtd\">"
         << endl << endl;

      os << "<corbacomponent>" << endl
         << "<corbaversion>3.0</corbaversion>" << endl;

      {
        ComponentIdEmitter component_id_emitter (os);
        HomeIdEmitter home_id_emitter (os);
        Traversal::Defines defines;
        Traversal::ComponentExecutor component_executor;
        Traversal::HomeExecutor home_executor;
        defines.node_traverser (component_executor);
        defines.node_traverser (home_executor);
        Traversal::Implements implements;
        component_executor.edge_traverser (implements);
        home_executor.edge_traverser (implements);
        implements.node_traverser (component_id_emitter);
        implements.node_traverser (home_id_emitter);

        names (c, defines);
      }

      os << "<componentkind>" << endl
         << "<session>" << endl
         << "<servant lifetime=\"container\"/>" << endl
         << "</session>" << endl
         << "</componentkind>" << endl
         << "<threading policy=\"multithread\"/>" << endl
         << "<configurationcomplete set=\"true\"/>" << endl << endl;

      {
        HomeFeaturesEmitter home_features_emitter (os);
        Traversal::Defines defines;
        Traversal::HomeExecutor home_executor;
        defines.node_traverser (home_executor);
        Traversal::Implements implements;
        home_executor.edge_traverser (implements);
        implements.node_traverser (home_features_emitter);

        Traversal::Inherits inherits;
        home_features_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_features_emitter);

        names (c, defines);
      }

      {
        ComponentFeaturesEmitter component_features_emitter (os);
        Traversal::Defines defines;
        Traversal::ComponentExecutor component_executor;
        defines.node_traverser (component_executor);
        Traversal::Implements implements;
        component_executor.edge_traverser (implements);
        implements.node_traverser (component_features_emitter);

        Traversal::Inherits inherits;
        component_features_emitter.edge_traverser (inherits);
        inherits.node_traverser (component_features_emitter);

        names (c, defines);
      }

      {
        Traversal::Component component;
        Traversal::Home home;
        Traversal::Defines defines;
        Traversal::ComponentExecutor component_executor;
        Traversal::HomeExecutor home_executor;
        defines.node_traverser (component_executor);
        defines.node_traverser (home_executor);
        Traversal::Implements implements;
        component_executor.edge_traverser (implements);
        home_executor.edge_traverser (implements);
        implements.node_traverser (component);
        implements.node_traverser (home);

        Traversal::Supports supports;
        InterfaceEmitter interface_emitter (os);
        supports.node_traverser (interface_emitter);
        component.edge_traverser (supports);
        home.edge_traverser (supports);

        Traversal::Defines component_defines;
        component.edge_traverser (component_defines);
        Traversal::Provider provider;
        Traversal::User user;
        component_defines.node_traverser (provider);
        component_defines.node_traverser (user);
        Traversal::Belongs belongs;
        belongs.node_traverser (interface_emitter);
        provider.edge_traverser (belongs);
        user.edge_traverser (belongs);

        Traversal::Inherits inherits;
        inherits.node_traverser (interface_emitter);
        interface_emitter.edge_traverser (inherits);

        names (c, defines);
      }

      os << "</corbacomponent>" << endl;
    }

  private:
    void configure_stream (string const& scoped_name)
    {
      string file_suffix = cl_.get_value ("desc-file-suffix",
                                          ".ccd");

      string desc_file_name = regex::perl_s (scoped_name, "/::/_/");

      desc_file_name += file_suffix;

      fs::path desc_file_path (desc_file_name);

      if (os.is_open ())
      {
        os.close ();
      }

      os.open (desc_file_path, std::ios_base::out);

      if (!os.is_open ())
      {
        cerr << desc_file_name
             << ": error: unable to open file in write mode"
             << endl;
      }
    }

  private:
    CommandLine const& cl_;
  };
}

// ==========================================================

void
DescriptorGenerator::options (CL::Description& d)
{
  d.add_option (CL::OptionDescription (
                  "desc-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'.ccd\' "
                  "when constructing name of descriptor file.",
                  true));

  d.add_option (CL::OptionDescription (
                  "desc-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of descriptor file.",
                  true));
}

void
DescriptorGenerator::generate (CommandLine const& cl,
                               TranslationUnit& u)
{
  // Set auto-indentation for os
  Indentation::Implanter<Indentation::XML> guard (ofs_);

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

  region.edge_traverser (includes);
  region.edge_traverser (contains_root);

  //--
  Traversal::Root root;
  includes.node_traverser (region);
  contains_root.node_traverser (root);

  // Layer 3
  //
  Traversal::Defines defines;
  root.edge_traverser (defines);

  //--
  Traversal::Module module;
  CompositionEmitter composition (ofs_, cl);
  defines.node_traverser (module);
  defines.node_traverser (composition);

  unit.traverse (u);
}
