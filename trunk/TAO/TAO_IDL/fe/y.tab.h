typedef union {
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
} YYSTYPE;
#define	IDENTIFIER	258
#define	CONST	259
#define	MODULE	260
#define	INTERFACE	261
#define	TYPEDEF	262
#define	LONG	263
#define	SHORT	264
#define	UNSIGNED	265
#define	DOUBLE	266
#define	FLOAT	267
#define	CHAR	268
#define	WCHAR	269
#define	OCTET	270
#define	BOOLEAN	271
#define	ANY	272
#define	STRUCT	273
#define	UNION	274
#define	SWITCH	275
#define	ENUM	276
#define	SEQUENCE	277
#define	STRING	278
#define	WSTRING	279
#define	EXCEPTION	280
#define	CASE	281
#define	DEFAULT	282
#define	READONLY	283
#define	ATTRIBUTE	284
#define	ONEWAY	285
#define	IDEMPOTENT	286
#define	VOID	287
#define	IN	288
#define	OUT	289
#define	INOUT	290
#define	RAISES	291
#define	CONTEXT	292
#define	INTEGER_LITERAL	293
#define	STRING_LITERAL	294
#define	CHARACTER_LITERAL	295
#define	FLOATING_PT_LITERAL	296
#define	TRUETOK	297
#define	FALSETOK	298
#define	SCOPE_DELIMITOR	299
#define	LEFT_SHIFT	300
#define	RIGHT_SHIFT	301


extern YYSTYPE yylval;
