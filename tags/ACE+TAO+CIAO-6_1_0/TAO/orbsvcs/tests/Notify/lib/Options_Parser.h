/* -*- C++ -*- */
/**
 *  @file Options_Parser.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_OPTIONS_PARSER_H
#define TAO_Notify_Tests_OPTIONS_PARSER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "ace/Arg_Shifter.h"

/**
 * @class TAO_Notify_Tests_Options_Parser
 *
 * @brief Common Options parsing code.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Options_Parser
{
public:
  /// Constructor
  TAO_Notify_Tests_Options_Parser (void);

  /// Destructor
  ~TAO_Notify_Tests_Options_Parser ();

  /// Populates <qos> with options apecified in <arg_shifter>
  void execute (CosNotification::QoSProperties& qos, ACE_Arg_Shifter& arg_shifter);

  /// Populates <added> and <removed> with options apecified in <arg_shifter>
  void execute (CosNotification::EventTypeSeq& added, CosNotification::EventTypeSeq& removed, ACE_Arg_Shifter& arg_shifter);
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_OPTIONS_PARSER_H */
