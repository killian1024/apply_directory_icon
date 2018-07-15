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

#ifndef APPLY_DIRECTORY_ICON_PROGRAM_HPP
#define APPLY_DIRECTORY_ICON_PROGRAM_HPP

#include <filesystem>
#include <string>
#include <unordered_set>


namespace diricon {


class program
{
public:
    program(
            std::filesystem::path src_pth,
            std::unordered_set<std::string> icon_fles_nmes
    ) noexcept;
    
    int execute();

private:
    bool execute_in_directory(const std::filesystem::path& cur_dir) const;
    
    bool apply_icon(
            const std::filesystem::path& cur_dir,
            const std::filesystem::path& icon_pth
    ) const;

private:
    std::filesystem::path src_pth_;
    
    std::unordered_set<std::string> icon_fles_nmes_;
};


}


#endif
