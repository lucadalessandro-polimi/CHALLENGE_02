#include "Sparse_matrix.hpp"
#include <iostream>

int main(){

   /* algebra::SparseMatrix<double,algebra::StorageOrder::column_wise> matrix(4,4);
    matrix(0,0)=4;
    matrix(0,1)=1;
    matrix(1,0)=-1;
    matrix(1,1)=4;
    matrix(1,2)=-1;
    matrix(2,1)=-1;
    matrix(2,2)=4;
    matrix(2,3)=-1;
    matrix(3,2)=-1;
    matrix(3,3)=4;
   

   matrix.compress();
   //matrix.uncompress();
   matrix.print();

    std::vector<double> vect={1,2,3,5};
    std::vector<double> res=matrix*vect;

    for(const auto &val:res)
      std::cout<<val<<std::endl;

*/
     algebra::SparseMatrix<double,algebra::StorageOrder::column_wise> mat(0,0);
     mat.readMatrixMarket("lnsp_131.mtx");
     mat.compress();
     std::vector<double> vect(131,1);
     std::vector<double> res=mat*vect;

     for(const auto &val:res)
      std::cout<<val<<std::endl;
    
     

return 0;
}