#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QList>
#include <QRandomGenerator64>
#include <QDebug>

#include "../GenAlg/GenAlg.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QThread *threadGA;
    GenAlg< int > *ga;

    threadGA = new QThread();

    // пример бинарной операции скрещивания, с параметром "баланс особей"
    // баланс всегда имеет величину от 0.0 до 1.0
    auto cross = [](int &a, int &b, float v) -> int {
        return qRound( a*v + b*(1.0 - v) );
    };

    // пример унарной операции мутирования, с параметром "амплитуда мутации"
    // амплитуда всегда не отрицательна
    auto mut = [](int &a, float m) -> int {
        int s = QRandomGenerator64::global()->bounded(2*m) - m;
        return a + s;
    };

    // пример унарной операции "работы" особи
    // (решение уравнений, рисование, генерация текста, ...)
    // в данном случае вычисления не требуются и функция возвращает свой аргумент
    auto fit = [](int a) -> int {
        return a;
    };

    // пример вычисления функции приспособленности особи
    auto adapt = [](int &a) -> float {
        return powf(a - 20, 2);// например оптимум на 20
    };

    ga = new GenAlg< int >(253, cross, mut, fit, adapt);

    // устанавливаем размер популяции
    ga->setSizePopulation(5);

    // устанавливаем баланс скрещивания
    ga->setBalanceCrossover();

    // устанавливаем вероятность мутации
    ga->setProbalityMutation(1);

    // устанавливаем максимальную величину мутаций
    ga->setMaxValueMutation(5);

    //устанавливаем величину мутаций начальной популяции
    ga->setGenerateMutation(20);

    ga->moveToThread(threadGA);

    // connect thread with genetic algorithm
    QObject::connect( threadGA, SIGNAL(started()), ga, SLOT(firstRunning()) );
    QObject::connect( ga, SIGNAL(finished()), threadGA, SLOT(quit()) );

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // start thread
    threadGA->start();

    return app.exec();
}
