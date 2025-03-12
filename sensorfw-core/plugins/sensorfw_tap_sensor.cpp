/*
 * Copyright © 2020 UBports foundation
 * Copyright © 2021 Andromeda Project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Marius Gripsgard <marius@ubports.com>
 * Authored by: Erfan Abdi <erfangplus@gmail.com>
 */

#include <plugins/sensorfw_tap_sensor.h>

#include <stdexcept>

namespace
{
auto const null_handler = [](TapData){};
}

andromeda::core::SensorfwTapSensor::SensorfwTapSensor(
    std::string const &dbus_bus_address)
    : Sensorfw(dbus_bus_address, "Tap", PluginType::TAP),
      handler{null_handler}
{
}

andromeda::core::HandlerRegistration andromeda::core::SensorfwTapSensor::register_tap_handler(
    TapHandler const& handler)
{
    return EventLoopHandlerRegistration{
        dbus_event_loop,
        [this, &handler]{ this->handler = handler; },
        [this]{ this->handler = null_handler; }};
}

void andromeda::core::SensorfwTapSensor::enable_tap_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            start();
        }).get();
}

void andromeda::core::SensorfwTapSensor::disable_tap_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            stop();
        }).get();
}

void andromeda::core::SensorfwTapSensor::data_recived_impl()
{
    std::vector<TapData> values;
    if(!m_socket->read<TapData>(values))
        return;

    handler(values[0]);
}
