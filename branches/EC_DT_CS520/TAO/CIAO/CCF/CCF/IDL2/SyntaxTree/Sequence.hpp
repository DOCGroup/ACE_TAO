// file      : CCF/IDL2/SyntaxTree/Sequence.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_SEQUENCE_HPP
#define CCF_IDL2_SYNTAX_TREE_SEQUENCE_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      //
      //
      //
      class SequenceDecl : public virtual TypeDecl
      {
      protected:
        virtual
        ~SequenceDecl () throw () {}

        SequenceDecl (ScopedName const& type)
            : type_ (table (), type)
        {
          type_info (static_type_info ());
        }

        // This c-tor is never called.
        //
        SequenceDecl ();

      public:
        TypeDeclPtr
        type () const
        {
          return type_.resolve ();
        }

      public:
        virtual bool
        complete () const
        {
          // Sequence is complete if and only if its type is complete.
          //
          return type ()->complete ();
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
      };

      typedef
      StrictPtr<SequenceDecl>
      SequenceDeclPtr;



      //
      //
      //
      class UnboundedSequenceDecl : public virtual SequenceDecl
      {
      public:
        virtual
        ~UnboundedSequenceDecl () throw () {}

        UnboundedSequenceDecl (SimpleName const& name,
                               Order const& order,
                               ScopePtr const& scope,
                               ScopedName const& type)
            : Declaration (name, order, scope),
              SequenceDecl (type)
        {
          type_info (static_type_info ());
        }

      protected:
        UnboundedSequenceDecl (SimpleName const& name,
                               Order const& order,
                               ScopePtr const& scope,
                               ContextHolderPtr const& ch,
                               ScopedName const& type)
            : Node (ch),
              Declaration (name, order, scope),
              SequenceDecl (type)
        {
          type_info (static_type_info ());
        }

      public:
        virtual TypeDeclPtr
        clone_typedef_temporary (SimpleName const& name,
                                 Order const& order,
                                 ScopePtr const& scope,
                                 ContextHolderPtr const& ch)
        {
          return TypeDeclPtr (new UnboundedSequenceDecl (
                                name, order, scope, ch, type ()->name ()));
        }


      public:
        virtual std::string
        declaration_class ()
        {
          return "unbounded sequence";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<UnboundedSequenceDecl>
      UnboundedSequenceDeclPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_SEQUENCE_HPP
