// Component configuration file automatically generated as ComponentConfig.h
// by ConfigureComponent on Mon Jan 09 23:58:56 2006


#define BON2COMPONENT

// COM UUID-s, names and progID

#define TYPELIB_UUID "79101B9E-B765-4902-B82B-DC33939722D6"
#define TYPELIB_NAME "MGA AddOn TypeLibrary (UUIDManager)"
#define COCLASS_UUID "B744B044-436A-4463-AF72-41D1D94815D9"
#define COCLASS_NAME "MGA AddOn CoClass (UUIDManager)"
#define COCLASS_PROGID "MGA.AddOn.UUIDManager"


// This name will appear in the popup window for interpreter selection.
#define COMPONENT_NAME "UUIDManager"


// This text will appear in the toolbar icon tooltip and in the menu.
#define TOOLTIP_TEXT "PICML Component UUID Manager"


// This #define determines the interpreter type:
#define GME_ADDON

#define ADDON_EVENTMASK   (OBJEVENT_CREATED | OBJEVENT_ATTR)
// 0x80000001

// The name of the paradigm(s). The GME will use this component
// for this paradigm. Separate the names of paradigms by commas.
#define PARADIGMS "PICML"


#define GME_INTERFACES_BASE Interfaces

// This is the location of the GME interfaces file (Mga.idl, Meta.idl, etc)
#define GME_BASE .


// not defined: #define BON_ICON_SUPPORT

// not defined: #define BON_CUSTOM_TRANSACTIONS

// not defined: #define REGISTER_SYSTEMWIDE

// Just to please the whims of those Microsoft jerks:
#define COCLASS_UUID_EXPLODED1 0xB744B044
#define COCLASS_UUID_EXPLODED2  0x436A
#define COCLASS_UUID_EXPLODED3  0x4463
#define COCLASS_UUID_EXPLODED4  0xAF
#define COCLASS_UUID_EXPLODED5  0x72
#define COCLASS_UUID_EXPLODED6  0x41
#define COCLASS_UUID_EXPLODED7  0xD1
#define COCLASS_UUID_EXPLODED8  0xD9
#define COCLASS_UUID_EXPLODED9  0x48
#define COCLASS_UUID_EXPLODED10  0x15
#define COCLASS_UUID_EXPLODED11  0xD9
