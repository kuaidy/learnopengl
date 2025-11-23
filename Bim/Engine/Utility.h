#pragma once
#include <cstddef>
#include <functional>

namespace Bim
{
	namespace Engine {

        constexpr float QUANTIZATION_SCALE = 100000.0f;

        struct QuantizedVertex {
            int x, y, z;
            // 可选：提供 operator==（但 unordered_map 默认不用它！）
            bool operator==(const QuantizedVertex& other) const {
                return x == other.x && y == other.y && z == other.z;
            }
        };

        struct QuantizedHash {
            std::size_t operator()(const QuantizedVertex& v) const noexcept {
                // 组合哈希（推荐使用 std::hash 和位运算）
                std::size_t h1 = std::hash<int>{}(v.x);
                std::size_t h2 = std::hash<int>{}(v.y);
                std::size_t h3 = std::hash<int>{}(v.z);
                // 简单组合（更好的方式见下文）
                return h1 ^ (h2 << 1) ^ (h3 << 2);
            }
        };
        struct QuantizedEqual {
            bool operator()(const QuantizedVertex& a, const QuantizedVertex& b) const noexcept {
                return a.x == b.x && a.y == b.y && a.z == b.z;
            }
        };
        inline QuantizedVertex QuantizeVertex(float x, float y, float z) {
            return QuantizedVertex{
                static_cast<int>(x * QUANTIZATION_SCALE),
                static_cast<int>(y * QUANTIZATION_SCALE),
                static_cast<int>(z * QUANTIZATION_SCALE)
            };
		}
	}
}