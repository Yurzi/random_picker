#ifndef __ANALYZAR_H__
#define __ANALYZAR_H__

#include <algorithm>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>
#include <memory>
#include <exception>
#include <Eigen/Eigen>

namespace yurzi {

class FileHandler {
public:
    FileHandler(const std::string& filename, const std::ios::openmode& openmode) {
        m_fd.open(filename, openmode);
        if(!m_fd.is_open()) {
            throw std::invalid_argument("File cannot be opend!");
        }
        this->status = true;
    }

    std::vector<int64_t> get_one_res() {
        std::string line;
        std::getline(m_fd, line);
        if(m_fd.eof()) {
            status = false;
            return {};
        }
        std::stringstream ss;
        ss << line;
        int64_t temp = 0;
        std::vector<int64_t> res;
        while(ss >> temp) {
            res.push_back(temp);
        }
        std::cout << "[FileHandler:get_one_res] res.size= " << res.size() <<std::endl;
        return std::move(res);
    }

    bool is_good() const { return this->status; }

    operator bool() {return status;}

private:
    std::fstream m_fd;
    bool status = true;
};


class Analyzar {
public:
    Analyzar(const std::string& filename) {
        m_fileHandler = std::make_shared<FileHandler>(filename, std::ios::in);
    }

    Analyzar(std::shared_ptr<FileHandler> fileHandler) : m_fileHandler(fileHandler) {}

    void init() {
        if (!m_fileHandler) {
            throw std::logic_error("Empty File");
        }
        std::vector<int64_t> record(m_fileHandler->get_one_res());

        // 找出最大组编号
        int max_group_index = 1;
        for (auto i : record) {
            if (i > max_group_index) {
                max_group_index = i;
            }
        }

        //  初始化计数矩阵
        count_Matrix = std::make_shared<Eigen::MatrixXi>(record.size(), max_group_index);
        count_Matrix->setZero();

        // 开始计数矩阵
        add_record(record);
        while (m_fileHandler->is_good()) {
            record = m_fileHandler->get_one_res();
            std::cout << "[Analyzar:init] check m_fileHandler" << m_fileHandler->is_good() << std::endl;
            if (m_fileHandler->is_good())add_record(record);
        } 

        std::cout << "[Analyzar:init] cout_Matrix: \n[" << *count_Matrix << "]" << std::endl;
    }


    std::vector<int64_t> predict() {
        // 找到每个人中被分到组最小的index;
        Eigen::MatrixXi &matrix = *count_Matrix;

        Eigen::MatrixXi temp_Matrix(matrix.rows(), matrix.cols());

        // 整理矩阵
        for (int64_t person = 0; person < matrix.rows(); ++person) {
            std::vector<std::pair<int64_t, int64_t>> temp_queue;
            for (int64_t index = 0; index  < matrix.cols(); ++index) {
                temp_queue.push_back(std::pair<int64_t, int64_t>(matrix(person, index), index + 1));
            }
            std::cout << "[Analyzar:predict:104] temp_queue before sort:" << std::endl;
            for (auto i : temp_queue) {
                std::cout << "{" << i.first << "," << i.second << "}" << " ";
            }
            std::cout << std::endl;
            // 将次数和组别以小到大排序
            std::sort(temp_queue.begin(), temp_queue.end(),
                      [](std::pair<int64_t, int64_t> &a, std::pair<int64_t, int64_t>& b) -> bool{
                          if (a.first == b.first) return a.second < b.second;
                          return a.first < b.first;
                      });
            // 将数据录入到矩阵
            std::cout << "[Analyzar:predict:116] temp_queue after sort:" << std::endl;
            for (auto i : temp_queue) {
                std::cout << "{" << i.first << "," << i.second << "}" << " ";
            }
            std::cout << std::endl;
            for (int64_t index = 0; index < temp_Matrix.cols(); ++index) {
                temp_Matrix(person, index) = temp_queue[index].second;
            }
        }

        std::cout << "[Analyzar:predict] temp_Matrix: \n[" << temp_Matrix << "]" <<std::endl;

        //进行统计和预测
        //获取每组限制
        int64_t persons = matrix.rows();
        int64_t groups = matrix.cols();
        int64_t per_group_count = persons / groups;
        std::vector<int64_t> group_slice;
        for (int index = 0; index < matrix.cols(); ++index) {
            if (persons > per_group_count) {
                group_slice.push_back(per_group_count);
            } else {
                group_slice.push_back(persons);
            }

            persons -= per_group_count;
        }
        std::cout << "[Analyzar:predict] group_slice:";
        for (auto i : group_slice) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        // 生成结果
        persons = matrix.rows();
        std::cout << "[Analyzar:preditc:140] persons = " << persons << std::endl;
        std::vector<int64_t> res;
        for (int64_t person = 0; person < persons; ++person) {
            int is_dispatched = false;
            for (int64_t index = 0; index < groups; ++index) {
                if (group_slice[temp_Matrix(person, index) - 1] > 0) {
                    std::cout << "[Analyzar:predict] res.push_back =" << temp_Matrix(person, index) << std::endl;
                    res.push_back(temp_Matrix(person, index));
                    is_dispatched = true;
                    group_slice[temp_Matrix(person, index) - 1] -= 1;
                    break;
                }
            }
        }

        return std::move(res);
    }
private:
    void add_record(const std::vector<int64_t>& record) {
        Eigen::MatrixXi &matrix = *count_Matrix;

        for (int64_t person = 0; person < record.size(); ++person) {
            std::cout << "[Analyzar:add_record] Add cout" << std::endl;
            matrix(person, record[person] - 1) += 1;
        }
    }
private:
    std::shared_ptr<FileHandler> m_fileHandler;
    std::shared_ptr<Eigen::MatrixXi> count_Matrix;
};


}   // namespace yurzi

#endif  // __ANALYZAR_H__
