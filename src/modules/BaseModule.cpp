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

#include <boost/archive/text_oarchive.hpp>
#include <boost/property_tree/ptree_serialization.hpp>
#include <tools/log.hpp>
#include <core/config/ConfigManager.hpp>
#include "BaseModule.hpp"
#include "tools/XmlPropertyTree.hpp"

using namespace Leosac::Module;
using namespace Leosac::Tools;

BaseModule::BaseModule(zmqpp::context &ctx,
        zmqpp::socket *pipe,
        boost::property_tree::ptree const &cfg) :
        ctx_(ctx),
        pipe_(*pipe),
        config_(cfg),
        is_running_(true),
        control_(ctx, zmqpp::socket_type::rep)
{
    name_ = cfg.get<std::string>("name");
    control_.bind("inproc://module-" + name_);

    reactor_.add(control_, std::bind(&BaseModule::handle_control, this));
    reactor_.add(pipe_, std::bind(&BaseModule::handle_pipe, this));
}

void BaseModule::run()
{
    while (is_running_)
    {
        reactor_.poll();
    }
}

void BaseModule::handle_pipe()
{
    zmqpp::message msg;
    zmqpp::signal sig;
    pipe_.receive(msg);

    assert(msg.is_signal());
    msg >> sig;
    if (sig == zmqpp::signal::stop)
        is_running_ = false;
    else
    {
        ERROR("Module receive a message on its pipe that wasn't a signal. Aborting.");
        assert(0);
        throw std::runtime_error("Module receive a message on its pipe that wasn't a signal. Aborting.");
    }
}

void BaseModule::dump_config(ConfigManager::ConfigFormat fmt, zmqpp::message *out_msg) const
{
    assert(out_msg);
    if (fmt == ConfigManager::ConfigFormat::BOOST_ARCHIVE)
    {
        std::ostringstream oss;
        boost::archive::text_oarchive archive(oss);
        boost::property_tree::save(archive, config_, 1);
        out_msg->add(oss.str());
    }
    else
    {
        out_msg->add(propertyTreeToXml(config_));
    }
    try
    {
        dump_additional_config(out_msg);
    }
    catch (std::exception &e)
    {
        ERROR("Problem while dumping config: " << e.what());
    }
}

void BaseModule::handle_control()
{
    zmqpp::message msg;
    std::string frame1;

    control_.receive(msg);
    msg >> frame1;
    if (frame1 == "DUMP_CONFIG")
    {
        zmqpp::message response;

        assert(msg.remaining() == 1);
        ConfigManager::ConfigFormat format;
        msg >> format;
        DEBUG("Module " << name_ << " is dumping config!");
        dump_config(format, &response);
        control_.send(response);
    }
    else
    {
        ERROR("Module received invalid request (" << frame1 << "). Aborting.");
        assert(0);
        throw std::runtime_error("Invalid request for module.");
    }
}

void BaseModule::dump_additional_config(zmqpp::message *) const
{

}
