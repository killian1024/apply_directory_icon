/* apply_directory_icon
 * Copyright (C) 2018 Killian Poulaud.
 *
 * This file is part of apply_directory_icon.
 *
 * apply_directory_icon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * apply_directory_icon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

//
// Created by Killian on 13/07/18.
//

#include <gio/gio.h>
#include <glib.h>

#include <iostream>

#include <speed/speed.hpp>

#include "program.hpp"


namespace apply_directory_icon {


program::program(
        std::filesystem::path src_pth,
        std::unordered_set<std::string> icon_fles_nmes
) noexcept
        : src_pth_(std::move(src_pth))
        , icon_fles_nmes_(std::move(icon_fles_nmes))
{
}


int program::execute()
{
    return execute_in_directory(src_pth_) ? 0 : -1;
}


bool program::execute_in_directory(const std::filesystem::path& cur_dir) const
{
    bool sucss = true;
    
    try
    {
        for (auto& x : std::filesystem::directory_iterator(cur_dir))
        {
            if (std::filesystem::is_regular_file(x) &&
                icon_fles_nmes_.count(x.path().filename()) > 0)
            {
                std::cout << spdios::set_light_blue_text
                          << "Applying icon " << x.path().filename() << " in " << cur_dir;
                
                if (!apply_icon(cur_dir, x.path()))
                {
                    sucss = false;
                    std::cout << spdios::set_light_red_text << " [fail]" << spdios::newl;
                }
                else
                {
                    std::cout << spdios::set_light_green_text << " [ok]" << spdios::newl;
                }
            }
            else if (std::filesystem::is_directory(x) || std::filesystem::is_symlink(x))
            {
                if (!execute_in_directory(x.path()))
                {
                    sucss = false;
                }
            }
        }
        
        return sucss;
    }
    catch (const std::filesystem::filesystem_error& ignored)
    {
        std::cerr << spdios::set_light_red_text
                  << "Error executing in directory: " << cur_dir
                  << spdios::newl;
    
        return false;
    }
}


bool program::apply_icon(
        const std::filesystem::path& cur_dir,
        const std::filesystem::path& icon_pth
) const
{
    GFile* file;
    const char* attribute;
    GFileAttributeType type;
    gpointer value;
    GError* error = nullptr;
    
    file = g_file_new_for_commandline_arg(cur_dir.c_str());
    attribute = "metadata::custom-icon";
    type = G_FILE_ATTRIBUTE_TYPE_STRING;
    value = (void*)icon_pth.filename().c_str();
    
    if (!g_file_set_attribute(file, attribute, type, value, G_FILE_QUERY_INFO_NONE, nullptr,
                              &error))
    {
        //std::cerr << spdios::set_light_red_text
        //          << "Error setting attribute: " << error->message
        //          << spdios::newl;
        
        g_error_free(error);
        
        return false;
    }
    
    return true;
}


}
