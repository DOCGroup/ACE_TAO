// file      : CCF/IDL3/Traversal/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL3/Traversal/Operation.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      // ProvidesDecl
      //
      //

      void ProvidesDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }


      void ProvidesDecl::
      pre (NodePtr const&)
      {
      }

      void ProvidesDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void ProvidesDecl::
      post (NodePtr const&)
      {
      }


      // UsesDecl
      //
      //

      void UsesDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }


      void UsesDecl::
      pre (NodePtr const&)
      {
      }

      void UsesDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void UsesDecl::
      post (NodePtr const&)
      {
      }


      // PublishesDecl
      //
      //

      void PublishesDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }


      void PublishesDecl::
      pre (NodePtr const&)
      {
      }

      void PublishesDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void PublishesDecl::
      post (NodePtr const&)
      {
      }


      // EmitsDecl
      //
      //

      void EmitsDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }


      void EmitsDecl::
      pre (NodePtr const&)
      {
      }

      void EmitsDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void EmitsDecl::
      post (NodePtr const&)
      {
      }


      // ConsumesDecl
      //
      //

      void ConsumesDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }


      void ConsumesDecl::
      pre (NodePtr const&)
      {
      }

      void ConsumesDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void ConsumesDecl::
      post (NodePtr const&)
      {
      }

      // HomeFactoryDecl
      //
      //

      void HomeFactoryDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          name (n);
          parameters (n);
          post (n);
        }
      }


      void HomeFactoryDecl::
      pre (NodePtr const&)
      {
      }

      void HomeFactoryDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void HomeFactoryDecl::
      name (NodePtr const&)
      {
      }

      void HomeFactoryDecl::
      parameters (NodePtr const& n)
      {
        parameter_delegate (n);
      }

      void HomeFactoryDecl::
      post (NodePtr const&)
      {
      }      
    }
  }
}
