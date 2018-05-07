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
#include "signserver.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QTcpSocket>
#include <QThread>
#include <QUrl>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif // QT_VERSION >= 0x050000

constexpr const char *authUrlStr = "https://my.nextgis.com/oauth2/authorize/";

constexpr const char *contentStr = "<html>"
"<head><meta charset=\"UTF-8\"><title>%1</title></head>"
"<body>"
"  <style>"
"    #message{ position: absolute; top: 0; right: 0; left: 0; bottom: 0; margin: auto; height: 226px; width: 680px; font-family: Roboto, Arial, sans-serif; text-align: center; line-height: 1.35; border-radius: 8px; background: #eef5fd; padding: 20px 40px; box-sizing: border-box; font-size: 16px; border: 1px solid #d3e3f2; }"
"    #message h1{ font-weight: normal; letter-spacing: -.4px; }"
"    #message span{ color: #0070c5; }"
"  </style>"
"  <div id=\"message\">"
"       <h1>%2</h1>"
"       <p>%3</p>"
"  </div>"
"</body>"
"</html>";

constexpr int listenPort = 65020;
constexpr const char *redirectUriStr = "http://127.0.0.1:65020";

NGSignServer::NGSignServer(const QString &clientId, const QString &scope,
                           QWidget *parent) :
    QProgressDialog(parent),
    m_redirectUri(redirectUriStr),
    m_clientId(clientId),
    m_scope(scope)
{
    setLabelText(tr("Please sign in\nvia the opened browser..."));
    setWindowModality(Qt::WindowModal);
    setMaximum(0);

    // TAdd localized content.
    QString mesageBody = tr("You have successfully signed in <br> at <span>%1</span> application")
            .arg(qApp->applicationName());
    m_replyContent = QString(contentStr)
            .arg(tr("Successfully signed"))
            .arg(mesageBody)
            .arg(tr("Now you can close this page"));

    // Start listen server
    m_listenServer = new QTcpServer(this);
    m_listenServer->listen(QHostAddress::LocalHost, listenPort);
    connect(m_listenServer, SIGNAL(newConnection()), this, SLOT(onIncomingConnection()));
}

NGSignServer::~NGSignServer()
{
    // FIXME: we need to process events to finish write to socket
    QCoreApplication::processEvents();
    m_listenServer->close();
}

QString NGSignServer::code() const
{
    return m_code;
}

QString NGSignServer::redirectUri() const
{
    return m_redirectUri;
}

void NGSignServer::onIncomingConnection()
{
    QTcpSocket *socket = m_listenServer->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onGetReply()), Qt::UniqueConnection);
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void NGSignServer::onGetReply()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }
    QByteArray reply;
    reply.append("HTTP/1.0 200 OK \r\n");
    reply.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
    reply.append(QString("Content-Length: %1\r\n\r\n")
                 .arg(m_replyContent.size()).toLatin1());
    reply.append(m_replyContent);
    socket->write(reply);

    QByteArray data = socket->readAll();
    int result = 0;
    QString dataStr(data);
    if(dataStr.startsWith("GET /?code=")) {
        int pos = dataStr.indexOf(" HTTP/1.1");
        m_code = dataStr.mid(11, pos - 11);
        result = m_code.isEmpty() ? 0 : 1;
    }


    socket->disconnectFromHost();

    // Close dialog
    done(result);
}

int NGSignServer::exec()
{
    // Prepare url
    QUrl url(authUrlStr);
    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QString("response_type"), QString("code")));
    parameters.append(qMakePair(QString("client_id"), m_clientId));
    parameters.append(qMakePair(QString("redirect_uri"), m_redirectUri));
    parameters.append(qMakePair(QString("scope"), m_scope));

#if QT_VERSION < 0x050000
    url.setQueryItems(parameters);
#else
    QUrlQuery query(url);
    query.setQueryItems(parameters);
    url.setQuery(query);
#endif

    // Open external browser
    QDesktopServices::openUrl(url);

    return QProgressDialog::exec();
}