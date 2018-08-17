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
        std::vector<std::string> icon_nmes
) noexcept
        : src_pth_(std::move(src_pth))
        , icon_nmes_(std::move(icon_nmes))
{
}


int program::execute()
{
    return execute_in_directory(src_pth_) ? 0 : -1;
}


bool program::execute_in_directory(const std::filesystem::path& dir_pth)
{
    std::filesystem::path icon_pth;
    bool sucss = true;
    
    visit_inode(dir_pth);
    
    icon_pth = dir_pth;
    icon_pth /= ".";
    
    for (auto& x : icon_nmes_)
    {
        icon_pth.replace_filename(x);
        
        if (std::filesystem::is_regular_file(icon_pth))
        {
            std::cout << spdios::set_light_blue_text
                      << "Applying icon " << icon_pth.filename() << " in " << dir_pth;
    
            if (!apply_icon(dir_pth, icon_pth))
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
        for (auto& x : std::filesystem::directory_iterator(dir_pth))
        {
            if (std::filesystem::is_directory(x) && !is_inode_visited(x))
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
                  << "Error executing in directory: " << dir_pth
                  << spdios::newl;
    
        return false;
    }
    
    return sucss;
}


void program::visit_inode(const std::filesystem::path& dir_pth)
{
    vistd_inos_.insert(spdsys::get_file_inode(dir_pth.c_str()));
}


bool program::is_inode_visited(const std::filesystem::path& dir_pth) const noexcept
{
    return vistd_inos_.count(spdsys::get_file_inode(dir_pth.c_str())) > 0;
}


bool program::apply_icon(
        const std::filesystem::path& dir_pth,
        const std::filesystem::path& icon_pth
) const
{
    GFile* file;
    const char* attribute;
    GFileAttributeType type;
    gpointer value;
    GError* error = nullptr;
    
    file = g_file_new_for_commandline_arg(dir_pth.c_str());
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
