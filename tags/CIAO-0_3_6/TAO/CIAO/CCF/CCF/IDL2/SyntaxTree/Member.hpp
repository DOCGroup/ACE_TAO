// file      : CCF/IDL2/SyntaxTree/Member.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_MEMBER_HPP
#define CCF_IDL2_SYNTAX_TREE_MEMBER_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      class MemberDecl : public virtual Declaration
      {
      public:
        virtual
        ~MemberDecl () throw () {}

        MemberDecl (SimpleName const& name,
                    ScopedName const& type,
                    ScopePtr const& scope)
            : Declaration (name, scope),
              type_ (table (), type)
          
        {
          type_info (static_type_info ());
        }
        
      public:
        TypeDeclPtr
        type () const
        {
          return type_.resolve ();
        }
        
      public:
        virtual std::string
        declaration_class ()
        {
          return "member";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
      };

      typedef
      StrictPtr<MemberDecl>
      MemberDeclPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_MEMBER_HPP
