#include "./p_qapr_log_telegram.h"
#include <QJsonDocument>

namespace QApr {

LogTelegram::LogTelegram(QObject *parent):QThread{nullptr}{
    Q_UNUSED(parent)
}

LogTelegram::~LogTelegram()
{
}

void LogTelegram::run()
{
    this->exec();
}

void LogTelegram::recebeMessage(const QVariant &v)
{
    Q_UNUSED(v)
    //auto msg=QJsonDocument::fromVariant(v).toJson(QJsonDocument::Indented);
}

}
