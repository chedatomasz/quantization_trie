#!/bin/bash

for f in "$2"/*.in
do
	echo "Testing on input "$f""
	valgrind -q --leak-check=full --track-origins=yes ./"$1" <"$f" > temp.out 2>temp.err
	if diff -q temp.out "${f::-3}.out"; then
		echo "ANS: OK"
	else
		echo "ANS: ERROR"
	fi

	if diff -q temp.err "${f::-3}.err"; then
		echo "ERR: OK"
	else
		echo "ERR: ERROR"
	fi
done
rm temp.out
rm temp.err
