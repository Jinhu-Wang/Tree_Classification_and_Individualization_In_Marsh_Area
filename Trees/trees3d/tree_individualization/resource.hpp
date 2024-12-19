
#pragma once

// System
#include <string>

// Local
#include <trees3d/tree_individualization/export.h>

namespace resource
{
    static std::string resource_dir = file_system::convert_to_native_style(Trees3D_RESOURCE_DIR);

    void initialize(const std::string &resource_dir = Trees3D_RESOURCE_DIR)
    {
        resource_dir = file_system::convert_to_native_style(res_dir);
    }

    std::string directory()
    {
        // first check if the resource directory (with the Trees3D distribution) exist
        std::string &dir = resource_dir;
        if (file_system::is_directory(dir))
        {
            VLOG_N_TIMES(1, 1) << "resource directory: " << dir;
            return dir;
        }

        std::string parent = file_system::executable_directory();
        dir = file_system::convert_to_native_style(parent + "/resources");
        if (file_system::is_directory(dir))
            return dir;
        else
        {
            // For macOS, if reached here, we may need to move "up" three times, because
            // macOS puts the executable file in an application bundle, e.g.,
            // "PolyFit.app/Contents/MacOS/PolyFit". Some IDEs may also put the 'exe' in
            // Debug/Release sub-folder, so we may try four times up at most.
            parent = file_system::parent_directory(parent);
            dir = file_system::convert_to_native_style(parent + "/resources");
            if (file_system::is_directory(dir))
                return dir;
            else
            {
                for (int i = 0; i < 4; ++i)
                {
                    parent = file_system::parent_directory(parent);
                    dir = file_system::convert_to_native_style(parent + "/resources");
                    if (file_system::is_directory(dir))
                        return dir;
                }
            }
            // if still could not find it, show an error and return the current working directory
            LOG_N_TIMES(1, ERROR) << "could not find the resource directory";
            return file_system::current_working_directory();
        }
    }
} //!- namespace resource