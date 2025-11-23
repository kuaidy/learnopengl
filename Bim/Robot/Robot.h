#pragma once
#include "Link.h"
#include "Joint.h"
#include "../Scene/Node.h"

namespace Bim
{
    namespace Robot 
    {
        struct Robot : public Scene::Node{
            std::shared_ptr<Link> root;
            std::vector<std::shared_ptr<Link>> links;
            std::vector<std::shared_ptr<Joint>> joints;
        };
    }
}
