// file      : CIDLC/SizeTypeCalculator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "SizeTypeCalculator.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include <stack>

using namespace CCF::CIDL;
using namespace SyntaxTree;

namespace
{
  char const* const
  variable_size_label = "variable-size";
}

namespace
{
  class Calculator : public Traversal::StringDecl,
                     public Traversal::StructDecl,
                     public Traversal::MemberDecl
  {
  public:
    Calculator ()
        : Traversal::MemberDecl (this)
    {
      push (false);
    }


  public:
    virtual void
    traverse (StringDeclPtr const&)
    {
      top () = true;
    }

    virtual void
    traverse (StructDeclPtr const& s)
    {
      if (s->context ().count (variable_size_label))
      {
        top () = s->context ().get<bool> (variable_size_label);
      }
      else
      {
        Traversal::StructDecl::traverse (s);
      }
    }

    virtual void
    pre (StructDeclPtr const& s)
    {
      push (false);
    }

    virtual void
    post (StructDeclPtr const& s)
    {
      bool r (top ());

      s->context ().set (variable_size_label, r);

      pop ();

      if (r) top () = r;
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
calculate (TranslationUnitPtr const& u)
{
  Calculator c;

  Traversal::Module module;
  module.add_scope_delegate (&c);

  Traversal::FileScope file_scope;
  file_scope.add_scope_delegate (&module);
  file_scope.add_scope_delegate (&c);

  Traversal::TranslationRegion region (&file_scope);

  Traversal::TranslationUnit unit;
  unit.add_content_delegate (&region);

  unit.dispatch (u);
}
