// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1756

#include "SHS_Daemon_Impl.h"

#include "DAnCE/Logger/Log_Macros.h"

#include <sstream>

namespace DAnCE
{
  // Implementation skeleton constructor
  SHS_Daemon_i::SHS_Daemon_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  // Implementation skeleton destructor
  SHS_Daemon_i::~SHS_Daemon_i (void)
  {
  }

  void
  SHS_Daemon_i::update (const ::DAnCE::SHS::Status_Update & update)
  {
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("SHS_Daemon_i::update - ")
                     ACE_TEXT ("Received status update from <%C>\n"),
                     update.id.in ()));
    
    std::stringstream str;
    
    str << "Instance with ID <" << update.id.in () << "> "
        << "and type <" << update.type.in () << ">: ";

    switch (update.new_status)
      {
      case ::DAnCE::SHS::INSTALLED:
        str << "INSTALLED.";
        break;

      case ::DAnCE::SHS::ACTIVE:
        str << "ACTIVE.";
        break;
        
      case ::DAnCE::SHS::PASSIVE:
        str << "PASSIVE.";
        break;
        
      case ::DAnCE::SHS::ERROR:
        str << "ERROR.";
        break;
        
      case ::DAnCE::SHS::DEFUNCT:
        str << "DEFUNCT.";
        break;
      }
    
    std::cerr << str.str () << std::endl;
  }

  void SHS_Daemon_i::shutdown (void)
  {
    this->orb_->shutdown ();
  }

}
