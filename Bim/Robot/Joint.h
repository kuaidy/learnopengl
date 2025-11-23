#pragma once

#include <string>
#include <Eigen/Dense>

namespace Bim
{
    namespace Robot 
    {
        struct Joint {
            std::string name;
            float position;                  // 角度或位移
            float minLimit, maxLimit;
            Eigen::Matrix4d globalMatrix;         // 父link到joint的transform
            Eigen::Matrix4d localMatrix;         // joint到子link
        };
    }
}

