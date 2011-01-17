// $Id$

#include "Literals.h"

namespace DAnCE
{
  namespace XMI
  {

    const ACE_TCHAR* const LITERALS[MAX_LITERAL] =
      {
        ACE_TEXT("xmi.version"),                            // VERSION_TAG
        ACE_TEXT("1.1"),                                    // VERSION_VALUE
        ACE_TEXT("xmlns:UML"),                              // NS_TAG
        ACE_TEXT("org.omg/UML1.3"),                         // NS_VALUE
        ACE_TEXT("XMI.header"),                             // HEADER_TAG
        ACE_TEXT("XMI.documentation"),                      // DOC_TAG
        ACE_TEXT("XMI.exporter"),                           // EXPORTER_TAG
        ACE_TEXT("DOC Group IDL to XMI converter"),         // EXPORTER_VALUE
        ACE_TEXT("XMI.content"),                            // CONTENT_TAG
        ACE_TEXT("UML:Model"),                              // MODEL_TAG
        ACE_TEXT("xmi.id"),                                 // XMI_ID
        ACE_TEXT("UML:Namespace.ownedElement"),             // OWNEDELEMENT_TAG
        ACE_TEXT("UML:Package"),                            // PACKAGE_TAG
        ACE_TEXT("UML:ModelElement"),                       // MODELELEMENT_TAG
        ACE_TEXT("UML:ModelElement.taggedValue"),           // ME_TV_TAG
        ACE_TEXT("UML:ModelElement.constraint"),            // ME_CONS_TAG
        ACE_TEXT("UML:Constraint"),                         // CONSTRAINT
        ACE_TEXT("UML:TaggedValue"),                        // TV_TAG
        ACE_TEXT("UML:Stereotype"),                         // STEREOTYPE_TAG
        ACE_TEXT("UML:Class"),                              // CLASS_TAG
        ACE_TEXT("UML:Operation"),                          // OP_TAG
        ACE_TEXT("UML:BehavioralFeature.parameter"),        // BFP_TAG
        ACE_TEXT("UML:Parameter"),                          // PARAM_TAG
        ACE_TEXT("UML:BehavioralFeature.raisedException"),  // BF_RE_TAG
        ACE_TEXT("UML:Exception"),                          // EXCEPT_TAG
        ACE_TEXT("UML:Attribute"),                          // ATTR_TAG
        ACE_TEXT("UML:Association"),                        // ASSOC_TAG
        ACE_TEXT("UML:Association.connection"),             // ASSOC_CONN_TAG
        ACE_TEXT("UML:AssociationEnd"),                     // ASSOC_END_TAG
        ACE_TEXT("UML:AssociationEnd.qualifier"),           // ASSOC_END_QUAL_TAG
        ACE_TEXT("UML:Generalization"),                     // GEN_TAG
        ACE_TEXT("UML:Generalization.subtype"),             // GEN_SUB_TAG
        ACE_TEXT("UML:Generalization.supertype"),           // GEN_SUPER_TAG
        ACE_TEXT("UML:GeneralizableElement"),               // GEN_ELEMENT_TAG
        ACE_TEXT("UML:GeneralizableElement.generalization"),// GEN_ELEMENT_GEN_TAG
        ACE_TEXT("UML:Exception"),                          // EXCEPTION_TAG

        // ******* Attributes
        ACE_TEXT("tag"),                                    // TAG
        ACE_TEXT("value"),                                  // VALUE
        ACE_TEXT("name"),                                   // NAME
        ACE_TEXT("baseClass"),                              // BASEC
        ACE_TEXT("visibility"),                             // VISIBIL
        ACE_TEXT("stereotype"),                             // STEREO_ATTR
        ACE_TEXT("namespace"),                              // NS
        ACE_TEXT("owner"),                                  // OWNER
        ACE_TEXT("kind"),                                   // KIND
        ACE_TEXT("behavioralFeature"),                      // BFEATURE
        ACE_TEXT("type"),                                   // TYPE
        ACE_TEXT("isAbstract"),                             // ABSTRACT
        ACE_TEXT("multiplicity"),                           // MULT
        ACE_TEXT("xmi.idref"),                              // IDREF
        ACE_TEXT("ownerScope"),                             // OWNER_SCOPE
        ACE_TEXT("changeable"),                             // CHANGEABLE

        // ******* Attribute values
        ACE_TEXT("Association"),                            // ASSOC
        ACE_TEXT("Class"),                                  // CLASS
        ACE_TEXT("DataType"),                               // DATATYPE
        ACE_TEXT("Package"),                                // PACKAGE
        ACE_TEXT("null"),                                   // NULL
        ACE_TEXT("Attribute"),                              // ATTR
        ACE_TEXT("public"),                                 // PUBLIC
        ACE_TEXT("private"),                                // _PRIVATE
        ACE_TEXT("typeid"),                                 // TYPEID
        ACE_TEXT("raises"),                                 // RAISES
        ACE_TEXT("return"),                                 // RETURN
        ACE_TEXT("in"),                                     // _IN
        ACE_TEXT("inout"),                                  // INOUT
        ACE_TEXT("out"),                                    // _OUT
        ACE_TEXT("true"),                                   // _TRUE
        ACE_TEXT("false"),                                  // _FALSE
        ACE_TEXT("TRUE"),                                   // CAPS_TRUE
        ACE_TEXT("FALSE"),                                  // CAPS_FALSE
        ACE_TEXT("1..1"),                                   // MULT_OTO
        ACE_TEXT("0..1"),                                   // MULT_ZTO
        ACE_TEXT("IDLOrder"),                               // IDLORDER
        ACE_TEXT("index"),                                  // INDEX
        ACE_TEXT("constraintUpperValue"),                   // CONST_UPPER
        ACE_TEXT("*"),                                      // UNBOUNDED_SEQ
        ACE_TEXT("Case"),                                   // CASE
        ACE_TEXT("default"),                                // DEFAULT_UNION
        ACE_TEXT("classifier"),                             // CLASSIFIER
        ACE_TEXT("frozen"),                                 // FROZEN
        ACE_TEXT("initialValue"),                           // INIT_VAL

        //******** Stereotypes
        ACE_TEXT("CCMConsumes"),                            // ST_CONSUMES
        ACE_TEXT("CORBAAnonymousSequence"),                 // ST_ANON_SEQ
        ACE_TEXT("CORBAPrincipal"),                         // ST_PRINC
        ACE_TEXT("CORBAUnion"),                             // ST_UNION
        ACE_TEXT("CCMUses"),                                // ST_USES
        ACE_TEXT("CORBASequence"),                          // ST_SEQ
        ACE_TEXT("readonlyEnd"),                            // ST_ROE
        ACE_TEXT("readonly"),                               // ST_RO
        ACE_TEXT("CORBAObject"),                            // ST_OBJ
        ACE_TEXT("CORBAstring"),                            // ST_STR
        ACE_TEXT("CORBAlongdouble"),                        // ST_LD
        ACE_TEXT("CORBAAnonymousArray"),                    // ST_ANON_ARR
        ACE_TEXT("CORBAAlias"),                             // ST_ALIAS
        ACE_TEXT("null"),                                   // ST_NULL
        ACE_TEXT("CORBAValue"),                             // ST_VALUE
        ACE_TEXT("void"),                                   // ST_VOID
        ACE_TEXT("CORBAException"),                         // ST_EX
        ACE_TEXT("CORBAoctet"),                             // ST_OCTET
        ACE_TEXT("CCMManages"),                             // ST_MANAGES
        ACE_TEXT("CORBAwstring"),                           // ST_WSTR
        ACE_TEXT("CORBAfloat"),                             // ST_FLOAT
        ACE_TEXT("CORBAEnum"),                              // ST_ENUM
        ACE_TEXT("CORBAValueSupports"),                     // ST_VAL_SUP
        ACE_TEXT("CORBAModule"),                            // ST_MODULE
        ACE_TEXT("CCMHomeFinder"),                          // ST_HFINDER
        ACE_TEXT("CORBAdouble"),                            // ST_DOUBLE
        ACE_TEXT("CORBAlonglong"),                          // ST_LL
        ACE_TEXT("CCMHomeFactory"),                         // ST_HFACT
        ACE_TEXT("CORBAArray"),                             // ST_ARR
        ACE_TEXT("CCMComponent"),                           // ST_COMP
        ACE_TEXT("CORBAshort"),                             // ST_SHORT
        ACE_TEXT("CORBATypedef"),                           // ST_TYPEDEF
        ACE_TEXT("CORBAunsignedlonglong"),                  // ST_ULL
        ACE_TEXT("CORBAlong"),                              // ST_LONG
        ACE_TEXT("CORBATypeCode"),                          // ST_TC
        ACE_TEXT("CCMHome"),                                // ST_HOME
        ACE_TEXT("CORBAStruct"),                            // ST_STRUCT
        ACE_TEXT("CORBAFixed"),                             // ST_FIXED
        ACE_TEXT("CORBAunsignedshort"),                     // ST_US
        ACE_TEXT("CCMEmits"),                               // ST_EMITS
        ACE_TEXT("CORBABoxedValue"),                        // ST_BOXVALUE
        ACE_TEXT("CORBAboolean"),                           // ST_BOOL
        ACE_TEXT("CORBAConstant"),                          // ST_CONST
        ACE_TEXT("CCMPublishes"),                           // ST_PUBLISH
        ACE_TEXT("CORBAConstants"),                         // ST_CONSTANTS
        ACE_TEXT("CCMProvides"),                            // ST_PROVIDES
        ACE_TEXT("CORBAwchar"),                             // ST_WCHAR
        ACE_TEXT("CORBAunsignedlong"),                      // ST_UL
        ACE_TEXT("CORBAValueFactory"),                      // ST_VAL_FACT
        ACE_TEXT("CCMEvent"),                               // ST_EVENT
        ACE_TEXT("CORBAchar"),                              // ST_CHAR
        ACE_TEXT("CORBAInterface"),                         // ST_INTF
        ACE_TEXT("CCMPrimaryKey"),                          // ST_PRI_KEY
        ACE_TEXT("CORBAany"),                               // ST_ANY
        ACE_TEXT("CORBAnative")                             // ST_NATIVE
      };
  }
}
