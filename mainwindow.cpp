#include "mainwindow.h"
#include "./ui_mainwindow.h"
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

    createDoubleSpinBox(mainLayout);
    createAllCheckboxes(mainLayout);
    mainLayout->addStretch();

    createExactValueLabels(mainLayout);

    bin.i = 0;
    bits = bin.i;
    lexactFloatValue->setText(QString::number(bin.f));
    lexactBinValue->setText(QString::fromStdString(bits.to_string()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillCheckBoxes()
{
    bits = bin.i;
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

    QLabel* lcbGroup = new QLabel(labelCaption, cbGroupWidget);
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

void MainWindow::createDoubleSpinBox(QBoxLayout* mainLayout)
{
    QHBoxLayout* floatTextLayout = new QHBoxLayout(this);

    QLabel* lFloat = new QLabel("Float value:", this);
    floatTextLayout->addWidget(lFloat);
    dspFloat = new QDoubleSpinBox(this);
    dspFloat->setDecimals(sizeof(float_type)*2-1);
    dspFloat->setMinimum(std::numeric_limits<float_type>::min());
    dspFloat->setMaximum(std::numeric_limits<float_type>::max());

    floatTextLayout->addWidget(dspFloat);
    mainLayout->addLayout(floatTextLayout);

    QObject::connect(dspFloat, &QDoubleSpinBox::textChanged, [&](){
        bin.f = dspFloat->value();
        bits = bin.i;
        for (size_t i = 0; i < bits.size(); i++)
        {
            cbVector[i]->disconnect();
            cbVector[i]->setChecked(bits[i]);
            QObject::connect(cbVector[i],  &QCheckBox::stateChanged, this, &MainWindow::fillCheckBoxes);
        }
        std::cout << "Bits "<< bits << std::endl;
        lexactFloatValue->setText(QString::number(bin.f));
        lexactBinValue->setText(QString::fromStdString(bits.to_string()));
    });
}

void MainWindow::createAllCheckboxes(QBoxLayout* mainLayout)
{
    size_t start_id = 0;

    QHBoxLayout* cbLayout = new QHBoxLayout(this);
    cbLayout->setSpacing(2);
    cbLayout->addStretch();

    QString labelsCaptions = "Sign";
    QWidget* wSign = addCheckBoxes(start_id, 1, labelsCaptions, Qt::green);
    cbLayout->addWidget(wSign);

    labelsCaptions = "Exponent";
    QWidget* wExp = addCheckBoxes(start_id, start_id + BASE_BITS, labelsCaptions, Qt::blue);
    cbLayout->addWidget(wExp);

    labelsCaptions = "Fraction";
    QWidget* wMantissa = addCheckBoxes(start_id, ALL_FLOAT_BITS , labelsCaptions, Qt::yellow);
    cbLayout->addWidget(wMantissa);
    mainLayout->addLayout(cbLayout);
}

void MainWindow::createExactValueLabels(QBoxLayout* mainLayout)
{
    QHBoxLayout* exactFloatTextLayout = new QHBoxLayout(this);
    QLabel* lexactFloatText = new QLabel("Exact float value:", this);
    exactFloatTextLayout->addWidget(lexactFloatText);

    lexactFloatValue = new QLabel("0", this);
    exactFloatTextLayout->addWidget(lexactFloatValue);
    mainLayout->addLayout(exactFloatTextLayout);

    QHBoxLayout* exactBinTextLayout = new QHBoxLayout(this);
    QLabel* lexactBinText = new QLabel("Exact binary value:", this);
    exactBinTextLayout->addWidget(lexactBinText);

    lexactBinValue = new QLabel("", this);
    exactBinTextLayout->addWidget(lexactBinValue);
    mainLayout->addLayout(exactBinTextLayout);
}

