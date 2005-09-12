// file      : XMLSchema/TypeInfo.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef XMLSCHEMA_TYPE_INFO_HPP
#define XMLSCHEMA_TYPE_INFO_HPP

#include <ciao/tools/Config_Handlers/XSCRT/ExtendedTypeInfo.hpp>

namespace XMLSchema
{
  template <typename C>
  struct TypeInfoInitializer
  {
    TypeInfoInitializer (XSCRT::ExtendedTypeInfoMap&);
  };
}

#include <ciao/tools/Config_Handlers/XMLSchema/TypeInfo.ipp>
#include <ciao/tools/Config_Handlers/XMLSchema/TypeInfo.tpp>

#endif  // XMLSCHEMA_TYPE_INFO_HPP
