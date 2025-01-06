#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./ani_lang "$input" > tmp.s
    cc -o tmp tmp.s testfunc.o
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 3 "a=1; b=2; a+b;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5;"
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 10 '-10+20;'
assert 10 "+10;" 
assert 10 "-(-10);" 
assert 246 "-10;" #シェルの使用上0を超えられないので246が正しい値
assert 0 '0==1;'
assert 1 '42==42;'
assert 1 '0!=1;'
assert 0 '42!=42;'
assert 1 '0<1;'
assert 0 '1<1;'
assert 0 '2<1;'
assert 1 '0<=1;'
assert 1 '1<=1;'
assert 0 '2<=1;'
assert 1 '1>0;'
assert 0 '1>1;'
assert 0 '1>2;'
assert 1 '1>=0;'
assert 1 '1>=1;'
assert 0 '1>=2;'
assert 3 "かえす 3;"
assert 5 "a=1; もし (a == 1) 5;"
assert 2 "a=1; ++a; かえす a;"
assert 10 "くりかえし(a=0;a<10;++a) a; かえす a;"
assert 50 "a=0; ずっとループ(a<50)++a; かえす a;"
assert 10 "a=1; もし (a == 1) かえす 10;"
assert 15 "a=0; もし(a == 0){ a=15; かえす a;}"
assert 11 "くりかえし(a=0;a<10;++a){b=1;} かえす a + b; "
assert 10 "a=0; ずっとループ(a<10){++a;} かえす a;"
assert 0 "foo();"
echo OK