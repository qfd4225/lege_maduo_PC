#include "sqlController.h"

void sqlController::inidatabase()
{
    QString baseName="qt_sql_SKU_Stack_connection";
    if (QSqlDatabase::contains(baseName))
    {
        database = QSqlDatabase::database(baseName);
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE",baseName);
        database.setDatabaseName("MyDataBase.db");
        database.setUserName("Loctek");
        database.setPassword("123456");
    }
    if(!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        isopen=false;
    }
    else
    {
        isopen=true;
        QSqlQuery sql_query;
        QString create_sql = "create table main (SKUplus varchar(60) primary key,goods_length double,goods_width double,"
                             "goods_height double,solutions int,layers int,layer_relation int)";
        /*
         * 分别为“SKU码+托盘长+托盘宽+托盘高”（作为主键）,货物长宽高,每层摆放的方案数,层数,交替关系
         */
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }
    }
    return;
}

void sqlController::insert_main(QString SKU,double stack_length ,
                                double stack_width,double stack_height,double goods_length ,double goods_width ,
                                double goods_height ,int solutions ,int layers ,int layer_relation)
{
    if(isopen)
    {
        QSqlQuery sql_query;
        QString insert_sql = "insert into main values(?,?,?,?,?,?,?)";
        QString SKUplus=SKU+"+"+QString::number(stack_length, 10, 2)+"+"+QString::number(stack_width, 10, 2)+"+"+QString::number(stack_height, 10, 2);
        sql_query.prepare(insert_sql);
        sql_query.addBindValue(SKUplus);
        sql_query.addBindValue(goods_length);
        sql_query.addBindValue(goods_width);
        sql_query.addBindValue(goods_height);
        sql_query.addBindValue(solutions);
        sql_query.addBindValue(layers);
        sql_query.addBindValue(layer_relation);
        if(!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
        }
        else
        {
            qDebug() << "inserted SKU:"<<SKU;
        }
    }
    else
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
}

void sqlController::change_main(QString SKU,double stack_length ,double stack_width,double stack_height,QString value_name,double value)
{
    if(isopen)
    {
        QSqlQuery sql_query;
        QString update_sql = "update main set :value_name = :name where SKUplus = :SKUplus";
        sql_query.prepare(update_sql);
        QString SKUplus=SKU+"+"+QString::number(stack_length, 10, 2)+"+"+QString::number(stack_width, 10, 2)+"+"+QString::number(stack_height, 10, 2);
        sql_query.bindValue(":value_name", value_name);
        if(value_name=="layers" || value_name=="solutions" || value_name=="layer_relation")
        {
            sql_query.bindValue(":name", qRound(value));
        }
        else
        {
            sql_query.bindValue(":name", value);
        }
        sql_query.bindValue(":SKUplus", SKUplus);
        if(!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
        }
        else
        {
            qDebug() << "updated "<<value_name;
        }
    }
    else
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
}

mainValue sqlController::get_main(QString SKU,double stack_length , double stack_width,double stack_height)
{
    mainValue re{-1,0,0,0,0,0};
    if(isopen)
    {
        QSqlQuery sql_query;
        QString SKUplus=SKU+"+"+QString::number(stack_length, 10, 2)+"+"+QString::number(stack_width, 10, 2)+"+"+QString::number(stack_height, 10, 2);
        QString get_sql = "select * from main where SKUplus = :SKUplus";
        sql_query.prepare(get_sql);
        sql_query.bindValue(":SKUplus", SKUplus);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            re.goods_length=sql_query.value(0).toDouble();
            re.goods_width=sql_query.value(1).toDouble();
            re.goods_height=sql_query.value(2).toDouble();
            re.solutions=sql_query.value(3).toInt();
            re.layers=sql_query.value(4).toInt();
            re.layer_relation=sql_query.value(5).toInt();
        }
    }
    else
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    return re;
}

void sqlController::delete_main(QString SKU,double stack_length , double stack_width,double stack_height)
{
    if(isopen)
    {
        QSqlQuery sql_query;
        QString SKUplus=SKU+"+"+QString::number(stack_length, 10, 2)+"+"+QString::number(stack_width, 10, 2)+"+"+QString::number(stack_height, 10, 2);
        QString delete_sql = "delete from main where SKUplus = ?";
        sql_query.prepare(delete_sql);
        sql_query.addBindValue(SKUplus);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"deleted!";
        }
    }
    else
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    return;
}

void sqlController::clear_main()
{
    if(isopen)
    {
        QSqlQuery sql_query;
        QString clear_sql = "delete from main";
        sql_query.prepare(clear_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"main clear!";
        }
    }
    else
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    return;
}

void sqlController::create_solutions(QString SKU,double stack_length , double stack_width,double stack_height)
{
    QString SKUplus=SKU+"+"+QString::number(stack_length, 10, 2)+"+"+QString::number(stack_width, 10, 2)+"+"+QString::number(stack_height, 10, 2);
    QSqlQuery sql_query;
    QString create_sql = "create table ? (id int primary key, solu int, height_compen double)";
    sql_query.prepare(create_sql);
    sql_query.addBindValue(SKUplus+"_solutions");
    if(!sql_query.exec())
    {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
}
