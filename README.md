compression.h
------------------------------------------
an implementation of huffman coding

generics.h
------------------------------------------
messing around with concepts feature added in C++20

main.cpp
------------------------------------------
an arbitray competition with friends,
the task being to iterate through an array
(thats indexable like a matrix), for each 
element re-seeding random func with current 
index and then setting random value as fast 
as possible. Originally this was seeded with
difference in time but this impacted langs
disproportionally due to sys calls, this
can be seen in past commits.

mat.h
------------------------------------------
a matrix data structure used for ML, Matrix 
competition and anything else thatll need it.

min_mat.h
------------------------------------------
a light version of mat.h for simpler usecases

parse.h
------------------------------------------
a small text parsing header

point.h
------------------------------------------
a data structure for points of varrying length
and type

tpool.h
------------------------------------------
a light performance focused thread pool used in
any thing i'll need multi threading for, for
example the matrix thing

stpool.h
------------------------------------------
a thread pool similiar to tpool except it has
one shared queue, this should be faster for 
programs that have poorly distributed jobs in
terms of workload consitency. This thread pool 
is also lock less
