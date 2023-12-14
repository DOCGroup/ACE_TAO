#!/bin/sh

# ACE's "make install" for an MPC-generated workspace will build all targets that aren't yet built
# This script looks for projects that have been built and installs just those projects
this_dir=$(realpath $(dirname $0))
for makefile in $(find . -type f -name 'GNUmakefile.*'); do
    if grep -q 'GNU Makefile' $makefile; then
        echo Checking $makefile
        cd $(dirname $makefile)
        make -f $this_dir/install_proj.mk PROJECT_MAKEFILE=$(basename $makefile) checked-install "$@"
        cd - >/dev/null
    fi
done
