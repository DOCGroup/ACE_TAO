// file      : CCF/IDL3/Traversal/Component.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/EventType.hpp"

#include "CCF/IDL3/Traversal/Component.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      // Component
      //
      //
      void Component::
      traverse (Type& c)
      {
        pre (c);
        name (c);
        inherits (c);
        supports (c);
        names (c);
        post (c);
      }

      void Component::
      pre (Type&)
      {
      }

      void Component::
      name (Type&)
      {
      }

      void Component::
      inherits (Type& c, EdgeDispatcherBase& d)
      {
        if (SemanticGraph::Inherits* inh  = c.inherits ())
        {
          d.traverse (*inh);
        }
      }

      void Component::
      inherits (Type& c)
      {
        if (SemanticGraph::Inherits* inh  = c.inherits ())
        {
          inherits_pre (c);
          edge_traverser ().traverse (*inh);
          inherits_post (c);
        }
        else
        {
          inherits_none (c);
        }
      }

      void Component::
      inherits_pre (Type&)
      {
      }

      void Component::
      inherits_post (Type&)
      {
      }

      void Component::
      inherits_none (Type&)
      {
      }

      void Component::
      supports (Type& c, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (c.supports_begin (), c.supports_end (), d);
      }

      void Component::
      supports (Type& c)
      {
        Type::SupportsIterator b (c.supports_begin ()), e (c.supports_end ());

        if (b != e)
        {
          supports_pre (c);
          iterate_and_traverse (
            b, e, edge_traverser (), *this, &Component::comma, c);
          supports_post (c);
        }
        else
        {
          supports_none (c);
        }
      }

      void Component::
      supports_pre (Type&)
      {
      }

      void Component::
      supports_post (Type&)
      {
      }

      void Component::
      supports_none (Type&)
      {
      }

      void Component::
      post (Type&)
      {
      }

      void Component::
      comma (Type&)
      {
      }
    }
  }
}
