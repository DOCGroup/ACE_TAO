package JACE.ServiceConfigurator;
import java.io.*;
import JACE.OS.*;
import java_cup.runtime.*;
// This was written for JLex version 1.2


class Yylex {
	private final int YY_BUFFER_SIZE = 512;
	private final int YY_F = -1;
	private final int YY_NO_STATE = -1;
	private final int YY_NOT_ACCEPT = 0;
	private final int YY_START = 1;
	private final int YY_END = 2;
	private final int YY_NO_ANCHOR = 4;
	private final byte YYEOF = -1;

  // Used to assemble the parameter string for a service
  private String params;  
	private java.io.DataInputStream yy_instream;
	private int yy_buffer_index;
	private int yy_buffer_read;
	private int yy_buffer_start;
	private int yy_buffer_end;
	private byte yy_buffer[];
	private int yyline;
	private int yy_lexical_state;
	Yylex (java.io.InputStream instream) {
		if (null == instream) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_instream = new java.io.DataInputStream(instream);
		yy_buffer = new byte[YY_BUFFER_SIZE];
		yy_buffer_read = 0;
		yy_buffer_index = 0;
		yy_buffer_start = 0;
		yy_buffer_end = 0;
		yyline = 0;
		yy_lexical_state = YYINITIAL;
	}
	private boolean yy_eof_done = false;
	private final int COMMENT = 1;
	private final int YYINITIAL = 0;
	private final int PARAMS = 2;
	private final int yy_state_dtrans[] = {
		0,
		23,
		30
	};
	private void yybegin (int state) {
		yy_lexical_state = state;
	}
	private byte yy_advance ()
		throws java.io.IOException {
		int next_read;
		int i;
		int j;

		if (yy_buffer_index < yy_buffer_read) {
			return yy_buffer[yy_buffer_index++];
		}

		if (0 != yy_buffer_start) {
			i = yy_buffer_start;
			j = 0;
			while (i < yy_buffer_read) {
				yy_buffer[j] = yy_buffer[i];
				++i;
				++j;
			}
			yy_buffer_end = yy_buffer_end - yy_buffer_start;
			yy_buffer_start = 0;
			yy_buffer_read = j;
			yy_buffer_index = j;
			next_read = yy_instream.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YYEOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}

		while (yy_buffer_index >= yy_buffer_read) {
			if (yy_buffer_index >= yy_buffer.length) {
				yy_buffer = yy_double(yy_buffer);
			}
			next_read = yy_instream.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YYEOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}
		return yy_buffer[yy_buffer_index++];
	}
	private void yy_move_start () {
		if ((byte) '\n' == yy_buffer[yy_buffer_start]) {
			++yyline;
		}
		++yy_buffer_start;
	}
	private void yy_pushback () {
		--yy_buffer_end;
	}
	private void yy_mark_start () {
		int i;
		for (i = yy_buffer_start; i < yy_buffer_index; ++i) {
			if ((byte) '\n' == yy_buffer[i]) {
				++yyline;
			}
		}
		yy_buffer_start = yy_buffer_index;
	}
	private void yy_mark_end () {
		yy_buffer_end = yy_buffer_index;
	}
	private void yy_to_mark () {
		yy_buffer_index = yy_buffer_end;
	}
	private java.lang.String yytext () {
		return (new java.lang.String(yy_buffer,0,
			yy_buffer_start,
			yy_buffer_end - yy_buffer_start));
	}
	private int yylength () {
		return yy_buffer_end - yy_buffer_start;
	}
	private byte[] yy_double (byte buf[]) {
		int i;
		byte newbuf[];
		newbuf = new byte[2*buf.length];
		for (i = 0; i < buf.length; ++i) {
			newbuf[i] = buf[i];
		}
		return newbuf;
	}
	private final int YY_E_INTERNAL = 0;
	private final int YY_E_MATCH = 1;
	private java.lang.String yy_error_string[] = {
		"Error: Internal error.\n",
		"Error: Unmatched input.\n"
	};
	private void yy_error (int code,boolean fatal) {
		java.lang.System.out.print(yy_error_string[code]);
		java.lang.System.out.flush();
		if (fatal) {
			throw new Error("Fatal Error.\n");
		}
	}
	private int yy_acpt[] = {
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NOT_ACCEPT,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NOT_ACCEPT,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR,
		YY_NO_ANCHOR
	};
	private int yy_cmap[] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 2, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 3, 4, 0, 0, 0, 0,
		5, 6, 7, 0, 0, 8, 8, 8,
		9, 9, 9, 9, 9, 9, 9, 9,
		9, 9, 10, 0, 0, 0, 0, 0,
		0, 11, 12, 12, 12, 13, 12, 12,
		12, 12, 12, 12, 12, 14, 12, 15,
		12, 12, 16, 17, 18, 12, 12, 12,
		12, 12, 12, 0, 8, 0, 0, 19,
		0, 20, 21, 22, 23, 24, 12, 12,
		12, 25, 26, 12, 27, 28, 29, 30,
		31, 12, 32, 33, 34, 35, 36, 12,
		12, 37, 12, 38, 0, 39, 0, 0
		
	};
	private int yy_rmap[] = {
		0, 1, 1, 1, 1, 1, 1, 2,
		1, 3, 1, 1, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 4,
		1, 1, 1, 1, 5, 6, 7, 8,
		9, 10, 11, 12, 13, 14, 15, 16,
		17, 18, 19, 20, 21, 22, 23, 24,
		25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48,
		49, 50, 51, 52, 53, 54, 55, 56,
		57, 58, 59, 60, 61, 62, 63, 64,
		65, 66, 67 
	};
	private int yy_nxt[][] = {
		{ 1, 28, 28, 2, 3, 4, 5, 6,
			7, 7, 8, 9, 9, 9, 72, 9,
			9, 73, 9, 9, 74, 9, 9, 81,
			9, 84, 9, 9, 9, 9, 9, 9,
			75, 76, 9, 9, 9, 9, 10, 11
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 7, -1, 7, 7, 7, 7, 7,
			7, 7, 7, 7, 7, 7, 7, 7,
			7, 7, 7, 7, 7, 7, 7, 7,
			7, 7, 7, 7, 7, 7, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ 24, 24, 25, 24, 24, 24, 24, 24,
			24, 24, 24, 24, 24, 24, 24, 24,
			24, 24, 24, 24, 24, 24, 24, 24,
			24, 24, 24, 24, 24, 24, 24, 24,
			24, 24, 24, 24, 24, 24, 24, 24
			
		},
		{ -1, 28, 28, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			12, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ 26, 26, 27, 27, 26, 26, 26, 26,
			26, 26, 26, 26, 26, 26, 26, 26,
			26, 26, 26, 26, 26, 26, 26, 26,
			26, 26, 26, 26, 26, 26, 26, 26,
			26, 26, 26, 26, 26, 26, 26, 26
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 13, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			14, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			15, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			16, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 17, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 18, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 19, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 20,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			21, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 22, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 29, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 31, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 32, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 33, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 34, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 35, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 36, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 37, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 38, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 39, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 40, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 41, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 42, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 43, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 44, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 45, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 46, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			47, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 48, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			49, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 50, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			51, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 52,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			53, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 54, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 55, 9, 9, 9,
			9, 56, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 57, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			58, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 59, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 60,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 61, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 62, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 63, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 64, 9, 9, 9, 9, 9,
			90, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 65, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			66, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 67, 78, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 68, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 69, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 70, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 71, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 77, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 79, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 80,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 82, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 83, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			85, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 86, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 87, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 88, 9, -1, -1
			
		},
		{ -1, -1, -1, -1, -1, -1, -1, -1,
			7, 9, -1, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			9, 9, 9, 9, 9, 9, 9, 9,
			89, 9, 9, 9, 9, 9, -1, -1
			
		}
	};
	public java_cup.runtime.token yylex ()
		throws java.io.IOException {
		byte yy_lookahead;
		int yy_anchor = YY_NO_ANCHOR;
		int yy_state = yy_state_dtrans[yy_lexical_state];
		int yy_next_state = YY_NO_STATE;
		int yy_last_accept_state = YY_NO_STATE;
		boolean yy_initial = true;
		int yy_this_accept;

		yy_mark_start();
		yy_this_accept = yy_acpt[yy_state];
		if (YY_NOT_ACCEPT != yy_this_accept) {
			yy_last_accept_state = yy_state;
			yy_mark_end();
		}
		while (true) {
			yy_lookahead = yy_advance();
			yy_next_state = YY_F;
			if (YYEOF != yy_lookahead) {
				yy_next_state = yy_nxt[yy_rmap[yy_state]][yy_cmap[yy_lookahead]];
			}
			if (YY_F != yy_next_state) {
				yy_state = yy_next_state;
				yy_initial = false;
				yy_this_accept = yy_acpt[yy_state];
				if (YY_NOT_ACCEPT != yy_this_accept) {
					yy_last_accept_state = yy_state;
					yy_mark_end();
				}
			}
			else {
				if (YYEOF == yy_lookahead && true == yy_initial) {
 
  return new java_cup.runtime.token (sym.EOF);
				}
				else if (YY_NO_STATE == yy_last_accept_state) {
					throw (new Error("Lexical Error: Unmatched Input."));
				}
				else {
					yy_to_mark();
					yy_anchor = yy_acpt[yy_last_accept_state];
					if (0 != (YY_END & yy_anchor)) {
						yy_pushback();
					}
					if (0 != (YY_START & yy_anchor)) {
						yy_move_start();
					}
					switch (yy_last_accept_state) {
					case 0:
						{ /* Skip all white space */ }
					case -2:
						break;
					case 1:
						{ 
  ACE.ERROR ("Unknown text, line " + (yyline + 1) + ": \"" + yytext() + "\""); 
  return new java_cup.runtime.str_token (sym.error); 
}
					case -3:
						break;
					case 2:
						{
  yybegin(PARAMS);
  params = new String();
}
					case -4:
						break;
					case 3:
						{ yybegin(COMMENT); }
					case -5:
						break;
					case 4:
						{ return new java_cup.runtime.token (sym.ACE_LPAREN); }
					case -6:
						break;
					case 5:
						{ return new java_cup.runtime.token (sym.ACE_RPAREN); }
					case -7:
						break;
					case 6:
						{ return new java_cup.runtime.token (sym.ACE_STAR); }
					case -8:
						break;
					case 7:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_PATHNAME, yytext()); 
}
					case -9:
						break;
					case 8:
						{ return new java_cup.runtime.token (sym.ACE_COLON); }
					case -10:
						break;
					case 9:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -11:
						break;
					case 10:
						{ return new java_cup.runtime.token (sym.ACE_LBRACE); }
					case -12:
						break;
					case 11:
						{ return new java_cup.runtime.token (sym.ACE_RBRACE); }
					case -13:
						break;
					case 12:
						{ return new java_cup.runtime.token (sym.ACE_MODULE_T); }
					case -14:
						break;
					case 13:
						{ return new java_cup.runtime.token (sym.ACE_STREAM_T); }
					case -15:
						break;
					case 14:
						{ return new java_cup.runtime.token (sym.ACE_ACTIVE); }
					case -16:
						break;
					case 15:
						{ return new java_cup.runtime.token (sym.ACE_REMOVE); }
					case -17:
						break;
					case 16:
						{ return new java_cup.runtime.token (sym.ACE_RESUME); }
					case -18:
						break;
					case 17:
						{ return new java_cup.runtime.token (sym.ACE_STATIC); }
					case -19:
						break;
					case 18:
						{ return new java_cup.runtime.token (sym.ACE_USTREAM); }
					case -20:
						break;
					case 19:
						{return new java_cup.runtime.token (sym.ACE_DYNAMIC); }
					case -21:
						break;
					case 20:
						{ return new java_cup.runtime.token (sym.ACE_SUSPEND); }
					case -22:
						break;
					case 21:
						{ return new java_cup.runtime.token (sym.ACE_INACTIVE); }
					case -23:
						break;
					case 22:
						{ return new java_cup.runtime.token (sym.ACE_SVC_OBJ_T); }
					case -24:
						break;
					case 24:
						{ /* Skip everything on a comment line */ }
					case -25:
						break;
					case 25:
						{ yybegin(YYINITIAL); }
					case -26:
						break;
					case 26:
						{
  params = params + yytext();
}
					case -27:
						break;
					case 27:
						{
  yybegin(YYINITIAL);
  return new java_cup.runtime.str_token (sym.ACE_STRING, params);
}
					case -28:
						break;
					case 28:
						{ /* Skip all white space */ }
					case -29:
						break;
					case 29:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -30:
						break;
					case 31:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -31:
						break;
					case 32:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -32:
						break;
					case 33:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -33:
						break;
					case 34:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -34:
						break;
					case 35:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -35:
						break;
					case 36:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -36:
						break;
					case 37:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -37:
						break;
					case 38:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -38:
						break;
					case 39:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -39:
						break;
					case 40:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -40:
						break;
					case 41:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -41:
						break;
					case 42:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -42:
						break;
					case 43:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -43:
						break;
					case 44:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -44:
						break;
					case 45:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -45:
						break;
					case 46:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -46:
						break;
					case 47:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -47:
						break;
					case 48:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -48:
						break;
					case 49:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -49:
						break;
					case 50:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -50:
						break;
					case 51:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -51:
						break;
					case 52:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -52:
						break;
					case 53:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -53:
						break;
					case 54:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -54:
						break;
					case 55:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -55:
						break;
					case 56:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -56:
						break;
					case 57:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -57:
						break;
					case 58:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -58:
						break;
					case 59:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -59:
						break;
					case 60:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -60:
						break;
					case 61:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -61:
						break;
					case 62:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -62:
						break;
					case 63:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -63:
						break;
					case 64:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -64:
						break;
					case 65:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -65:
						break;
					case 66:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -66:
						break;
					case 67:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -67:
						break;
					case 68:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -68:
						break;
					case 69:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -69:
						break;
					case 70:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -70:
						break;
					case 71:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -71:
						break;
					case 72:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -72:
						break;
					case 73:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -73:
						break;
					case 74:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -74:
						break;
					case 75:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -75:
						break;
					case 76:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -76:
						break;
					case 77:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -77:
						break;
					case 78:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -78:
						break;
					case 79:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -79:
						break;
					case 80:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -80:
						break;
					case 81:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -81:
						break;
					case 82:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -82:
						break;
					case 83:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -83:
						break;
					case 84:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -84:
						break;
					case 85:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -85:
						break;
					case 86:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -86:
						break;
					case 87:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -87:
						break;
					case 88:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -88:
						break;
					case 89:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -89:
						break;
					case 90:
						{ 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
					case -90:
						break;
					default:
						yy_error(YY_E_INTERNAL,false);
					case -1:
					}
					yy_initial = true;
					yy_state = yy_state_dtrans[yy_lexical_state];
					yy_next_state = YY_NO_STATE;
					yy_last_accept_state = YY_NO_STATE;
					yy_mark_start();
					yy_this_accept = yy_acpt[yy_state];
					if (YY_NOT_ACCEPT != yy_this_accept) {
						yy_last_accept_state = yy_state;
					}
				}
			}
		}
	}
}
