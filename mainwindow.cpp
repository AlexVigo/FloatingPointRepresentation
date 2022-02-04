#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <bitset>
#include <iostream>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* main = new QWidget(this);
    main->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    main->setMinimumWidth(minWindowWidth());
    this->setCentralWidget(main);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->centralWidget()->setLayout(mainLayout);

    QHBoxLayout* floatTextLayout = new QHBoxLayout(this);
    mainLayout->addLayout(floatTextLayout);

    auto lFloat = new QLabel("Float value:", this);
    floatTextLayout->addWidget(lFloat);
    dspFloat = new QDoubleSpinBox(this);
    dspFloat->setDecimals(sizeof(float_type)*2-1);
    dspFloat->setMinimum(std::numeric_limits<float_type>::min());
    dspFloat->setMaximum(std::numeric_limits<float_type>::max());

    floatTextLayout->addWidget(dspFloat);

    QHBoxLayout* cbLayout = new QHBoxLayout(this);
    cbLayout->setSpacing(2);
    cbLayout->addStretch();
    mainLayout->addLayout(cbLayout);

    size_t start_id = 0;

    QString labelsCaptions = "Sign";
    QWidget* wSign = addCheckBoxes(start_id, 1, labelsCaptions, Qt::green);
    cbLayout->addWidget(wSign);

    labelsCaptions = "Exponent";
    QWidget* wExp = addCheckBoxes(start_id, start_id + baseBitsAmount, labelsCaptions, Qt::blue);
    cbLayout->addWidget(wExp);

    labelsCaptions = "Fraction";
    QWidget* wMantissa = addCheckBoxes(start_id, cbMax , labelsCaptions, Qt::yellow);
    cbLayout->addWidget(wMantissa);

    mainLayout->addStretch();
    QHBoxLayout* exactFloatTextLayout = new QHBoxLayout(this);
    mainLayout->addLayout(exactFloatTextLayout);

    auto lexactFloatText = new QLabel("Exact float value:", this);
    exactFloatTextLayout->addWidget(lexactFloatText);
    lexactFloatValue = new QLabel("0", this);
    exactFloatTextLayout->addWidget(lexactFloatValue);

    QHBoxLayout* exactBinTextLayout = new QHBoxLayout(this);
    mainLayout->addLayout(exactBinTextLayout);

    auto lexactBinText = new QLabel("Exact binary value:", this);
    exactBinTextLayout->addWidget(lexactBinText);
    lexactBinValue = new QLabel("", this);
    exactBinTextLayout->addWidget(lexactBinValue);

    bin.i = 0;
    std::bitset<sizeof(bin.i)* CHAR_BIT> bits(bin.i);
    lexactFloatValue->setText(QString::number(bin.f));
    lexactBinValue->setText(QString::fromStdString(bits.to_string()));

    QObject::connect(dspFloat, &QDoubleSpinBox::textChanged, [&](){
        bin.f = dspFloat->value();
        std::bitset<sizeof(bin.i)* CHAR_BIT> bits(bin.i);
        for (size_t i = 0; i < bits.size(); i++)
        {
            cbVector[i]->setChecked(bits[i]);

        }
        std::cout << "Bits "<< bits << std::endl;
        lexactFloatValue->setText(QString::number(bin.f));
        lexactBinValue->setText(QString::fromStdString(bits.to_string()));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillCheckBoxes()
{
    std::bitset<sizeof(bin.i)* CHAR_BIT> bits(bin.i);
    for (size_t i = 0; i < bits.size(); ++i)
    {
        bits[i] = cbVector[i]->isChecked();
    }
    bin.i = bits.to_ullong();
    std::cout << "Bits "<< bits << std::endl;

    lexactFloatValue->setText(QString::number(bin.f));
    lexactBinValue->setText(QString::fromStdString(bits.to_string()));
}

QWidget* MainWindow::addCheckBoxes(size_t &start, const size_t until, QString labelCaption, QColor backgroundColor)
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Window , backgroundColor);

    QWidget* cbGroupWidget = new QWidget(this);
    cbGroupWidget->setAutoFillBackground(true);
    cbGroupWidget->setPalette(Pal);
    QVBoxLayout* cbGroupLayoutV = new QVBoxLayout(this);
    cbGroupWidget->setLayout(cbGroupLayoutV);

    auto lcbGroup = new QLabel(labelCaption, cbGroupWidget);
    lcbGroup->setAlignment(Qt::AlignCenter| Qt::AlignVCenter);
    cbGroupLayoutV->addWidget(lcbGroup);

    QHBoxLayout* cbGroupLayout = new QHBoxLayout(this);
    cbGroupLayout->setAlignment(Qt::AlignHCenter);
    cbGroupLayoutV->addLayout(cbGroupLayout);
    for (; start < until; ++start)
    {
        auto cbBit = new QCheckBox("", cbGroupWidget);
        cbVector.insert(cbVector.begin(), cbBit);
        QObject::connect(cbBit,  &QCheckBox::stateChanged, this, &MainWindow::fillCheckBoxes);
        cbGroupLayout->addWidget(cbBit);
    }
    return cbGroupWidget;
}

