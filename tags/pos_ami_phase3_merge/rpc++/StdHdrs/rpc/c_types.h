#ifndef _rpc_c_types_h_
#define _rpc_c_types_h_

#if defined(__cplusplus)
    /*
     * Definitions for C++ 2.0 and later require extern "C" { decl; }
     */
#   define EXTERN_FUNCTION( rtn, args ) extern "C" { rtn args; }
#   define FUN_ARGS( args ) args
#   define STRUCT_TAG( tag_name ) /* the tag disappears */
#   define ENUM_BITFIELD( enum_type ) unsigned
#   define ENUM_TYPE( enum_sp, enum_ty ) enum_ty

#if defined(__STDC__) || defined(__cplusplus) || defined(c_plusplus)
#   define NAME_CONFLICT( name ) _##name
#else
#   define NAME_CONFLICT( name ) _/**/name
#endif

#   define DOTDOTDOT ...
#   define _VOID_ /* anachronism */
#   define CONST const

/*
 * This is not necessary for 2.0 since 2.0 has corrected the void (*) () problem
 */
typedef void (*_PFV_)();
typedef int (*_PFI_)();

#elif defined(c_plusplus)
    /*
     * Definitions for C++ 1.2
     */
#   define EXTERN_FUNCTION( rtn, args ) rtn args
#   define FUN_ARGS( args ) args
#   define STRUCT_TAG( tag_name )  /* the tag disappears */
#   define ENUM_BITFIELD( enum_type ) unsigned
#   define ENUM_TYPE( enum_sp, enum_ty ) enum_ty
#   define NAME_CONFLICT( name ) _/**/name
#   define DOTDOTDOT ...
#   define _VOID_ /* anachronism */
#   define CONST const 

typedef void (*_PFV_)();
typedef int (*_PFI_)();

#elif defined(__STDC__)
    /*
     * Definitions for ANSI C
     */
#   define EXTERN_FUNCTION( rtn, args ) rtn args
#   define FUN_ARGS( args ) args
#   define STRUCT_TAG( tag_name ) tag_name
#   define ENUM_BITFIELD( enum_type ) unsigned
#   define ENUM_TYPE( enum_sp, enum_ty ) enum_sp enum_ty
#   define NAME_CONFLICT( name ) name
#   define DOTDOTDOT ...
#   define _VOID_ void
#   define CONST   

#else
    /*
     * Definitions for Sun/K&R C -- ignore function prototypes,
     * but preserve tag names and enum bitfield declarations.
     */
#   define EXTERN_FUNCTION( rtn, args ) rtn()
#   define FUN_ARGS( args ) ()
#   define STRUCT_TAG( tag_name ) tag_name
#   define ENUM_BITFIELD( enum_type ) enum_type
#   define ENUM_TYPE( enum_sp, enum_ty ) enum_sp enum_ty
#   define NAME_CONFLICT( name ) name
#   define DOTDOTDOT
#   define _VOID_
    /* VOID is only used where it disappears anyway */
#   define CONST   

#endif /* Which type of C/C++ compiler are we using? */

#endif
