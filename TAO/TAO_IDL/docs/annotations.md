# Implementing Annotations

**How to use the internal API to implement built-in annotations in `tao_idl` or a
compiler that uses `tao_idl`.**

**Table of Contents:**

* [IDL Annotations](#idl-annotations)
* [What Can Be Annotated](#what-can-be-annotated)
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
* [Extending Annotation Support](#extending-annotation-support)
* [Limitations](#limitations)
* [History](#history)
  * [TAO 2.5.5](#tao-255)
  * [TAO 2.5.6](#tao-256)
  * [TAO 2.5.7](#tao-257)
  * [TAO 2.5.10](#tao-2510)

## IDL Annotations

Annotations are a feature of IDLv4 that allows IDL authors to pass hints to the
IDL compiler that can change compiler behavior and code generation. They are
similar to some uses of `#pragma`, but are more powerful because they are
integrated with IDL and are more expressive. In the latest IDL specification as
of writing, version 4.2, they are described in section 7.4.15.1.

The concept behind annotations exists in other languages like Java and Python,
as decorators, and in C++11 and C#, as attributes. Like Java and Python,
annotations can appear in front of declarations, have `@` at the beginning, and
look like function calls.

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

## What Can Be Annotated

Annotations "may be applied to any IDL constructs or sub-constructs", as
defined by the OMG. This is very vague and the OMG has not clarified this as of
IDL 4.2 [(Limitation #1)](#limitation1). Because of the lack of standardization
of what can be annotated and how, annotations for specific IDL elements have to
be added on a as-needed basis.

You can put annotations before the following things in IDL and a backend using
TAO IDL's front-end library can read them:

- modules
- `typedef`s
- constants
- structures and their member values
- unions and their cases
- enumerations and their enumerators
- interfaces, porttypes, eventtypes, components and all of their direct
  contents
- valuetypes and most of their direct contents with the exception of these
  kinds of valuetype statements:
  - import: not supported by TAO
  - typeid: not supported by TAO within valuetypes
  - typeprefix: No corresponding AST Node to attach annotations to

These are the general cases. The rest of the cases are defined in the next
section. If an annotation application isn't listed in the general case list or
the special case section, it is almost certainly not supported and might cause
warnings if the usage is a general case and syntax errors if the usage a
special case. See ["Extending Annotation
Support"](#extending-annotation-support) if you're interested in adding it and
are familiar with GNU Bison.

### Special Cases of Annotations

The annotations on all the elements in the list above can be accessed using the
`annotations()` method covered later in [Reading Annotations in the
AST](#reading-annotations-in-the-ast). In the cases listed below, the
annotation is used within the declaration and therefore require special access
methods. See ["Reading Special Cases of
Annotations"](#reading-special-cases-of-annotations) for how to have a backend
read these kinds of annotations.

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
for this purpose, `idl_global->eval (const char* idl, bool disable_output =
false)`. `eval ()` processes IDL as if it were in an IDL file so annotations
can be defined using the IDL annotation notation. You can disable warning and
error messages generated by parsing by passing `true` as the second argument.

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

The new lines aren't strictly necessary but might help if a error occurs during
`eval` because it will refer to the line number of this string as though it was
a file called `builtin-N`, where `N` is times `eval` has been called.

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
        dynamic_cast<AST_Annotation_Member *> ((*document)["comment"])->
          value ()->ev ()->u.strval->get_string ();

      bool deprecated =
        dynamic_cast<AST_Annotation_Member *> ((*document)["deprecated"])->
          value ()->ev ()->u.bval;

      /*
       * This is more complicated because we are trying to get the name of
       * the enumerator. If we just wanted the number value, we could treat the
       * AST_Expresssion from the annotation member as an unsigned long by using
       * ev ()->u.ulval.
       */
      const char *api_type = 0;
      AST_Expression *api_type_val =
        dynamic_cast<AST_Annotation_Member *> ((*document)["api_type"])->
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
struct struct1
{
  // ...
};

// ...

/**
 * Struct with 2 members
 *
 * API_TYPE: USER_API
 */
struct struct2
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
struct struct3
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

The following cases show how to read annotations from these special cases.

**NOTE:** To access these methods on a type that has been "`typedef`-ed", it
must be resolved completely using `AST_Type *primitive_base_type ()` and a
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

## Extending Annotation Support

**NOTE: This section assumes familarity with GNU Bison and only covers the
general concept of extending annotation support.**

How to extend support for annotations on a particular IDL element depends on a
few things. In the `fe/idl.ypp` bison file, if the annotation would be matched
by the `at_least_one_definition` token, like it would when annotating a
structure, or by the `at_least_one_export` token, like it would be when
annotating a interface operation, the change is simple in principle:

1. Make sure the `AST_Decl*` node of what you want to annotate is being passed
   up the to generic annotation code in `at_least_one_export` or
   `at_least_one_definition`.

1. Implement `virutal bool annotatable() const` in the `AST_*` files of the
   node type to return `true`. The default implementation of `AST_Decl` returns
   `false`.

If you want to implement a annotation that goes within an IDL element, like
annotating a union discriminator, that is more complicated. It will involve
modifying the grammar to accept the annotations and adding a special cases
method to the node's class, like in ["Reading Special Cases of
Annotations"](#reading-special-cases-of-annotations).

Finally, if you do extend annotation support, please update the annotation test
in `$TAO_ROOT/tests/IDLv4/annotations/be_init.cpp` and this file, specifically
the ["What Can Be Annotated"](#what-can-be-annotated) and ["History"](#history)
sections. Also update ["Reading Special Cases of
Annotations"](#reading-special-cases-of-annotations) if you've added support for
a special case.

## Limitations

The current limitations exist in TAO\_IDL annotation implementation as of writing:

<a name="limitation1"></a>
1. Because of lack of any specification in IDL for where annotations can go,
   annotations in places other than what's listed in ["What Can be
   Annotated"](#what-can-be-annotated) can cause syntax errors, even if the IDL
   works with other IDL tools.

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

### TAO 2.5.7

- The TAO IDL Frontend now supports annotations on interfaces, operations, and
  attributes.

- `idl_global->eval` now will produce error and warning messages. This can be
  silenced by passing `true` as a second argument.

- Expanded documentation on what can be annotated and how to extend annotation
  support.

### TAO 2.5.10

- Annotation support extended:
  - All the direct contents of interfaces
  - The porttypes, eventtypes, components and all their direct contents
  - Valuetypes and most of their direct contents
