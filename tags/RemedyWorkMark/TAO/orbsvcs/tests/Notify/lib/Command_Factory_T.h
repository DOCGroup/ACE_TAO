/* -*- C++ -*- */
/**
 *  @file Command_Factory_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
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
 *
 */
#ifdef HPUX_11
   // Remove clash with /usr/include/machine/cpu.h and /usr/include/pa/cpu.h
#undef COMMAND
#endif
template <class COMMAND>
class TAO_Notify_Tests_Command_Factory_T : public TAO_Notify_Tests_Command_Factory, public ACE_Service_Object
{
public:
  /// Constructor
  TAO_Notify_Tests_Command_Factory_T (void);

  /// Destructor
  ~TAO_Notify_Tests_Command_Factory_T ();

  /// = Service_Object virtual method overloads.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini (void);

  /// Create the <COMMAND> command.
  TAO_Notify_Tests_Command* create (void);
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Command_Factory_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Command_Factory_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_COMMAND_FACTORY_T_H */
