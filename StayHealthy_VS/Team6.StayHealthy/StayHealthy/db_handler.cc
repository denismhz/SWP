#include "db_handler.h"
#include <string>
#include "trainingsplan.h"
#include "ernaerungsplan.h"

DBHandler::DBHandler() {
  db_ = QSqlDatabase::addDatabase("QODBC");
  db_.setDatabaseName(
      "DRIVER={SQL "
      "Server};Server=194.95.108.25;Database=StayHealthyDB;Uid=sa;Port=1433;"
      "Pwd=asdasd1!2;WSID=.");
  if (db_.open()) {
    qDebug() << "Database Connected!";
  } else
    qDebug() << "No Database Connected";
}

DBHandler::~DBHandler() {
    db_.close();
}

bool DBHandler::CustomQuery(QString prep){
    qDebug() << prep;
    QSqlQuery query;
    query.prepare(prep);
    query.exec();
    query.finish();
    return 1;
}

std::vector<Uebungsposition*> DBHandler::GetRandomUebungen(int anzahl, QString art)
{
    std::vector<Uebungsposition*> uebungen;
    QString prep;
    if(!art.isEmpty()) prep = "SELECT TOP(:anzahl) * FROM dbo.Uebung WHERE Art = :art ORDER BY NEWID() ";
    else prep = "SELECT TOP(:anzahl) * FROM dbo.Uebung ORDER BY NEWID()";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":anzahl", anzahl);
    query.bindValue(":art", art);
    query.exec();
    while (query.next()) {
        Uebungsposition* new_pos = new Uebungsposition(1, query.value(0).toInt(), query.value(2).toInt(), 0, query.value(3).toString());
        uebungen.push_back(new_pos);
    }
    query.finish();

    return uebungen;
}


