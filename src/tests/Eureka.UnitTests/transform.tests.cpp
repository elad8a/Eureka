
//#include <Camera.hpp>
#include <catch.hpp>
#include <debugger_trace.hpp>
#include <eigen_graphics.hpp>
EUREKA_MSVC_WARNING_PUSH_DISABLE(4201)
#include <glm/glm.hpp>
#include <glm/ext.hpp>
EUREKA_MSVC_WARNING_POP

TEST_CASE("perspective", "[transofmations]")
{
    auto glmPerspective = glm::perspective(glm::pi<float>() / 4.0f, 4.0f / 3.0f, 0.01f, 1000.0f);
    auto eigenPerspective = Eigen::perspective(glm::pi<float>() / 4.0f, 4.0f / 3.0f, 0.01f, 1000.0f);

    std::vector<float> rawGlm(reinterpret_cast<float*>(&glmPerspective), reinterpret_cast<float*>(&glmPerspective) + 16);
    std::vector<float> rawEigen(reinterpret_cast<float*>(&eigenPerspective), reinterpret_cast<float*>(&eigenPerspective) + 16);

    CHECK(rawGlm == rawEigen);
}

