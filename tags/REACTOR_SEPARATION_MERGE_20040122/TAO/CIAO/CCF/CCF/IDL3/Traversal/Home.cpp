// file      : CCF/IDL3/Traversal/Home.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/Traversal/Home.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      // Home
      //
      //
      void Home::
      traverse (Type& h)
      {
        pre (h);
        name (h);
        inherits (h);
        supports (h);
        manages (h);
        names (h);
        post (h);
      }

      void Home::
      pre (Type&)
      {
      }

      void Home::
      name (Type&)
      {
      }

      void Home::
      inherits (Type& h, EdgeDispatcherBase& d)
      {
        if (SemanticGraph::Inherits* inh  = h.inherits ())
        {
          d.traverse (*inh);
        }
      }

      void Home::
      inherits (Type& h)
      {
        if (SemanticGraph::Inherits* inh  = h.inherits ())
        {
          inherits_pre (h);
          edge_traverser ().traverse (*inh);
          inherits_post (h);
        }
        else
        {
          inherits_none (h);
        }
      }

      void Home::
      inherits_pre (Type&)
      {
      }

      void Home::
      inherits_post (Type&)
      {
      }

      void Home::
      inherits_none (Type&)
      {
      }

      void Home::
      supports (Type& h, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (h.supports_begin (), h.supports_end (), d);
      }

      void Home::
      supports (Type& h)
      {
        Type::SupportsIterator b (h.supports_begin ()), e (h.supports_end ());

        if (b != e)
        {
          supports_pre (h);
          iterate_and_traverse (
            b, e, edge_traverser (), *this, &Home::comma, h);
          supports_post (h);
        }
        else
        {
          supports_none (h);
        }
      }

      void Home::
      supports_pre (Type&)
      {
      }

      void Home::
      supports_post (Type&)
      {
      }

      void Home::
      supports_none (Type&)
      {
      }

      void Home::
      manages (Type& h, EdgeDispatcherBase& d)
      {
        d.traverse (h.manages ());
      }

      void Home::
      manages (Type& h)
      {
        manages_pre (h);
        manages (h, edge_traverser ());
        manages_post (h);
      }

      void Home::
      manages_pre (Type&)
      {
      }

      void Home::
      manages_post (Type&)
      {
      }

      void Home::
      post (Type&)
      {
      }

      void Home::
      comma (Type&)
      {
      }

/*
      // HomeFactory
      //
      //
      void HomeFactory::
      traverse (Type& hf)
      {
        pre (hf);
        returns (hf);
        name (hf);
        receives (hf);
        raises (hf);
        post (hf);
      }

      void HomeFactory::
      pre (Type&)
      {
      }

      void HomeFactory::
      returns (Type& hf, EdgeDispatcherBase& d)
      {
        d.traverse (hf.returns ());
      }

      void HomeFactory::
      returns (Type& hf)
      {
        returns (hf, edge_traverser ());
      }

      void HomeFactory::
      name (Type&)
      {
      }

      void HomeFactory::
      receives (Type& hf, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (hf.receives_begin (), hf.receives_end (), d);
      }

      void HomeFactory::
      receives (Type& hf)
      {
        receives_pre (hf);
        iterate_and_traverse (hf.receives_begin (),
                              hf.receives_end (),
                              edge_traverser (),
                              *this,
                              &HomeFactory::comma,
                              hf);
        receives_post (hf);
      }

      void HomeFactory::
      receives_pre (Type&)
      {
      }

      void HomeFactory::
      receives_post (Type&)
      {
      }

      void HomeFactory::
      raises (Type& hf, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (hf.raises_begin (), hf.raises_end (), d);
      }

      void HomeFactory::
      raises (Type& hf)
      {
        Type::RaisesIterator b (hf.raises_begin ()), e (hf.raises_end ());

        if (b != e)
        {
          raises_pre (hf);
          iterate_and_traverse (
            b, e, edge_traverser (), *this, &HomeFactory::comma, hf);
          raises_post (hf);
        }
        else
        {
          raises_none (hf);
        }
      }

      void HomeFactory::
      raises_pre (Type&)
      {
      }

      void HomeFactory::
      raises_post (Type&)
      {
      }

      void HomeFactory::
      raises_none (Type&)
      {
      }

      void HomeFactory::
      post (Type&)
      {
      }

      void HomeFactory::
      comma (Type&)
      {
      }

      // HomeFinder
      //
      //
      void HomeFinder::
      traverse (Type& hf)
      {
        pre (hf);
        returns (hf);
        name (hf);
        receives (hf);
        raises (hf);
        post (hf);
      }

      void HomeFinder::
      pre (Type&)
      {
      }

      void HomeFinder::
      returns (Type& hf, EdgeDispatcherBase& d)
      {
        d.traverse (hf.returns ());
      }

      void HomeFinder::
      returns (Type& hf)
      {
        returns (hf, edge_traverser ());
      }

      void HomeFinder::
      name (Type&)
      {
      }

      void HomeFinder::
      receives (Type& hf, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (hf.receives_begin (), hf.receives_end (), d);
      }

      void HomeFinder::
      receives (Type& hf)
      {
        receives_pre (hf);
        iterate_and_traverse (hf.receives_begin (),
                              hf.receives_end (),
                              edge_traverser (),
                              *this,
                              &HomeFinder::comma,
                              hf);
        receives_post (hf);
      }

      void HomeFinder::
      receives_pre (Type&)
      {
      }

      void HomeFinder::
      receives_post (Type&)
      {
      }

      void HomeFinder::
      raises (Type& hf, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (hf.raises_begin (), hf.raises_end (), d);
      }

      void HomeFinder::
      raises (Type& hf)
      {
        Type::RaisesIterator b (hf.raises_begin ()), e (hf.raises_end ());

        if (b != e)
        {
          raises_pre (hf);
          iterate_and_traverse (
            b, e, edge_traverser (), *this, &HomeFinder::comma, hf);
          raises_post (hf);
        }
        else
        {
          raises_none (hf);
        }
      }

      void HomeFinder::
      raises_pre (Type&)
      {
      }

      void HomeFinder::
      raises_post (Type&)
      {
      }

      void HomeFinder::
      raises_none (Type&)
      {
      }

      void HomeFinder::
      post (Type&)
      {
      }

      void HomeFinder::
      comma (Type&)
      {
      }
*/
    }
  }
}
