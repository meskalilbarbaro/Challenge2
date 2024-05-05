#include<map>
#include<vector>
#include<array>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

namespace algebra{
    enum class StorageOrder { RowMajor, ColumnMajor };


    template <typename T, StorageOrder Order> class Matrix{
    

    private:
    std::size_t n_rows=0;
    std::size_t n_columns=0;


    // Data Uncompressed
    std::map<std::array<std::size_t,2>,T> value_map;


    // Data Compressed
    std::vector<T> values;
    std::vector<std::size_t> outer_index;
    std::vector<std::size_t> inner_index;

    public:
    // Constructors
    Matrix()=default;

    // Setters
    
    void read_matrix(const std::string&);
    void set_dimensions(std::size_t,std::size_t);

     // Compressing Methods & Functions
    bool is_compressed()const;
    void compress();
    void uncompress();
    void transpose();


    // Printer
    void print_matrix()const;

    // Call Operator
    T operator()(std::array<std::size_t,2>)const;
    T& operator()(std::array<std::size_t,2>);
    std::vector<T> operator*(const std::vector<T>&);





    };
    template <typename T, StorageOrder Order>
    void Matrix<T,Order>::transpose(){
        if(is_compressed()){
            throw std::runtime_error("Trying to transpose a compressed matrix");
        }
        else{
            std::map<std::array<std::size_t,2>,T> new_map;
            for(auto iter=value_map.begin();iter!=value_map.end();iter++){
                std::array<std::size_t,2> temp;
                temp[0]=iter->first[1];
                temp[1]=iter->first[0];
                new_map[temp]=iter->second;
            }
            value_map=new_map;
        }
    }
    template <typename T, StorageOrder Order>
    std::vector<T> Matrix<T,Order>::operator*(const std::vector<T>& vector){
        std::vector<T> result;
        if(!is_compressed()){
            for(std::size_t r=0;r<n_rows;r++){
                T sum=0;
                for(std::size_t i=0;i<vector.size();i++){
                    sum=sum+vector[i]*operator()({r,i});
                }
                result.push_back(sum);
            }
        }
        else{
            if(Order==StorageOrder::ColumnMajor){
                uncompress();
                transpose();
                compress();
            }
            std::size_t count=0;
            for(std::size_t r=0;r<inner_index.size()-1;r++){
                std::size_t bound=inner_index[r+1]-inner_index[r];
                T sum=0;
                for(std::size_t i=0;i<bound;i++){
                    sum=sum+vector[i]*values[count];
                    count++;
                }
                result.push_back(sum);
            }
        }
        return result;
    }
    template <typename T, StorageOrder Order>
    void Matrix<T,Order>::uncompress(){
        if(!is_compressed()){
            std::cout<<"The matrix is already uncompressed..."<<std::endl;
        }
        else{
            std::size_t count=0;
            for(std::size_t r=0;r<inner_index.size()-1;r++){
                std::size_t bound=inner_index[r+1]-inner_index[r];
                for(std::size_t i=0;i<bound;i++){
                    value_map[{r,outer_index[count]}]=values[count];
                    count++;
                }
            }
            values.clear();
            outer_index.clear();
            inner_index.clear();
            if(Order==StorageOrder::ColumnMajor){
                transpose();
            }
        }
    }
    template <typename T, StorageOrder Order>
    void Matrix<T,Order>::compress(){
        if(is_compressed()){
            std::cout<<"The matrix is already compressed..."<<std::endl;
        }
        else{
            if(Order==StorageOrder::ColumnMajor){
                transpose();
            }
            std::size_t count=0;
            std::size_t r=0;
            inner_index.push_back(0);
            for(auto iter=value_map.begin();iter!=value_map.end();iter++){
                std::size_t row=iter->first[0];
                std::size_t col=iter->first[1];
                outer_index.push_back(col);
                values.push_back(iter->second);
                while(r<row){
                    inner_index.push_back(count);
                    r++;
                }
                count++;
            }
            while(r<n_rows){
                inner_index.push_back(count);
                r++;
            }
            value_map.clear();
        }
    }

    // Setters
    template <typename T, StorageOrder Order>
    void Matrix<T,Order>::set_dimensions(std::size_t n,std::size_t m){
        n_rows=n;
        n_columns=m;
    }
    // Read Matrix
    template <typename T, StorageOrder Order>
    void Matrix<T,Order>::read_matrix(const std::string& filename){
        

        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file");
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line[0] == '%') continue;  // Skip comments
            break;
        }
        std::istringstream iss(line);
        std::size_t rows,cols;
        iss >> n_rows >> n_columns;
        while (std::getline(file, line)){
            std::istringstream iss(line);
            std::size_t a,b;
            double value;
            iss>>a>>b>>value;
            operator()({a-1,b-1})=value;
        }
    }
    template <typename T, StorageOrder Order>
    void Matrix<T,Order>::print_matrix()const{
        if(is_compressed()){
            std::cout<<"The matrix is compressed... "<<std::endl;
            // Printing values
            std::cout<<"The values vector is: "<<std::endl;
            for(std::size_t i=0;i<values.size();i++){
                std::cout<<values[i]<<" ";
            }
            std::cout<<std::endl;
            // Printing the outer indexes
            std::cout<<"The outer indexes are: "<<std::endl;
            for(std::size_t i=0;i<outer_index.size();i++){
                std::cout<<outer_index[i]<<" ";
            }
            std::cout<<std::endl;
            // Printing the inner indexes
            std::cout<<"The inner indexes are: "<<std::endl;
            for(std::size_t i=0;i<inner_index.size();i++){
                std::cout<<inner_index[i]<<" ";
            }
            std::cout<<std::endl;
        }
        else{
            std::cout<<"The matrix is not compressed... "<<std::endl;
            // Printing the mapped values
            for(auto iter=value_map.begin();iter!=value_map.end();iter++){
                std::cout<<"[("<<iter->first[0]<<","<<iter->first[1]<<"),"<<iter->second<<"] ";
            }
            std::cout<<std::endl;
        }
    }
    template <typename T, StorageOrder Order>
    bool Matrix<T,Order>::is_compressed()const{
        if(inner_index.size()>0) return true;
        else return false;
    }
    template <typename T, StorageOrder Order>
    T Matrix<T,Order>::operator()(std::array<std::size_t,2> coordinates)const{
        if(is_compressed()){
            throw std::runtime_error("Trying to access a compressed matrix");
        }
        if((coordinates[0]>=n_rows)||(coordinates[1]>=n_columns)){
            throw std::runtime_error("Wrong matrix dimensions - matrix not initialized");
        }
        if(value_map.find(coordinates)!=value_map.end()){
            return value_map.find(coordinates)->second;
        }
        else return 0;
    }
    template <typename T, StorageOrder Order>
    T& Matrix<T,Order>::operator()(std::array<std::size_t,2> coordinates){
        if(is_compressed()){
            throw std::runtime_error("Trying to access a compressed matrix");
        }
        if((coordinates[0]>=n_rows)||(coordinates[1]>=n_columns)){
            throw std::runtime_error("Wrong matrix dimensions - matrix not initialized");
        }
        return value_map[coordinates];
        
    }
};
