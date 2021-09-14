#include "gtest/gtest.h"
#include "qservice/parser.hpp"
#include "qservice/qservice.hpp"

TEST(first_test, try)
{

    qservice::QService  service("0.0.0.0", 8088);
    service.Run();
}