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
#define IDL_OBJECT 273
#define IDL_STRUCT 274
#define IDL_UNION 275
#define IDL_SWITCH 276
#define IDL_ENUM 277
#define IDL_SEQUENCE 278
#define IDL_STRING 279
#define IDL_WSTRING 280
#define IDL_EXCEPTION 281
#define IDL_CASE 282
#define IDL_DEFAULT 283
#define IDL_READONLY 284
#define IDL_ATTRIBUTE 285
#define IDL_ONEWAY 286
#define IDL_IDEMPOTENT 287
#define IDL_VOID 288
#define IDL_IN 289
#define IDL_OUT 290
#define IDL_INOUT 291
#define IDL_RAISES 292
#define IDL_CONTEXT 293
#define IDL_NATIVE 294
#define IDL_LOCAL 295
#define IDL_ABSTRACT 296
#define IDL_CUSTOM 297
#define IDL_FACTORY 298
#define IDL_PRIVATE 299
#define IDL_PUBLIC 300
#define IDL_SUPPORTS 301
#define IDL_TRUNCATABLE 302
#define IDL_VALUETYPE 303
#define IDL_COMPONENT 304
#define IDL_CONSUMES 305
#define IDL_EMITS 306
#define IDL_EVENTTYPE 307
#define IDL_FINDER 308
#define IDL_GETRAISES 309
#define IDL_HOME 310
#define IDL_IMPORT 311
#define IDL_MULTIPLE 312
#define IDL_PRIMARYKEY 313
#define IDL_PROVIDES 314
#define IDL_PUBLISHES 315
#define IDL_SETRAISES 316
#define IDL_TYPEID 317
#define IDL_TYPEPREFIX 318
#define IDL_USES 319
#define IDL_MANAGES 320
#define IDL_INTEGER_LITERAL 321
#define IDL_UINTEGER_LITERAL 322
#define IDL_STRING_LITERAL 323
#define IDL_CHARACTER_LITERAL 324
#define IDL_FLOATING_PT_LITERAL 325
#define IDL_TRUETOK 326
#define IDL_FALSETOK 327
#define IDL_SCOPE_DELIMITOR 328
#define IDL_LEFT_SHIFT 329
#define IDL_RIGHT_SHIFT 330
#define IDL_WCHAR_LITERAL 331
#define IDL_WSTRING_LITERAL 332
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
