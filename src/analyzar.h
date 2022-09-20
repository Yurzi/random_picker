#ifndef __ANALYZAR_H__
#define __ANALYZAR_H__

#include <stdexcept>
#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>
#include <memory>
#include <exception>

namespace yurzi {

class FileHandler {
public:
    FileHandler(const std::string& filename, const std::ios::openmode& openmode) {
        m_fd.open(filename, openmode);
        if(!m_fd.is_open()) {
            throw std::invalid_argument("File cannot be opend!");
        }
    }

    std::vector<int64_t> get_one_res() {
        char temp = 0;
        int64_t group_res = 0;
        std::vector<int64_t> list;
        do {
            m_fd >> group_res;
            list.push_back(group_res);
        } while (m_fd.get(temp) && temp != '\n');
        return std::move(list);
    }

private:
    std::fstream m_fd;
};


class Analyzar {
public:
    Analyzar(const std::string& filename) {
        m_fileHandler = std::make_shared<FileHandler>(filename, std::ios::in);
    }

    Analyzar(std::shared_ptr<FileHandler> fileHandler) : m_fileHandler(fileHandler) {}

    void init() {
        //todo something
    }

    std::vector<int64_t> predict() {
        return {};
    }
private:
    std::shared_ptr<FileHandler> m_fileHandler;
};


}   // namespace yurzi

#endif  // __ANALYZAR_H__
