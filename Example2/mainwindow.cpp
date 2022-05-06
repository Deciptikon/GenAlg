#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CustomType u;

    u.setS(0.0004);
    u.setN(1500);

    qDebug() << "V = " << u.v();
    qDebug() << "W = " << u.W();
    qDebug() << "Cost = " << u.cost();


    threadGA = new QThread();

    // пример бинарной операции скрещивания, с параметром "баланс особей"
    // баланс всегда имеет величину от 0.0 до 1.0
    auto cross = [](CustomType &a, CustomType &b, float v) -> CustomType {
        CustomType c;
        double newS = a.getS()*v + b.getS()*(1.0 - v);
        double newN = a.getN()*v + b.getN()*(1.0 - v);
        c.setS(newS);
        c.setN(newN);
        //qDebug() << "s:" << a.getS() << "\tn:" << a.getN() << "\tv:" << a.v() << "\tW:" << a.W();
        return c;
    };

    // пример унарной операции мутирования, с параметром "амплитуда мутации"
    // амплитуда всегда не отрицательна
    auto mut = [](CustomType &a, float m) -> CustomType {

        double muts = a.muts*(double)m;
        double mutn = a.mutn*(double)m;

        double sm = a.getS() + QRandomGenerator64::global()->bounded(2.0*muts) - muts;
        double nm = a.getN() + QRandomGenerator64::global()->bounded(2.0*mutn) - mutn;

        CustomType p;
        p.setS(sm);
        p.setN(nm);

        return p;
    };

    // пример унарной операции "работы" особи
    // (решение уравнений, рисование, генерация текста, ...)
    auto fit = [](CustomType a) -> CustomType {

        double vv = a.v();
        double f = a.cost();

        if(vv > a.vmax) {
            f += pow(vv - a.vmax, 2);
        }
        if(vv < a.vmin) {
            f += pow(a.vmin - vv, 2);
        }

        a.setAdapt(f);
        qDebug() << "s:" << a.getS() << "\tn:" << a.getN() << "\tv:" << a.v() << "\tW:" << a.W() << "\tAdapt:" << a.getAdapt();

        return a;
    };

    // пример получения приспособленности особи
    auto adapt = [](CustomType &a) -> float {
        float f = a.getAdapt();
        return f;
    };

    CustomType q;
    q.setS();
    q.setN();

    ga = new GenAlg< CustomType >(q, cross, mut, fit, adapt);

    // устанавливаем размер популяции
    ga->setSizePopulation(3);

    // устанавливаем баланс скрещивания
    ga->setBalanceCrossover();

    // устанавливаем вероятность мутации
    ga->setProbalityMutation(1);

    // устанавливаем параметр величины мутаций
    ga->setMaxValueMutation(1);

    //устанавливаем параметр мутаций начальной популяции
    ga->setGenerateMutation(100);

    ga->moveToThread(threadGA);

    // connect thread with genetic algorithm
    QObject::connect( threadGA, SIGNAL(started()), ga, SLOT(firstRunning()) );
    QObject::connect( ga, SIGNAL(finished()), threadGA, SLOT(quit()) );

    // start thread
    threadGA->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

