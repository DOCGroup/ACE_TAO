// file      : CCF/IDL2/Traversal/BuiltIn.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_BUILT_IN_HPP
#define CCF_IDL2_TRAVERSAL_BUILT_IN_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/BuiltIn.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {

      //
      //
      //
      struct BuiltInTypeDef : Traverser
      {
        typedef
        SyntaxTree::BuiltInTypeDefPtr
        NodePtr;

        BuiltInTypeDef ()
        {
          map (typeid (SyntaxTree::BuiltInTypeDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::BuiltInTypeDef> ());
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };


      //
      //
      //
      struct Void : Traverser
      {
        typedef
        SyntaxTree::VoidPtr
        NodePtr;

        Void ()
        {
          map (typeid (SyntaxTree::Void), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Void> ());
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };


      //
      //
      //
      struct Long : Traverser
      {
        typedef
        SyntaxTree::LongPtr
        NodePtr;

        Long ()
        {
          map (typeid (SyntaxTree::Long), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Long> ());
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };


      //
      //
      //
      struct Boolean : Traverser
      {
        typedef
        SyntaxTree::BooleanPtr
        NodePtr;

        Boolean ()
        {
          map (typeid (SyntaxTree::Boolean), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Boolean> ());
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };


      //
      //
      //
      struct String : Traverser
      {
        typedef
        SyntaxTree::StringPtr
        NodePtr;

        String ()
        {
          map (typeid (SyntaxTree::String), this);
        }


        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::String> ());
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_BUILT_IN_HPP
