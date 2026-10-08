// file      : XMLSchema/Traversal.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef XMLSCHEMA_TRAVERSAL_HPP
#define XMLSCHEMA_TRAVERSAL_HPP

#include <ace/XML_Utils/XSCRT/Traversal.hpp>
#include <ace/XML_Utils/XMLSchema/Types.hpp>

namespace XMLSchema
{
  namespace Traversal
  {
    // Automatic traversal of IDREFs.
    struct IDREF :
    XSCRT::Traversal::Traverser<XMLSchema::IDREF_Base, XSCRT::Type>
    {
      virtual void
      traverse (XMLSchema::IDREF_Base& r)
      {
        if (r.get ()) dispatch (*(r.get ()));
      }

      virtual void
      traverse (XMLSchema::IDREF_Base const& r)
      {
        if (r.get ()) dispatch (*(r.get ()));
      }
    };


    template <typename T>
    struct Traverser : XSCRT::Traversal::Traverser<T, XSCRT::Type>
    {
    };

    using byte = Traverser<byte>;
    using unsignedByte = Traverser<unsignedByte>;

    using short_ = Traverser<short_>;
    using unsignedShort = Traverser<unsignedShort>;

    using int_ = Traverser<int_>;
    using unsignedInt = Traverser<unsignedInt>;

    using long_ = Traverser<long_>;
    using unsignedLong = Traverser<unsignedLong>;

    using boolean = Traverser<boolean>;

    using float_ = Traverser<float_>;
    using double_ = Traverser<double_>;

    template <typename C>
    struct string : Traverser<XMLSchema::string<C> >
    {
    };

    template <typename C>
    struct ID : Traverser<XMLSchema::ID<C> >
    {
    };

    template <typename C>
    struct anyURI : Traverser <XMLSchema::anyURI<C> >
    {
    };
  }
}

#endif  // XMLSCHEMA_TRAVERSAL_HPP
