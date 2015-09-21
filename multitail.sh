#!/bin/bash
path="logs/"
files=`ls $path`
command="multitail -n 1";

for i in $files; do
	command="$command -I $path/$i";
done
echo $command;
$command;
