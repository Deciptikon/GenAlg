#ifndef GENALG_H
#define GENALG_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QRandomGenerator64>

class IGenAlg : public QObject
{
    Q_OBJECT

signals:
    void toRun();

    void finished();

    void currentEpoha(const int &epoha);

    void currentAdaptation(const float &adapt);

public slots:
    virtual void run() = 0;

    virtual void running() = 0;

    virtual void firstRunning() = 0;

    virtual void stopping() = 0;

    virtual void generatePopulation() = 0;

private:
    virtual void crossingover() = 0;

    virtual void mutation() = 0;

    virtual void fitness() = 0;

    virtual void selection() = 0;
};


template<typename T>
class GenAlg : public IGenAlg
{
public:
    GenAlg(// Начальная особь
           T basicIndivid,

           // Скрещивание двух особей
           T (*crossIndivid)(T& indA, T& indB, float balanceCrossover),

           // Мутация особи
           T (*mutIndivid)(T& indA, float maxValueMutation),

           // заставляем особь поработать (если есть необходимость)
           T (*fitIndivid)(T indA),

           // Приспособленность особи
           float (*adaptIndivid)(T& indA)):
    sizePopulation(10),
    balanceCrossover(0.5),
    probalityMutation(0.5),
    maxValueMutation(2),
    generateMutation(20)
    {
        qDebug() << "GenAlg()";

        individ = basicIndivid;
        cross   = crossIndivid;
        mut     = mutIndivid;
        fit     = fitIndivid;
        adapt   = adaptIndivid;

        // зацикливаем
        connect(this, SIGNAL(toRun()),
                this, SLOT(run()) );

        // если финиш, останавливаем
        connect(this, SIGNAL(finished()),
                this, SLOT(stopping()) );
    }


    ~GenAlg(){
        qDebug() << "~GenAlg()";
    }

//public slots:
    void run() override {//
        if(stop) {
            return;
        }
        qDebug() << "Epoha:" << count;
        emit currentEpoha(count);

        crossingover();
        mutation();
        fitness();
        selection();

        count++;

        if(count == 1000) {
            emit finished();
        }

        qDebug() << "";
        this->thread()->msleep(100);

        emit toRun();
    }

    void running() override {
        stop = false;
        emit toRun();
    }

    void firstRunning() override {
        stop = false;
        generatePopulation();
        emit toRun();
    }

    void stopping() override {
        stop = true;
    }

    void crossingover() override {
        //qDebug() << "crossingover()";

        if(population.isEmpty()) {
            return;
        }

        QList<T> crossPop;
        for(auto a: population) {
            // случайный номер особи
            size_t r = QRandomGenerator::global()->bounded(population.size());

            T b = population.at(r);
            T c = cross(a, b, balanceCrossover);

            crossPop.append(c);
        }

        if(crossPop.isEmpty()) {
            return;
        }

        population.append(crossPop);
        //qDebug() << "crossingover()" << "\t" << population;
    }

    void mutation() override {
        //qDebug() << "mutation()";

        if(population.isEmpty()) {
            return;
        }

        QList<T> mutPop;
        for(auto a: population) {
            float r = QRandomGenerator::global()->bounded(1.0);
            T mutA = a;
            if(r < probalityMutation) {
                mutA = mut(a, maxValueMutation);
                mutPop.append(mutA);
            }
            mutPop.append(mutA);
        }

        if(mutPop.isEmpty()) {
            return;
        }

        population.append(mutPop);
        //qDebug() << "mutation()" << "\t" << population;
    }

    void fitness() override {
        //qDebug() << "fitness()";

        if(population.isEmpty()) {
            return;
        }

        QList<T> fitPop;
        for(auto a: population) {
            T fitA = fit(a);
            fitPop.append(fitA);
        }

        if(fitPop.isEmpty()) {
            return;
        }

        population.clear();
        population = fitPop;
        //qDebug() << "fitness()" << "\t" << population;
    }

    void selection() override {
        //qDebug() << "selection()";
        if(population.isEmpty()) {
            return;
        }
        std::sort(population.begin(), population.end(),
                  [&](T a,  T b) -> bool {
            return adapt(a) < adapt(b);
        } );

        while(population.size() > sizePopulation) {
            population.removeLast();
        }

        //qDebug() << "population:" << population;

        float ada = adapt(population.first());
        emit currentAdaptation(ada);

        qDebug() << "Отклонение лучшей особи =" << ada;
    }

    void generatePopulation() override {

        population.clear();
        population.append(individ);

        while(population.size() < sizePopulation) {
            T ind = mut(individ, generateMutation);
            population.append(ind);
        }
    }

    void setSizePopulation(size_t newSizePopulation = 10) {
        if(newSizePopulation<2) {
            return;
        }
        sizePopulation = newSizePopulation;
    }

    void setBalanceCrossover(float newBalanceCrossover = 0.5) {
        if(newBalanceCrossover>1) {
            return;
        }
        if(newBalanceCrossover<0) {
            return;
        }
        balanceCrossover = newBalanceCrossover;
    }

    void setProbalityMutation(float newProbalityMutation = 0.5) {
        if(newProbalityMutation>1) {
            return;
        }
        if(newProbalityMutation<0) {
            return;
        }
        probalityMutation = newProbalityMutation;
    }

    void setMaxValueMutation(float newMaxValueMutation = 2) {
        if(newMaxValueMutation < 0) {
            return;
        }
        maxValueMutation = newMaxValueMutation;
    }

    void setGenerateMutation(float newGenerateMutation = 20) {
        if(newGenerateMutation < 0) {
            return;
        }
        generateMutation = newGenerateMutation;
    }

    void setMaxCount(size_t newMaxCount = 1000) {
        if(newMaxCount < 0) {
            return;
        }
        maxCount = newMaxCount;
    }

private:

    T individ;

    size_t sizePopulation;
    float balanceCrossover;
    float probalityMutation;
    float maxValueMutation;
    float generateMutation;

    bool stop = false;
    size_t maxCount;

    T (*cross)(T& indA, T& indB, float balanceCrossover);
    T (*mut)(T& indA, float maxValueMutation);
    T (*fit)(T indA);
    float (*adapt)(T& indA);

    QList<T> population;

    int count=0;
};

#endif // GENALG_H
