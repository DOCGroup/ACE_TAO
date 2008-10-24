
/* -*- C++ -*- */

//==================================================================
/**
 *  file  PC_Updater_T.h
 *
 * $Id$
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 */
//=====================================================================

#ifndef CIAO_CONFIG_HANDLERS_PC_UPDATER_T_H
#define CIAO_CONFIG_HANDLERS_PC_UPDATER_T_H
#include /**/ "ace/pre.h"

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace PC_Updater_T
{
  template <typename SEQUENCE>
  static void update_sequence (SEQUENCE &seq);
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "PC_Updater_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("PC_Updater_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"
#endif /*CIAO_CONFIG_HANDLERS_PC_Updater_T_H*/
