// $Id$
#ifndef CCF_CIDL_SYNTAX_TREE_HPP
#define CCF_CIDL_SYNTAX_TREE_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL3/IDL3_SyntaxTree.hpp"

namespace CIDL
{
  namespace SyntaxTree
  {

    // Import all nodes of IDL3
    using namespace IDL3::SyntaxTree;


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
      declaration_type ()
      {
        return "composition";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      Category::Value category_;
    };

    typedef
    StrictPtr<Composition>
    CompositionPtr;

    typedef
    DeclarationOrderComparator<CompositionPtr>
    CompositionOrderComparator;

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
      declaration_type ()
      {
        return "home executor";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      HomeDefRef implements_;
      SimpleName manages_;
    };

    typedef
    StrictPtr<HomeExecutor>
    HomeExecutorPtr;
  }
}

#endif // CCF_CIDL_SYNTAX_TREE_HPP
