var X1 : array of integer,
var X2 : integer,
var X3 : integer,
var X4 : array of integer,
var sizeTab : integer,
var test : boolean

test := false;
X2 := 10;
X3 := X2;
sizeTab := 10;
X1 := new array of integer[sizeTab];
X1[4] := 15;
X1[5] := 11;
if X3 < 11 then {
	X2:=X1[4]
} else {
	X2:=X1[5]
};
X4 := new array of integer[sizeTab];
while not (X3 = 0) do {
	X1[X3-1] := 255;
	X4[sizeTab-X3] := 15;
	X3 := X3 - 1
}
/*
*/
