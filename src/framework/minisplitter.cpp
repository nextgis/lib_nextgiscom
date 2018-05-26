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

#include "minisplitter.h"

#if QT_VERSION >= 0x050000

#include <QResizeEvent>

NGMiniSplitter::NGMiniSplitter(QWidget *parent) : QSplitter (parent)
{
    init();
}

NGMiniSplitter::NGMiniSplitter(Qt::Orientation orientation) : QSplitter (orientation)
{
    init();
}

void NGMiniSplitter::init()
{
    setHandleWidth(1);
    setChildrenCollapsible(false);
    setProperty("minisplitter", true);
}
 #endif // QT_VERSION >= 0x050000
