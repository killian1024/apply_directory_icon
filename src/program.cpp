/* diricon
 * Copyright (C) 2018 Killian Poulaud.
 *
 * This file is part of diricon.
 *
 * diricon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * diricon is distributed in the hope that it will be useful,
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


namespace diricon {


program::program(
        std::filesystem::path src_pth,
        std::vector<std::string> icon_fles_nmes
) noexcept
        : src_pth_(std::move(src_pth))
        , icon_fles_nmes_(std::move(icon_fles_nmes))
{
}


int program::execute() const
{
    return execute_in_directory(src_pth_) ? 0 : -1;
}


bool program::execute_in_directory(const std::filesystem::path& cur_dir_pth) const
{
    std::filesystem::path cur_icon_pth;
    bool sucss = true;
    
    cur_icon_pth = cur_dir_pth;
    cur_icon_pth /= ".";
    for (auto& x : icon_fles_nmes_)
    {
        cur_icon_pth.replace_filename(x);
        
        if (std::filesystem::is_regular_file(cur_icon_pth))
        {
            std::cout << spdios::set_light_blue_text
                      << "Applying icon " << cur_icon_pth.filename() << " in " << cur_dir_pth;
    
            if (!apply_icon(cur_dir_pth, cur_icon_pth))
            {
                sucss = false;
                std::cout << spdios::set_light_red_text << " [fail]" << spdios::newl;
            }
            else
            {
                std::cout << spdios::set_light_green_text << " [ok]" << spdios::newl;
            }
        }
    }
    
    try
    {
        for (auto& x : std::filesystem::directory_iterator(cur_dir_pth))
        {
            if (std::filesystem::is_directory(x) || std::filesystem::is_symlink(x))
            {
                if (!execute_in_directory(x.path()))
                {
                    sucss = false;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& fe)
    {
        std::cerr << spdios::set_light_red_text
                  << "Error executing in directory: " << cur_dir_pth
                  << spdios::newl;
    
        return false;
    }
    
    return sucss;
}


bool program::apply_icon(
        const std::filesystem::path& cur_dir,
        const std::filesystem::path& cur_icon_pth
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
    value = (void*)cur_icon_pth.filename().c_str();
    
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
