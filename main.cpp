#include "Sparse_matrix.hpp"
#include "chrono.hpp"
#include <iostream>

int main(){
/*
    algebra::SparseMatrix<double, algebra::StorageOrder::row_wise> mat_row(4,4);
    mat_row(0,0)=4;
    mat_row(0,1)=1;
    mat_row(1,0)=-1;
    mat_row(1,1)=4;
    mat_row(1,2)=-1;
    mat_row(2,1)=-1;
    mat_row(2,2)=4;
    mat_row(2,3)=-1;
    mat_row(3,2)=-1;
    mat_row(3,3)=4;

    algebra::SparseMatrix<double, algebra::StorageOrder::column_wise> mat_col(4,4);
    mat_col(0,0)=4;
    mat_col(0,1)=1;
    mat_col(1,0)=-1;
    mat_col(1,1)=4;
    mat_col(1,2)=-1;
    mat_col(2,1)=-1;
    mat_col(2,2)=4;
    mat_col(2,3)=-1;
    mat_col(3,2)=-1;
    mat_col(3,3)=4;
*/



    algebra::SparseMatrix<double, algebra::StorageOrder::row_wise> mat_row(0,0);
    mat_row.readMatrixMarket("lnsp_131.mtx");

    algebra::SparseMatrix<double, algebra::StorageOrder::column_wise> mat_col(0,0);
    mat_col.readMatrixMarket("lnsp_131.mtx");

    std::vector<double> vect(131, 1);

    Timings::Chrono chrono;

    // Test di velocità per la moltiplicazione con matrice non compressa row-wise
    chrono.start();
    std::vector<double> res_row = mat_row * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Tempo per la moltiplicazione con matrice non compressa row-wise: " << chrono;

    //for(const auto &val:res_row)
    //std::cout<<val<<std::endl;

    // Test di velocità per la moltiplicazione con matrice compressa row-wise
    mat_row.compress();
    chrono.start();
    std::vector<double> res_row_compressed = mat_row * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Tempo per la moltiplicazione con matrice compressa row-wise: " << chrono;

   //  for(const auto &val:res_row_compressed)
    //std::cout<<val<<std::endl;

        // Test di velocità per la moltiplicazione con matrice non compressa column-wise
    chrono.start();
    std::vector<double> res_col = mat_col * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Tempo per la moltiplicazione con matrice non compressa column-wise: " << chrono;
    
//for(const auto &val:res_col)
//    std::cout<<val<<std::endl;
    
    // Test di velocità per la moltiplicazione con matrice compressa column-wise
    mat_col.compress();
    chrono.start();
    std::vector<double> res_col_compressed = mat_col * vect;
    chrono.stop();
    std::cout<<std::endl;
    std::cout << "Tempo per la moltiplicazione con matrice compressa column-wise: " << chrono;

//for(const auto &val:res_col_compressed)
   // std::cout<<val<<std::endl;
    
    std::cout<<std::endl;

    return 0;
}

