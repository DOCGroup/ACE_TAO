// file      : Test/IDL2/Traversal/Recreate/GeneratorImpl.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TEST_IDL2_TRAVERSAL_RECREATE_GENERATOR_IMPL_HPP
#define TEST_IDL2_TRAVERSAL_RECREATE_GENERATOR_IMPL_HPP

#include <iostream>

#include "CCF/CodeGenerationKit/IndentationIDL.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

#include "CCF/IDL2/SemanticGraph.hpp"
#include "CCF/IDL2/Traversal.hpp"

namespace IDL2
{
  using namespace CCF::IDL2;
  using namespace CCF::IDL2::SemanticGraph;

  using std::cout;
  using std::endl;

  class GeneratorImpl
  {
  protected:
    // Layer 1
    //

    // Layer 2
    //
    struct BracketIncludes : Traversal::BracketIncludes
    {
      virtual void
      traverse (Type& qi)
      {
        cout << "include <" << qi.file ().string () << ">" << endl;
      }
    };

    struct QuoteIncludes : Traversal::QuoteIncludes
    {
      virtual void
      traverse (Type& qi)
      {
        cout << "include \"" << qi.file ().string () << "\"" << endl;
      }
    };

    //--

    // Layer 3
    //

    struct Aliases : Traversal::Aliases
    {
      virtual void
      pre (Type&)
      {
        cout << "typedef ";
      }

      virtual void
      name (Type& a)
      {
        cout << " " << a.name ();
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }
    };

    //--

    struct AbstractInterfaceFwd : Traversal::AbstractInterface
    {
      virtual void
      traverse (Type& i)
      {
        cout << "abstract interface " << i.name () << ";";
      }
    };

    struct LocalInterfaceFwd : Traversal::LocalInterface
    {
      virtual void
      traverse (Type& i)
      {
        cout << "local interface " << i.name () << ";";
      }
    };

    struct UnconstrainedInterfaceFwd : Traversal::UnconstrainedInterface
    {
      virtual void
      traverse (Type& i)
      {
        cout << "interface " << i.name () << ";";
      }
    };

    struct StructFwd : Traversal::Struct
    {
      virtual void
      traverse (Type& i)
      {
        cout << "struct " << i.name () << ";";
      }
    };


