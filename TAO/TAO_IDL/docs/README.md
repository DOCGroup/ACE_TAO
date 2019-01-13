# TAO\_IDL Developer Documentation

## Topics

- [Historical Document Describing How to Write a Backend](WRITING_A_BE).
- [Creating Annotations for IDLv4](annotations.md)
- [Less Relevant Historical Documents from Sun](historical)

## TAO\_IDL Layout

TAO\_IDL is broken up into a few different modules, these can be generalized
as:

- `AST`
  - "Abstract Syntax Tree"
  - Nodes representing the Abstract Syntax Tree
- `FE`
  - "Front End"
  - Parsing functionality that generates the Abstract Syntax Tree.
  - Among other things, contains the Bison grammar `fe/idl.ypp` and Flex
    token `fe/idl.ll` files.
- `BE`
  - "Back End"
  - Transformed Abstract Syntax Tree
- `UTL`
  - "Util"
  - Utility Classes and Functions.
  - Contains class for the `idl_global` singleton which is made up of
    `include/idl_global.h` and `utl/utl_global.cpp`.
- `DRV`
  - "Driver"
  - The `tao_idl` program it self which drives the other modules.

Lacking namespaces for the most part, in TAO\_IDL function and class names
generally start with these modules, using the "C" way of emulating namespaces.

The modules are broken into these places on the file system:

- `include`
  - Contains the header files for AST, FE, and UTL modules.
- `ast`
  - Contains the source files for AST modules classes.
- `fe`
  - Contains the source files and some header files for the FE module
- `be_include`
  - Contains the header files for the BE module.
- `be`
  - Contains the source files for the BE module.
- `driver`
  - Contains some parts of DRV.
- `util`
  - Contains the souce files for the UTL module.
- `narrow`
  - Leftover from a custom casting system.
