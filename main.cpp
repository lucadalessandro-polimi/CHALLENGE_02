#include "Sparse_matrix.hpp"
#include "chrono.hpp"
#include <iostream>


int main(){

    algebra::SparseMatrix<double, algebra::StorageOrder::row_wise> mat_row(0,0);
    mat_row.readMatrixMarket("lnsp_131.mtx");

    algebra::SparseMatrix<double, algebra::StorageOrder::column_wise> mat_col(0,0);
    mat_col.readMatrixMarket("lnsp_131.mtx");

    std::vector<double> vect(131,1);

    
    Timings::Chrono chrono;

    // speed test for non compressed row-wise matrix multiplication
    chrono.start();
    std::vector<double> res_row = mat_row * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Time for multiplication with non compressed row-wise matrix: " << chrono;


    // speed test for compressed row-wise matrix multiplication
    mat_row.compress();
    chrono.start();
    std::vector<double> res_row_compressed = mat_row * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Time for multiplication with compressed row-wise matrix: " << chrono;

     
   // speed test for non compressed column-wise matrix multiplication
    chrono.start();
    std::vector<double> res_col = mat_col * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Time for multiplication with non compressed column-wise matrix: " << chrono;

    
    // speed test for compressed column-wise matrix multiplication
    mat_col.compress();
    chrono.start();
    std::vector<double> res_col_compressed = mat_col * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Time for multiplication with compressed column-wise matrix: " << chrono;

    std::cout<<std::endl;

/*
algebra::SparseMatrix<std::complex<double>, algebra::StorageOrder::row_wise> mat(4, 4);
    mat(0, 0) = {4.0, 0.0};
    mat(0, 1) = {1.0, 0.0};
    mat(1, 0) = {-1.0, 0.0};
    mat(1, 1) = {4.0, 0.0};
    mat(1, 2) = {-1.0, 0.0};
    mat(2, 1) = {-1.0, 0.0};
    mat(2, 2) = {4.0, 0.0};
    mat(2, 3) = {-1.0, 0.0};
    mat(3, 2) = {-1.0, 0.0};
    mat(3, 3) = {4.0, 0.0};

    std::vector<std::complex<double>> rhs(4, {1.0, 0.0});
    std::vector<std::complex<double>> res=mat*rhs;

    for(auto const &val:res)
       std::cout<<val<<std::endl;
*/
    return 0;
}

