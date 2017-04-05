
var nbLoop: integer,
var array2d: array of array of integer,
var i: integer

defun newArray(size: integer) : array of integer
{
        newArray := new array of integer[size]
}

defpro set(index: integer, value: array of integer)
{
        array2d[index] := value
}

defpro fillArray(size: integer, factor: integer)
       var i: integer,
       var j: integer,
       var tmp: array of array of integer
{
        tmp := new array of array of integer[size];
        array2d := tmp;
        tmp := new array of array of integer[0];

        i := 0;
        while (i < size) do
        {
                set(i, new array of integer[i * factor + 1]);

                j := 0;
                while (j < i * factor + 1) do
                {
                        array2d[i][j] := j + 1;
                        j := j + 1
                };
                i := i + 1
        }
}

nbLoop := 100;

i := 0;
while (i < nbLoop) do
{
        fillArray(i * 3, 2);
        i := i + 1
};

array2d := new array of array of integer[0]