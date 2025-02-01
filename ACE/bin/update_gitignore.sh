#!/usr/bin/env bash

cmd="$1"
if [[ "$cmd" == "help" ]] || [[ "$cmd" != "run" && "$cmd" != "dry-run" ]]
then
	if [[ "$cmd" != "help" ]]
	then
		echo "Error: Unrecognized command '$0 $1'"
		echo ""
	fi

	echo "Usage:"
	echo "  update_gitignore.sh run"
	echo "  update_gitignore.sh dry-run"
	echo ""
	echo "This command finds all untracked and un-git-add-ed files in the repository and adds each file name to "
	echo "a '.gitignore' file in the same directory. If a '.gitignore' file does not exist, one will be created. "
	echo "Otherwise, the existing '.gitignore' file will be appended to. The 'dry-run' command enables you to "
	echo "see a list of all detected file names that will be added to '.gitignore' files without making any "
	echo "changes."
	echo ""
	echo "If the 'run' command does its job properly, running 'dry-run' immediately after should display no "
	echo "planned actions."

	if [[ "$cmd" == "help" ]]
	then
		exit 0
	fi

	exit 1
fi

export IFS=$'\n';

for line in $(git status --short --untracked-files)
do
	if [[ $line == '??'* ]]
	then
		file="${line:3}"
		d="$(dirname "${file}")"
		f="$(basename "${file}")"

		if [[ "$cmd" == "run" ]]
		then
			echo "Ignoring ${f} in ${d}/.gitignore"
			echo "${f}" >> "${d}/.gitignore"
		else
			echo "Will ignore ${f} in ${d}/.gitignore"
		fi
	fi
done
