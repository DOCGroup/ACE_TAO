// file      : Test/CIDL/Compiler/GeneratorImpl.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TEST_CIDL_COMPILER_GENERATOR_IMPL_HPP
#define TEST_CIDL_COMPILER_GENERATOR_IMPL_HPP

#include <iostream>

#include "CCF/CodeGenerationKit/IndentationIDL.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "../../IDL3/Compiler/GeneratorImpl.hpp"

namespace CIDL
{
  using namespace CCF::CIDL;

  using std::cout;
  using std::endl;

  class GeneratorImpl : public IDL3::GeneratorImpl
  {
  protected:
    // Layer 1
    //

    // Layer 2
    //

    // Layer 3
    //

    //--

    struct Composition : Traversal::Composition
    {
      virtual void
      pre (Type& c)
      {
        //@@ TODO kind
        cout << "composition " << "session" << " ";
      }

      virtual void
      name (Type& i)
      {
        cout << i.name ();
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
    };


    struct HomeExecutor : Traversal::HomeExecutor
    {
      virtual void
      pre (Type&)
      {
        cout << "home executor ";
      }

      virtual void
      name (Type& he)
      {
        cout << he.name () << "{";
      }

      virtual void
      implements_pre (Type&)
      {
        cout << "implements ";
      }

      virtual void
      implements_post (Type&)
      {
        cout << ";";
      }

      virtual void
      manages_pre (Type&)
      {
        cout << "manages ";
      }

      virtual void
      manages_post (Type&)
      {
        cout << ";";
      }

      virtual void
      post (Type& he)
      {
        cout << "};";
      }
    };


    // Layer 4
    //

    //--

    // Layer 5
    //

    //--

    struct SimpleName : Traversal::Nameable
    {
      virtual void
      traverse (Type& n)
      {
        cout << n.name ();
      }
    };

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

      defines.node_traverser (composition);

      // Layer 4
      //

      composition.edge_traverser (composition_defines);

      //--

      composition_defines.node_traverser (home_executor);

      // Layer 5
      //
      home_executor.edge_traverser (home_executor_implements);
      home_executor.edge_traverser (home_executor_manages);

      //--

      home_executor_implements.node_traverser (type_name);
      home_executor_manages.node_traverser (simple_name);

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

    Composition composition;

    // Layer 4
    //

    Traversal::Defines composition_defines;

    //--

    HomeExecutor home_executor;

    // Layer 5
    //
    Traversal::Implements home_executor_implements;
    Traversal::Manages home_executor_manages;

    //--

    SimpleName simple_name;

    // Layer 6
    //

  public:
    void
    generate (CCF::CIDL::SemanticGraph::TranslationUnit& tu)
    {
      // Plug automatic IDL indenter.
      //
      Indentation::Implanter<Indentation::IDL> guard (cout);

      unit.traverse (tu);
    }
  };
}

#endif  // TEST_CIDL_COMPILER_GENERATOR_IMPL_HPP
