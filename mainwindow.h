#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <bitset>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using float_type = float;
using decimal_type = std::conditional<sizeof(float_type) == sizeof(float), unsigned int, unsigned long long>::type;

union float_decimal {
    float_type f;
    decimal_type i;
};

consteval size_t floatBitsAmount() {return sizeof(decimal_type) * CHAR_BIT;};
consteval size_t floatBaseBitsAmount() {return std::is_same<float_type, float>::value ? 8 : 11;};
consteval size_t minWindowWidth() {return std::is_same<float_type, float>::value ? 700 : 1300;};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    static constexpr size_t ALL_FLOAT_BITS = floatBitsAmount();
    static constexpr size_t BASE_BITS = floatBaseBitsAmount();

    Ui::MainWindow *ui;
    float_decimal bin;
    std::bitset<ALL_FLOAT_BITS> bits;
    QDoubleSpinBox* dspFloat;
    QLabel* lexactFloatValue, *lexactBinValue;
    std::vector<QCheckBox*> cbVector;

    QWidget *addCheckBoxes(size_t &start, const size_t until, QString labelCaption, QColor backgroundColor);
    void createDoubleSpinBox(QBoxLayout *mainLayout);
    void createAllCheckboxes(QBoxLayout* mainLayout);
    void createExactValueLabels(QBoxLayout *mainLayout);
private slots:
    void fillCheckBoxes();
};
#endif // MAINWINDOW_H
