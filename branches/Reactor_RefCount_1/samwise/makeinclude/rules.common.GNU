#
# $Id$
#

$(TARGETS_LOCAL:.local=): %: %.local %.makefiles %.nested

all.nested: all.makefiles
