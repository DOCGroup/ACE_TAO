// file      : CIDLC/RepositoryIdGenerator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "RepositoryIdGenerator.hpp"

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

  char const* const
  rep_id_label = "CIDLC::RepositoryIdGenerator::RepId";
}

namespace
{
  //
  //
  //
  class TypeIdLabelGenerator : public Traversal::TypeId
  {
  public:
    TypeIdLabelGenerator (Diagnostic::Stream dout)
        : dout_ (dout)
    {
    }

    virtual void
    traverse (TypeIdDeclPtr const& ti)
    {
      ScopedName decl_name (ti->declaration ());

      DeclarationTable::IteratorPair iters (
        ti->scope ()->table ()->lookup (decl_name));

      for (; iters.first != iters.second; ++iters.first)
      {
        DeclarationPtr decl (*iters.first);

        if (decl->context ().count (type_id_label))
        {
          //@@ Seems ti should containt filename and line
          //   of it's origin.
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
          decl->context ().set (type_id_label, ti);
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
    TypePrefixLabelGenerator (Diagnostic::Stream dout)
        : dout_ (dout)
    {
    }

    virtual void
    traverse (TypePrefixDeclPtr const& tp)
    {
      ScopedName decl_name (tp->declaration ());

      DeclarationTable::IteratorPair iters (
        tp->scope ()->table ()->lookup (decl_name));

      for (; iters.first != iters.second; ++iters.first)
      {
        DeclarationPtr decl (*iters.first);

        if (decl->context ().count (type_prefix_label))
        {
          TypePrefixDeclPtr prev (
            decl->context ().get<TypePrefixDeclPtr> (type_prefix_label));

          if (prev->prefix != tp->prefix ())
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
          decl->context ().set (type_prefix_label, tp);
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
  // Phase One: generate labels
  //
  {
    Diagnostic::Stream dout;

    TypeIdLabelGenerator type_id (dout);
    TypePrefixLabelGenerator type_prefix (dout);

    Traversal::Scope scope;
    module.add_scope_delegate (&type_id);
    module.add_scope_delegate (&type_prefix);

    Traversal::TranslationRegion region (&scope);

    Traversal::TranslationUnit unit;
    unit.add_content_delegate (&region);

    unit.dispatch (u);

    //@@ check errors
  }


  // Phase Two: generate repository id's (@@ that's for you, Jeff ;-)
  //

  // A few notes:
  //
  // (1) There is no need to generate repid's for declarations outside
  //     PrincipalTranslationUnit (i.e. for declarations that were
  //     #includ'ed) since we normally don't generate any code for them.
  //
  //


  return true;
}
