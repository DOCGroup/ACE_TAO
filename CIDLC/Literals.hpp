// file      : CIDLC/Literals.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef LITERALS_HPP
#define LITERALS_HPP

namespace StringLiterals
{
  extern char const* const COPYRIGHT;

  // Index into the string array in Literals.cpp.
  enum
  {
    EXCP_SYS,
    EXCP_BP,
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
    COMP_CD,
    COMP_RD,
    COMP_PD,
    COMP_ED,
    CIDL_NS,
    FACET_PREFIX,
    TYPE_ID,
    TYPE_PREFIX,
    REPO_ID,
    YCH,
    SEP,
    VAR_SIZE,
    EXTRACT,
    EXTRACT_TMP,
    ACE_UA,
    LIT_SIZE
  };

  extern char const* const STRS[LIT_SIZE];
}

#endif  // LITERALS_HPP
