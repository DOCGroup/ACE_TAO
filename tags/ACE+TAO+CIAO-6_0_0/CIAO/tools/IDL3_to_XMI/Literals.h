/**
 * @file Literals.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 *  $Id$
 *
 */

#ifndef XMI_LITERALS_H
#define XMI_LITERALS_H

#include "ace/config-lite.h"

namespace DAnCE
{
  namespace XMI
  {
    enum LITERAL_T
      {
        VERSION_TAG,
        VERSION_VALUE,
        NS_TAG,
        NS_VALUE,
        HEADER_TAG,
        DOC_TAG,
        EXPORTER_TAG,
        EXPORTER_VALUE,
        CONTENT_TAG,
        MODEL_TAG,
        XMI_ID,
        OWNEDELEMENT_TAG,
        PACKAGE_TAG,
        MODELELEMENT_TAG,
        ME_TV_TAG,
        ME_CONS_TAG,
        CONSTRAINT,
        TV_TAG,
        STEREOTYPE_TAG,
        CLASS_TAG,
        OP_TAG,
        BFP_TAG,
        PARAM_TAG,
        BF_RE_TAG,
        EXCEPT_TAG,
        ATTR_TAG,
        ASSOC_TAG,
        ASSOC_CONN_TAG,
        ASSOC_END_TAG,
        ASSOC_END_QUAL_TAG,
        GEN_TAG,
        GEN_SUB_TAG,
        GEN_SUPER_TAG,
        GEN_ELEMENT_TAG,
        GEN_ELEMENT_GEN_TAG,
        EXCEPTION_TAG,

        // Attributes
        TAG,
        VALUE,
        NAME,
        BASEC,
        VISIBIL,
        STEREO_ATTR,
        NS,
        OWNER,
        KIND,
        BFEATURE,
        TYPE,
        ABSTRACT,
        MULT,
        IDREF,
        OWNER_SCOPE,
        CHANGEABLE,

        // attribute values
        ASSOC,
        CLASS,
        DATATYPE,
        PACKAGE,
        __NULL,
        ATTR,
        PUBLIC,
        _PRIVATE,
        TYPEID,
        RAISES,
        RETURN,
        _IN,
        INOUT,
        _OUT,
        _TRUE,
        _FALSE,
        CAPS_TRUE,
        CAPS_FALSE,
        MULT_OTO,
        MULT_ZTO,
        IDLORDER,
        INDEX,
        CONST_UPPER,
        UNBOUNDED_SEQ,
        CASE,
        DEFAULT_UNION,
        CLASSIFIER,
        FROZEN,
        INIT_VAL,

        // Stereotypes
        ST_CONSUMES,
        ST_ANON_SEQ,
        ST_PRINC,
        ST_UNION,
        ST_USES,
        ST_SEQ,
        ST_ROE,
        ST_RO,
        ST_OBJ,
        ST_STR,
        ST_LD,
        ST_ANON_ARR,
        ST_ALIAS,
        ST_NULL,
        ST_VALUE,
        ST_VOID,
        ST_EX,
        ST_OCTET,
        ST_MANAGES,
        ST_WSTR,
        ST_FLOAT,
        ST_ENUM,
        ST_VAL_SUP,
        ST_MODULE,
        ST_HFINDER,
        ST_DOUBLE,
        ST_LL,
        ST_HFACT,
        ST_ARR,
        ST_COMP,
        ST_SHORT,
        ST_TYPEDEF,
        ST_ULL,
        ST_LONG,
        ST_TC,
        ST_HOME,
        ST_STRUCT,
        ST_FIXED,
        ST_US,
        ST_EMITS,
        ST_BOXVALUE,
        ST_BOOL,
        ST_CONST,
        ST_PUBLISH,
        ST_CONSTANTS,
        ST_PROVIDES,
        ST_WCHAR,
        ST_UL,
        ST_VAL_FACT,
        ST_EVENT,
        ST_CHAR,
        ST_INTF,
        ST_PRI_KEY,
        ST_ANY,
        ST_NATIVE,

        MAX_LITERAL
      };

    extern  const ACE_TCHAR* const LITERALS[MAX_LITERAL];
  }
}

#endif /* XMI_LITERALS_H */
