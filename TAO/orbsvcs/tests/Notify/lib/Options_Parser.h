/* -*- C++ -*- */
/**
 *  @file Options_Parser.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_OPTIONS_PARSER_H
#define TAO_NS_OPTIONS_PARSER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Arg_Shifter.h"
#include "orbsvcs/CosNotificationC.h"

/**
 * @class TAO_NS_Options_Parser
 *
 * @brief Common Options parsing code.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Options_Parser
{
public:
  /// Constuctor
  TAO_NS_Options_Parser (void);

  /// Destructor
  ~TAO_NS_Options_Parser ();

  /// Populates <qos> with options apecified in <arg_shifter>
  void execute (CosNotification::QoSProperties& qos, ACE_Arg_Shifter& arg_shifter);

  /// Populates <added> and <removed> with options apecified in <arg_shifter>
  void execute (CosNotification::EventTypeSeq& added, CosNotification::EventTypeSeq& removed, ACE_Arg_Shifter& arg_shifter);
};

#include "ace/post.h"
#endif /* TAO_NS_OPTIONS_PARSER_H */
