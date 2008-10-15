// file      : CIDLC/RepositoryIdGenerator.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "RepositoryIdGenerator.hpp"
#include "Literals.hpp"

#include "CCF/CompilerElements/Diagnostic.hpp"
#include "CCF/CIDL/Traversal.hpp"
#include "CCF/CodeGenerationKit/Regex.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;
using namespace StringLiterals;
using namespace std;

namespace
{
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

  void
  compute_new_repo_id (Nameable& d)
  {
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

    string scope_name = regex::perl_s (stripped.unescaped_str (), "%::%/%");

    string repo_id = "IDL:" + prefix + scope_name + ":1.0";

    d.context ().set<string> (STRS[REPO_ID], repo_id);
  }

  void
  compute_repo_id (Nameable& d)
  {
    if (d.context ().count (STRS[REPO_ID]))
    {
      return;
    }
    else
    {
      compute_new_repo_id (d);
    }
  }
}

namespace
{
  class TypeIdLabelGenerator : public Traversal::TypeId
  {
  public:
    TypeIdLabelGenerator (Diagnostic::Stream& dout,
                          TranslationUnit& tu)
      : dout_ (dout),
        tu_ (tu)
    {
    }

    virtual void
    traverse (SemanticGraph::TypeId& ti)
    {
      ScopedName decl_name (ti.declaration ());
      Nameables ref = tu_.lookup (decl_name);

      for (Nameables::const_iterator iter (ref.begin ());
           iter != ref.end ();
           ++iter)
      {
        Nameable *decl = *iter;

        if (decl->context ().count (STRS[TYPE_ID]))
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
          decl->context ().set (STRS[TYPE_ID], &ti);
          decl->context ().set (STRS[REPO_ID], ti.id ().literal ());
        }
      }
    }

  private:
    Diagnostic::Stream& dout_;
    SemanticGraph::TranslationUnit& tu_;
  };

  class TypePrefixLabelGenerator : public Traversal::TypePrefix
  {
  public:
    TypePrefixLabelGenerator (Diagnostic::Stream& dout,
                              TranslationUnit& tu)
      : dout_ (dout),
        tu_ (tu)
    {
    }

    virtual void
    traverse (SemanticGraph::TypePrefix& tp)
    {
      ScopedName decl_name (tp.declaration ());
      Nameables ref = tu_.lookup (decl_name);

      for (Nameables::const_iterator iter (ref.begin ());
           iter != ref.end ();
           ++iter)
      {
        Nameable *decl = *iter;
        CCF::CompilerElements::Context& ctx = decl->context ();

        if (ctx.count (STRS[TYPE_PREFIX]))
        {
          SemanticGraph::TypePrefix* prev =
            ctx.get<SemanticGraph::TypePrefix*> (STRS[TYPE_PREFIX]);

          if (prev->prefix () != tp.prefix ())
          {
            //@@ Seems tp should contain filename and line
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
          decl->context ().set (STRS[TYPE_PREFIX], &tp);

          // A typeprefix declaration could appear after the IDL
          // type declaration, so we want to update the repo id
          // every time.
          compute_new_repo_id (*decl);
        }
      }
    }

  private:
    Diagnostic::Stream& dout_;
    SemanticGraph::TranslationUnit& tu_;
  };

  class RepoIdGenerator : public Traversal::Interface,
                          public Traversal::EventType,
                          public Traversal::Home,
                          public Traversal::Component
  {
    virtual void
    traverse (SemanticGraph::Interface& i)
    {
      compute_repo_id (i);
    }

    virtual void
    traverse (SemanticGraph::EventType& e)
    {
      compute_repo_id (e);
    }

    virtual void
    traverse (SemanticGraph::Home& h)
    {
      compute_repo_id (h);
    }

    virtual void
    traverse (SemanticGraph::Component& c)
    {
      compute_repo_id (c);
    }
  };
}


bool RepositoryIdGenerator::generate (TranslationUnit& u)
{
  Diagnostic::Stream dout;

  TypeIdLabelGenerator type_id (dout, u);
  TypePrefixLabelGenerator type_prefix (dout, u);
  RepoIdGenerator repo_id;

  Traversal::TranslationUnit unit;

  Traversal::ContainsPrincipal contains_principal;
  unit.edge_traverser (contains_principal);

  Traversal::TranslationRegion region;
  contains_principal.node_traverser (region);

  Traversal::ContainsRoot contains_root;
  Traversal::Includes includes;

  region.edge_traverser (includes);
  region.edge_traverser (contains_root);

  Traversal::Root root;
  includes.node_traverser (region);
  contains_root.node_traverser (root);

  Traversal::Defines defines;
  root.edge_traverser (defines);

  Traversal::Module module;

  defines.node_traverser (module);
  defines.node_traverser (type_id);
  defines.node_traverser (type_prefix);
  defines.node_traverser (repo_id);

  module.edge_traverser (defines);

  Traversal::Interface iface;
  defines.node_traverser (iface);

  unit.traverse (u);

  if (dout.error_count () != 0) return false;

  //@@ check errors

  return true;
}
