#include "./p_qapr_log_elk.h"
#include <QJsonDocument>

namespace QApr{

LogELK::LogELK(QObject *parent):QThread{nullptr}
{
    Q_UNUSED(parent)
}

LogELK::~LogELK()
{
}

void LogELK::run()
{
    this->exec();
}

void LogELK::recebeMessage(const QVariant &v)
{
    Q_UNUSED(v)
    //auto msg=QJsonDocument::fromVariant(v).toJson(QJsonDocument::Indented);

}


}

