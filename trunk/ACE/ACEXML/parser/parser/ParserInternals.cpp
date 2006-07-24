// $Id$

#include "ACEXML/parser/parser/ParserInternals.h"


const ACEXML_Char* ACEXML_ParserInt::predef_ent_[] = {
  ACE_TEXT ("amp"),
  ACE_TEXT ("lt"),
  ACE_TEXT ("gt"),
  ACE_TEXT ("apos"),
  ACE_TEXT ("quot")
};

const ACEXML_Char* ACEXML_ParserInt::predef_val_[] = {
  ACE_TEXT ("&"),
  ACE_TEXT ("<"),
  ACE_TEXT (">"),
  ACE_TEXT ("'"),
  ACE_TEXT ("\"")
};

// Optimize away the most common cases. Any compiler worth it's salt should
// give generate a single memory access.

const ACEXML_Char ACEXML_ParserInt::base_char_table_[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x0000 - 0x000F */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x0010 - 0x001F */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x0020 - 0x002F */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x0030 - 0x003F */
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x0040 - 0x004F */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, /* 0x0050 - 0x005F */
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x0060 - 0x006F */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, /* 0x0070 - 0x007F */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x0080 - 0x008F */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x0090 - 0x009F */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x00A0 - 0x00AF */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x00B0 - 0x00BF */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x00C0 - 0x00CF */
  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x00D0 - 0x00DF */
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x00E0 - 0x00EF */
  1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x00F0 - 0x00FF */
};

#if defined (ACE_USES_WCHAR)

