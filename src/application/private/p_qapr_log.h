#pragma once

#include <QLoggingCategory>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

namespace QApr {

//!
//! \brief qtMessageHandlerDefault
//!
static const QtMessageHandler qtMessageHandlerDefault = qInstallMessageHandler(0);

//!
//! \brief qtMsgTypeMap
//!
static QHash<int, QString> qtMsgTypeMap;

}

