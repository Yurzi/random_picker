#ifndef __GUI_H__
#define __GUI_H__

#include <QMainWindow>
#include <memory>
#include <stdint.h>

#include "randomizer.h"
#include "picker.h"
#include "analyzar.h"

namespace Ui {
class MainWindow;
}

namespace yurzi {

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// slots
private slots:
    void slot_enable_picker();
    void slot_on_picker_clicked();
    void slot_enable_read_file();
    void slot_on_read_data_clicked();
    void slot_on_predict_clicked();

private:
    // 分组用关键组件
    std::shared_ptr<yurzi::Randomizer<int64_t>> randomizer;
    std::shared_ptr<yurzi::Picker<int64_t>> picker;

    std::shared_ptr<yurzi::Analyzar> analyzar;
private:
    std::shared_ptr<Ui::MainWindow> ui;
};

}   // namespace yurzi

#endif  //__GUI_H__
