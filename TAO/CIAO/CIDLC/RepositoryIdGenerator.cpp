// file      : CIDLC/RepositoryIdGenerator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "RepositoryIdGenerator.hpp"
#include "Literals.hpp"

#include "CCF/CompilerElements/Diagnostic.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

using namespace CCF::CIDL;
using namespace SyntaxTree;

namespace
{
  char const* const
  type_id_label = "CIDLC::RepositoryIdGenerator::TypeId";

  char const* const
  type_prefix_label = "CIDLC::RepositoryIdGenerator::TypePrefix";
}

namespace
{
  //
  //
  //
  class TypeIdLabelGenerator : public Traversal::TypeId
  {
  public:
    TypeIdLabelGenerator (Diagnostic::Stream& dout)
        : dout_ (dout)
    {
    }

    virtual void
    traverse (TypeIdPtr const& ti)
    {
      ScopedName decl_name (ti->declaration ());

      DeclarationTable::IteratorPair iters (
        ti->table ().lookup (decl_name));

      for (; iters.first != iters.second; ++iters.first)
      {
        DeclarationPtr decl (*iters.first);

        if (decl->context ().count (
              StringLiterals::STRS[StringLiterals::TYPE_ID]))
        {
          //@@ Seems it should contain filename and line
          //   of its origin.
          //

          Diagnostic::Error err ("???", 0);

          err << "repetition of typeid for " << decl_name
              << " is illegal";

          dout_ << err;

          Diagnostic::Info inf ("???", 0);

          err << "original typeid here";

          dout_ << inf;
        }
        else
        {
          decl->context ().set (
            StringLiterals::STRS[StringLiterals::TYPE_ID], ti);
          decl->context ().set (
            StringLiterals::STRS[StringLiterals::REPO_ID], ti->id ().str ());
        }
      }
    }

  private:
    Diagnostic::Stream& dout_;
  };


  //
  //
  //
  class TypePrefixLabelGenerator : public Traversal::TypePrefix
  {
  public:
    TypePrefixLabelGenerator (Diagnostic::Stream& dout)
        : dout_ (dout)
    {
    }

    virtual void
    traverse (TypePrefixPtr const& tp)
    {
      ScopedName decl_name (tp->declaration ());

      DeclarationTable::IteratorPair iters (
        tp->table ().lookup (decl_name));

      for (; iters.first != iters.second; ++iters.first)
      {
        DeclarationPtr decl (*iters.first);

        if (decl->context ().count (
              StringLiterals::STRS[StringLiterals::TYPE_PREFIX]))
        {
          TypePrefixPtr prev (
            decl->context ().get<TypePrefixPtr> (
              StringLiterals::STRS[StringLiterals::TYPE_PREFIX]));

          if ((prev->prefix ()) != (tp->prefix ()))
          {

            //@@ Seems tp should containt filename and line
            //   of it's origin.
            //

            Diagnostic::Error err ("???", 0);

            err << "resetting type prefix for " << decl_name
                << " is illegal";

            dout_ << err;

            Diagnostic::Info inf ("???", 0);

            err << "original typeprefix here";

            dout_ << inf;
          }
        }
        else
        {
          decl->context ().set (
            StringLiterals::STRS[StringLiterals::TYPE_PREFIX], tp);
        }
      }
    }

  private:
    Diagnostic::Stream& dout_;
  };
}


bool RepositoryIdGenerator::
generate (TranslationUnitPtr const& u)
{
  {
    Diagnostic::Stream dout;

    TypeIdLabelGenerator type_id (dout);
    TypePrefixLabelGenerator type_prefix (dout);

    Traversal::Scope scope;
    scope.add_scope_delegate (&type_id);
    scope.add_scope_delegate (&type_prefix);

    Traversal::TranslationRegion region (&scope);

    Traversal::TranslationUnit unit;
    unit.add_content_delegate (&region);

    unit.dispatch (u);

    if (dout.error_count () != 0) return false;

    //@@ check errors
  }

  return true;
}
