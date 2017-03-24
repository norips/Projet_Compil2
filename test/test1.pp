var a:integer, var b:boolean, var res:integer, var arr:array of integer

defun sum(x:integer,y:integer) : integer {
	sum := x + y
}

defpro setRes(x:integer) {
	res := x
}

defun equal(x:integer,y:integer) : boolean {
	equal := x=y
}
a:=2;
arr:=new array of integer[a];

setRes(a);
b := equal(res,a) and true;
a := 1 and 1;
res:=sum(a,2)
