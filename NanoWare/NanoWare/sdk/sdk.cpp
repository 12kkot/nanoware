#include "sdk.h"
#include "offsets.hpp"
#include "matrix.h"
#include "../memory/memory.h"

uintptr_t sdk::get_datamodel(uintptr_t base)
{
    uintptr_t fake_holder = memory::read<uintptr_t>(base + offsets::FakeDataModelPointer);
    return memory::read<uintptr_t>(fake_holder + offsets::FakeDataModelToDataModel);;
}

uintptr_t sdk::get_visualengine(uintptr_t base)
{
	return memory::read<uintptr_t>(base + offsets::VisualEnginePointer);
}

std::vector<uintptr_t> sdk::get_children(uintptr_t instance)
{
    std::vector<uintptr_t> children;
    
    uintptr_t start = memory::read<uintptr_t>(instance + offsets::Children);
    uintptr_t end = memory::read<uintptr_t>(start + offsets::ChildrenEnd);
    
    for (uintptr_t child = memory::read<uintptr_t>(start); child != end; child += 0x10)
    {
        children.push_back(memory::read<uintptr_t>(child));
    }
    
    return children;
}

uintptr_t sdk::find_first_child_by_class(uintptr_t parent, const std::string& class_name)
{
    for (auto child : sdk::get_children(parent))
    {
        uintptr_t class_desc = memory::read<uintptr_t>(child + offsets::ClassDescriptor);
        if (!class_desc)
            continue;

        uintptr_t class_name_ptr = memory::read<uintptr_t>(class_desc + offsets::ClassDescriptorToClassName);
        if (!class_name_ptr)
            continue;

        if (memory::read_string(class_name_ptr) == class_name)
            return child;
    }

    return 0;
}

uintptr_t sdk::get_camera(uintptr_t workspace)
{
    return sdk::find_first_child_by_class(workspace, "Camera");;
}

vector2 sdk::get_viewport_size(uintptr_t camera)
{
    return memory::read<vector2>(camera + offsets::ViewportSize);
}