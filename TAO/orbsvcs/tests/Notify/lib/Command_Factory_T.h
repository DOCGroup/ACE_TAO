/* -*- C++ -*- */
/**
 *  @file Command_Factory_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_COMMAND_FACTORY_T_H
#define TAO_COMMAND_FACTORY_T_H
#include /**/ "ace/pre.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "Command_Factory.h"

class TAO_Notify_Tests_Command;

/**
 * @class TAO_Command_Factory_T
 *
 * @brief Template class to create COMMAND specific factories.
 */
template <class COMMAND>
class TAO_Notify_Tests_Command_Factory_T : public TAO_Notify_Tests_Command_Factory, public ACE_Service_Object
{
public:
  /// Constructor
  TAO_Notify_Tests_Command_Factory_T ();

  /// Destructor
  ~TAO_Notify_Tests_Command_Factory_T ();

  /// = Service_Object virtual method overloads.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini ();

  /// Create the <COMMAND> command.
  TAO_Notify_Tests_Command* create ();
};

#include "Command_Factory_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_COMMAND_FACTORY_T_H */
