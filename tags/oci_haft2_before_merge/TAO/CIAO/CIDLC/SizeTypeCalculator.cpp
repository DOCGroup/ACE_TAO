// file      : CIDLC/SizeTypeCalculator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "SizeTypeCalculator.hpp"
#include "Literals.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include <stack>

using namespace CCF::CIDL;
using namespace SyntaxTree;
using namespace StringLiterals;

namespace
{
  class Calculator : public Traversal::StringDecl,
                     public Traversal::StructDef,
                     public Traversal::MemberDecl,
                     public Traversal::WstringDecl,
                     public Traversal::SequenceDecl,
                     public Traversal::InterfaceDecl
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
    traverse (WstringDeclPtr const&)
    {
      top () = true;
    }

    virtual void
    traverse (SequenceDeclPtr const&)
    {
      top () = true;
    }

    virtual void
    traverse (InterfaceDeclPtr const&)
    {
      top () = true;
    }

    virtual void
    traverse (StructDefPtr const& s)
    {
      if (s->context ().count (STRS[VAR_SIZE]))
      {
        top () = s->context ().get<bool> (STRS[VAR_SIZE]);
      }
      else
      {
        Traversal::StructDef::traverse (s);
      }
    }

    virtual void
    pre (StructDefPtr const& s)
    {
      push (false);
    }

    virtual void
    post (StructDefPtr const& s)
    {
      bool r (top ());

      s->context ().set (STRS[VAR_SIZE], r);

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
