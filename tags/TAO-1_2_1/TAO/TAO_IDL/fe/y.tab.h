// $Id$
#define IDENTIFIER 257
#define IDL_CONST 258
#define IDL_MODULE 259
#define IDL_INTERFACE 260
#define IDL_TYPEDEF 261
#define IDL_LONG 262
#define IDL_SHORT 263
#define IDL_UNSIGNED 264
#define IDL_DOUBLE 265
#define IDL_FLOAT 266
#define IDL_CHAR 267
#define IDL_WCHAR 268
#define IDL_OCTET 269
#define IDL_BOOLEAN 270
#define IDL_FIXED 271
#define IDL_ANY 272
#define IDL_STRUCT 273
#define IDL_UNION 274
#define IDL_SWITCH 275
#define IDL_ENUM 276
#define IDL_SEQUENCE 277
#define IDL_STRING 278
#define IDL_WSTRING 279
#define IDL_EXCEPTION 280
#define IDL_CASE 281
#define IDL_DEFAULT 282
#define IDL_READONLY 283
#define IDL_ATTRIBUTE 284
#define IDL_ONEWAY 285
#define IDL_IDEMPOTENT 286
#define IDL_VOID 287
#define IDL_IN 288
#define IDL_OUT 289
#define IDL_INOUT 290
#define IDL_RAISES 291
#define IDL_CONTEXT 292
#define IDL_NATIVE 293
#define IDL_LOCAL 294
#define IDL_ABSTRACT 295
#define IDL_CUSTOM 296
#define IDL_FACTORY 297
#define IDL_PRIVATE 298
#define IDL_PUBLIC 299
#define IDL_SUPPORTS 300
#define IDL_TRUNCATABLE 301
#define IDL_VALUETYPE 302
#define IDL_INTEGER_LITERAL 303
#define IDL_UINTEGER_LITERAL 304
#define IDL_STRING_LITERAL 305
#define IDL_CHARACTER_LITERAL 306
#define IDL_FLOATING_PT_LITERAL 307
#define IDL_TRUETOK 308
#define IDL_FALSETOK 309
#define IDL_SCOPE_DELIMITOR 310
#define IDL_LEFT_SHIFT 311
#define IDL_RIGHT_SHIFT 312
#define IDL_WCHAR_LITERAL 313
#define IDL_WSTRING_LITERAL 314
typedef union {
  AST_Decl                      *dcval;         /* Decl value           */
  UTL_StrList                   *slval;         /* String list          */
  UTL_NameList                  *nlval;         /* Name list            */
  UTL_ExprList                  *elval;         /* Expression list      */
  UTL_LabelList                 *llval;         /* Label list           */
  UTL_DeclList                  *dlval;         /* Declaration list     */
  FE_InterfaceHeader            *ihval;         /* Interface header     */
  FE_obv_header                 *vhval;         /* Valuetype header     */
  AST_Expression                *exval;         /* Expression value     */
  AST_UnionLabel                *ulval;         /* Union label          */
  AST_Field                     *ffval;         /* Field value          */
  AST_Field::Visibility         vival;          /* N/A, pub or priv     */
  AST_Expression::ExprType      etval;          /* Expression type      */
  AST_Argument::Direction       dival;          /* Argument direction   */
  AST_Operation::Flags          ofval;          /* Operation flags      */
  FE_Declarator                 *deval;         /* Declarator value     */
  idl_bool                      bval;           /* Boolean value        */
  long                          ival;           /* Long value           */
  unsigned long                 uival;          /* Unsigned long value  */
  double                        dval;           /* Double value         */
  float                         fval;           /* Float value          */
  char                          cval;           /* Char value           */
  ACE_CDR::WChar                wcval;          /* WChar value          */
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
} TAO_YYSTYPE;
extern TAO_YYSTYPE tao_yylval;
