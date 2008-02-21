//==================================================================
/**
*  @file  DnC_Dump_T.h
*
* $Id$
*
*  @author Gan Deng <gan.deng@vanderbilt.edu>
*/
//=====================================================================

#ifndef CIAO_CONFIG_HANDLERS_DNC_DUMP_T_H
#define CIAO_CONFIG_HANDLERS_DNC_DUMP_T_H
#include /**/ "ace/pre.h"

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
namespace DnC_Dump_T
{
template <typename SEQUENCE>
void dump_sequence (const char* caption, const SEQUENCE &seq);

template <typename ROOT, typename SEQUENCE,
typename DATA_TYPE, typename CLASS>
void dump_ref_seq (const char* caption, SEQUENCE& seq,
const char* root, DATA_TYPE CLASS::*data);

template <typename ROOT, typename REFERENCE, typename DATA_TYPE,
typename CLASS>
void dump_ref (const char* caption, REFERENCE& ref,
const char* root, DATA_TYPE CLASS::*data);
}
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DnC_Dump_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DnC_Dump_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"
#endif /*CIAO_CONFIG_HANDLERS_DNC_DUMP_T_H*/
