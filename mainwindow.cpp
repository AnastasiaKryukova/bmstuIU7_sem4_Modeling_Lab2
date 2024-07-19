#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <cmath>

#include <QStandardItemModel>
#include "QStandardItem"
#include <QTableView>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    n = 0;
    m = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->tableView->setModel(NULL);
    ui->lineEdit_3->clear();

    matrix_txt.clear();
    for (int i = 0; i < ui->gridLayout->count(); i++)
    {
       ui->gridLayout->itemAt(i)->widget()->deleteLater();
    }

    QString a = ui->lineEdit->text();
    n = a.toInt();
    QString b = ui->lineEdit_2->text();
    m = b.toInt();


    for(int i = 0; i < n; i++)
    {
        matrix_txt.push_back(QVector<QLineEdit*>());
        for(int j = 0; j< m; j++)
        {
            matrix_txt.back().push_back(new QLineEdit());
        }
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j< m; j++)
        {
            ui->gridLayout->addWidget(matrix_txt[i][j],i,j);
        }
    }

//    QTextEdit *txt1 = new QTextEdit();
//    QTextEdit *txt2 = new QTextEdit();
//    QTextEdit *txt3 = new QTextEdit();
//    ui->gridLayout->addWidget(txt1,0,0);
//    ui->gridLayout->addWidget(txt2,0,1);
//    ui->gridLayout->addWidget(txt3,1,0);


}


void MainWindow::on_pushButton_2_clicked()
{

    ui->tableView->setModel(NULL);
    ui->lineEdit_3->clear();

    matrix_lambda.clear();
    QString a_val;
    int data;
    for(int i = 0; i < matrix_txt.size(); i++)
    {
        matrix_lambda.push_back(QVector<int>());
        for(int j = 0; j< matrix_txt.back().size(); j++)
        {
            a_val = matrix_txt[i][j]->text();
            data = a_val.toInt();
            matrix_lambda.back().push_back(data);
        }
    }




//создание матрицы расчетной
    QVector<QVector<double>> matrix;
    for(int i = 0; i < matrix_lambda.size(); i++) //4
    {
        matrix.push_back(QVector<double>());
        for(int j = 0; j< matrix_lambda.back().size()+1; j++) //5
        {
            matrix.back().push_back(0.0);
        }
    }
// заполнение по строкам
    int sum = 0;
    for(int i = 0; i < matrix_lambda.size(); i++)
    {
        sum = 0;
        for(int j = 0;j<matrix_lambda.back().size(); j++)
        {
            if(i != j) sum+=matrix_lambda[i][j];
        }
        sum = sum*(-1);
        matrix[i][i] = sum;
    }
// заполнение по столбцам
    for(int i = 0; i < matrix_lambda.size(); i++)
    {
        for(int j = 0;j<matrix_lambda.back().size(); j++)
        {
            if(i != j) matrix[i][j] = matrix_lambda[j][i];
        }
    }

// замена последнего уравнения на p0+p1+p2+p3 = 1
    for(int j = 0; j<matrix.back().size(); j++)
    {
        matrix[matrix.size()-1][j] = 1.0;
    }




//    QVector<double> xx(matrix.size(),0.0);
// расчет методом Гаусса
    int SIZE = 50;
    double a[SIZE][SIZE], x[SIZE], ratio;
    int i,j,k,n;
    n = matrix.size();
    /* 2. Reading Augmented Matrix */
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n+1;j++)
        {
            a[i][j] = matrix[i-1][j-1];
        }
    }
    /* Applying Gauss Elimination */
    ui->lineEdit_3->clear();
    for(i=1;i<=n-1;i++)
    {
      if(a[i][i] == 0.0)
      {
//           qDebug()<<"Mathematical Error!";
//           exit(0);
           ui->lineEdit_3->setText("Введите данные верно!");
           return;
      }
      for(j=i+1;j<=n;j++)
      {
           ratio = a[j][i]/a[i][i];

           for(k=1;k<=n+1;k++)
           {
                a[j][k] = a[j][k] - ratio*a[i][k];
           }
      }
    }
    /* Obtaining Solution by Back Substitution Method */
    x[n] = a[n][n+1]/a[n][n];

    for(i=n-1;i>=1;i--)
    {
        x[i] = a[i][n+1];
        for(j=i+1;j<=n;j++)
        {
            x[i] = x[i] - a[i][j]*x[j];
        }
        x[i] = x[i]/a[i][i];
    }
    /* Displaying Solution */
