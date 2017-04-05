var a:integer, var b:boolean, var c:boolean, var res:integer, var arr:array of integer
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

defun equal(x:integer,y:integer) : boolean
        var local1 : integer,
        var local2 : boolean,
        var local3 : boolean
{
        local3 := x=y;
        local2 := true or false;

	    equal := local3;

        x := 42
}

a:=2;
arr:=new array of integer[a];
arr:=new array of integer[42];
arr[10] := 12;
arr[0] := 42;
setRes(arr[10]);
c := arr[10] = res;
b := equal(2,a);
a := 1 and 0;
res:=fibo(11)




/* Failing
sum := 2;
equal(true,2);
a := setRes(2)
*/

