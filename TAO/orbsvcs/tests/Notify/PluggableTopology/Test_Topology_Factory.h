/* -*- C++ -*- */
#ifndef TEST_TOPOLOGY_FACTORY_H
#define TEST_TOPOLOGY_FACTORY_H
#include /**/ "ace/pre.h"

#include "plugtop_export.h"

#include "orbsvcs/Notify/Topology_Factory.h"

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class PLUGTOP_Export Test_Topology_Factory : public TAO_Notify::Topology_Factory
{
public:
  Test_Topology_Factory () = default;
  Test_Topology_Factory (Test_Topology_Factory const&) = delete;
  Test_Topology_Factory (Test_Topology_Factory&&) = delete;
  Test_Topology_Factory& operator= (Test_Topology_Factory const&) = delete;
  Test_Topology_Factory& operator= (Test_Topology_Factory&&) = delete;

  TAO_Notify::Topology_Saver* create_saver () override;
  TAO_Notify::Topology_Loader* create_loader () override;
};

ACE_FACTORY_DECLARE (PLUGTOP, Test_Topology_Factory)

#include /**/ "ace/post.h"
#endif /* TEST_TOPOLOGY_FACTORY_H */
