#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QDoubleSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using float_type = float;
using decimal_type = std::conditional<sizeof(float_type) == sizeof(float), unsigned int, unsigned long long>::type;

union floatint {
    float_type f;
    decimal_type i;
};

consteval size_t floatBitsAmount() {return sizeof(float_type) * 8;};
consteval size_t floatBitsBaseAmount() {return std::is_same<float_type, float>::value ? 8 : 11;};
consteval size_t minWindowWidth() {return std::is_same<float_type, float>::value ? 700 : 1300;};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    /*volatile*/ floatint bin;
    QDoubleSpinBox* dspFloat;
    QLabel* lexactFloatValue, *lexactBinValue;
    static constexpr size_t cbMax = floatBitsAmount();
    static constexpr size_t baseBitsAmount = floatBitsBaseAmount();
    std::vector<QCheckBox*> cbVector;
    QWidget *addCheckBoxes(size_t &start, const size_t until, QString labelCaption, QColor backgroundColor);
public slots:
    void fillCheckBoxes();
};
#endif // MAINWINDOW_H
