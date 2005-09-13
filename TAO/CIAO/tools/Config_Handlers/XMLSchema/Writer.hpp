// file      : XMLSchema/Writer.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef XMLSCHEMA_WRITER_HPP
#define XMLSCHEMA_WRITER_HPP

#include <sstream>

#include <XSCRT/Writer.hpp>

#include <XMLSchema/Types.hpp>
#include <XMLSchema/Traversal.hpp>

namespace XMLSchema
{
  namespace Writer
  {
    template <typename T, typename C>
    struct FundamentalType : Traversal::Traverser<T>,
                             virtual XSCRT::Writer<C>
    {
      FundamentalType (XSCRT::XML::Element<C>& e)
          : XSCRT::Writer<C> (e)
      {
      }

      using XSCRT::Writer<C>::top_;
      using XSCRT::Writer<C>::attr_;

      virtual void
      traverse (T const& o)
      {
        using namespace XSCRT::XML;

        std::basic_ostringstream<C> os;

        os << o;

        if (Attribute<C>* a = attr_ ())
        {
          a->value (os.str ());
        }
        else
        {
          top_().value (os.str ());
        }
      }

    protected:
      FundamentalType ()
      {
      }
    };


    template <typename C>
    struct IDREF : Traversal::Traverser<XMLSchema::IDREF<C> >,
                   virtual XSCRT::Writer<C>
    {
      IDREF (XSCRT::XML::Element<C>& e)
          : XSCRT::Writer<C> (e)
      {
      }

      using XSCRT::Writer<C>::top_;
      using XSCRT::Writer<C>::attr_;

      virtual void
      traverse (
        typename Traversal::Traverser<XMLSchema::IDREF<C> >::Type const& o)
      {
        using namespace XSCRT::XML;

        if (Attribute<C>* a = attr_ ())
        {
          a->value (o.id ());
        }
        else
        {
          top_().value (o.id ());
        }
      }

    protected:
      IDREF ()
      {
      }
    };
  }
}

#include <XMLSchema/Writer.ipp>
#include <XMLSchema/Writer.tpp>

#endif  // XMLSCHEMA_WRITER_HPP
