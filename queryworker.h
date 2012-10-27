/*
   Copyright (C) 2012 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef QUERYWORKER_H
#define QUERYWORKER_H

#include <QThread>
#include <QSqlQuery>
#include "connections/connectionsmodel.h"

namespace Sql {

class QueryWorker : public QThread
{
    Q_OBJECT

public:
    QueryWorker(QObject *parent = 0);

protected:
    virtual void run();

public Q_SLOTS:
    void execute( const QString& query );
    void changeDatabaseConnection(ConnectionsModel::Connection c);

Q_SIGNALS:
    void results(QSqlQuery query, int elapsedTime);
    void error(QString errorMessage);
private:
    QSqlDatabase m_db;
};

}

#endif // QUERYWORKER_H
