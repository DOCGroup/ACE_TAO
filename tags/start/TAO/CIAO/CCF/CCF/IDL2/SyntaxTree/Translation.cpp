// file      : CCF/IDL2/SyntaxTree/Translation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL2/SyntaxTree/Translation.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      // FileScope
      //
      //
      namespace
      {
        TypeInfo
        file_scope_init_ ()
        {
          TypeInfo ti (typeid (FileScope));
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo file_scope_ (file_scope_init_ ());
      }

      TypeInfo const& FileScope::
      static_type_info () { return file_scope_; }

      FileScope::
      FileScope (DeclarationTable& table, Order const& order)
          : Declaration (ScopedName (""), order, table),
            Scope (table, ScopedName (""), order)
      {
        type_info (static_type_info ());
      }


      // TranslationRegion
      //
      //
      namespace
      {
        TypeInfo
        translation_region_init_ ()
        {
          TypeInfo ti (typeid (TranslationRegion));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo translation_region_ (translation_region_init_ ());
      }

      TypeInfo const& TranslationRegion::
      static_type_info () { return translation_region_; }

      void TranslationRegion::
      insert (TranslationRegionPtr const& tr) throw (InvalidArgument)
      {
        if (tr == 0) throw InvalidArgument ();

        region_list_.push_back (tr);
      }

      TranslationRegion::Iterator TranslationRegion::
      begin () const
      {
        return region_list_.begin ();
      }

      TranslationRegion::Iterator TranslationRegion::
      end () const
      {
        return region_list_.end ();
      }


      // IncludeTranslationRegion
      //
      //
      namespace
      {
        TypeInfo
        include_translation_region_init_ ()
        {
          TypeInfo ti (typeid (IncludeTranslationRegion));
          ti.add_base (Access::PUBLIC,
                       true,
                       TranslationRegion::static_type_info ());
          return ti;
        }

        TypeInfo include_translation_region_ (
          include_translation_region_init_ ());
      }

      TypeInfo const& IncludeTranslationRegion::
      static_type_info () { return include_translation_region_; }


      // UserIncludeTranslationRegion
      //
      //
      namespace
      {
        TypeInfo
        user_include_translation_region_init_ ()
        {
          TypeInfo ti (typeid (UserIncludeTranslationRegion));
          ti.add_base (Access::PUBLIC,
                       true,
                       IncludeTranslationRegion::static_type_info ());
          return ti;
        }

        TypeInfo user_include_translation_region_ (
          user_include_translation_region_init_ ());
      }

      TypeInfo const& UserIncludeTranslationRegion::
      static_type_info () { return user_include_translation_region_; }


      // SysIncludeTranslationRegion
      //
      //
      namespace
      {
        TypeInfo
        sys_include_translation_region_init_ ()
        {
          TypeInfo ti (typeid (SysIncludeTranslationRegion));
          ti.add_base (Access::PUBLIC,
                       true,
                       IncludeTranslationRegion::static_type_info ());
          return ti;
        }

        TypeInfo sys_include_translation_region_ (
          sys_include_translation_region_init_ ());
      }

      TypeInfo const& SysIncludeTranslationRegion::
      static_type_info () { return sys_include_translation_region_; }


      // ImpliedIncludeTranslationRegion
      //
      //
      namespace
      {
        TypeInfo
        implied_include_translation_region_init_ ()
        {
          TypeInfo ti (typeid (ImpliedIncludeTranslationRegion));
          ti.add_base (Access::PUBLIC,
                       true,
                       IncludeTranslationRegion::static_type_info ());
          return ti;
        }

        TypeInfo implied_include_translation_region_ (
          implied_include_translation_region_init_ ());
      }

      TypeInfo const& ImpliedIncludeTranslationRegion::
      static_type_info () { return implied_include_translation_region_; }


      // PrincipalTranslationRegion
      //
      //
      namespace
      {
        TypeInfo
        principal_translation_region_init_ ()
        {
          TypeInfo ti (typeid (PrincipalTranslationRegion));
          ti.add_base (Access::PUBLIC,
                       true,
                       TranslationRegion::static_type_info ());
          return ti;
        }

        TypeInfo principal_translation_region_ (
          principal_translation_region_init_ ());
      }

      TypeInfo const& PrincipalTranslationRegion::
      static_type_info () { return principal_translation_region_; }


      // TranslationUnit
      //
      //
      namespace
      {
        TypeInfo
        translation_unit_init_ ()
        {
          TypeInfo ti (typeid (TranslationUnit));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo translation_unit_ (translation_unit_init_ ());
      }

      TypeInfo const& TranslationUnit::
      static_type_info () { return translation_unit_; }

      void TranslationUnit::
      insert (TranslationRegionPtr const& tr) throw (InvalidArgument)
      {
        if (tr == 0) throw InvalidArgument ();

        region_list_.push_back (tr);
      }

      TranslationUnit::Iterator TranslationUnit::
      begin () const
      {
        return region_list_.begin ();
      }

      TranslationUnit::Iterator TranslationUnit::
      end () const
      {
        return region_list_.end ();
      }
    }
  }
}
