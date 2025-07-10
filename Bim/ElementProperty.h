#pragma once
#include <string>
#include <vector>

/// <summary>
/// ģ������
/// </summary>
struct ElementProperty
{
	int node_index;
	std::string name;
	std::string value;
	std::vector<ElementProperty> children;
}; 