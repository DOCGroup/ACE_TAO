// file      : CIDLC/Literals.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef LITERALS_HPP
#define LITERALS_HPP

namespace StringLiterals
{
  extern char const* const COPYRIGHT;

  // Index into the string array below.
  enum
  {
    ENV_HDR,
    ENV_SNGL_HDR,
    ENV_SNGL_HDR_NOTUSED,
    ENV_SRC,
    ENV_SRC_NOTUSED,
    ENV_SNGL_SRC,
    ENV_SNGL_SRC_NOTUSED,
    ENV_ARG,
    ENV_SNGL_ARG,
    EXCP_SNGL,
    EXCP_START,
    EXCP_SYS,
    EXCP_IS,
    EXCP_AC,
    EXCP_IC,
    EXCP_NC,
    EXCP_ECL,
    EXCP_IN,
    EXCP_RF,
    EXCP_NKA,
    EXCP_ICF,
    EXCP_CR,
    EXCP_BET,
    EXCP_CE,
    COMP_CK,
    COMP_NAMES,
    COMP_ECB,
    COMP_EC,
    COMP_SC,
    GLUE_NS,
    INH_RCSB,
    TYPE_ID,
    TYPE_PREFIX,
    REPO_ID,
    YCH,
    SEP,
    VAR_SIZE,
    EXTRACT,
    EXTRACT_TMP,
    SIZE
  };

  extern char const* const STRS[SIZE];
}

#endif  // LITERALS_HPP
