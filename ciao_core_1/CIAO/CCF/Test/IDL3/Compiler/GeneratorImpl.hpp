// file      : Test/IDL3/Compiler/GeneratorImpl.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TEST_IDL3_COMPILER_GENERATOR_IMPL_HPP
#define TEST_IDL3_COMPILER_GENERATOR_IMPL_HPP

#include <iostream>

#include "CCF/CodeGenerationKit/IndentationIDL.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

#include "CCF/IDL3/SemanticGraph.hpp"
#include "CCF/IDL3/Traversal.hpp"

#include "../../IDL2/Traversal/Recreate/GeneratorImpl.hpp"

namespace IDL3
{
  using namespace CCF::IDL3;

  using std::cout;
  using std::endl;

  class GeneratorImpl : public IDL2::GeneratorImpl
  {
  protected:
    // Layer 1
    //

    // Layer 2
    //

    // Layer 3
    //

    //--
    struct ComponentFwd : Traversal::Component
    {
      virtual void
      traverse (Type& i)
      {
        cout << "component " << i.name () << ";";
      }
    };

    struct Component : Traversal::Component
    {
      virtual void
      pre (Type&)
      {
        cout << "component ";
      }

      virtual void
      name (Type& i)
      {
        cout << i.name ();
      }

      virtual void
      inherits_pre (Type&)
      {
        cout << " : ";
      }

      virtual void
      supports_pre (Type&)
      {
        cout << " supports ";
      }

      virtual void
      names_pre (Type&)
      {
        cout << "{";
      }

      virtual void
      names_post (Type&)
      {
        cout << "}";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };


    struct Home : Traversal::Home
    {
      virtual void
      pre (Type&)
      {
        cout << "home ";
      }

      virtual void
      name (Type& i)
      {
        cout << i.name ();
      }

      virtual void
      inherits_pre (Type&)
      {
        cout << " : ";
      }

      virtual void
      supports_pre (Type&)
      {
        cout << " supports ";
      }

      virtual void
      manages_pre (Type&)
      {
        cout << " manages ";
      }

      virtual void
      names_pre (Type&)
      {
        cout << "{";
      }

      virtual void
      names_post (Type&)
      {
        cout << "}";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };

    struct AbstractEventType : Traversal::AbstractEventType
    {
      virtual void
      pre (Type&)
      {
        cout << "abstract eventtype ";
      }

      virtual void
      name (Type& i)
      {
        cout << i.name ();
      }

      virtual void
      inherits_pre (Type&)
      {
        cout << " : ";
      }

      virtual void
      supports_pre (Type&)
      {
        cout << " supports ";
      }

      virtual void
      names_pre (Type&)
      {
        cout << "{";
      }

      virtual void
      names_post (Type&)
      {
        cout << "}";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };

    struct ConcreteEventType : Traversal::ConcreteEventType
    {
      virtual void
      pre (Type&)
      {
        cout << "eventtype ";
      }

      virtual void
      name (Type& i)
      {
        cout << i.name ();
      }

      virtual void
      inherits_pre (Type&)
      {
        cout << " : ";
      }

      virtual void
      supports_pre (Type&)
      {
        cout << " supports ";
      }

      virtual void
      names_pre (Type&)
      {
        cout << "{";
      }

      virtual void
      names_post (Type&)
      {
        cout << "}";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };

    // Layer 4
    //

    struct Provider : Traversal::ProviderData
    {
      virtual void
      pre (Type&)
      {
        cout << "provides ";
      }

      virtual void
      name (Type& e)
      {
        cout << " " << e.name ();
      }

      virtual void
      post (Type& e)
      {
        cout << ";";
      }
    };

    struct User : Traversal::UserData
    {
      virtual void
      pre (Type&)
      {
        cout << "uses ";
      }

      virtual void
      name (Type& e)
      {
        cout << " " << e.name ();
      }

      virtual void
      post (Type& e)
      {
        cout << ";";
      }
    };

    struct Publisher : Traversal::PublisherData
    {
      virtual void
      pre (Type&)
      {
        cout << "publishes ";
      }

      virtual void
      name (Type& e)
      {
        cout << " " << e.name ();
      }

      virtual void
      post (Type& e)
      {
        cout << ";";
      }
    };

    struct Emitter : Traversal::EmitterData
    {
      virtual void
      pre (Type&)
      {
        cout << "emits ";
      }

