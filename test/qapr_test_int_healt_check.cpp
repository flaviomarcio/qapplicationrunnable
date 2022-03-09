#ifndef Q_APR_TestIntHeatCheck_H
#define Q_APR_TestIntHeatCheck_H

#include "./qapr_test_integration.h"

namespace QApr{

class Q_APR_TestIntHeatCheck : public SDKGoogleTestIntegration {
public:
};

TEST_F(Q_APR_TestIntHeatCheck, check)
{
    EXPECT_TRUE(true)<<"fail";
}

}

#endif
