/* -*- C++ -*- */

#ifndef TEST_SAVER_H
#define TEST_SAVER_H
#include /**/ "ace/pre.h"

#include "plugtop_export.h"

#include "orbsvcs/Notify/Topology_Saver.h"

#include "tao/corba.h"
#include "ace/streams.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class PLUGTOP_Export Test_Saver : public TAO_Notify::Topology_Saver
{
public:
  Test_Saver () = default;
  Test_Saver (Test_Saver const&) = delete;
  Test_Saver (Test_Saver&&) = delete;
  Test_Saver& operator= (Test_Saver const&) = delete;
  Test_Saver& operator= (Test_Saver&&) = delete;

  bool begin_object (CORBA::Long id,
                     ACE_CString const& type,
                     TAO_Notify::NVPList const& attrs,
                     bool changed) override;

  void end_object (CORBA::Long id, ACE_CString const& type) override;
};

#include /**/ "ace/post.h"
#endif /* TEST_SAVER_H */
