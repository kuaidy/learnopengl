#ifndef CONTEXT_H
#define CONTEXT_H
#include <memory>
#include "../Scene/Node.h"

namespace Bim
{
	namespace Engine {
		struct Context {
			std::shared_ptr<Scene::Node> scene;
		};

		// 全局访问点（线程安全，延迟初始化）
		inline Context& GetGlobalContext() {
			static Context instance; // C++11 起，函数内 static 初始化是线程安全的
			return instance;
		}
	}
}
#endif