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
          data({{qsl_fy(uuid), QUuid()},
                {qsl_fy(text), qv_null},
                {qsl_fy(route), qv_null},
                {qsl_fy(routeLoad), qv_null},
                {qsl_fy(routeLoader), qv_null},
                {qsl_fy(menu), qv_null}})
    {
        this->parent = parent;
    }

    void setUuid()
    {
        Q_DECLARE_VU;
        auto route = this->parent->basePath();
        auto routeLoad = this->parent->routeLoad();
        auto codeBase = route + qsl("/") + routeLoad;
        this->data[qsl_fy(uuid)] = vu.toMd5Uuid(codeBase);
    }

    void permissionAdd(const QVariant &v)
    {
        if (v.isValid()) {
            auto vMenu = this->data[qsl_fy(permission)].toList();
            vMenu.append(v);
            this->data[qsl_fy(permission)] = vMenu;
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
                    route = (routeBase + qsl_fy(/) + route);
            }
            auto vHash = menu.basePath(route).build();
            if (!vHash.isEmpty()) {
                auto &vMenu = this->dataMenu[qsl_fy(menu)];
                if (!vMenu.contains(menu.text())) {
                    vMenu.insert(menu.text(), vHash);
                    this->dataMenu[qsl_fy(menu)] = vMenu;
                    {
                        auto vMenuList = this->data[qsl_fy(menu)].toList();
                        vMenuList.append(vHash);
                        this->data[qsl_fy(menu)] = vMenuList;
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
    return p->data.value(qsl_fy(uuid)).toUuid();
}

const MenuObject &MenuObject::uuid(const QVariant &v)
{

    Q_DECLARE_VU;
    p->data[qsl_fy(uuid)] = vu.toUuid(v);
    return *this;
}

QString MenuObject::text() const
{

    auto __return = p->data.value(qsl_fy(text)).toString().trimmed();
    return __return;
}

MenuObject &MenuObject::text(const QVariant &v)
{

    p->data[qsl_fy(text)] = v;
    return *this;
}

QString MenuObject::basePath() const
{

    return p->data.value(qsl_fy(basePath)).toString().trimmed();
}

MenuObject &MenuObject::basePath(const QVariant &v)
{

    p->data[qsl_fy(basePath)] = v;
    return *this;
}

QString MenuObject::routeLoad() const
{

    return p->data.value(qsl_fy(routeLoad)).toString().trimmed();
}

MenuObject &MenuObject::routeLoad(const QVariant &v)
{

    p->data[qsl_fy(routeLoad)] = v;
    return *this;
}

QVariantHash MenuObject::routeLoader() const
{

    return p->data.value(qsl_fy(routeLoader)).toHash();
}

MenuObject &MenuObject::routeLoader(const QVariant &v)
{

    p->data[qsl_fy(routeLoader)] = v;
    return *this;
}

QVariantList MenuObject::menu() const
{

    auto menu = p->data.value(qsl_fy(menu)).toList();
    return menu;
}

MenuObject &MenuObject::menu(const QVariant &v)
{

    if (qTypeId(v) == QMetaType_QVariantList) {
        for (auto &v : v.toList())
            if (qTypeId(v) == QMetaType_QVariantMap || qTypeId(v) == QMetaType_QVariantHash)
                p->menuAdd(v.toMap());
    } else if (qTypeId(v) == QMetaType_QVariantMap || qTypeId(v) == QMetaType_QVariantHash) {
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

    auto menu = p->data.value(qsl_fy(permission)).toList();
    return menu;
}

MenuObject &MenuObject::permission(const QVariant &v)
{
    if (v.isValid()) {

        if (qTypeId(v) == QMetaType_QVariantList) {
            for (auto &v : v.toList()) {
                if (qTypeId(v) == QMetaType_QVariantMap || qTypeId(v) == QMetaType_QVariantHash)
                    this->menu(v);
            }
        } else if (qTypeId(v) == QMetaType_QVariantMap || qTypeId(v) == QMetaType_QVariantHash) {
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
