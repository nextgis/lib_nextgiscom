/******************************************************************************
*  Project: NextGIS GIS libraries
*  Purpose: Framework library
*  Author:  Dmitry Baryshnikov, bishop.dev@gmail.com
*******************************************************************************
*  Copyright (C) 2012-2018 NextGIS, info@nextgis.ru
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 2 of the License, or
*   (at your option) any later version.
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#ifndef NGFRAMEWORK_MAINWINDOW_H
#define NGFRAMEWORK_MAINWINDOW_H

#include "framework/framework.h"

#include <QAction>
#include <QJsonArray>
#include <QHash>
#include <QMainWindow>

/**
 * @brief The NGMainWindow class
 */
class NGFRAMEWORK_EXPORT NGMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit NGMainWindow(QWidget *parent = nullptr );
    virtual ~NGMainWindow() = default;
    virtual void init();
    QAction* commandByKey(const QString &key) const;

//signals:

protected slots:
    virtual void open();
    virtual void about();
    virtual void quit();
    virtual void preferences();

protected:
    void closeEvent(QCloseEvent *event);
    virtual bool maybeSave();
    virtual void writeSettings();
    virtual void readSettings();
    virtual void createCommands();
    virtual void loadInterface();
    virtual void loadMenus(const QJsonArray &array);
    virtual void loadMenuActions(QMenu *menu, const QJsonArray &array);
    virtual void loadToolbars(const QJsonArray &array);

protected:
    QHash<QString, QAction*> m_commands;
};

#endif // NGFRAMEWORK_MAINWINDOW_H

// #if (QT_VERSION <= QT_VERSION_CHECK(5, 7, 0))