int
ACEXML_ParserInt::isBasechar_i (const ACEXML_Char c)
{
  if (c < 0x0100)
    return ACEXML_ParserInt::base_char_table_[c];
  else if (c < 0x0905)
    return ((c >= 0x0100 && c <= 0x0131) ||
            (c >= 0x0134 && c <= 0x013E) ||
            (c >= 0x0141 && c <= 0x0148) ||
            (c >= 0x014A && c <= 0x017E) ||
            (c >= 0x0180 && c <= 0x01C3) ||
            (c >= 0x01CD && c <= 0x01F0) ||
            (c >= 0x01F4 && c <= 0x01F5) ||
            (c >= 0x01FA && c <= 0x0217) ||
            (c >= 0x0250 && c <= 0x02A8) ||
            (c >= 0x02BB && c <= 0x02C1) ||
            (c == 0x0386) ||
            (c >= 0x0388 && c <= 0x038A) ||
            (c == 0x038C) ||
            (c >= 0x038E && c <= 0x03A1) ||
            (c >= 0x03A3 && c <= 0x03CE) ||
            (c >= 0x03D0 && c <= 0x03D6) ||
            (c == 0x03DA) ||
            (c == 0x03DC) ||
            (c == 0x03DE) ||
            (c == 0x03E0) ||
            (c >= 0x03E2 && c <= 0x03F3) ||
            (c >= 0x0401 && c <= 0x040C) ||
            (c >= 0x040E && c <= 0x044F) ||
            (c >= 0x0451 && c <= 0x045C) ||
            (c >= 0x045E && c <= 0x0481) ||
            (c >= 0x0490 && c <= 0x04C4) ||
            (c >= 0x04C7 && c <= 0x04C8) ||
            (c >= 0x04CB && c <= 0x04CC) ||
            (c >= 0x04D0 && c <= 0x04EB) ||
            (c >= 0x04EE && c <= 0x04F5) ||
            (c >= 0x04F8 && c <= 0x04F9) ||
            (c >= 0x0531 && c <= 0x0556) ||
            (c == 0x0559) ||
            (c >= 0x0561 && c <= 0x0586) ||
            (c >= 0x05D0 && c <= 0x05EA) ||
            (c >= 0x05F0 && c <= 0x05F2) ||
            (c >= 0x0621 && c <= 0x063A) ||
            (c >= 0x0641 && c <= 0x064A) ||
            (c >= 0x0671 && c <= 0x06B7) ||
            (c >= 0x06BA && c <= 0x06BE) ||
            (c >= 0x06C0 && c <= 0x06CE) ||
            (c >= 0x06D0 && c <= 0x06D3) ||
            (c == 0x06D5) ||
            (c >= 0x06E5 && c <= 0x06E6));
  else if (c < 0x10A0)
    return ((c >= 0x0905 && c <= 0x0939) ||
            (c == 0x093D) ||
            (c >= 0x0958 && c <= 0x0961) ||
            (c >= 0x0985 && c <= 0x098C) ||
            (c >= 0x098F && c <= 0x0990) ||
            (c >= 0x0993 && c <= 0x09A8) ||
            (c >= 0x09AA && c <= 0x09B0) ||
            (c == 0x09B2) ||
            (c >= 0x09B6 && c <= 0x09B9) ||
            (c >= 0x09DC && c <= 0x09DD) ||
            (c >= 0x09DF && c <= 0x09E1) ||
            (c >= 0x09F0 && c <= 0x09F1) ||
            (c >= 0x0A05 && c <= 0x0A0A) ||
            (c >= 0x0A0F && c <= 0x0A10) ||
            (c >= 0x0A13 && c <= 0x0A28) ||
            (c >= 0x0A2A && c <= 0x0A30) ||
            (c >= 0x0A32 && c <= 0x0A33) ||
            (c >= 0x0A35 && c <= 0x0A36) ||
            (c >= 0x0A38 && c <= 0x0A39) ||
            (c >= 0x0A59 && c <= 0x0A5C) ||
            (c == 0x0A5E) ||
            (c >= 0x0A72 && c <= 0x0A74) ||
            (c >= 0x0A85 && c <= 0x0A8B) ||
            (c == 0x0A8D) ||
            (c >= 0x0A8F && c <= 0x0A91) ||
            (c >= 0x0A93 && c <= 0x0AA8) ||
            (c >= 0x0AAA && c <= 0x0AB0) ||
            (c >= 0x0AB2 && c <= 0x0AB3) ||
            (c >= 0x0AB5 && c <= 0x0AB9) ||
            (c == 0x0ABD || c == 0x0AE0) ||
            (c >= 0x0B05 && c <= 0x0B0C) ||
            (c >= 0x0B0F && c <= 0x0B10) ||
            (c >= 0x0B13 && c <= 0x0B28) ||
            (c >= 0x0B2A && c <= 0x0B30) ||
            (c >= 0x0B32 && c <= 0x0B33) ||
            (c >= 0x0B36 && c <= 0x0B39) ||
            (c == 0x0B3D) ||
            (c >= 0x0B5C && c <= 0x0B5D) ||
            (c >= 0x0B5F && c <= 0x0B61) ||
            (c >= 0x0B85 && c <= 0x0B8A) ||
            (c >= 0x0B8E && c <= 0x0B90) ||
            (c >= 0x0B92 && c <= 0x0B95) ||
            (c >= 0x0B99 && c <= 0x0B9A) ||
            (c == 0x0B9C) ||
            (c >= 0x0B9E && c <= 0x0B9F) ||
            (c >= 0x0BA3 && c <= 0x0BA4) ||
            (c >= 0x0BA8 && c <= 0x0BAA) ||
            (c >= 0x0BAE && c <= 0x0BB5) ||
            (c >= 0x0BB7 && c <= 0x0BB9) ||
            (c >= 0x0C05 && c <= 0x0C0C) ||
            (c >= 0x0C0E && c <= 0x0C10) ||
            (c >= 0x0C12 && c <= 0x0C28) ||
            (c >= 0x0C2A && c <= 0x0C33) ||
            (c >= 0x0C35 && c <= 0x0C39) ||
            (c >= 0x0C60 && c <= 0x0C61) ||
            (c >= 0x0C85 && c <= 0x0C8C) ||
            (c >= 0x0C8E && c <= 0x0C90) ||
            (c >= 0x0C92 && c <= 0x0CA8) ||
            (c >= 0x0CAA && c <= 0x0CB3) ||
            (c >= 0x0CB5 && c <= 0x0CB9) ||
            (c == 0x0CDE) ||
            (c >= 0x0CE0 && c <= 0x0CE1) ||
            (c >= 0x0D05 && c <= 0x0D0C) ||
            (c >= 0x0D0E && c <= 0x0D10) ||
            (c >= 0x0D12 && c <= 0x0D28) ||
            (c >= 0x0D2A && c <= 0x0D39) ||
            (c >= 0x0D60 && c <= 0x0D61) ||
            (c >= 0x0E01 && c <= 0x0E2E) ||
            (c == 0x0E30) ||
            (c >= 0x0E32 && c <= 0x0E33) ||
            (c >= 0x0E40 && c <= 0x0E45) ||
            (c >= 0x0E81 && c <= 0x0E82) ||
            (c == 0x0E84) ||
            (c >= 0x0E87 && c <= 0x0E88) ||
            (c == 0x0E8A || c == 0x0E8D) ||
            (c >= 0x0E94 && c <= 0x0E97) ||
            (c >= 0x0E99 && c <= 0x0E9F) ||
            (c >= 0x0EA1 && c <= 0x0EA3) ||
            (c == 0x0EA5 || c == 0x0EA7) ||
            (c >= 0x0EAA && c <= 0x0EAB) ||
            (c >= 0x0EAD && c <= 0x0EAE) ||
            (c == 0x0EB0) ||
            (c >= 0x0EB2 && c <= 0x0EB3) ||
            (c == 0x0EBD) ||
            (c >= 0x0EC0 && c <= 0x0EC4) ||
            (c >= 0x0F40 && c <= 0x0F47) ||
            (c >= 0x0F49 && c <= 0x0F69));
  else
    return ((c >= 0x10A0 && c <= 0x10C5) ||
            (c >= 0x10D0 && c <= 0x10F6) ||
            (c == 0x1100) ||
            (c >= 0x1102 && c <= 0x1103) ||
            (c >= 0x1105 && c <= 0x1107) ||
            (c == 0x1109) ||
            (c >= 0x110B && c <= 0x110C) ||
            (c >= 0x110E && c <= 0x1112) ||
            (c == 0x113C || c == 0x113E || c == 0x1140) ||
            (c == 0x114C || c == 0x114E || c == 0x1150) ||
            (c >= 0x1154 && c <= 0x1155) ||
            (c == 0x1159) ||
            (c >= 0x115F && c <= 0x1161) ||
            (c == 0x1163) ||
            (c == 0x1165) ||
            (c == 0x1167) ||
            (c == 0x1169) ||
            (c >= 0x116D && c <= 0x116E) ||
            (c >= 0x1172 && c <= 0x1173) ||
            (c == 0x1175) ||
            (c == 0x119E) ||
            (c == 0x11A8) ||
            (c == 0x11AB) ||
            (c >= 0x11AE && c <= 0x11AF) ||
            (c >= 0x11B7 && c <= 0x11B8) ||
            (c == 0x11BA) ||
            (c >= 0x11BC && c <= 0x11C2) ||
            (c == 0x11EB) ||
            (c == 0x11F0) ||
            (c == 0x11F9) ||
            (c >= 0x1E00 && c <= 0x1E9B) ||
            (c >= 0x1EA0 && c <= 0x1EF9) ||
            (c >= 0x1F00 && c <= 0x1F15) ||
            (c >= 0x1F18 && c <= 0x1F1D) ||
            (c >= 0x1F20 && c <= 0x1F45) ||
            (c >= 0x1F48 && c <= 0x1F4D) ||
            (c >= 0x1F50 && c <= 0x1F57) ||
            (c == 0x1F59) ||
            (c == 0x1F5B) ||
            (c == 0x1F5D) ||
            (c >= 0x1F5F && c <= 0x1F7D) ||
            (c >= 0x1F80 && c <= 0x1FB4) ||
            (c >= 0x1FB6 && c <= 0x1FBC) ||
            (c == 0x1FBE) ||
            (c >= 0x1FC2 && c <= 0x1FC4) ||
            (c >= 0x1FC6 && c <= 0x1FCC) ||
            (c >= 0x1FD0 && c <= 0x1FD3) ||
            (c >= 0x1FD6 && c <= 0x1FDB) ||
            (c >= 0x1FE0 && c <= 0x1FEC) ||
            (c >= 0x1FF2 && c <= 0x1FF4) ||
            (c >= 0x1FF6 && c <= 0x1FFC) ||
            (c == 0x2126) ||
            (c >= 0x212A && c <= 0x212B) ||
            (c == 0x212E) ||
            (c >= 0x2180 && c <= 0x2182) ||
            (c >= 0x3041 && c <= 0x3094) ||
            (c >= 0x30A1 && c <= 0x30FA) ||
            (c >= 0x3105 && c <= 0x312C) ||
            (c >= 0xAC00 && c <= 0xD7A3));
};

