# Implementing Annotations

**How to use the internal API to implement built-in annotations in `tao_idl` or a
compiler that uses `tao_idl`.**

## Annotations

Annotations are a feature of IDLv4 that allows IDL authors to pass hints to the
IDL compiler that can change compiler behavior and code generation. They are
similar to `#pragmas`, but more powerful because they are integrated with IDL
and are more expressive. In the latest IDL specification as of writing, version
4.2, they are described in section 7.4.15.1.

Annotations exist in other languages like, Python, Java, and C#, although in
the former they are called attributes and have a significantly different
syntax. Like Python and Java, annotations can appear front of declarations and
look like function calls preceded by an `@` symbol. Unlike Python and Java,
though they "may be applied to any IDL constructs or sub-constructs", but the
OMG has not clarified this as of IDL 4.2. What this means though is that
annotations can be applied more places than the other languages allow.

In `tao_idl`, here is an example of how annotations may be used:
```
enum Urgency {
  SUPPLEMENTARY,
  @default_literal INFORMATIVE,
  CRITICAL
};

@range(min=0,max=24)
typedef Hours short;

@extensibility(FINAL)
struct Report {
  @key
  unsigned long index;

  @default(12)
  Hours expiration;

  Urgency urgency;

  string message;
};
```

