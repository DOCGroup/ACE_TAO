// -*- C++ -*-

//=============================================================================
/**
 *  @file    XML_Loader.h
 *
 *  $Id$
 *
 *  A topology saver class that outputs XML.
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef XML_LOADER_H
#define XML_LOADER_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Topology_Loader.h"

#include "ACEXML/common/DefaultHandler.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"

#include "ace/streams.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
 /// \brief Load Notification Service Topology from an XML file.
class XML_Loader : public ACEXML_DefaultHandler , public Topology_Loader
{
public:
  /// The constructor.
  XML_Loader ();

  virtual ~XML_Loader ();

  /// Open a file and perform preliminary validation to determine whether
  /// the file is complete and valid.
  bool open (const ACE_TString & file_name);

  ///////////////////////////////////
  // Override Topology_Loader methods
  // see Topology_Loader.h for documentation
  virtual void load (Topology_Object *root);


  /////////////////////////////////////////
  // Override ACEXML_DefaultHandler methods

  virtual void startElement (const ACEXML_Char* namespaceURI,
    const ACEXML_Char* localName,
    const ACEXML_Char* qName,
    ACEXML_Attributes* atts);

  virtual void endElement (const ACEXML_Char*,
    const ACEXML_Char*,
    const ACEXML_Char* name);

private:
  /// The name of the file from which data is read.
  ACE_TString file_name_;

  typedef ACE_Unbounded_Stack<Topology_Object*> TopoStack;
  TopoStack object_stack_;
  /// If false, then we're just checking syntax of topology file.
  bool live_;
};

} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* XML_LOADER_H */
