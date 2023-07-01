#!/bin/bash
assert() {
	expected="$1"
	input="$2"

	./9cc "$input" > tmp.s
	cc -o tmp tmp.s
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$input => $expected expected, but got $actual"
		exit 1
	fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34   -  5  "
assert 13 "1+2+(3+7)"
assert 13 "1+(2+3+7)"
assert 17 "1-(2+  3)+7  +( 44  - 30)"

echo OK
