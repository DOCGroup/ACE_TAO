# Implementing Annotations

**How to use the internal API to implement built-in annotations in `tao_idl` or a
compiler that uses `tao_idl`.**

**Table of Contents:**

* [IDL Annotations](#idl-annotations)
  * [Special Cases of Annotations](#special-cases-of-annotations)
    * [Union Discriminators](#union-discriminators)
    * [Base Types in Sequences](#base-types-in-sequences)
    * [Base Types in Arrays](#base-types-in-arrays)
* [Defining Annotations](#defining-annotations)
  * [`@document` Example](#document-example)
  * [What Can Go in Annotations](#what-can-go-in-annotations)
* [Reading Annotations in the AST](#reading-annotations-in-the-ast)
  * [Reading `@document` Annotations](#reading-document-annotations)
    * [Reading Annotations Manually](#reading-annotations-manually)
  * [Reading Special Cases of Annotations](#reading-special-cases-of-annotations)
    * [Union Discriminators](#union-discriminators-1)
    * [Base Types in Sequences](#base-types-in-sequences-1)
    * [Base Types in Arrays](#base-types-in-arrays-1)
* [Limitations](#limitations)
* [History](#history)
  * [TAO 2.5.5](#tao-255)

## IDL Annotations

Annotations are a feature of IDLv4 that allows IDL authors to pass hints to the
IDL compiler that can change compiler behavior and code generation. They are
similar to some uses of `#pragma`, but are more powerful because they are
integrated with IDL and are more expressive. In the latest IDL specification as
of writing, version 4.2, they are described in section 7.4.15.1.

The concept behind annotations exists in other languages like Java and Python,
as decorators, and in C++11 and C#, as attributes. Like Java and Python,
annotations can appear in front of declarations, have `@` at the beginning, and
can look like function call.

Here is an example of what IDL using some OMG standard annotations might look
like:

```
enum Urgency_t {
  SUPPLEMENTARY,
  @default_literal INFORMATIVE,
  CRITICAL
};

@unit("Hour(s)")
@range(min=0,max=23)
typedef short Hours_t;

@unit("Day(s)")
typedef unsigned long Days_t;

struct Time_t {
  Hours_t hours;
  Days_t days;
};

@mutable
struct Report {
  @key
  unsigned long index;

  @optional
  Time_t expiration;

  @optional
  Urgency_t urgency;

  string message;
};
```

### Special Cases of Annotations

Annotations "may be applied to any IDL constructs or sub-constructs", as
defined by the OMG. This is very vague and the OMG has not clarified this as of
IDL 4.2. [(Also see Limitation #1)](#limitation1). What can be said about it
though is that other than before normal declarations, like before structs,
typedefs, and constants, in TAO\_IDL as of writing, annotations can also be
applied in the following cases. See ["Reading Special Cases of
Annotations"](#reading-special-cases-of-annotations) for how to have the
compiler use these kinds of annotations.

#### Union Discriminators

**[See Compiler Example](#union-discriminators-1)**

```
enum GradingSystem_t {
  PASS_FAIL,
  PASS_70,
  PASS_80
};
union Grade_t switch (@key GradingSystem_t) {
case PASS_FAIL:
  boolean pass;
case PASS_70:
case PASS_80:
default:
  short grade;
};
```

#### Base Types in Sequences

**[See Compiler Example](#base-types-in-sequences-1)**

```
struct Event {
  short data;
};
typedef sequence<@external Report, 12> Dozen_Events;
```

#### Base Types in Arrays

**[See Compiler Example](#base-types-in-arrays-1)**

```
struct Event {
  short data;
};
typedef Dozen_Events Event @external [12];
```

## Defining Annotations

Annotations should be defined after the AST is initialized and ready to be
used, but before any user defined IDL is processed. The recommended place for
this is `BE_post_init ()` which is located in `be/be_init.cpp` in `tao_idl`.
Annotations are nodes in the AST and could be defined by hand, simulating what
happens in `fe/idl.yy`. However a string parsing utility has been added just
for this purpose, `idl_global->eval (const char* idl)`. `eval ()` processes IDL
as if it were in an IDL file so annotations can be defined using the IDL
annotation notation.

### `@document` Example

As a simple example, if we wanted to make an annotation that inserted comments
into the product files for documentation purposes, we could design an
annotation like this:

```
@annotation document {
  enum API_Type {
    INTERNAL_API,
    USER_API,
    LEGACY_API
  };
  string comment;
  API_Type api_type default INTERNAL_API;
  boolean deprecated default FALSE;
};
```

To use it without defining it in every IDL file, we need to embed it into
`BE_post_init()`.

```C++
void BE_post_init (char *[], long)
{
  // ...
  if (idl_global->idl_version_ > IDL_VERSION_3)
    {
      idl_global->eval (
        "@annotation document {\n"
        "  enum API_Type {\n"
        "    INTERNAL_API,\n"
        "    USER_API,\n"
        "    LEGACY_API\n"
        "  };\n"
        "  string comment;\n"
        "  API_Type api_type default INTERNAL_API;\n"
        "  boolean deprecated default FALSE;\n"
        "};\n"
      );
    }
  // ...
}
```

The new lines aren't strictly necessary but might help if a syntax error occurs
because it will refer to the line number of this string as though it was a file
called `builtin`. This might not be helpful as it could be, because it won't
distinguish between multiple calls to `eval` when reporting an error.

By default TAO\_IDL uses IDL3 and this will cause an error when parsing the
annotations. Version is controlled using `--idl-version` command line argument
and ultimately `idl_global->idl_version_`. In the example above we would have
to pass `--idl-version 4`.

We can set it to use IDL4 by default in `BE_init ()`:

```C++
int BE_init (int &, ACE_TCHAR *[])
{
  // ...
  idl_global->default_idl_version_ = IDL_VERSION_4;
  // ...
}

void BE_post_init (char *[], long)
{
  // Same as above ...
}
```

In TAO\_IDL, `idl_global->default_idl_version_` sets `idl_global->idl_version_`
after `BE_init` is called but before arguments are parsed. This gives the user
a chance to override it if they really want to and allows them to query the
version we're setting using `--default-idl-version`.

Alternatively if it is desired to retain compatibility with older versions of
TAO, use the `TAO_IDL_HAS_ANNOTATIONS` macro.

```C++
int BE_init (int &, ACE_TCHAR *[])
{
  // ...
#ifdef TAO_IDL_HAS_ANNOTATIONS
  idl_global->default_idl_version_ = IDL_VERSION_4;
#endif
  // ...
}

void BE_post_init (char *[], long)
{
  // ...
#ifdef TAO_IDL_HAS_ANNOTATIONS
  if (idl_global->idl_version_ > IDL_VERSION_3)
    {
      idl_global->eval (
        // ...
      );
    }
#endif
  // ...
}
```

This would also be used when reading the annotations later.

<a name="document-usage"></a>
Once the annotation is declared, it can be used in IDL:

```
@document("Struct with 1 member")
struct struct1 {
  short x;
};

@document(
  comment="Struct with 2 members",
  api_type=USER_API
)
struct struct2 {
  short x, y;
};

@document(
  comment="Struct with 3 members",
  api_type=LEGACY_API,
  deprecated=TRUE
)
struct struct3 {
  short x, y, z;
};
```

However it won't do anything because nothing using the AST is looking for it,
so it will be ignored. To make the program aware of the annotations, see
["Reading Annotations in the AST"](#reading-annotations-in-the-ast) below.

### What Can Go in Annotations

- Annotation members can be of any type that constants can be. This includes
booleans, integers, floats, enumerations, characters, and strings.
- Enumerations, constants, and typedefs can be declared inside the annotation
declaration, however they can not used outside the annotation expect for when
passing them as parameters to the same annotation. Otherwise normal scope rules
apply: Valid constant types and values from outside the annotation can be used
inside it.

## Reading Annotations in the AST

To get the annotations for most nodes types, use
`node->annotations ().find (annotation_decl)` where `annotation_decl` can be the
`AST_Annotation_Decl` object or its canonical internal TAO IDL name (see next
paragraph). This will return the last `AST_Annotation_Appl*` of that type on
the node or `NULL` if there no annotation of that type. Because
`AST_Annotation_Appls::find` can take a `AST_Annotation_Decl`, they can be looked
up after `idl_eval` creates them and cached for a slightly faster
`find`.

Internally, annotation local names are prefixed with `@` to prevent clashes
with other elements in IDL with the same name. For example when trying to use
`AST_Annotation_Appls::find (const char*)` with annotation named `bar` in a module
named `foo`, the proper internal scoped name to pass as the second argument is
either `foo::@bar` or `::foo::@bar` if we want to be explicit that `foo` is in
the root module. In IDL, this annotation's full name would be `@foo::bar`or
`@::foo::bar`.

After that check, you can use index operators `[const char*]` on the annotation
to get the individual members and `value()` to get the value.

The last part is not straightforward, as the value is an `AST_Expression` object
and `AST_Expression` is a complex class that handles constant values in
TAO\_IDL. There are examples below but see `AST_Expression::AST_ExprValue` for
how values can be accessed.

See `include/ast_expression.h` and `ast/ast_expression.cpp` for how
to use `AST_Expression`.

### Reading `@document` Annotations

In this example we will use the [`@document` annotation defined
above](#document-example) to generate Doxygen comments in the C++ code
generated. For simplicity's sake, we will limit this example to `struct`s defined
in TAO client headers. This can be accomplished by modifying the struct
visitor in `be/be_visitor_structure/structure_ch.cpp`.

At the top of the file, these includes should be added:

```C++
#include "ast_annotation_member.h"
#include "utl_string.h"
#include "ast_enum_val.h"
```

About midway though the file, in
`int be_visitor_structure_ch::visit_structure (be_structure *node)`
right before
```C++
*os << be_nl_2
    << "struct " // ...
```
these lines would also need to be added:

```C++
  AST_Annotation_Appl *document = node->annotations ().find ("::@document");
  if (document)
    {
      const char *comment =
        AST_Annotation_Member::narrow_from_decl ((*document)["comment"])->
          value ()->ev ()->u.strval->get_string ();

      bool deprecated =
        AST_Annotation_Member::narrow_from_decl ((*document)["deprecated"])->
          value ()->ev ()->u.bval;

      /*
       * This is more complicated because we are trying to get the name of
       * the enumerator. If we just wanted the number value, we could treat the
       * AST_Expresssion from the annotation member as an unsigned long by using
       * ev ()->u.ulval.
       */
      const char *api_type = 0;
      AST_Expression *api_type_val =
        AST_Annotation_Member::narrow_from_decl ((*document)["api_type"])->
          value ();
      AST_Enum *api_type_enum = api_type_val->enum_parent();
      if (api_type_enum)
        {
          AST_EnumVal *enum_val =
            api_type_enum->lookup_by_value (api_type_val);
          if (enum_val)
            {
              api_type = enum_val->local_name ()->get_string ();
            }
        }

      *os
        << "/**" << be_nl
        << " * " << comment << be_nl
        ;

      if (api_type)
        {
          *os
            << " *" << be_nl
            << " * API_TYPE: " << api_type << be_nl
            ;
        }

      if (deprecated)
        {
          *os
            << " *" << be_nl
            << " * \\deprecated This is deprecated" << be_nl
            ;
        }
      *os << " */";
    }
```

Using the [`@document` use example from above](#document-usage), these are
inserted into the client header file:

```C++
// ...
/**
 * Struct with 1 member
 *
 * API_TYPE: INTERNAL_API
 */
struct  struct1
{
  // ...
};

// ...

/**
 * Struct with 2 members
 *
 * API_TYPE: USER_API
 */
struct  struct2
{
  // ...
};

/**
 * Struct with 3 members
 *
 * API_TYPE: LEGACY_API
 *
 * \deprecated This is deprecated
 */
struct  struct3
{
  // ...
};
// ...
```

#### Reading Annotations Manually

`AST_Annotation_Appls::find ()` is convenient but only returns the last
annotation of the passed annotation type. If we want the first one, handle
multiple annotations of the same type, read all the annotations, or some other
subset, we will have to do what `find ()` is doing for us, which is just
iterating over the `AST_Annotation_Appl`s and checking `annotation_decl ()`.

```C++
  AST_Annotation_Appls &annotations = /* ... */;
  AST_Annotation_Decl *annotation = /* ... */;
  for (AST_Annotation_Appls::iterator i = annotations.begin ();
    i != annotations.end (); ++i)
    {
      AST_Annotation_Appl *appl = i->get ();
      if (appl && appl->annotation_decl () == annotation)
        {
          // Do work with annotation application
        }
    }
```

### Reading Special Cases of Annotations

Annotations placed before a definition in a scope are interpreted as annotating
the node that is being defined. Annotations in other places require special
grammar and special handling in the API.

The following cases show how to get the last annotation called `anno` from
these special cases.

To access these methods on a type that has been "`typedef`-ed", it must be
resolved completely using `AST_Type *primitive_base_type ()` and a
`dynamic_cast` to the correct type as these methods are specific to these
classes.

#### Union Discriminators

**[See IDL Example](#union-discriminators)**

```C++
  AST_Union *node = /* ... */;
  AST_Annotation_Appl *document = node->disc_annotations ().find ("::@key");
```

#### Base Types in Sequences

**[See IDL Example](#base-types-in-sequences)**

```C++
  AST_Sequence *node = /* ... */;
  AST_Annotation_Appl *document = node->base_type_annotations ().find ("::@external");
```

#### Base Types in Arrays

**[See IDL Example](#base-types-in-arrays)**

```C++
  AST_Array *node = /* ... */;
  AST_Annotation_Appl *document = node->base_type_annotations ().find ("::@external");
```

## Limitations

The current limitations exist in TAO\_IDL annotation implementation as of writing:

<a name="limitation1"></a>
1. Because of lack of a proper grammar specification in IDL for where
   annotations can go, annotations in places other than before declarations in
   scopes and other places listed above will result in syntax errors, even if
   they work with other IDL tools.

2. Even though this is implicitly allowed by the IDL specification, Annotations
   whose local names clash with IDL keywords are not supported. This includes
   the OMG standard annotations `default` and `oneway`.

## History

### TAO 2.5.5

**Any usage of `UTL_find_annotation(X, Y)` should be replaced with `X.find(Y)`.**

After TAO 2.5.4 was released, which introduced annotations, there was a change
to fix memory leaks caused by annotations. This change involved replacing
`typedef ACE_Vector<AST_Annotation_Appl> AST_Annotation_Appls` with a class of
the same name. This also allowed for moving `UTL_find_annotation` into
`AST_Annotation_Appls` as `find` for a nicer design.

### TAO 2.5.6

The TAO IDL Frontend no longer internally prefixes annotation names and
annotation member names with `_cxx_` if they are also a C++ keyword.
