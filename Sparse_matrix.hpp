#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>

namespace algebra{

enum class StorageOrder{
    row_wise,
    column_wise,
    };

// Functore per confrontare due array per ordinare per colonne
struct ColumnComparator {
    bool operator()(const std::array<std::size_t, 2>& a, const std::array<std::size_t, 2>& b) const {
        return std::tie(a[1], a[0]) < std::tie(b[1], b[0]);
    }
};    

template <class T,StorageOrder order>
class SparseMatrix{
public:
  SparseMatrix(std::size_t,std::size_t);
  
  T& operator()(std::size_t row, std::size_t col);
  T operator()(std::size_t row, std::size_t col) const;
  
 
  friend std::vector<T> operator*(const SparseMatrix<T, order>& matrix,const std::vector<T> &rhs){
    std::vector<T> res(matrix.n_rows);
    
    if(matrix.n_cols!=rhs.size()){
       std::cerr<<"Not compatible dimensions for product"<<std::endl;
       static T default_value;
       return std::vector(matrix.n_rows,default_value);
    }
    else{
  
        for (std::size_t i = 0; i < matrix.n_rows; ++i) {
               T sum = 0;
                    
                for (std::size_t j = 0; j < matrix.n_cols; ++j) {
                        sum += matrix(i, j) * rhs[j];
                }
                res[i] = sum;
        }   
    
    return res;
    }
  }

  void compress();
  void uncompress();
  void print() const;

  void readMatrixMarket(const std::string& filename);

  inline bool isCompressed() const {
        return compressed;
  }

private:
// Usiamo ColumnComparator per ordinare la mappa in base alle colonne
   using ColumnOrderedMap = std::map<std::array<std::size_t, 2>, T, ColumnComparator>;
// Usiamo l'ordinamento per righe di default
   using RowOrderedMap = std::map<std::array<std::size_t, 2>, T>;
// Se l'ordine di memorizzazione è per colonne, utilizziamo ColumnOrderedMap, altrimenti RowOrderedMap
   using SelectedMap = std::conditional_t<order == StorageOrder::column_wise, ColumnOrderedMap, RowOrderedMap>;
   
   std::size_t n_rows;
   std::size_t n_cols;
   
   //for compressed version
   SelectedMap coo_map;
   bool compressed = false;
   //for uncompressed version
   std::vector<T> val;
   std::vector<std::size_t> row_idx;
   std::vector<std::size_t> col_idx;
   
   void resize_matrix(std::size_t,std::size_t);
};
}


#include "Sparse_matrix_impl.hpp"

#endif /* SPARSE_MATRIX_HPP */