// -*- C++ -*-

//=============================================================================
/**
 *  @file    XML_Loader.h
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

#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
 /// \brief Load Notification Service Topology from an XML file.
class XML_Loader : public ACEXML_DefaultHandler , public Topology_Loader
{
public:
  /// The constructor.
  XML_Loader ();

  ~XML_Loader () override;

  XML_Loader (XML_Loader const &) = delete;
  XML_Loader (XML_Loader &&) = delete;
  XML_Loader &operator= (XML_Loader const &) = delete;
  XML_Loader &operator= (XML_Loader &&) = delete;

  /// Open a file and perform preliminary validation to determine whether
  /// the file is complete and valid.
  bool open (const ACE_TString & file_name);

  ///////////////////////////////////
  // Override Topology_Loader methods
  // see Topology_Loader.h for documentation
  void load (Topology_Object *root) override;


  /////////////////////////////////////////
  // Override ACEXML_DefaultHandler methods

  void startElement (ACEXML_Char const *namespaceURI,
    ACEXML_Char const *localName,
    ACEXML_Char const *qName,
    ACEXML_Attributes *atts) override;

  void endElement (ACEXML_Char const *,
    ACEXML_Char const *,
    ACEXML_Char const *name) override;

private:
  /// The name of the file from which data is read.
  ACE_TString file_name_;

  using TopoStack = std::vector<Topology_Object *>;
  TopoStack object_stack_;
  /// If false, then we're just checking syntax of topology file.
  bool live_ = false;
};
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* XML_LOADER_H */
