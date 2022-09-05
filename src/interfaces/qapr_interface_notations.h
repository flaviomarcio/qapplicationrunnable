#pragma once

#include "../../../qnotation/includes/QAnnotation"
#include <QObject>
#include <QVariant>
#include <QVariantHash>
#include <QVariantList>

namespace QAprPrivate {

//!
//! \brief The InterfaceNotations class
//!
class InterfaceNotations
{
public:
    enum Classification { DataBase, Audit, Security };
    //!
    //! \brief InterfaceNotations
    //! \param parent
    //!
    explicit InterfaceNotations(QObject *parent = nullptr);
    virtual ~InterfaceNotations();

public:
    //!
    //! \brief dbIgnore
    //!
    Q_ANNOTATION_DECLARE(dbNoConnection, DataBase)

    //!
    //! \brief dbNoTransaction
    //!
    Q_ANNOTATION_DECLARE(dbNoTransaction, DataBase)

    //!
    //! \brief dbReadOnly
    //!
    Q_ANNOTATION_DECLARE(dbReadOnly, DataBase)

    //!
    //! \brief healtCheck
    //!
    Q_ANNOTATION_DECLARE(healtCheck, Audit);

    //!
    //! \brief dbSetting
    //! \param setting
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(dbSetting, DataBase)

    //!
    //! \brief aclSetting
    //! \param setting
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(aclSetting, DataBase)
private:
};

} // namespace QAprPrivate
