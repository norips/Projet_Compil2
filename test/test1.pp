var a:integer, var b:boolean, var res:integer, var arr:array of integer

defun sum(x:integer,y:integer) : integer {
	sum := x + y
}
/* Fibo recursif */
defun fibo(n:integer) : integer {
	if n=0 then {
		fibo:=0
	} else {
		if n=1 then {
			fibo:=1
		} else {
			fibo:= fibo(n-1) + fibo(n-2)
		}
	}
}

defpro setRes(x:integer) {
	res := x
}

defun newArray(n: integer) : array of integer
{
        newArray := new array of integer[n]
}

defun equal(x:integer,y:integer) : boolean
        var local1 : integer,
        var local2 : boolean
{
        local1 := 1 + 2;
        local2 := true or false;

	equal := x=y;

        x := 42
}

defpro set(value: integer, arr: array of integer, index: integer)
{
        arr[index] := value
}

a:=2;
arr:=new array of integer[a];
arr[10] := 12;
arr[0] := 42;
setRes(arr[10]);
b := equal(2,a);
a := 1 and 0;
set(42, new array of integer[10], 0)
/*res:=fibo(11)*/




/* Failing
sum := 2;
equal(true,2);
a := setRes(2)
*/

