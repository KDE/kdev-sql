/*
   Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

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

#include "sqllanguagesupport.h"

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableView>

#include <KPluginFactory>
#include <KPluginLoader>
#include <KAboutData>
#include <KActionCollection>
#include <KAction>
#include <KTextEditor/Document>

#include <interfaces/iuicontroller.h>
#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>

#include "resulttablewidget.h"
#include "version.h"

K_PLUGIN_FACTORY(KDevSqlSupportFactory, registerPlugin<Sql::LanguageSupport>();)
K_EXPORT_PLUGIN(KDevSqlSupportFactory(KAboutData("kdevsqlsupport","kdevsql", ki18n("SQL Support"), KDEVSQL_VERSION_STR, ki18n("Support for SQL Language"), KAboutData::License_GPL)
    .addAuthor(ki18n("Niko Sams"), ki18n("Author"), "niko.sams@gmail.com", "http://nikosams.blogspot.com")
))

namespace Sql
{
LanguageSupport* LanguageSupport::m_self = 0;

#if KDE_VERSION > KDE_MAKE_VERSION(4, 3, 80)
int debugArea() { static int s_area = KDebug::registerArea("kdevsqlsupport"); return s_area; }
#else
int debugArea() { return 1; }
#endif



template<class T>
class ToolFactory : public KDevelop::IToolViewFactory
{
public:
  ToolFactory(const QString &id, Qt::DockWidgetArea defaultArea)
  : m_id(id), m_defaultArea(defaultArea)
  {}

  virtual QWidget* create(QWidget *parent = 0)
  {
    return new T(parent);
  }

  virtual QString id() const
  {
    return m_id;
  }

  virtual Qt::DockWidgetArea defaultPosition()
  {
    return m_defaultArea;
  }

private:
  QString m_id;
  Qt::DockWidgetArea m_defaultArea;
};


LanguageSupport::LanguageSupport(QObject* parent, const QVariantList& /*args*/)
    : KDevelop::IPlugin(KDevSqlSupportFactory::componentData(), parent),
      KDevelop::ILanguageSupport()
{
    KDEV_USE_EXTENSION_INTERFACE(KDevelop::ILanguageSupport)

    setXMLFile("kdevsqlui.rc");

    m_self = this;

    m_resultTableFactory = new ToolFactory<ResultTableWidget>(
        "org.kdevelop.sql.ResultTable", Qt::BottomDockWidgetArea);

    KActionCollection* ac = actionCollection();

    KAction* action = new KAction(KIcon("system-run"), i18n("Run SQL"), this);
    action->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(action, SIGNAL(triggered(bool)), SLOT(runSql()));
    ac->addAction("run_sql", action);
}

QString LanguageSupport::name() const
{
    return "Sql";
}

LanguageSupport *LanguageSupport::self()
{
    return m_self;
}

void LanguageSupport::runSql()
{
    QWidget* w = core()->uiController()->findToolView(i18n("SQL Query"), m_resultTableFactory, KDevelop::IUiController::CreateAndRaise);
    Q_ASSERT(w);
    ResultTableWidget* resTable = dynamic_cast<ResultTableWidget*>(w);
    Q_ASSERT(resTable);
    KTextEditor::Range selection = core()->documentController()->activeDocument()->textSelection();
    QString text;
    if (selection.isEmpty()) {
        text = core()->documentController()->activeDocument()->textDocument()->text();
    } else {
        text = core()->documentController()->activeDocument()->textDocument()->text(selection);
    }
    resTable->runSql(text);
}

}

#include "sqllanguagesupport.moc"
