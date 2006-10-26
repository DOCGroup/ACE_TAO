/* -*- C++ -*- */
/**
 *  $Id$
 */

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
  Test_Saver();

   virtual bool begin_object (CORBA::Long id,
      const ACE_CString &type,
      const TAO_Notify::NVPList& attrs,
      bool changed
      ACE_ENV_ARG_DECL);

    virtual void end_object (CORBA::Long id,
      const ACE_CString &type
      ACE_ENV_ARG_DECL);
};


#include /**/ "ace/post.h"
#endif /* TEST_SAVER_H */
