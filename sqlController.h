#ifndef SQLCONTROLLER_H
#define SQLCONTROLLER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
 * 数据库连接qt_sql_SKU_Stack_connection
 * 主表格main
 * 各层方案表格SKUplus+"_solutions"
 * 层间交替关系 1 重复 2 交替 3 自定义
 * */
struct mainValue
{
    double goods_length;
    double goods_width;
    double goods_height;
    int solutions;
    int layers;
    int layer_relation;
};

class sqlController
{
private:
    QSqlDatabase database;
    bool isopen=false;
public:
    void inidatabase();
    void insert_main(QString SKU,double stack_length ,
                     double stack_width,double stack_height,double goods_length ,double goods_width ,
                     double goods_height ,int solutions ,int layers ,int layer_relation);
    void change_main(QString SKU,double stack_length , double stack_width,double stack_height,QString value_name,double value);
    mainValue get_main(QString SKU,double stack_length , double stack_width,double stack_height);
    void delete_main(QString SKU,double stack_length , double stack_width,double stack_height);
    void clear_main();

    void create_solutions(QString SKU,double stack_length , double stack_width,double stack_height);
    void ini_solutions(QString SKU,double stack_length , double stack_width,double stack_height);
};


#endif // SQLCONTROLLER_H
