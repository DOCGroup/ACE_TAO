// $Id$

typedef union
#ifdef __cplusplus
	TAO_YYSTYPE
#endif
 {
  AST_Decl		*dcval;		/* Decl value		*/
  UTL_StrList		*slval;		/* String list		*/
  UTL_NameList		*nlval;		/* Name list		*/
  UTL_ExprList		*elval;		/* Expression list	*/
  UTL_LabelList		*llval;		/* Label list		*/
  UTL_DeclList		*dlval;		/* Declaration list	*/
  FE_InterfaceHeader	*ihval;		/* Interface header	*/
  AST_Expression	*exval;		/* Expression value	*/
  AST_UnionLabel	*ulval;		/* Union label		*/
  AST_Field		*ffval;		/* Field value		*/
  AST_Expression::ExprType etval;	/* Expression type	*/
  AST_Argument::Direction dival;	/* Argument direction	*/
  AST_Operation::Flags	ofval;		/* Operation flags	*/
  FE_Declarator		*deval;		/* Declarator value	*/
  idl_bool		bval;		/* Boolean value	*/
  long			ival;		/* Long value		*/
  double		dval;		/* Double value		*/
  float			fval;		/* Float value		*/
  char			cval;		/* Char value		*/

  String		*sval;		/* String value		*/
  char			*strval;	/* char * value		*/
  Identifier		*idval;		/* Identifier		*/
  UTL_IdList		*idlist;	/* Identifier list	*/
} TAO_YYSTYPE;
extern TAO_YYSTYPE tao_yylval;
# define IDENTIFIER 257
# define IDL_CONST 258
# define IDL_MODULE 259
# define IDL_INTERFACE 260
# define IDL_TYPEDEF 261
# define IDL_LONG 262
# define IDL_SHORT 263
# define IDL_UNSIGNED 264
# define IDL_DOUBLE 265
# define IDL_FLOAT 266
# define IDL_CHAR 267
# define IDL_WCHAR 268
# define IDL_OCTET 269
# define IDL_BOOLEAN 270
# define IDL_ANY 271
# define IDL_STRUCT 272
# define IDL_UNION 273
# define IDL_SWITCH 274
# define IDL_ENUM 275
# define IDL_SEQUENCE 276
# define IDL_STRING 277
# define IDL_WSTRING 278
# define IDL_EXCEPTION 279
# define IDL_CASE 280
# define IDL_DEFAULT 281
# define IDL_READONLY 282
# define IDL_ATTRIBUTE 283
# define IDL_ONEWAY 284
# define IDL_IDEMPOTENT 285
# define IDL_VOID 286
# define IDL_IN 287
# define IDL_OUT 288
# define IDL_INOUT 289
# define IDL_RAISES 290
# define IDL_CONTEXT 291
# define IDL_NATIVE 292
# define IDL_INTEGER_LITERAL 293
# define IDL_STRING_LITERAL 294
# define IDL_CHARACTER_LITERAL 295
# define IDL_FLOATING_PT_LITERAL 296
# define IDL_TRUETOK 297
# define IDL_FALSETOK 298
# define IDL_SCOPE_DELIMITOR 299
# define IDL_LEFT_SHIFT 300
# define IDL_RIGHT_SHIFT 301
