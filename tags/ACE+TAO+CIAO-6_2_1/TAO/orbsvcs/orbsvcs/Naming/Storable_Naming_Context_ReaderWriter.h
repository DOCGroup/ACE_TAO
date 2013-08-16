// -*- C++ -*-

//=============================================================================
/**
 * @file  Storable_Naming_Context_ReaderWriter.h
 *
 * $Id$
 *
 * @author Marina Spivak <marina@cs.wustl.edu>
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef TAO_STORABLE_NAMING_CONTEXT_READERWRITER_H
#define TAO_STORABLE_NAMING_CONTEXT_READERWRITER_H

#include /**/ "ace/pre.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Storable_Base;
}

class TAO_Storable_Naming_Context;
class TAO_NS_Persistence_Record;
class TAO_NS_Persistence_Header;
class TAO_NS_Persistence_Global;

class TAO_Storable_Naming_Context_ReaderWriter
{
public:

  TAO_Storable_Naming_Context_ReaderWriter (TAO::Storable_Base & stream);

  int read (TAO_Storable_Naming_Context & context);

  void write (TAO_Storable_Naming_Context & context);

  void write_global (const TAO_NS_Persistence_Global & global);
  void read_global (TAO_NS_Persistence_Global & global);

private:

  void write_header (const TAO_NS_Persistence_Header & header);
  void read_header (TAO_NS_Persistence_Header & header);

  void write_record (const TAO_NS_Persistence_Record & record);
  void read_record (TAO_NS_Persistence_Record & record);

  TAO::Storable_Base &stream_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_STORABLE_NAMING_CONTEXT_READERWRITER_H */
