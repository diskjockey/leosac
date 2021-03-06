/*
    Copyright (C) 2014-2015 Islog

    This file is part of Leosac.

    Leosac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leosac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/auth/CredentialValidity.hpp"

using namespace Leosac::Auth;

bool CredentialValidity::is_valid() const
{
    return is_enabled() && is_in_range();
}

bool CredentialValidity::is_enabled() const
{
    return enabled_;
}

bool CredentialValidity::is_in_range() const
{
    TimePoint now = std::chrono::system_clock::now();

    if (now >= validity_start && now <= validity_end)
        return true;
    return false;
}
