// file      : CCF/IDL2/Traversal/Sequence.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_SEQUENCE_HPP
#define CCF_IDL2_TRAVERSAL_SEQUENCE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Sequence.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      //
      //
      //
      struct SequenceDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::SequenceDeclPtr
        NodePtr;

        SequenceDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::SequenceDecl), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::SequenceDecl> ());
	  return true;
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };


      //
      //
      //
      struct UnboundedSequenceDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::UnboundedSequenceDeclPtr
        NodePtr;

        UnboundedSequenceDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::UnboundedSequenceDecl), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::UnboundedSequenceDecl> ());
	  return true;
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_SEQUENCE_HPP
