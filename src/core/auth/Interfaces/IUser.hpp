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

#pragma once

#include <memory>
#include "IAccessProfile.hpp"

namespace Leosac
{
    namespace Auth
    {
        class IUser;
        using IUserPtr = std::shared_ptr<IUser>;

        /**
        * Represent a user
        */
        class IUser
        {
        public:
            IUser(const std::string &user_id);
            virtual ~IUser() = default;

            /**
            * Get the current id.
            */
            const std::string &id() const noexcept;

            /**
            * Set a new id.
            */
            void id(const std::string &id_new);

            IAccessProfilePtr profile() const noexcept;

            void profile(IAccessProfilePtr user_profile);

        protected:
            /**
            * This is an (unique) identifier for the user.
            */
            std::string id_;

            IAccessProfilePtr profile_;
        };

    }
}
