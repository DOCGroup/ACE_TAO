// file      : CCF/IDL2/SyntaxTree/Module.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_SYNTAX_TREE_MODULE_HPP
#define CCF_IDL2_SYNTAX_TREE_MODULE_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      //
      // Module
      //
      class Module : public virtual Scope
      {
      public:
        virtual
        ~Module () throw () {}

        Module (SimpleName const& name, ScopePtr const& scope)
            : Declaration (name, scope),
              Scope (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "module";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<Module>
      ModulePtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_MODULE_HPP
