#include <QString>
#include <QMessageBox>

#include <sstream>
#include <exception>

#include "gui.h"

#include "ui/ui_main.h"

namespace yurzi {

MainWindow::MainWindow(QWidget *parent)
: QWidget(parent)
, ui(std::make_shared<Ui::MainWindow>()) {

    // init
    ui->setupUi(this);

    //slots connection
    connect(ui->input_person_number, SIGNAL(textChanged(QString)), this, SLOT(slot_enable_picker()));
    connect(ui->input_group_number, SIGNAL(textChanged(QString)), this, SLOT(slot_enable_picker()));
    connect(ui->btn_picker, SIGNAL(clicked()), this, SLOT(slot_on_picker_clicked()));
    connect(ui->input_file_addr, SIGNAL(textChanged(QString)), this, SLOT(slot_enable_read_file()));
    connect(ui->btn_data_read, SIGNAL(clicked()), this, SLOT(slot_on_read_data_clicked()));
    connect(ui->btn_predict, SIGNAL(clicked()), this, SLOT(slot_on_predict_clicked()));
}

MainWindow::~MainWindow() {
    ui.reset();
}

void MainWindow::slot_enable_picker() {
    int temp = 0;

    bool bAok = false;
    temp = ui->input_person_number->text().toInt(&bAok);
    if (bAok) bAok = temp > 0;

    bool bBok = false;
    temp = ui->input_group_number->text().toInt(&bBok);
    if (bBok) bBok = temp > 0;
    
    ui->btn_picker->setEnabled(bAok&bBok);
}

void MainWindow::slot_on_picker_clicked() {
    // 生成vector
    int64_t person_number = ui->input_person_number->text().toInt();
    std::vector<int64_t> person_list;
    for (int64_t i = 1; i <= person_number; ++i) {
        person_list.push_back(i);
    }

    // 生成randomizer
    this->randomizer = std::make_shared<yurzi::Randomizer<int64_t>>(person_list);
    std::vector<int64_t> temp = randomizer->get_result();

    // 生成picker
    int64_t group_number = ui->input_group_number->text().toInt();
    int64_t per_group_number = person_number / group_number;
    this->picker = std::make_shared<yurzi::Picker<int64_t>>(temp);
    this->picker->set_per_group_count(per_group_number);

    // 输出
    ui->output_picker_res->setText("");
    bool flag = true;
    int64_t group_count = 1;
    std::stringstream ss;
    while(flag) {
        auto temp = this->picker->get_one_group();
        flag = temp.second;

        if (temp.first.empty()) break;

        ss << "第" << group_count << "组：{";
        for (int i = 0; i < temp.first.size(); ++i) {
            ss << temp.first.at(i) << (i < temp.first.size() -1 ? ",":"");
        }
        ss << "}" << std::endl;
        ui->output_picker_res->setText(QString::fromStdString(ss.str()));

        // 计数器加1
        ++group_count;
    }
    // 输出到文件
    this->picker->wirte_to_file("res.txt", std::ios::app, this->picker, person_list);
}

void MainWindow::slot_enable_read_file() {
    int ok = 0;
    ok = ui->input_file_addr->text().toStdString().size();
    ui->btn_data_read->setEnabled(ok);
}

void MainWindow::slot_on_read_data_clicked() {
    ui->btn_predict->setEnabled(false);
    std::string filename = ui->input_file_addr->text().toStdString();
    try{
        this->analyzar = std::make_shared<yurzi::Analyzar>(filename);
    }catch(std::exception e) {
        QMessageBox::warning(this, "Error", "File does not exist or can not be found");
        return;
    }

    ui->btn_predict->setText("初始化...");
    this->analyzar->init();
    ui->btn_predict->setText("预测");
    ui->btn_predict->setEnabled(true);
}

void MainWindow::slot_on_predict_clicked() {
    std::vector<int64_t> res = this->analyzar->predict();
    std::cout << "[MainWindow:slot_on_predict_clicked] clicked and res.size=" << res.size() << std::endl;
    // 简单输出
    std::stringstream ss;
    for(auto i : res) {
        ss << i << " ";
    }
    ss << std::endl;
    std::string display = ss.str();
    
    // 找到最大组别
    int64_t max_group_index = 1;
    for (auto i : res) {
        max_group_index = i > max_group_index ? i : max_group_index;
    }

    ss.clear();
    for (int index = 1; index <= max_group_index; ++index) {
        ss << "第" << index << "组: {";
        std::vector<int64_t> temp;
        for (int64_t person = 0; person < res.size(); ++person) {
            if (res[person] == index) {
                temp.push_back(person + 1);
            }
        }

        for (int i = 0; i < temp.size(); ++i) {
            ss << temp[i] << (i < temp.size() - 1 ? "," : " ");
        }
        ss << "}" <<std::endl;
    }

    display += ss.str();

    ui->output_predict_res->setText(QString::fromStdString(display));

}

}   // namespace yurzi