      virtual void
      name (Type& e)
      {
        cout << " " << e.name ();
      }

      virtual void
      post (Type& e)
      {
        cout << ";";
      }
    };

    struct Consumer : Traversal::ConsumerData
    {
      virtual void
      pre (Type&)
      {
        cout << "consumes ";
      }

      virtual void
      name (Type& e)
      {
        cout << " " << e.name ();
      }

      virtual void
      post (Type& e)
      {
        cout << ";";
      }
    };

    //--

    struct HomeFactory : Traversal::HomeFactory
    {
      virtual void
      returns (Type&)
      {
        cout << "factory ";
      }

      virtual void
      name (Type& hf)
      {
        cout << hf.name ();
      }

      virtual void
      receives_pre (Type&)
      {
        cout << " (";
      }

      virtual void
      receives_post (Type&)
      {
        cout << ")";
      }

      virtual void
      raises_pre (Type&)
      {
        cout << " raises (";
      }

      virtual void
      raises_post (Type&)
      {
        cout << ")";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };


    struct HomeFinder : Traversal::HomeFinder
    {
      virtual void
      returns (Type&)
      {
        cout << "finder ";
      }

      virtual void
      name (Type& hf)
      {
        cout << hf.name ();
      }

      virtual void
      receives_pre (Type&)
      {
        cout << " (";
      }

      virtual void
      receives_post (Type&)
      {
        cout << ")";
      }

      virtual void
      raises_pre (Type&)
      {
        cout << " raises (";
      }

      virtual void
      raises_post (Type&)
      {
        cout << ")";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };

    // Layer 5
    //

    //--

  public:

    GeneratorImpl ()
    {
      // Layer 1
      //

      // Layer 2
      //

      // Layer 3
      //

      //--

      mentions.node_traverser (component_fwd);

      defines.node_traverser (component);

      defines.node_traverser (home);

      defines.node_traverser (abstract_event_type);
      defines.node_traverser (concrete_event_type);

      // Layer 4
      //

      component.edge_traverser (inherits);
      component.edge_traverser (supports);
      component.edge_traverser (component_defines);

      home.edge_traverser (inherits);
      home.edge_traverser (supports);
      home.edge_traverser (manages);
      home.edge_traverser (home_defines);

      //@@ eventtype can define the whole bunch of stuff
      //   just like valuetype.
      //
      abstract_event_type.edge_traverser (inherits);
      concrete_event_type.edge_traverser (inherits);

      //--

      supports.node_traverser (type_name);

      component_defines.node_traverser (attribute);
      component_defines.node_traverser (provider);
      component_defines.node_traverser (user);
      component_defines.node_traverser (publisher);
      component_defines.node_traverser (emitter);
      component_defines.node_traverser (consumer);

      manages.node_traverser (type_name);

      //@@ home can define the whole bunch of stuff just like
      // interface & valuetype.

      home_defines.node_traverser (home_factory);
      home_defines.node_traverser (home_finder);

      // Layer 5
      //

      provider.edge_traverser (belongs);
      user.edge_traverser (belongs);
      publisher.edge_traverser (belongs);
      emitter.edge_traverser (belongs);
      consumer.edge_traverser (belongs);

      home_factory.edge_traverser (receives);
      home_finder.edge_traverser (receives);

      //--

      // Layer 6
      //

    }

  protected:
    // Layer 1
    //

    // Layer 2
    //

    // Layer 3
    //

    //--

    ComponentFwd component_fwd;

    Component component;

    Home home;

    AbstractEventType abstract_event_type;
    ConcreteEventType concrete_event_type;


    // Layer 4
    //
    Traversal::Supports supports;
    Traversal::Defines component_defines;

    Traversal::Manages manages;
    Traversal::Defines home_defines;

    //--

    Provider provider;
    User user;
    Publisher publisher;
    Emitter emitter;
    Consumer consumer;

    HomeFactory home_factory;
    HomeFinder home_finder;

    // Layer 5
    //

    // Layer 6
    //

  public:
    void
    generate (CCF::IDL3::SemanticGraph::TranslationUnit& tu)
    {
      // Plug automatic IDL indenter.
      //
      Indentation::Implanter<Indentation::IDL> guard (cout);

      unit.traverse (tu);
    }
  };
}

#endif  // TEST_IDL3_COMPILER_GENERATOR_IMPL_HPP
