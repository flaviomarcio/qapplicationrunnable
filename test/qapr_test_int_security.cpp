#ifndef Q_APR_TestIntSecurity_H
#define Q_APR_TestIntSecurity_H

#include "./qapr_test_integration.h"

namespace QApr{

class Q_APR_TestIntSecurity : public SDKGoogleTestIntegration {
public:
};

TEST_F(Q_APR_TestIntSecurity, check)
{
    EXPECT_TRUE(true)<<"fail";
}

}

#endif
