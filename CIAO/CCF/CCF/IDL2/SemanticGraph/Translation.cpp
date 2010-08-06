// file      : CCF/IDL2/SemanticGraph/Translation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Translation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // ContainsPrincipal
      //
      //
      namespace
      {
        TypeInfo
        contains_principal_init_ ()
        {
          TypeInfo ti (typeid (ContainsPrincipal));
          ti.add_base (Access::PUBLIC, true, Contains::static_type_info ());
          return ti;
        }

        TypeInfo contains_principal_ (contains_principal_init_ ());
      }

      TypeInfo const& ContainsPrincipal::
      static_type_info () { return contains_principal_; }


      // ContainsImplied
      //
      //
      namespace
      {
        TypeInfo
        contains_implied_init_ ()
        {
          TypeInfo ti (typeid (ContainsImplied));
          ti.add_base (Access::PUBLIC, true, Contains::static_type_info ());
          return ti;
        }

        TypeInfo contains_implied_ (contains_implied_init_ ());
      }

      TypeInfo const& ContainsImplied::
      static_type_info () { return contains_implied_; }


      // ContainsRoot
      //
      //
      namespace
      {
        TypeInfo
        contains_root_init_ ()
        {
          TypeInfo ti (typeid (ContainsRoot));
          ti.add_base (Access::PUBLIC, true, Contains::static_type_info ());
          return ti;
        }

        TypeInfo contains_root_ (contains_root_init_ ());
      }

      TypeInfo const& ContainsRoot::
      static_type_info () { return contains_root_; }


      // Includes
      //
      //
      namespace
      {
        TypeInfo
        includes_init_ ()
        {
          TypeInfo ti (typeid (Includes));
          ti.add_base (Access::PUBLIC, true, Contains::static_type_info ());
          return ti;
        }

        TypeInfo includes_ (includes_init_ ());
      }

      TypeInfo const& Includes::
      static_type_info () { return includes_; }


      // QuoteIncludes
      //
      //
      namespace
      {
        TypeInfo
        quote_includes_init_ ()
        {
          TypeInfo ti (typeid (QuoteIncludes));
          ti.add_base (Access::PUBLIC, true, Includes::static_type_info ());
          return ti;
        }

        TypeInfo quote_includes_ (quote_includes_init_ ());
      }

      TypeInfo const& QuoteIncludes::
      static_type_info () { return quote_includes_; }



      // BracketIncludes
      //
      //
      namespace
      {
        TypeInfo
        bracket_includes_init_ ()
        {
          TypeInfo ti (typeid (BracketIncludes));
          ti.add_base (Access::PUBLIC, true, Includes::static_type_info ());
          return ti;
        }

        TypeInfo bracket_includes_ (bracket_includes_init_ ());
      }

      TypeInfo const& BracketIncludes::
      static_type_info () { return bracket_includes_; }


      // Root
      //
      //
      namespace
      {
        TypeInfo
        root_init_ ()
        {
          TypeInfo ti (typeid (Root));
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo root_ (root_init_ ());
      }

      TypeInfo const& Root::
      static_type_info () { return root_; }


      // TranslationRegion
      //
      //

      Nameables TranslationRegion::
      lookup (ScopedName const& name) const
      {
        Nameables r;
        lookup (name, r);
        return r;
      }

      void TranslationRegion::
      lookup (ScopedName const& name, Nameables& result) const
      {
        bool root (name.simple ());

        Name rest (name.begin () + (root ? 0 : 1), name.end ());

        for (ContainsIterator i (contains_begin ()), end (contains_end ());
             i != end; ++i)
        {
          Node& node ((**i).element ());

          if (Root* r = dynamic_cast<Root*> (&node))
          {
            if (root)
            {
              result.insert (r);
            }
            else
            {
              r->lookup (rest, result);
            }
          }
          else
          {
            dynamic_cast<TranslationRegion&> (node).lookup (name, result);
          }
        }
      }


      namespace
      {
        TypeInfo
        translation_region_init_ ()
        {
          TypeInfo ti (typeid (TranslationRegion));
          ti.add_base (Access::PUBLIC, true, Container::static_type_info ());
          return ti;
        }

        TypeInfo translation_region_ (translation_region_init_ ());
      }

      TypeInfo const& TranslationRegion::
      static_type_info () { return translation_region_; }


      // TranslationUnit
      //
      //

      Nameables TranslationUnit::
      lookup (ScopedName const& name) const
      {
        Nameables r;

        for (ContainsIterator i (contains_begin ()), end (contains_end ());
             i != end; ++i)
        {
          TranslationRegion& tr (
            dynamic_cast<TranslationRegion&>((**i).element ()));

          tr.lookup (name, r);
        }

        return r;
      }

      namespace
      {
        TypeInfo
        translation_unit_init_ ()
        {
          TypeInfo ti (typeid (TranslationUnit));
          ti.add_base (Access::PUBLIC, true, Container::static_type_info ());
          return ti;
        }

        TypeInfo translation_unit_ (translation_unit_init_ ());
      }

      TypeInfo const& TranslationUnit::
      static_type_info () { return translation_unit_; }
    }
  }
}
