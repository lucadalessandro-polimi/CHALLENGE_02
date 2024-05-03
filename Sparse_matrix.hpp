#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <complex>

namespace algebra{

// Definition of the enum for the matrix storage order
enum class StorageOrder{
    row_wise,
    column_wise,
    };

// Functor for comparing two arrays to sort by columns
struct ColumnComparator {
    bool operator()(const std::array<std::size_t, 2>& a, const std::array<std::size_t, 2>& b) const {
        return std::tie(a[1], a[0]) < std::tie(b[1], b[0]);
    }
};    

// Declaration of the SparseMatrix class
template <class T,StorageOrder order>
class SparseMatrix{
public:
  // Constructor of the class
  SparseMatrix(std::size_t,std::size_t);
  
  // Operator for accessing matrix elements
  T& operator()(std::size_t row, std::size_t col);
  T operator()(std::size_t row, std::size_t col) const;
  
  // Static member function for comparing two values of type U
  template <typename U>
    static bool not_equal(const U& a, const U& b) {
        return a != b;
    }

  // Declaration of the matrix-vector multiplication operator
  friend std::vector<T> operator*(const SparseMatrix<T, order>& matrix,const std::vector<T> &rhs){
    std::vector<T> res(matrix.n_rows);
    
    if(matrix.n_cols!=rhs.size()){
       std::cerr<<"Not compatible dimensions for product"<<std::endl;
       static T default_value;
       return std::vector(matrix.n_rows,default_value);
    }
    else{

        if (order == StorageOrder::row_wise) {
            if (matrix.isCompressed()) {
                
                for (std::size_t i = 0; i < matrix.n_rows; ++i) {
                  T sum = 0;
                  std::size_t row_begin = matrix.row_idx[i];
                  std::size_t row_end = matrix.row_idx[i + 1];
                      for (std::size_t k = row_begin; k < row_end; ++k) {
                            std::size_t col = matrix.col_idx[k];
                            sum += matrix.val[k] * rhs[col];
                        }
                      res[i] = sum;
                }
            }
            else{
                for (const auto& entry : matrix.coo_map) {
                   std::size_t row = entry.first[0];
                   std::size_t col = entry.first[1];
                   T value = entry.second;
                   
                   res[row] += value * rhs[col];
                }
            }
          }
        

        else{
            if (matrix.isCompressed()) {
               
              for (std::size_t j = 0; j < matrix.n_cols; ++j) {
                    T col_val = rhs[j];
                    if (not_equal(col_val,T(0))) {
                        std::size_t col_begin = matrix.col_idx[j];
                        std::size_t col_end = matrix.col_idx[j + 1];
                        for (std::size_t k = col_begin; k < col_end; ++k) {
                            std::size_t row = matrix.row_idx[k];
                            res[row] += matrix.val[k] * col_val;
                        }
                    }
              }
               
            
           }else{
                for (const auto& entry : matrix.coo_map) {
                    std::size_t row = entry.first[0];
                    std::size_t col = entry.first[1];
                    T value = entry.second;

                    res[row] += value * rhs[col];
                }
           }   
        }
    
    return res;
    }
  }

// Declaration of the stream operator for printing the matrix
  friend std::ostream& operator<<(std::ostream& os, const SparseMatrix<T, order>& matrix) {
        if (matrix.isCompressed()) {
            os << (order == StorageOrder::row_wise ? "Compressed Sparse Row (CSR) format:" : "Compressed Sparse Column (CSC) format:") << std::endl;
            os << "val:";
            for (const auto& v : matrix.val) {
                os << " " << v;
            }
            os << std::endl;

            os << "col_idx:";
            for (const auto& idx : matrix.col_idx) {
                os << " " << idx;
            }
            os << std::endl;

            os << "row_idx:";
            for (const auto& idx : matrix.row_idx) {
                os << " " << idx;
            }
            os << std::endl;
        } else {
            os << "Matrix is not compressed. Printing COO format:" << std::endl;
            if (matrix.coo_map.empty()) {
                os << "NO ENTRIES" << std::endl;
            } else {
                for (const auto& pair : matrix.coo_map) {
                    const auto& key = pair.first;
                    const auto& value = pair.second;
                    os << "Indices: [" << key[0] << ", " << key[1] << "] - Value: " << value << std::endl;
                }
            }
        }
        return os;
    }


  // Declaration of functions to compress and uncompress the matrix 
  void compress();
  void uncompress();
 
 // Declaration to read and populate the matrix from the mtx file 
  void readMatrixMarket(const std::string& filename);

  // Declaration of function to check if the matrix is compressed 
  inline bool isCompressed() const {
        return compressed;
  }

private:
// Use ColumnComparator to sort the map by columns
   using ColumnOrderedMap = std::map<std::array<std::size_t, 2>, T, ColumnComparator>;
// Use default row-wise ordering
   using RowOrderedMap = std::map<std::array<std::size_t, 2>, T>;
// If the storage order is column-wise, use ColumnOrderedMap, otherwise RowOrderedMap
   using SelectedMap = std::conditional_t<order == StorageOrder::column_wise, ColumnOrderedMap, RowOrderedMap>;
   
   std::size_t n_rows;
   std::size_t n_cols;
   
   // For the uncompressed version of the matrix
   SelectedMap coo_map;
   bool compressed = false;
   
   // For the compressed version of the matrix
   std::vector<T> val;
   std::vector<std::size_t> row_idx;
   std::vector<std::size_t> col_idx;
   
   //Declaration to resize the matrix with the new dimensions
   void resize_matrix(std::size_t,std::size_t);
};
}


#include "Sparse_matrix_impl.hpp"

#endif /* SPARSE_MATRIX_HPP */