# $Id$

# Common definitions which were in multiple files adding to the confusion

# All versions of 4.x...Why doesn't Digital have 4.[a-z] ? That would be
# more fun...

ifeq (4.,$(findstring 4.,$(ACE_DU_VERSION)))
  ifeq (4.0D,$(findstring 4.0D,$(ACE_DU_SUBVERSION)))
    CFLAGS += -DDIGITAL_UNIX=0x40D
  else
    ifeq (4.0E,$(findstring 4.0E,$(ACE_DU_SUBVERSION)))
      CFLAGS += -DDIGITAL_UNIX=0x40E
    else
      ifeq (4.0F,$(findstring 4.0F,$(ACE_DU_SUBVERSION)))
        CFLAGS += -DDIGITAL_UNIX=0x40F
      else
        ifeq (4.0G,$(findstring 4.0G,$(ACE_DU_SUBVERSION)))
          CFLAGS += -DDIGITAL_UNIX=0x40F # can't have 0x40G
        else
          CFLAGS += -DDIGITAL_UNIX=0x400
        endif
      endif
    endif
  endif
else # ! 4.x
  ifeq (V5.1,$(findstring V5.1,$(ACE_DU_SUBVERSION)))
    CFLAGS += -DDIGITAL_UNIX=0x510
  else
    CFLAGS += -DDIGITAL_UNIX=0x500
  endif
endif # ! 5.x
