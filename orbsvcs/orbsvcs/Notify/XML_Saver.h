// -*- C++ -*-

//=============================================================================
/**
 *  @file    XML_Saver.h
 *
 *  $Id$
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef XML_SAVER_H
#define XML_SAVER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/Topology_Saver.h"

#include "ace/streams.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

/**
 * \brief Save Notification Service Topology to an XML file.
 */
class XML_Saver : public Topology_Saver
{
public:
  /// Construct an XML_Saver.
  /// Initialization is deferred to "open()"
  XML_Saver (bool timestamp = true);

  virtual ~XML_Saver ();

  /// Open the output file.
  /// \param file_name the fully qualified file name
  /// \return true if successful
  bool open (const ACE_TString & file_name, size_t backup_count);

  //////////////////////////////////
  // Override Topology_Saver methods
  // see Topology_Saver.h for doc
  virtual bool begin_object (CORBA::Long id,
    const ACE_CString& type,
    const NVPList& attrs,
    bool changed);

  virtual void end_object (CORBA::Long id,
    const ACE_CString& type);

  virtual void close (void);

private:
  void backup_file_name (ACE_TCHAR * file_path, size_t nfile);

private:
  /// A stream representing our current output.
  FILE * output_;
  bool close_out_;

  /// the name of the output file
  ACE_TString base_name_;
  size_t backup_count_;

  /// true to enable timestamping
  bool timestamp_;

  /// A string consisting of spaces that is our current indentation level.
  ACE_CString indent_;

};

} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* XML_SAVER_H */