int
ACEXML_ParserInt::isIdeographic_i (const ACEXML_Char c)
{
  return ((c >= 0x4E00 && c <= 0x9FA5) ||
          (c == 3007) ||
          (c >= 0x3021 && c <= 0x3029));
}

int
ACEXML_ParserInt::isCombiningchar_i (const ACEXML_Char c)
{
  if (c < 0x0901)
    return ((c >= 0x0300 && c <= 0x0345) ||
            (c >= 0x0360 && c <= 0x0361) ||
            (c >= 0x0483 && c <= 0x0486) ||
            (c >= 0x0591 && c <= 0x05A1) ||
            (c >= 0x05A3 && c <= 0x05B9) ||
            (c >= 0x05BB && c <= 0x05BD) ||
            (c == 0x05BF) ||
            (c >= 0x05C1 && c <= 0x05C2) ||
            (c == 0x05C4) ||
            (c >= 0x064B && c <= 0x0652) ||
            (c == 0x0670) ||
            (c >= 0x06D6 && c <= 0x06DC) ||
            (c >= 0x06DD && c <= 0x06DF) ||
            (c >= 0x06E0 && c <= 0x06E4) ||
            (c >= 0x06E7 && c <= 0x06E8) ||
            (c >= 0x06EA && c <= 0x06ED));
  else
    return ((c >= 0x0901 && c <= 0x0903) ||
            (c == 0x093C) ||
            (c >= 0x093E && c <= 0x094C) ||
            (c == 0x094D) ||
            (c >= 0x0951 && c <= 0x0954) ||
            (c >= 0x0962 && c <= 0x0963) ||
            (c >= 0x0981 && c <= 0x0983) ||
            (c == 0x09BC) ||
            (c == 0x09BE) ||
            (c == 0x09BF) ||
            (c >= 0x09C0 && c <= 0x09C4) ||
            (c >= 0x09C7 && c <= 0x09C8) ||
            (c >= 0x09CB && c <= 0x09CD) ||
            (c == 0x09D7) ||
            (c >= 0x09E2 && c <= 0x09E3) ||
            (c == 0x0A02) ||
            (c == 0x0A3C) ||
            (c == 0x0A3E) ||
            (c == 0x0A3F) ||
            (c >= 0x0A40 && c <= 0x0A42) ||
            (c >= 0x0A47 && c <= 0x0A48) ||
            (c >= 0x0A4B && c <= 0x0A4D) ||
            (c >= 0x0A70 && c <= 0x0A71) ||
            (c >= 0x0A81 && c <= 0x0A83) ||
            (c == 0x0ABC) ||
            (c >= 0x0ABE && c <= 0x0AC5) ||
            (c >= 0x0AC7 && c <= 0x0AC9) ||
            (c >= 0x0ACB && c <= 0x0ACD) ||
            (c >= 0x0B01 && c <= 0x0B03) ||
            (c == 0x0B3C) ||
            (c >= 0x0B3E && c <= 0x0B43) ||
            (c >= 0x0B47 && c <= 0x0B48) ||
            (c >= 0x0B4B && c <= 0x0B4D) ||
            (c >= 0x0B56 && c <= 0x0B57) ||
            (c >= 0x0B82 && c <= 0x0B83) ||
            (c >= 0x0BBE && c <= 0x0BC2) ||
            (c >= 0x0BC6 && c <= 0x0BC8) ||
            (c >= 0x0BCA && c <= 0x0BCD) ||
            (c == 0x0BD7) ||
            (c >= 0x0C01 && c <= 0x0C03) ||
            (c >= 0x0C3E && c <= 0x0C44) ||
            (c >= 0x0C46 && c <= 0x0C48) ||
            (c >= 0x0C4A && c <= 0x0C4D) ||
            (c >= 0x0C55 && c <= 0x0C56) ||
            (c >= 0x0C82 && c <= 0x0C83) ||
            (c >= 0x0CBE && c <= 0x0CC4) ||
            (c >= 0x0CC6 && c <= 0x0CC8) ||
            (c >= 0x0CCA && c <= 0x0CCD) ||
            (c >= 0x0CD5 && c <= 0x0CD6) ||
            (c >= 0x0D02 && c <= 0x0D03) ||
            (c >= 0x0D3E && c <= 0x0D43) ||
            (c >= 0x0D46 && c <= 0x0D48) ||
            (c >= 0x0D4A && c <= 0x0D4D) ||
            (c == 0x0D57) ||
            (c == 0x0E31) ||
            (c >= 0x0E34 && c <= 0x0E3A) ||
            (c >= 0x0E47 && c <= 0x0E4E) ||
            (c == 0x0EB1) ||
            (c >= 0x0EB4 && c <= 0x0EB9) ||
            (c >= 0x0EBB && c <= 0x0EBC) ||
            (c >= 0x0EC8 && c <= 0x0ECD) ||
            (c >= 0x0F18 && c <= 0x0F19) ||
            (c == 0x0F35) ||
            (c == 0x0F37) ||
            (c == 0x0F39) ||
            (c == 0x0F3E) ||
            (c == 0x0F3F) ||
            (c >= 0x0F71 && c <= 0x0F84) ||
            (c >= 0x0F86 && c <= 0x0F8B) ||
            (c >= 0x0F90 && c <= 0x0F95) ||
            (c == 0x0F97) ||
            (c >= 0x0F99 && c <= 0x0FAD) ||
            (c >= 0x0FB1 && c <= 0x0FB7) ||
            (c == 0x0FB9) ||
            (c >= 0x20D0 && c <= 0x20DC) ||
            (c == 0x20E1) ||
            (c >= 0x302A && c <= 0x302F) ||
            (c == 0x3099) ||
            (c == 0x309A));
}

