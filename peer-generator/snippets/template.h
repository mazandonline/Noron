#ifndef %3_H
#define %3_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#if __cplusplus >= 201103L
#   include <functional>
#endif

#ifdef QT_QML_LIB
#   include <QJSValue>
#endif

#include <NoronPeer>
%4

QT_BEGIN_NAMESPACE

class %1 : public NoronPeer
{
    Q_OBJECT
%5

public:
    Q_INVOKABLE %1(QObject *parent = 0);
%2
};

QT_END_NAMESPACE

#endif // %3_H