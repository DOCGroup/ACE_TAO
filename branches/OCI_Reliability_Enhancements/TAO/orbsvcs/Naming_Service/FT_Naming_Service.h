// -*- C++ -*-

//==========================================================================
/**
 *  @file    FT_Naming_Service.h
 *
 *  $Id$
 *
 *    This class provides an override for the Naming_Service class factory
 *    method: create_naming_server ()
 *
 *  @author Kevin Stanley <stanleyk@ociweb.com>
 */
//==========================================================================


#ifndef TAO_FT_NAMING_SERVICE_H
#define TAO_FT_NAMING_SERVICE_H

#include /**/ "Naming_Service.h"

#include "tao/ORB.h"

class TAO_Naming_Server;

/**
 * @class TAO_FT_Naming_Service
 *
 * @brief Defines a class that encapsulates the implementation of the
 * Fault Tolerant Naming Service.
 *
 * This class overrides the create_naming_server factory method inherited
 * from <TAO_Naming_Service>.
 */
class TAO_FT_Naming_Service : public TAO_Naming_Service
{
public:
  /// Default Constructor.
  TAO_FT_Naming_Service (void);

  /// Constructor taking the command-line arguments.
  TAO_FT_Naming_Service (int argc, ACE_TCHAR* argv[]);

  /// Factory method to create a server object for the naming service
  virtual TAO_Naming_Server* create_naming_server ();

  /// Destructor.
  virtual ~TAO_FT_Naming_Service (void);

};

#endif /* TAO_FT_NAMING_SERVICE_H */
