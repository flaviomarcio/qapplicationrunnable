#include "./qapr_menu_object.h"
#include "../../../qstm/src/qstm_util_variant.h"

namespace QApr {

class MenuObjectPvt:public QObject
{
public:
    QVariantMap data, dataEmpty;
    QHash<QString, QVariantHash> dataMenu;
    MenuObject *parent = nullptr;

    explicit MenuObjectPvt(MenuObject *parent)
        : QObject{parent},
          data({{QT_STRINGIFY(uuid), QUuid{}},
                {QT_STRINGIFY(text), {}},
                {QT_STRINGIFY(route), {}},
                {QT_STRINGIFY(routeLoad), {}},
                {QT_STRINGIFY(routeLoader), {}},
                {QT_STRINGIFY(menu), {}}})
    {
        this->parent = parent;
    }

    void setUuid()
    {
        Q_DECLARE_VU;
        auto route = this->parent->basePath();
        auto routeLoad = this->parent->routeLoad();
        auto codeBase = route + QStringLiteral("/") + routeLoad;
        this->data[QT_STRINGIFY(uuid)] = vu.toMd5Uuid(codeBase);
    }

    void permissionAdd(const QVariant &v)
    {
        if (v.isValid()) {
            auto vMenu = this->data[QT_STRINGIFY(permission)].toList();
            vMenu.append(v);
            this->data[QT_STRINGIFY(permission)] = vMenu;
        }
    }

    void menuAdd(const QVariantMap &v)
    {
        if (!v.isEmpty()) {
            MenuObject menu(v);

            if (menu.text().isEmpty())
                menu.text(menu.routeLoad());

            auto routeBase = this->parent->basePath();
            auto route = menu.basePath();
            if (!route.startsWith(routeBase)) {
                if (route.isEmpty())
                    route = routeBase;
                else
                    route = (routeBase + QT_STRINGIFY(/) + route);
            }
            auto vHash = menu.basePath(route).build();
            if (!vHash.isEmpty()) {
                auto &vMenu = this->dataMenu[QT_STRINGIFY(menu)];
                if (!vMenu.contains(menu.text())) {
                    vMenu.insert(menu.text(), vHash);
                    this->dataMenu[QT_STRINGIFY(menu)] = vMenu;
                    {
                        auto vMenuList = this->data[QT_STRINGIFY(menu)].toList();
                        vMenuList.append(vHash);
                        this->data[QT_STRINGIFY(menu)] = vMenuList;
                    }
                }
            }
        }
    }

    auto &build()
    {
        this->setUuid();
        return this->data;
    }
};

MenuObject::MenuObject(QObject *parent) : QObject{parent}
{
    this->p = new MenuObjectPvt{this};
}

MenuObject::MenuObject(const QVariant &v, QObject *parent) : QObject{parent}
{
    this->p = new MenuObjectPvt{this};

    p->data = v.toMap();
}

QVariantMap MenuObject::build() const
{

    auto v = p->build();
    return v;
}

QUuid MenuObject::uuid() const
{

    Q_DECLARE_VU;
    return p->data.value(QT_STRINGIFY(uuid)).toUuid();
}

const MenuObject &MenuObject::uuid(const QVariant &v)
{

    Q_DECLARE_VU;
    p->data[QT_STRINGIFY(uuid)] = vu.toUuid(v);
    return *this;
}

QString MenuObject::text() const
{

    auto __return = p->data.value(QT_STRINGIFY(text)).toString().trimmed();
    return __return;
}

MenuObject &MenuObject::text(const QVariant &v)
{

    p->data[QT_STRINGIFY(text)] = v;
    return *this;
}

QString MenuObject::basePath() const
{

    return p->data.value(QT_STRINGIFY(basePath)).toString().trimmed();
}

MenuObject &MenuObject::basePath(const QVariant &v)
{

    p->data[QT_STRINGIFY(basePath)] = v;
    return *this;
}

QString MenuObject::routeLoad() const
{

    return p->data.value(QT_STRINGIFY(routeLoad)).toString().trimmed();
}

MenuObject &MenuObject::routeLoad(const QVariant &v)
{

    p->data[QT_STRINGIFY(routeLoad)] = v;
    return *this;
}

QVariantHash MenuObject::routeLoader() const
{

    return p->data.value(QT_STRINGIFY(routeLoader)).toHash();
}

MenuObject &MenuObject::routeLoader(const QVariant &v)
{

    p->data[QT_STRINGIFY(routeLoader)] = v;
    return *this;
}

QVariantList MenuObject::menu() const
{

    auto menu = p->data.value(QT_STRINGIFY(menu)).toList();
    return menu;
}

MenuObject &MenuObject::menu(const QVariant &v)
{

    if (v.typeId() == QMetaType::QVariantList) {
        for (auto &v : v.toList())
            if (v.typeId() == QMetaType::QVariantMap || v.typeId() == QMetaType::QVariantHash)
                p->menuAdd(v.toMap());
    } else if (v.typeId() == QMetaType::QVariantMap || v.typeId() == QMetaType::QVariantHash) {
        p->menuAdd(v.toMap());
    }
    return *this;
}

MenuObject &MenuObject::menu(const MenuObject &v)
{
    if (v.isValid()) {

        p->menuAdd(v.toMap());
    }
    return *this;
}

QVariantList MenuObject::permission() const
{

    auto menu = p->data.value(QT_STRINGIFY(permission)).toList();
    return menu;
}

MenuObject &MenuObject::permission(const QVariant &v)
{
    if (v.isValid()) {

        if (v.typeId() == QMetaType::QVariantList) {
            for (auto &v : v.toList()) {
                if (v.typeId() == QMetaType::QVariantMap || v.typeId() == QMetaType::QVariantHash)
                    this->menu(v);
            }
        } else if (v.typeId() == QMetaType::QVariantMap || v.typeId() == QMetaType::QVariantHash) {
            p->permissionAdd(v);
        }
    }
    return *this;
}

QVariantHash MenuObject::toHash() const
{

    p->setUuid();
    return QVariant(p->data).toHash();
}

QVariantMap MenuObject::toMap() const
{

    p->setUuid();
    return p->data;
}

bool MenuObject::isEmpty() const
{
    return this->menu().isEmpty();
}

bool MenuObject::isValid() const
{
    if (this->text().isNull())
        return false;

    if (this->basePath().isNull() && this->routeLoad().isNull())
        return false;

    return true;
}

} // namespace QApr
