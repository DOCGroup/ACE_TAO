// file      : CCF/CIDL/SyntaxTree/Composition.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SYNTAX_TREE_COMPOSITION_HPP
#define CCF_CIDL_SYNTAX_TREE_COMPOSITION_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SyntaxTree
    {
      using namespace IDL2::SyntaxTree;

      //
      //
      //
      class Composition : public virtual Scope
      {
      public:
        struct Category
        {
          enum Value
          {
            ENTITY,
            PROCESS,
            SERVICE,
            SESSION
          };

          friend std::ostream&
          operator<< (std::ostream& o, Value v)
          {
            if (v == ENTITY) o << "entity";
            else if (v == PROCESS) o << "process";
            else if (v == SERVICE) o << "service";
            else o << "session";
            return o;
          }
        };

      public:
        virtual
        ~Composition () throw () {}

        Composition (SimpleName const& name,
                     Category::Value category,
                     ScopePtr const& scope)
            : Declaration (name, scope),
              Scope (name, scope),
              category_ (category)
        {
          type_info (static_type_info ());
        }

      public:
        Category::Value
        category () const
        {
          return category_;
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "composition";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        Category::Value category_;
      };

      typedef
      StrictPtr<Composition>
      CompositionPtr;

      typedef
      DeclarationOrderComparator<CompositionPtr>
      CompositionOrderComparator;
    }
  }
}

#endif  // CCF_CIDL_SYNTAX_TREE_COMPOSITION_HPP
