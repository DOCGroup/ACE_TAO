// file      : CCF/IDL2/SyntaxTree/BuiltIn.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/BuiltIn.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      // BuiltInTypeDef
      //
      //
      namespace
      {
        TypeInfo
        built_in_type_def_init_ ()
        {
          TypeInfo ti (typeid (BuiltInTypeDef));
          ti.add_base (Access::PUBLIC, true, TypeDef::static_type_info ());
          return ti;
        }

        TypeInfo built_in_type_def_ (built_in_type_def_init_ ());
      }

      TypeInfo const& BuiltInTypeDef::
      static_type_info () { return built_in_type_def_; }


      // Void
      //
      //
      namespace
      {
        TypeInfo
        void_init_ ()
        {
          TypeInfo ti (typeid (Void));
          ti.add_base (Access::PUBLIC,
                       true,
                       BuiltInTypeDef::static_type_info ());
          return ti;
        }

        TypeInfo void_ (void_init_ ());
      }

      TypeInfo const& Void::
      static_type_info () { return void_; }


      // Long
      //
      //
      namespace
      {
        TypeInfo
        long_init_ ()
        {
          TypeInfo ti (typeid (Long));
          ti.add_base (Access::PUBLIC,
                       true,
                       BuiltInTypeDef::static_type_info ());
          return ti;
        }

        TypeInfo long_ (long_init_ ());
      }

      TypeInfo const& Long::
      static_type_info () { return long_; }


      // Boolean
      //
      //
      namespace
      {
        TypeInfo
        boolean_init_ ()
        {
          TypeInfo ti (typeid (Boolean));
          ti.add_base (Access::PUBLIC,
                       true,
                       BuiltInTypeDef::static_type_info ());
          return ti;
        }

        TypeInfo boolean_ (boolean_init_ ());
      }

      TypeInfo const& Boolean::
      static_type_info () { return boolean_; }


      // String
      //
      //
      namespace
      {
        TypeInfo
        string_init_ ()
        {
          TypeInfo ti (typeid (String));
          ti.add_base (Access::PUBLIC,
                       true,
                       BuiltInTypeDef::static_type_info ());
          return ti;
        }

        TypeInfo string_ (string_init_ ());
      }

      TypeInfo const& String::
      static_type_info () { return string_; }
    }
  }
}
