// file      : CCF/CIDL/SyntaxTree/HomeExecutor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SYNTAX_TREE_HOME_EXECUTOR_HPP
#define CCF_CIDL_SYNTAX_TREE_HOME_EXECUTOR_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"
#include "CCF/IDL3/SyntaxTree/Home.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SyntaxTree
    {
      using namespace IDL3::SyntaxTree;

      //
      //
      //
      class HomeExecutor : public virtual Scope
      {
      public:
        virtual
        ~HomeExecutor () throw () {}

        HomeExecutor (SimpleName const& name,
                      ScopePtr const& scope,
                      ScopedName implements,
                      SimpleName manages)
            : Declaration (name, scope),
              Scope (name, scope),
              implements_ (scope->table (), implements),
              manages_ (manages)
        {
          type_info (static_type_info ());
        }

      public:
        HomeDefPtr
        implements () const
        {
          return implements_.resolve ();
        }

        SimpleName
        manages () const
        {
          return manages_;
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "home executor";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        HomeDefRef implements_;
        SimpleName manages_;
      };

      typedef
      StrictPtr<HomeExecutor>
      HomeExecutorPtr;
    }
  }
}

#endif  // CCF_CIDL_SYNTAX_TREE_HOME_EXECUTOR_HPP
