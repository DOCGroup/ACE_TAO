# default link map for INTEGRITY
# text (readonly) and data (read/write) areas must be page-aligned

-sec
{
# text/readonly segment
        .picbase	0x10000 	:
	.text				:
        .syscall 			:
	.intercall			:
	.interfunc			:
        .secinfo 			:
        .rodata 			:
	.sdata2 			:
        .fixaddr 			:
        .fixtype 			:
# .textchecksum MUST be the last section of the text/readonly segment
	.textchecksum			:

# data/readwrite segment
	.pidbase	align(0x4000)	:
        .data 				:
	.sdabase 			:
        .sdata 				:
# .datachecksum MUST be the last section of the data portion of the
# data/read/write segment (i.e., it must come before the BSS sections).
	.datachecksum			:
        .sbss 				:
        .bss 				:
	.heap	align(16) pad(0x800000) 	:
}