int
ACEXML_ParserInt::isDigit_i (const ACEXML_Char c)
{
  if (c < 0x0040)
    return (c >= 0x0030 && c <= 0x0039);
  else
    return ((c >= 0x0660 && c <= 0x0669) ||
            (c >= 0x06F0 && c <= 0x06F9) ||
            (c >= 0x0966 && c <= 0x096F) ||
            (c >= 0x09E6 && c <= 0x09EF) ||
            (c >= 0x0A66 && c <= 0x0A6F) ||
            (c >= 0x0AE6 && c <= 0x0AEF) ||
            (c >= 0x0B66 && c <= 0x0B6F) ||
            (c >= 0x0BE7 && c <= 0x0BEF) ||
            (c >= 0x0C66 && c <= 0x0C6F) ||
            (c >= 0x0CE6 && c <= 0x0CEF) ||
            (c >= 0x0D66 && c <= 0x0D6F) ||
            (c >= 0x0E50 && c <= 0x0E59) ||
            (c >= 0x0ED0 && c <= 0x0ED9) ||
            (c >= 0x0F20 && c <= 0x0F29));
}

int
ACEXML_ParserInt::isExtender_i (const ACEXML_Char c)
{
  // The compiler should generate a jump table and index into it directly.
  switch (c)
    {
      case 0x00B7: case 0x02D0: case 0x02D1: case 0x0387: case 0x0640:
      case 0x0E46: case 0x0EC6: case 0x3005: case 0x3031: case 0x3032:
      case 0x3033: case 0x3034: case 0x3035: case 0x309D: case 0x309E:
      case 0x30FC: case 0x30FD: case 0x30FE:
        return 1;
      default:
        return 0;
    }
}

#endif /* ACE_USES_WCHAR */