//    for(i=1;i<=n;i++)
//    {
//        qDebug()<<"p["<< i<<"] = "<< x[i];
//    }

    QVector<double> p_table;
    for(i=1;i<=n;i++)
    {
        p_table.push_back(x[i]);
//        qDebug() << x[i];
    }
//    for(int i = 0; i<p_table.size(); i++)
//        qDebug()<<"p["<< i<<"] = "<< p_table[i];
// расчет методом Гаусса ---конец

// считаем время

//создание матрицы расчетной
QVector<QVector<double>> matrix_t;
for(int i = 0; i < matrix_lambda.size(); i++)
{
    matrix_t.push_back(QVector<double>());
    for(int j = 0; j< matrix_lambda.back().size(); j++)
    {
        matrix_t.back().push_back(0.0);
    }
}


double delta_t = 0.001;
double delta_tt = 0.001; //считаем время
QVector<double> result_t; //результирующие время
for(int i = 0; i< matrix_t.size(); i++)
{
    result_t.push_back(0.0);
}
double eps = 0.01;
int ex = 0;
QVector <double> Pt(matrix_t.size(), 0.0); //при каждой итерации пересчитывается вероятность
Pt[0]=1.0; //Начальные условия вероятностей
QVector <bool> check(matrix_t.size(), 0); // проверка, на уже заполнение i индекса матрицы result_t


while(true)
{

// заполнение по строкам
    double sum_t = 0.0;
    for(int i = 0; i < matrix_lambda.size(); i++)
    {
        sum_t = 0.0;
        for(int j = 0;j<matrix_lambda.back().size(); j++)
        {
            if(i != j) sum_t+=matrix_lambda[i][j];
        }
        matrix_t[i][i] = Pt[i]*(1-sum_t*delta_t);
    }
//// заполнение по столбцам
    for(int i = 0; i < matrix_lambda.size(); i++)
    {
        for(int j = 0;j<matrix_lambda.back().size(); j++)
        {
            if(i != j) matrix_t[i][j] = Pt[j]*matrix_lambda[j][i]*delta_t;
//           qDebug() << matrix_t[i][j];
        }
    }

    double sum_tt = 0.0;
    for(int i = 0; i<matrix_t.size(); i++)
    {
        sum_tt = 0.0;
        for(int j = 0; j< matrix_t.back().size(); j++)
        {
            sum_tt += matrix_t[i][j];
        }
//        bool x = std::fabs(p_table[i]-sum_tt)<eps;
//        qDebug() << p_table[i] << " " << sum_tt << " " << x << " "<< std::fabs(p_table[i]-sum_tt)  << delta_tt;
        Pt[i] = sum_tt;
//        if(sum_tt <0.001 && check[i] == 0)
//        {
//            result_t[i]=delta_tt;
//            ex+=1;
//            check[i] = 1;
//            for(int i = 0; i < matrix_t.size(); i++)
//            {
//                for(int j = 0;j<matrix_t.back().size(); j++)
//                {

//                    qDebug() << matrix_t[i][j];
//                }
//            }
//        }
        if(std::fabs(p_table[i]-sum_tt) < eps && check[i] == 0)
        {
            result_t[i]=delta_tt;
//            qDebug() << p_table[i] << " " << sum_tt << " " << delta_t;
//            qDebug() << result_t[i];
            ex+=1;
            check[i] = 1;
        }
//        qDebug()<<sum_tt;
    }

    delta_tt+= 0.001;
//    delta_t+= 0.001;
    if(ex == result_t.size()) break;
}

//for(int i = 0; i< result_t.size(); i++)
//{
//    qDebug() << result_t[i];
//}




// считаем время --- конец

// рисуем таблицу
    ui->tableView->setModel(NULL);
    QStandardItemModel* model = new QStandardItemModel();
    model->setRowCount(p_table.size());
    model->setColumnCount(3);

    // установка заголовков таблицы

        model->setHeaderData(0, Qt::Horizontal, "Вероятности)");
        model->setHeaderData(1, Qt::Horizontal, "Значения");
        model->setHeaderData(2, Qt::Horizontal, "Время");

    for(int i = 0; i < p_table.size(); i++)
    {
        QString p = QString("P%1").arg(i+1);
        QString valueAsString = QString::number(p_table[i]);
        QString valueAsString_time = QString::number(result_t[i]);
        model->setItem(i, 0, new QStandardItem(p));
        model->setItem(i, 1, new QStandardItem(valueAsString));
        model->setItem(i, 2, new QStandardItem(valueAsString_time));
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //ширина
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //запрет редактирования

    ui->tableView->setModel(model);
// рисуем таблицу --- конец



}

