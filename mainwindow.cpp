#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSharedPointer>
#include <vector>
#include <QLineEdit>

using namespace std;

class SortingStrategy {
public:
    virtual ~SortingStrategy() {}

    virtual void sort(std::vector<int>& data) const = 0;
};

class BubbleSort : public SortingStrategy {
public:
    BubbleSort() {}
    ~BubbleSort() {}

    void sort(std::vector<int>& data) const override {
        int n = data.size();
        for (int i = 0; i < n-1; ++i) {
            for (int j = 0; j < n-i-1; ++j) {
                if (data[j] > data[j+1]) {
                    std::swap(data[j], data[j+1]);
                }
            }
        }
    }
};

class SelectionSort : public SortingStrategy {
public:
    SelectionSort() {}
    ~SelectionSort() {}

    void sort(std::vector<int>& data) const override {
        int n = data.size();
        for (int i = 0; i < n-1; ++i) {
            int minIndex = i;
            for (int j = i+1; j < n; ++j) {
                if (data[j] < data[minIndex]) {
                    minIndex = j;
                }
            }
            std::swap(data[i], data[minIndex]);
        }
    }
};

class FileHandler {
private:
    QFile file;

public:
    FileHandler(const QString& fileName) : file(fileName) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw std::runtime_error("Failed to open file");
        }
    }

    ~FileHandler() {
        file.close();
    }

    void write(const QString& data) {
        QTextStream out(&file);
        out << data;
    }
};

class SortingApplication {
private:
    QSharedPointer<FileHandler> fileHandler;
    QSharedPointer<SortingStrategy> sortingStrategy;

public:
    SortingApplication(const QString& fileName, SortingStrategy* strategy)
        : sortingStrategy(strategy) {
        fileHandler = QSharedPointer<FileHandler>::create(fileName);
    }

    ~SortingApplication() {}

    void sortData(std::vector<int>& data) {
        sortingStrategy->sort(data);
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString fileName = "data.txt";
    SortingStrategy* strategy = nullptr;

    std::vector<int> array = {8, 2, 7, 11, 9};

    QLineEdit lineEdit;
    qDebug() << "Choose sorting strategy (B for Bubble Sort, S for Selection Sort): ";
    lineEdit.setMaxLength(1);
    lineEdit.show();
    QObject::connect(&lineEdit, &QLineEdit::returnPressed, [&]() {
        char choice = lineEdit.text().at(0).toLatin1();
        switch (choice) {
        case 'B':
            strategy = new BubbleSort();
            break;
        case 'S':
            strategy = new SelectionSort();
            break;
        default:
            qDebug() << "Invalid choice. Exiting...";
            return;
        }
        try {
            SortingApplication app(fileName, strategy);
            app.sortData(array);

            //output of the sorted array
            qDebug() << "Sorted array:";
            for (int num : array) {
                qDebug() << num;
            }
            delete strategy;
        } catch (const std::exception& e) {
            qDebug() << "Exception found: " << e.what();
            if (strategy) {
                delete strategy;
            }
            return;
        }
    });

    return a.exec();
}


