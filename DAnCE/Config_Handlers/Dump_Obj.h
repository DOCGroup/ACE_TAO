//==================================================================
/**
*  @file  Dump_Obj.h
*
* $Id$
*
*  @author Gan Deng <gan.deng@vanderbilt.edu>
*/
//=====================================================================

#ifndef CIAO_DUMP_OBJ_H
#define CIAO_DUMP_OBJ_H
#include /**/ "ace/pre.h"

#include "Deployment/DeploymentC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Config_Handlers_Export.h"
#include "tao/AnyTypeCode/Any.h"
#include <string>
#include <map>

namespace Deployment
{
/*
*  @brief class Dump_Obj handles the indentation and keeps a map of
* structures for reference look-up.
*/
class Config_Handlers_Export Dump_Obj
{
  public:
  /// Exception thrown when there is a node with the same name in the
  /// map
  class Node_Exist { };

  /// Exception thrown when there is no node with the name given in
  /// the map
  class Node_Not_Exist { };

  /// Constructors prints out the caption and increases indent
  Dump_Obj (const char *caption);

  /// Additionally insets @a val into the map
  Dump_Obj (const char* caption, CORBA::Any &val);

  /// Destructor
  ~Dump_Obj (void);

  /// return the indent string
  static const char* indent()
  {
    return indent_.c_str();
  }

  static CORBA::Any& desc(const char* caption)
  {
  return desc_map_[caption];
  }

private:
  /// indent string
  static std::string indent_;

  /// map for reference lookup
  static std::map<std::string, CORBA::Any> desc_map_;

  /// descriptor - used by destructor for deleting the
  /// correct descriptor from the map
  std::string desc_;
};
}

#include /**/ "ace/post.h"
#endif /*CIAO_DUMP_OBJ_H*/
