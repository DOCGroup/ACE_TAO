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
    ENV_SRC,
    ENV_SNGL_SRC,
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
    COMP_CK,
    COMP_NAMES,
    COMP_ECB,
    GLUE_NS,
    INH_RCSB,
    TYPE_ID,
    TYPE_PREFIX,
    REPO_ID,
    VAR_SIZE,
    SIZE
  };

  extern char const* const STRS[SIZE];
}

#endif  // LITERALS_HPP
