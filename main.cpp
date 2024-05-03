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
    std::cout << "Tempo per la moltiplicazione con matrice non compressa row-wise: " << chrono;


    // speed test for compressed row-wise matrix multiplication
    mat_row.compress();
    chrono.start();
    std::vector<double> res_row_compressed = mat_row * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Tempo per la moltiplicazione con matrice compressa row-wise: " << chrono;

     
   // speed test for non compressed column-wise matrix multiplication
    chrono.start();
    std::vector<double> res_col = mat_col * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Tempo per la moltiplicazione con matrice non compressa column-wise: " << chrono;

    
    // speed test for compressed column-wise matrix multiplication
    mat_col.compress();
    chrono.start();
    std::vector<double> res_col_compressed = mat_col * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Tempo per la moltiplicazione con matrice compressa column-wise: " << chrono;

    std::cout<<std::endl;
    

    return 0;
}

