#ifndef Q_APR_TestIntDoc_H
#define Q_APR_TestIntDoc_H

#include "./qapr_test_integration.h"

namespace QApr{

class Q_APR_TestIntDoc : public SDKGoogleTestIntegration {
public:
};

TEST_F(Q_APR_TestIntDoc, check)
{
    EXPECT_TRUE(true)<<"fail";
}

}

#endif
