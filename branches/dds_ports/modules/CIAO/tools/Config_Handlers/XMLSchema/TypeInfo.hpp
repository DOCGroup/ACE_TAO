// file      : XMLSchema/TypeInfo.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef XMLSCHEMA_TYPE_INFO_HPP
#define XMLSCHEMA_TYPE_INFO_HPP

#include <XSCRT/ExtendedTypeInfo.hpp>

namespace XMLSchema
{
  template <typename C>
  struct TypeInfoInitializer
  {
    TypeInfoInitializer (XSCRT::ExtendedTypeInfoMap&);
  };
}

#include <XMLSchema/TypeInfo.ipp>
#include <XMLSchema/TypeInfo.tpp>

#endif  // XMLSCHEMA_TYPE_INFO_HPP
