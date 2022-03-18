#pragma once

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QVariantHash>
#include <QNotation>

namespace QAprPrivate
{

//!
//! \brief The NotationsExtended class
//!
class NotationsExtended
{
public:
    enum Classification{
        DataBase,
        Audit,
        Security
    };
    //!
    //! \brief NotationsExtended
    //! \param parent
    //!
    explicit NotationsExtended(QObject*parent=nullptr);
    virtual ~NotationsExtended();
public:

    //!
    //! \brief dbIgnore
    //!
    Q_NOTATION_DECLARE(dbNoConnection, DataBase)

    //!
    //! \brief dbNoTransaction
    //!
    Q_NOTATION_DECLARE(dbNoTransaction, DataBase)

    //!
    //! \brief dbReadOnly
    //!
    Q_NOTATION_DECLARE(dbReadOnly, DataBase)

    //!
    //! \brief healtCheck
    //!
    Q_NOTATION_DECLARE(healtCheck, Audit);

    //!
    //! \brief dbSetting
    //! \param setting
    //! \return
    //!
    Q_NOTATION_DECLARE_FUNC(dbSetting, DataBase)

    //!
    //! \brief aclSetting
    //! \param setting
    //! \return
    //!
    Q_NOTATION_DECLARE_FUNC(aclSetting, DataBase)
private:

};

}
