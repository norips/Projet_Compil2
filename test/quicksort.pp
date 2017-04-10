
var result: array of integer,
var first: integer,
var second: integer,
var third: integer,
var fourth: integer,
var fifth: integer

defun getArray() : array of integer
      var array2d : array of array of integer
{
        array2d    := new array of array of integer[2];
        array2d[0] := new array of integer[5];
        array2d[1] := new array of integer[5];

        array2d[0][0] := 10;
        array2d[0][1] := 1;
        array2d[0][2] := 0;
        array2d[0][3] := 5;
        array2d[0][4] := 8;
        /* [10, 1, 0, 5, 8] -> [0, 1, 5, 8, 10] */

        array2d[1][0] := 5;
        array2d[1][1] := 4;
        array2d[1][2] := 1;
        array2d[1][3] := 4;
        array2d[1][4] := 3;
        /* [5, 4, 1, 4, 3] -> [1, 3, 4, 4, 5] */

        getArray := array2d[0]
}

defun quicksort(arr : array of integer, low : integer, high : integer) : array of integer
       var pivot  : integer,
       var temp   : integer,
       var i      : integer,
       var j      : integer
{
        if (low < high) then
        {
                pivot  := arr[low];

                i := low;
                j := high;
                
                while ((i < j) or (i = j)) do
                {
                        while(arr[i] < pivot) do i := i + 1;
                        while(not(arr[j] < pivot) and not(arr[j] = pivot)) do j := j - 1;

                        if ((i < j) or (i = j)) then
                        {
                                temp   := arr[i];
                                arr[i] := arr[j];
                                arr[j] := temp;
                                i := i + 1;
                                j := j - 1
                        }
                        else skip
                };

                if (low  < j) then quicksort(arr, low, j) else skip;
                if (not(high < i) and not(high = i)) then quicksort(arr, i, high) else skip
        }
        else skip;

        quicksort := arr        
}


result := quicksort(getArray(), 0, 4);

first  := result[0];
second := result[1];
third  := result[2];
fourth := result[3];
fifth  := result[4]