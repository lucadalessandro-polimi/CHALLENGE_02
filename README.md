# CHALLENGE_02: SPARSE MATRIX IMPLEMENTATION
the folder contains the following files:

1. Insp_131.mtx: file that collects the values and dimension of the matrix to test

2. Sparse_matrix.hpp: the file has the template class SparseMatrix, with the declarations of the methods, the private members, and two definitions of the * operator and ostream operator.
(notice that the * operator is consistent also in the complex case, i put an easy example commented in the main.cpp to test it)
A specific tool tie is used in the struct ColumnComparator in order to manipulate the order of the matrix.

3. Sparse_matrix_impl_hpp: here we have all the definitions of the methods of the template class SparseMatrix, including the last two overloaded operators, the call one with its cost/non cost version

4. chrono.hpp: file to test the speed of some part of the code, in this case the * operator in the four cases requested.

5. main.cpp: here i test the matrix of the mtx file agaist a unitary vector to see the diffirences in speed of computation for the four cases; as expected the compressed case in both row/column wise order works faster.

6. Makefile: without any specific path for examples since i integreted chrono.hpp in the folder
