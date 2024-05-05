#include "SparseMatrix.hpp"
#include <chrono>
int main(){
    algebra::Matrix<double,algebra::StorageOrder::RowMajor> A;
    



    A.read_matrix("lnsp_131.mtx");
    std::vector<double> b(131,1);
    std::vector<double> c;

    // Timing the multiplication (UNCOMPRESSED)
    auto start1 = std::chrono::high_resolution_clock::now();
    c=A*b;
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds1 = end1 - start1;
    std::cout << "Matrix multiplication (UNCOMPRESSED) took: " << elapsed_seconds1.count() << " seconds\n";
    // Compressing
    A.compress();

    // Timing the multiplication (COMPRESSED)
    auto start2 = std::chrono::high_resolution_clock::now();
    c=A*b;
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds2 = end2 - start2;
    std::cout << "Matrix multiplication (COMPRESSED) took: " << elapsed_seconds2.count() << " seconds\n";

    // Printing result

    for(std::size_t i=0;i<c.size();i++){
        std::cout<<c[i]<<" ";
    }
    std::cout<<std::endl;
    
}