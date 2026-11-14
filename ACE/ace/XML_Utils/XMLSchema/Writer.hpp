// file      : XMLSchema/Writer.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef XMLSCHEMA_WRITER_HPP
#define XMLSCHEMA_WRITER_HPP

#include <sstream>

#include <ace/XML_Utils/XSCRT/Writer.hpp>
#include <ace/XML_Utils/XMLSchema/Types.hpp>
#include <ace/XML_Utils/XMLSchema/Traversal.hpp>

namespace XMLSchema
{
  namespace Writer
  {
    template <typename T, typename C>
    struct FundamentalType : Traversal::Traverser<T>, virtual XSCRT::Writer<C>
    {
      explicit FundamentalType (XSCRT::XML::Element<C> &e)
        : XSCRT::Writer<C> (e)
      {
      }

      using XSCRT::Writer<C>::top_;
      using XSCRT::Writer<C>::attr_;

      void traverse (T const &o) override
      {
        std::basic_ostringstream<C> os;
        os << o;

        XSCRT::XML::Attribute<C> *const a (attr_ ());
        if (a != nullptr)
          a->value (os.str ());
        else
          top_ ().value (os.str ());
      }

    protected:
      void traverse (T &t) override
      {
        Traversal::Traverser<T>::traverse (t);
      }

      FundamentalType () = default;
    };

    template <typename C>
    struct FundamentalType<XSCRT::FundamentalType<bool>, C>
      : Traversal::Traverser<XSCRT::FundamentalType<bool>>
      , virtual XSCRT::Writer<C>
    {
      explicit FundamentalType (XSCRT::XML::Element<C> &e)
        : XSCRT::Writer<C> (e)
      {
      }

      using XSCRT::Writer<C>::top_;
      using XSCRT::Writer<C>::attr_;

      void traverse (XSCRT::FundamentalType<bool> const &o) override
      {
        std::basic_ostringstream<C> os;
        if (o)
          os << "true";
        else
          os << "false";

        XSCRT::XML::Attribute<C> *const a (attr_ ());
        if (a != nullptr)
          a->value (os.str ());
        else
          top_ ().value (os.str ());
      }

    protected:
      void traverse (XSCRT::FundamentalType<bool> &t) override
      {
        Traversal::Traverser<XSCRT::FundamentalType<bool>>::traverse (t);
      }

      FundamentalType () = default;
    };

    template <typename C>
    struct IDREF : Traversal::Traverser<XMLSchema::IDREF<C>>, virtual XSCRT::Writer<C>
    {
      explicit IDREF (XSCRT::XML::Element<C> &e)
        : XSCRT::Writer<C> (e)
      {
      }

      void traverse (typename Traversal::Traverser<XMLSchema::IDREF<C>>::Type const &o) override
      {
        XSCRT::XML::Attribute<C> *const a (XSCRT::Writer<C>::attr_ ());
        if (a != nullptr)
          a->value (o.id ());
        else
          XSCRT::Writer<C>::top_ ().value (o.id ());
      }

    protected:
      void traverse (typename Traversal::Traverser<XMLSchema::IDREF<C>>::Type &o) override
      {
        Traversal::Traverser<XMLSchema::IDREF<C>>::traverse (o);
      }

      IDREF () = default;
    };
  }
}

#endif  // XMLSCHEMA_WRITER_HPP