    struct AbstractInterface : Traversal::AbstractInterface
    {
      virtual void
      pre (Type&)
      {
        cout << "abstract interface ";
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

    struct LocalInterface : Traversal::LocalInterface
    {
      virtual void
      pre (Type&)
      {
        cout << "local interface ";
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

    struct UnconstrainedInterface : Traversal::UnconstrainedInterface
    {
      virtual void
      pre (Type&)
      {
        cout << "interface ";
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


    struct Module : Traversal::Module
    {
      virtual void
      pre (Type& m)
      {
        cout << "module " << m.name ();
      }

      virtual void
      names_pre (Type& m)
      {
        cout << "{";
      }

      virtual void
      names_post (Type& m)
      {
        cout << "}";
      }

      virtual void
      post (Type& m)
      {
        cout << ";";
      }
    };

    struct Struct : Traversal::Struct
    {
      virtual void
      pre (Type&)
      {
        cout << "struct ";
      }

      virtual void
      name (Type& s)
      {
        cout << s.name ();
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


    struct TypeId : Traversal::TypeId
    {
      virtual void
      traverse (Type& ti)
      {
        cout << "typeid " << ti.declaration () << " " << ti.id () << ";";
      }
    };


    struct TypePrefix : Traversal::TypePrefix
    {
      virtual void
      traverse (Type& ti)
      {
        cout << "typeprefix " <<  ti.declaration () << " "
             << ti.prefix () << ";";
      }
    };

    struct TypeName : Traversal::Nameable,
                      Traversal::FundamentalType
    {
      virtual void
      traverse (SemanticGraph::Nameable& n)
      {
        cout << n.scoped_name ();
      }

      virtual void
      traverse (SemanticGraph::FundamentalType& ft)
      {
        cout << ft.name ();
      }
    };

    struct UnboundedSequence : Traversal::UnboundedSequence
    {
      virtual void
      pre (Type&)
      {
        cout << "sequence<";
      }

      virtual void
      post (Type&)
      {
        cout << ">";
      }
    };

    // Layer 4
    //

    //--

    struct Operation : Traversal::Operation
    {
      virtual void
      name (Type& o)
      {
        cout << " " << o.name ();
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

    struct Attribute : Traversal::Attribute
    {
      virtual void
      pre (Type&)
      {
        cout << "attribute ";
      }

      virtual void
      name (Type& a)
      {
        cout << " " << a.name ();
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

    };

    struct Member : Traversal::Member
    {
      virtual void
      name (Type& m)
      {
        cout << " " << m.name ();
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }
    };

    // Layer 5
    //

    //--

    struct Parameter : Traversal::InParameter,
                       Traversal::OutParameter,
                       Traversal::InOutParameter
    {
      virtual void
      pre (InParameter& p)
      {
        cout << " in ";
      }

      virtual void
      pre (OutParameter& p)
      {
        cout << " out ";
      }

      virtual void
      pre (InOutParameter& p)
      {
        cout << " inout ";
      }

      virtual void
      name (InParameter& p)
      {
        cout << p.name ();
      }

      virtual void
      name (OutParameter& p)
      {
        cout << p.name ();
      }

      virtual void
      name (InOutParameter& p)
      {
        cout << p.name ();
      }
    };

  public:

    GeneratorImpl ()
    {
      // Layer 1
      //
      unit.edge_traverser (contains_principal);

      //--

      contains_principal.node_traverser (region);

      // Layer 2
      //
      region.edge_traverser (quote_includes);
      region.edge_traverser (bracket_includes);
      region.edge_traverser (contains_root);

      //--

      contains_root.node_traverser (root);

      // Layer 3
      //
      root.edge_traverser (mentions);
      root.edge_traverser (defines);
      root.edge_traverser (aliases);

      //--

      mentions.node_traverser (abstract_interface_fwd);
      mentions.node_traverser (local_interface_fwd);
      mentions.node_traverser (unconstrained_interface_fwd);
      mentions.node_traverser (struct_fwd);

      defines.node_traverser (abstract_interface);
      defines.node_traverser (local_interface);
      defines.node_traverser (unconstrained_interface);
      defines.node_traverser (module);
      defines.node_traverser (struct_);
      defines.node_traverser (type_id);
      defines.node_traverser (type_prefix);

      aliases.node_traverser (type_name);
      aliases.node_traverser (unbounded_sequence);

      // Layer 4
      //
      module.edge_traverser (mentions);
      module.edge_traverser (defines);
      module.edge_traverser (aliases);

      abstract_interface.edge_traverser (inherits);
      local_interface.edge_traverser (inherits);
      unconstrained_interface.edge_traverser (inherits);

      abstract_interface.edge_traverser (interface_defines);
      local_interface.edge_traverser (interface_defines);
      unconstrained_interface.edge_traverser (interface_defines);

      struct_.edge_traverser (struct_defines);

      unbounded_sequence.edge_traverser (specialized);

      //--

      inherits.node_traverser (type_name);

      interface_defines.node_traverser (operation);
      interface_defines.node_traverser (attribute);

      struct_defines.node_traverser (member);

      specialized.node_traverser (type_name);

      // Layer 5
      //

      operation.edge_traverser (receives);
      operation.edge_traverser (returns);

      attribute.edge_traverser (belongs);

      member.edge_traverser (belongs);

      //--

      receives.node_traverser (parameter);
      returns.node_traverser (type_name);
      belongs.node_traverser (type_name);

      // Layer 6
      //
      parameter.edge_traverser (belongs);

    }

  protected:
    Traversal::TranslationUnit unit;

    // Layer 1
    //
    Traversal::ContainsPrincipal contains_principal;

    //--

    Traversal::TranslationRegion region;

    // Layer 2
    //
    BracketIncludes bracket_includes;
    QuoteIncludes quote_includes;
    Traversal::ContainsRoot contains_root;

    //--

    Traversal::Root root;

    // Layer 3
    //
    Traversal::Mentions mentions;
    Traversal::Defines defines;
    Aliases aliases;

    //--

    AbstractInterfaceFwd abstract_interface_fwd;
    LocalInterfaceFwd local_interface_fwd;
    UnconstrainedInterfaceFwd unconstrained_interface_fwd;

    StructFwd struct_fwd;

    AbstractInterface abstract_interface;
    LocalInterface local_interface;
    UnconstrainedInterface unconstrained_interface;

    Module module;

    Struct struct_;

    TypeId type_id;
    TypePrefix type_prefix;

    TypeName type_name;

    UnboundedSequence unbounded_sequence;

    // Layer 4
    //
    Traversal::Inherits inherits;
    Traversal::Defines interface_defines;
    Traversal::Defines struct_defines;
    Traversal::Specialized specialized;

    //--

    Operation operation;
    Attribute attribute;
    Member member;

    // Layer 5
    //
    Traversal::Receives receives;
    Traversal::Returns returns;
    Traversal::Belongs belongs;

    //--

    Parameter parameter;

    // Layer 6
    //

  public:
    void
    generate (CCF::IDL2::SemanticGraph::TranslationUnit& tu)
    {
      // Plug automatic IDL indenter.
      //
      Indentation::Implanter<Indentation::IDL> guard (cout);

      unit.traverse (tu);
    }
  };
}

#endif  // TEST_IDL2_TRAVERSAL_RECREATE_GENERATOR_IMPL_HPP
