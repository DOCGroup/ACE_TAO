// assert DEFINE_GUID is defined
#undef	DEFINE_GUID

// XXX assign these a value ...
#define DEFINE_GUID(name,b,c,d,e,f,g,h,i,j,k,l) \
	const IID name = { 0 }
