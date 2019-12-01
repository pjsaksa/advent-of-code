# 02

`02b-2` implements the same algorithm as `02b-1`, but the code has been split to use multiple functions.

# 03

`03a-1` solves the puzzle by allocating an area from memory, which is used to count how many claims are using every
square-inch. Then the number of square-inches claimed by multiple elves are counted.

`03a-2` solves the puzzle using analytic geometry. All the claims are stored in their coordinate form. Area of their
intersections are accumulated, which leads to some areas counted more than once. These excess areas are then removed by
scanning which claims are overlapping.

There is a 'slight' difference in efficiency, as is shown by the following print. (`03a-2` was changed to use
multi-threading to save some time.)

```
$ time ./03a-1
total area: 120408

real  0m0,019s
user  0m0,013s
sys   0m0,007s

$ time ./03a-2
total area: 120408

real  471m38,844s
user  899m17,004s
sys   0m8,190s
```
