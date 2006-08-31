// $Id$

#define RAWCOMPONENT

// COM UUID-s, names and progID

#define TYPELIB_UUID "04DBB307-9F4F-42B6-90A9-D59D5743F80B"
#define TYPELIB_NAME "CUTS Interpreter TypeLibrary"
#define COCLASS_UUID "DD5752C3-12EF-43D0-978C-F5C395D5BF13"
#define COCLASS_NAME "CUTS Interpreter CoClass"
#define COCLASS_PROGID "CUTS.Interpreter.PICML.Impl"


// This name will appear in the popup window for interpreter selection.
#define COMPONENT_NAME "CUTS Interpreter"


// This text will appear in the toolbar icon tooltip and in the menu.
#define TOOLTIP_TEXT "CUTS interpreter"


// This #define determines the interpreter type:
#define GME_INTERPRETER
// The name of the paradigm(s). The GME will use this component
// for this paradigm. Separate the names of paradigms by commas.
#define PARADIGMS "PICML"

// This is the location of the GME interfaces file (Mga.idl, Meta.idl, etc)
#define GME_BASE .

#define BON_ICON_SUPPORT

// Just to please the whims of those Microsoft jerks:
#define COCLASS_UUID_EXPLODED1 0xDD5752C3
#define COCLASS_UUID_EXPLODED2  0x12EF
#define COCLASS_UUID_EXPLODED3  0x43D0
#define COCLASS_UUID_EXPLODED4  0x97
#define COCLASS_UUID_EXPLODED5  0x8C
#define COCLASS_UUID_EXPLODED6  0xF5
#define COCLASS_UUID_EXPLODED7  0xC3
#define COCLASS_UUID_EXPLODED8  0x95
#define COCLASS_UUID_EXPLODED9  0xD5
#define COCLASS_UUID_EXPLODED10  0xBF
#define COCLASS_UUID_EXPLODED11  0x13
