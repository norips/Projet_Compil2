var X1 : array of integer,
var X2 : integer,
var X3 : integer,
var test : boolean

test := false;
X3 := 1;
X1 := new array of integer[X2];
X1[4] := 15;
X1[5] := 10;
if test then {
	X2:=X1[4]
} else {
	X2:=X1[5]
}
;
while not (X3 = 0) do {
	X3 := 0
}
/*
*/
