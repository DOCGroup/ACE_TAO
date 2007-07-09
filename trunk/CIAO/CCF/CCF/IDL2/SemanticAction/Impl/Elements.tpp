// file      : CCF/IDL2/SemanticAction/Impl/Elements.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        template <typename T>
        T& Base::
        resolve (SemanticGraph::ScopedName const& from,
                 SemanticGraph::Name const& name,
                 Flags::Value flags)
        {
          using namespace SemanticGraph;

          try
          {
            Nameables nodes (Impl::resolve (ctx.tu (), from, name));


            if (nodes.size () != 1) throw NotUnique ();

            Nameable& node (**(nodes.begin ()));

            try
            {
              T& t (dynamic_cast<T&> (node));

              if (flags & Flags::defined)
              {
                if (!t.defined ()) throw NotDefined (node.scoped_name ());
              }

              if (flags & Flags::complete)
              {
                SemanticGraph::Type& type (
                  dynamic_cast<SemanticGraph::Type&> (t));

                if (!type.complete ())
                {
                  throw NotComplete (node.scoped_name ());
                }
              }

              return t;
            }
            catch (std::bad_cast const&)
            {
              throw WrongType (node.scoped_name ());
            }
          }
          catch (Impl::NotFound const&)
          {
            throw NotFound ();
          }
        }

        template<typename I>
        void Base::
        check_inheritance (I begin, I end, SemanticGraph::Nameable& t)
        {
          for (;begin != end; ++begin)
          {
            if ((**begin).inheritee () == t)
            {
              throw AlreadyInherited (t.scoped_name ());
            }
          }
        }

        template<typename I>
        void Base::
        check_support (I begin, I end, SemanticGraph::Nameable& t)
        {
          for (;begin != end; ++begin)
          {
            if ((**begin).supportee () == t)
            {
              throw AlreadySupported (t.scoped_name ());
            }
          }
        }
      }
    }
  }
}
