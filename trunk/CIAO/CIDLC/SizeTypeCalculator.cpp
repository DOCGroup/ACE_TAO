// file      : CIDLC/SizeTypeCalculator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "SizeTypeCalculator.hpp"
#include "Literals.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include <stack>

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;
using namespace StringLiterals;

namespace
{
  class Calculator : public Traversal::String,
                     public Traversal::Struct,
                     public Traversal::Union,
                     public Traversal::Wstring,
                     public Traversal::UnboundedSequence,
                     public Traversal::BoundedSequence,
                     public Traversal::Interface,
                     public Traversal::ValueType
  {
  public:
    Calculator ()
    {
      push (false);
    }

  public:
    virtual void
    traverse (SemanticGraph::String&)
    {
      top () = true;
    }

    virtual void
    traverse (SemanticGraph::Wstring&)
    {
      top () = true;
    }

    virtual void
    traverse (SemanticGraph::UnboundedSequence&)
    {
      top () = true;
    }

    virtual void
    traverse (SemanticGraph::BoundedSequence&)
    {
      top () = true;
    }

    virtual void
    traverse (SemanticGraph::Interface&)
    {
      top () = true;
    }

    virtual void
    traverse (SemanticGraph::ValueType&)
    {
      top () = true;
    }

    virtual void
    pre (SemanticGraph::Struct&)
    {
      push (false);
    }

    virtual void
    traverse (SemanticGraph::Struct& s)
    {
      std::string n (s.scoped_name ().str ());
      if (s.context ().count (STRS[VAR_SIZE]))
      {
        // Never set 'top' to false (except in pre() above),
        // so a 'true' value will propagate up the scopes.
        bool r = s.context ().get<bool> (STRS[VAR_SIZE]);
        if (r) top () = r;
      }
      else
      {
        Traversal::Struct::traverse (s);
      }
    }

    virtual void
    post (SemanticGraph::Struct& s)
    {
      // Set our context to the result of nested scope traversal.
      s.context ().set (STRS[VAR_SIZE], top ());
    }

    virtual void
    pre (SemanticGraph::Union&)
    {
      push (false);
    }

    virtual void
    traverse (SemanticGraph::Union& u)
    {
      if (u.context ().count (STRS[VAR_SIZE]))
      {
        // Never set 'top' to false (except in pre() above),
        // so a 'true' value will propagate up the scopes.
        bool r = u.context ().get<bool> (STRS[VAR_SIZE]);
        if (r) top () = r;
      }
      else
      {
        Traversal::Union::traverse (u);
      }
    }

    virtual void
    post (SemanticGraph::Union& u)
    {
      // Set our context to the result of nested scope traversal.
      u.context ().set (STRS[VAR_SIZE], top ());
    }

  private:
    void
    push (bool v)
    {
      stack_.push (v);
    }

    bool&
    top ()
    {
      return stack_.top ();
    }

    void
    pop ()
    {
      stack_.pop ();
    }

  private:
    std::stack<bool> stack_;
  };
}


void SizeTypeCalculator::
calculate (SemanticGraph::TranslationUnit& u)
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
  // Everything that can contain a struct or union.
  Traversal::Module module;
  Traversal::UnconstrainedInterface uinterface;
  Traversal::ValueType vtype;
  Traversal::Home component_home;

  Calculator calculator;

  defines.node_traverser (module);
  defines.node_traverser (uinterface);
  defines.node_traverser (vtype);
  defines.node_traverser (component_home);
  defines.node_traverser (calculator);

  // Layer 4
  //
  Traversal::Defines struct_defines;
  module.edge_traverser (defines);
  uinterface.edge_traverser (defines);
  vtype.edge_traverser (defines);
  component_home.edge_traverser (defines);
  calculator.edge_traverser (struct_defines);

  //--
  Traversal::Member member;
  struct_defines.node_traverser (member);

  // Layer 5
  //
  Traversal::Belongs belongs;
  member.edge_traverser (belongs);

  //--
  belongs.node_traverser (calculator);

  unit.traverse (u);
}
