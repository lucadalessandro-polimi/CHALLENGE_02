#ifndef HH_SPARSE_MATRIX_HPP_HH
#define HH_SPARSE_MATRIX_HPP_HH

#include "Sparse_matrix.hpp"

namespace algebra{

template <class T,StorageOrder order>
SparseMatrix<T,order>::SparseMatrix(std::size_t r,std::size_t c){
      resize_matrix(r,c);
}

//in this way the resize matrix just save the new dimensions in case of creation of the object 
//then i must populate the matrix through the call operator 
template <class T, StorageOrder order>
void SparseMatrix<T,order>::resize_matrix(std::size_t r, std::size_t c) {
    
    if (!coo_map.empty() && isCompressed())
        uncompress();

    if (r < n_rows || c < n_cols) {
        auto it = coo_map.begin();
        while (it != coo_map.end()) {
            if (it->first[0] >= r || it->first[1] >= c) {
                it = coo_map.erase(it);
            } else {
                ++it;
            }
        }
        n_rows = r;
        n_cols = c;
    }
}


template <class T,StorageOrder order>
    T& SparseMatrix<T,order>:: operator()(std::size_t row, std::size_t col) {
     
        
    if (isCompressed()) {
        
        if (order == StorageOrder::row_wise) {

            auto row_begin = row_idx[row];
            auto row_end = row_idx[row + 1];
            
            for (std::size_t i = row_begin; i < row_end; ++i) {
               if (col_idx[i] == col) 
                return val[i];
            }
            
            std::cerr << "Element not present in the compressed matrix" << std::endl;
            static T default_value;
            return default_value;
        }
        
        else{
            auto col_begin = col_idx[col];
            auto col_end = col_idx[col + 1];
            
            for (std::size_t i = col_begin; i < col_end; ++i) {
               if (row_idx[i] == row) 
                return val[i];
            }
            
            std::cerr << "Element not present in the compressed matrix" << std::endl;
            static T default_value;
            return default_value;
        }
    }
    else{
    
        auto it = coo_map.find({row, col});
        
          if (it == coo_map.end()) 
             return coo_map[{row, col}];
         else 
            // La coppia di indici esiste già nella mappa, restituisci il valore esistente
            return it->second;
      }
    }
    
template <class T,StorageOrder order>
    T SparseMatrix<T,order>:: operator()(std::size_t row, std::size_t col) const {
        
    if(isCompressed()){
            
        if (order == StorageOrder::row_wise) {

            auto row_begin = row_idx[row];
            auto row_end = row_idx[row + 1];
            
            for (std::size_t i = row_begin; i < row_end; ++i) {
               if (col_idx[i] == col) 
                return val[i];
            }
            
            if(row<n_rows && col<n_cols)
              return 0;
            
            else{
            std::cerr << "Element not present in the compressed matrix" << std::endl;
            static T default_value;
            return default_value;
            }
        }
        
        else{
            auto col_begin = col_idx[col];
            auto col_end = col_idx[col + 1];
            
            for (std::size_t i = col_begin; i < col_end; ++i) {
               if (row_idx[i] == row) 
                return val[i];
            }
            
            if(row<n_rows && col<n_cols)
              return 0;
            
            else{
            std::cerr << "Element not present in the compressed matrix" << std::endl;
            static T default_value;
            return default_value;
            }
        }
    }
    
    else{
     
        auto it = coo_map.find({row, col});
        
          if (it == coo_map.end()) {
            if(row<n_rows && col<n_cols)
              return 0;
            else{
                std::cerr << "Element not in the bounds of the matrix" << std::endl;
                static T default_value;
                return default_value;
              }
          }
         else 
        
            return it->second;
    }

} 

template <class T, StorageOrder order>
void SparseMatrix<T, order>::compress() {
    if (compressed)
        return;

 if (order == StorageOrder::row_wise) {
    val.clear();
    col_idx.clear();
    row_idx.resize(n_rows + 1);
 
    row_idx[0] = 0;

        std::size_t total_values = 0;

        for (std::size_t i = 0; i < n_rows; ++i) {
            for (std::size_t j = 0; j < n_cols; ++j) {
                auto it = coo_map.find({i, j});
                if (it != coo_map.end()) {
                    val.push_back(it->second);
                    col_idx.push_back(j);
                    total_values++;
                }
            }
            row_idx[i + 1] = total_values;
        }
    } 

    else {
    val.clear();
    col_idx.resize(n_cols+1);
    row_idx.clear();
 
    col_idx[0] = 0;

        std::size_t total_values = 0;

        for (std::size_t j = 0; j < n_cols; ++j) {
            for (std::size_t i = 0; i < n_rows; ++i) {
                auto it = coo_map.find({i, j});
                if (it != coo_map.end()) {
                    val.push_back(it->second);
                    row_idx.push_back(i);
                    total_values++;
                }
            }
            col_idx[j + 1] = total_values;
        }
    } 

    compressed = true;
}



template <class T, StorageOrder order>
void SparseMatrix<T, order>::uncompress() {
    if (!compressed)
        return;

    coo_map.clear();

    if (order == StorageOrder::row_wise) {
        for (std::size_t i = 0; i < n_rows; ++i) {
            for (std::size_t j = row_idx[i]; j < row_idx[i + 1]; ++j) {
                const std::size_t col = col_idx[j];
                const T value = val[j];
                coo_map[{i, col}] = value;
            }
        }
    } else if (order == StorageOrder::column_wise) {
        for (std::size_t j = 0; j < n_cols; ++j) {
            for (std::size_t i = col_idx[j]; i < col_idx[j + 1]; ++i) {
                const std::size_t row = row_idx[i]; // In ordine per colonne, invertiamo riga e colonna
                const T value = val[i];
                coo_map[{row, j}] = value;
            }
        }
    }

   
    val.clear();
    col_idx.clear();
    row_idx.clear();
    
    compressed = false;
}

template <class T, StorageOrder order>
void SparseMatrix<T, order>::print() const {

        if (isCompressed()) {

            if(order == StorageOrder::row_wise)
            std::cout << "Compressed Sparse Row (CSR) format:" << std::endl;
            else
            std::cout << "Compressed Sparse Column (CSC) format:" << std::endl;
            std::cout << "val:";
            for (const auto& v : val) {
                std::cout << " " << v;
            }
            std::cout << std::endl;

            std::cout << "col_idx:";
            for (const auto& idx : col_idx) {
                std::cout << " " << idx;
            }
            std::cout << std::endl;

            std::cout << "row_idx:";
            for (const auto& idx : row_idx) {
                std::cout << " " << idx;
            }
            std::cout << std::endl;
        } else {
            std::cout << "Matrix is not compressed. Printing COO format:" << std::endl;
            if (coo_map.empty()) {
                std::cout << "NO ENTRIES" << std::endl;
            } else {
                for (const auto& pair : coo_map) {
                    const auto& key = pair.first;
                    const auto& value = pair.second;
                    std::cout << "Indices: [" << key[0] << ", " << key[1] << "] - Value: " << value << std::endl;
                }
            }
        }
    }


template <class T, StorageOrder order>
void SparseMatrix<T,order>::readMatrixMarket(const std::string& filename) {
    std::ifstream in(filename); 
    std::string line;
    std::getline(in, line);

    // Ignora i commenti (linee che iniziano con "%")
    while (line[0] == '%') {
        std::getline(in, line);
    }

    std::istringstream first_line(line);
    std::size_t rows,cols,m_nonZeros;
    first_line >> rows >> cols >> m_nonZeros; 
    resize_matrix(rows,cols);

   for (std::size_t k = 0; k < m_nonZeros; ++k) {
        std::getline(in, line);
        std::istringstream current_line(line);
        std::size_t i, j;
        T value;
        current_line >> i >> j >> value;
        std::array<std::size_t, 2> key = {i - 1, j - 1};
        coo_map[key] = value;
    }
}


}




#endif