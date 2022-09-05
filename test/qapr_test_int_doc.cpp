#ifndef Q_APR_TestIntDoc_H
#define Q_APR_TestIntDoc_H

#include "./qapr_test_integration.h"
#include "./qapr_interface.h"

namespace QApr{

class Q_APR_TestIntDoc : public SDKGoogleTestIntegration {
public:
};

TEST_F(Q_APR_TestIntDoc, checkMethodsHealtCheck)
{
    QApr::Interface interface;

    auto vhash=interface.documentation().toHash();

    EXPECT_TRUE(!vhash.isEmpty())<<"fail no documentation";
    EXPECT_TRUE(vhash.contains("paths"))<<"invalid format";


    {
        auto vPathsList=vhash.value("paths").toList();
        QVariantHash vPaths;
        for(auto&v:vPathsList){
            auto vHash=v.toHash();
            QHashIterator <QString, QVariant> i(vHash);
            while(i.hasNext()){
                i.next();
                vPaths[i.key()]=i.value();
                auto vOperations=i.value().toHash().values();
                EXPECT_TRUE(!vOperations.isEmpty())<<"operation is empty";
                EXPECT_EQ(vOperations.size(),1)<<"most operations";
            }
        }

        EXPECT_TRUE(!vPaths.isEmpty())<<"No Paths";
        auto methodsCheck=QStringList{QStringLiteral("/backofficemenu"), QStringLiteral("/check"), QStringLiteral("/ping"), QStringLiteral("/fullcheck"), QStringLiteral("/connectionscheck"), QStringLiteral("/businesscheck")};
        for(auto&methodName:methodsCheck){
            EXPECT_TRUE(vPaths.contains(methodName))<<"path not found";
        }
    }
}

}

#endif
