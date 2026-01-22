#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "matrix.h"

namespace sdk
{
    uintptr_t get_datamodel(uintptr_t base);
	uintptr_t get_visualengine(uintptr_t base);
	uintptr_t get_camera(uintptr_t workspace);
	vector2 get_viewport_size(uintptr_t camera);
	std::vector<uintptr_t> get_children(uintptr_t instance);
	uintptr_t find_first_child_by_class(uintptr_t parent, const std::string& class_name);
}