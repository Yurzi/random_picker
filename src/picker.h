#ifndef __PICKER_H__
#define __PICKER_H__

#include <fstream>
#include <memory>
#include <vector>
#include <deque>

// 分组器
// 用于将一个数组分为几组
namespace yurzi {
template <class T>
class Picker{
public:
    typedef std::shared_ptr<Picker> ptr;

    Picker( T * const arr, int size) {
        data_.clear();
        for (int i = 0; i < size; ++i) {
            data_.push_back(arr[i]);
            data_orginal.push_back(arr[i]);
        }
    }
    Picker(std::vector<T>& arr) {
        data_.clear();
        for(T i : arr) {
            data_.push_back(i);
            data_orginal.push_back(i);
        }
    }

    // 设置每组数量
    void set_per_group_count(int count_) { per_group_count_ = count_; };
    // 获取每组数量
    int get_per_group_count() const { return per_group_count_; }

    std::pair<std::vector<T>, bool> get_one_group() {
        std::pair<std::vector<T>, bool> res;
        res.second = false;

        int data_size = data_.size();
        int count = 0;
        for (count = 0; count < per_group_count_ && count < data_size; count++) {
            res.first.push_back(data_.front());
            data_.pop_front();
        }

        if (count == per_group_count_) {
            res.second = true;
        }

        return std::move(res);
    }

public:
    static std::vector<int64_t> index_to_gruop_res(const std::deque<T>& groups_, const int64_t per_group_count, const std::vector<T>& meta_list) {
        std::vector<int64_t> list;
        std::deque<T> groups;

        // init deque
        groups = groups_;
        // init list
        for(int64_t i =0; i < meta_list.size(); ++i) {
            list.push_back(i);
        }

        int64_t count = 1;
        while(!groups.empty()) {
            // 从队列中取出 per_group_count的人将其分为count
            for (int i = 0; i < per_group_count && !groups.empty(); ++i) {
                T temp = groups.front();
                // 查找temp所对应与meta_list中的编号
                int index = 0;
                for(index = 0; index < meta_list.size(); ++index) {
                    if (meta_list[index] == temp) break;
                }
                // 将index对应的list中的值设为count;
                list[index] = count;
                groups.pop_front();
            }
            ++count;
        }
        return std::move(list);
    }

    static std::vector<int64_t> index_to_gruop_res(const std::vector<T>& groups_, const int64_t per_group_count, const std::vector<T>& meta_list) {
        std::deque<T> groups;
        for (auto i : groups_) {
            groups.push_back(i);
        }
        return std::move(index_to_gruop_res(groups, per_group_count, meta_list));
    }

    static void wirte_to_file(const std::string& filename, const std::ios::openmode opt_mode, std::shared_ptr<yurzi::Picker<T>> picker, std::vector<T>& meta_list) {
        std::vector<int64_t> list = index_to_gruop_res(picker->data_orginal, picker->get_per_group_count(), meta_list);
        // 将list 写入到文件
        std::ofstream fd(filename, opt_mode);
        for(int i = 0; i < list.size(); ++i) {
            fd << list[i] << (i < list.size() -1 ? " ":"");
        }
        fd << std::endl;
    }
private:
    std::deque<T> data_orginal;
    std::deque<T> data_;
    int per_group_count_ = 2;
};

}

#endif  //__PICKER_H__
