/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK FmRdsSimulator.
 *
 * REDHAWK FmRdsSimulator is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK FmRdsSimulator is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */
#ifndef RDC_STRUCTPROPS_H
#define RDC_STRUCTPROPS_H

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

*******************************************************************************************/

#include <ossie/CorbaUtils.h>
#include <CF/cf.h>
#include <ossie/PropertyMap.h>


namespace RDC_ns {
struct device_characteristics_struct {
    device_characteristics_struct ()
    {
    }

    static std::string getId() {
        return std::string("device_characteristics");
    }

    static const char* getFormat() {
        return "sshsdddddddddddddd[s]";
    }

    std::string ch_name;
    std::string tuner_type;
    short chan_num;
    std::string antenna;
    double bandwidth_current;
    double bandwidth_min;
    double bandwidth_max;
    double rate_current;
    double rate_min;
    double rate_max;
    double freq_current;
    double freq_min;
    double freq_max;
    double gain_current;
    double gain_min;
    double gain_max;
    double clock_min;
    double clock_max;
    std::vector<std::string> available_antennas;
};
};

inline bool operator>>= (const CORBA::Any& a, RDC_ns::device_characteristics_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("device_characteristics::ch_name")) {
        if (!(props["device_characteristics::ch_name"] >>= s.ch_name)) return false;
    }
    if (props.contains("device_characteristics::tuner_type")) {
        if (!(props["device_characteristics::tuner_type"] >>= s.tuner_type)) return false;
    }
    if (props.contains("device_characteristics::chan_num")) {
        if (!(props["device_characteristics::chan_num"] >>= s.chan_num)) return false;
    }
    if (props.contains("device_characteristics::antenna")) {
        if (!(props["device_characteristics::antenna"] >>= s.antenna)) return false;
    }
    if (props.contains("device_characteristics::bandwidth_current")) {
        if (!(props["device_characteristics::bandwidth_current"] >>= s.bandwidth_current)) return false;
    }
    if (props.contains("device_characteristics::bandwidth_min")) {
        if (!(props["device_characteristics::bandwidth_min"] >>= s.bandwidth_min)) return false;
    }
    if (props.contains("device_characteristics::bandwidth_max")) {
        if (!(props["device_characteristics::bandwidth_max"] >>= s.bandwidth_max)) return false;
    }
    if (props.contains("device_characteristics::rate_current")) {
        if (!(props["device_characteristics::rate_current"] >>= s.rate_current)) return false;
    }
    if (props.contains("device_characteristics::rate_min")) {
        if (!(props["device_characteristics::rate_min"] >>= s.rate_min)) return false;
    }
    if (props.contains("device_characteristics::rate_max")) {
        if (!(props["device_characteristics::rate_max"] >>= s.rate_max)) return false;
    }
    if (props.contains("device_characteristics::freq_current")) {
        if (!(props["device_characteristics::freq_current"] >>= s.freq_current)) return false;
    }
    if (props.contains("device_characteristics::freq_min")) {
        if (!(props["device_characteristics::freq_min"] >>= s.freq_min)) return false;
    }
    if (props.contains("device_characteristics::freq_max")) {
        if (!(props["device_characteristics::freq_max"] >>= s.freq_max)) return false;
    }
    if (props.contains("device_characteristics::gain_current")) {
        if (!(props["device_characteristics::gain_current"] >>= s.gain_current)) return false;
    }
    if (props.contains("device_characteristics::gain_min")) {
        if (!(props["device_characteristics::gain_min"] >>= s.gain_min)) return false;
    }
    if (props.contains("device_characteristics::gain_max")) {
        if (!(props["device_characteristics::gain_max"] >>= s.gain_max)) return false;
    }
    if (props.contains("device_characteristics::clock_min")) {
        if (!(props["device_characteristics::clock_min"] >>= s.clock_min)) return false;
    }
    if (props.contains("device_characteristics::clock_max")) {
        if (!(props["device_characteristics::clock_max"] >>= s.clock_max)) return false;
    }
    if (props.contains("device_characteristics::available_antennas")) {
        if (!(props["device_characteristics::available_antennas"] >>= s.available_antennas)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const RDC_ns::device_characteristics_struct& s) {
    redhawk::PropertyMap props;
 
    props["device_characteristics::ch_name"] = s.ch_name;
 
    props["device_characteristics::tuner_type"] = s.tuner_type;
 
    props["device_characteristics::chan_num"] = s.chan_num;
 
    props["device_characteristics::antenna"] = s.antenna;
 
    props["device_characteristics::bandwidth_current"] = s.bandwidth_current;
 
    props["device_characteristics::bandwidth_min"] = s.bandwidth_min;
 
    props["device_characteristics::bandwidth_max"] = s.bandwidth_max;
 
    props["device_characteristics::rate_current"] = s.rate_current;
 
    props["device_characteristics::rate_min"] = s.rate_min;
 
    props["device_characteristics::rate_max"] = s.rate_max;
 
    props["device_characteristics::freq_current"] = s.freq_current;
 
    props["device_characteristics::freq_min"] = s.freq_min;
 
    props["device_characteristics::freq_max"] = s.freq_max;
 
    props["device_characteristics::gain_current"] = s.gain_current;
 
    props["device_characteristics::gain_min"] = s.gain_min;
 
    props["device_characteristics::gain_max"] = s.gain_max;
 
    props["device_characteristics::clock_min"] = s.clock_min;
 
    props["device_characteristics::clock_max"] = s.clock_max;
 
    props["device_characteristics::available_antennas"] = s.available_antennas;
    a <<= props;
}

inline bool operator== (const RDC_ns::device_characteristics_struct& s1, const RDC_ns::device_characteristics_struct& s2) {
    if (s1.ch_name!=s2.ch_name)
        return false;
    if (s1.tuner_type!=s2.tuner_type)
        return false;
    if (s1.chan_num!=s2.chan_num)
        return false;
    if (s1.antenna!=s2.antenna)
        return false;
    if (s1.bandwidth_current!=s2.bandwidth_current)
        return false;
    if (s1.bandwidth_min!=s2.bandwidth_min)
        return false;
    if (s1.bandwidth_max!=s2.bandwidth_max)
        return false;
    if (s1.rate_current!=s2.rate_current)
        return false;
    if (s1.rate_min!=s2.rate_min)
        return false;
    if (s1.rate_max!=s2.rate_max)
        return false;
    if (s1.freq_current!=s2.freq_current)
        return false;
    if (s1.freq_min!=s2.freq_min)
        return false;
    if (s1.freq_max!=s2.freq_max)
        return false;
    if (s1.gain_current!=s2.gain_current)
        return false;
    if (s1.gain_min!=s2.gain_min)
        return false;
    if (s1.gain_max!=s2.gain_max)
        return false;
    if (s1.clock_min!=s2.clock_min)
        return false;
    if (s1.clock_max!=s2.clock_max)
        return false;
    if (s1.available_antennas!=s2.available_antennas)
        return false;
    return true;
}

inline bool operator!= (const RDC_ns::device_characteristics_struct& s1, const RDC_ns::device_characteristics_struct& s2) {
    return !(s1==s2);
}

namespace RDC_ns {
struct frontend_tuner_status_struct_struct : public frontend::default_frontend_tuner_status_struct_struct {
    frontend_tuner_status_struct_struct () : frontend::default_frontend_tuner_status_struct_struct()
    {
    }

    static std::string getId() {
        return std::string("FRONTEND::tuner_status_struct");
    }

    static const char* getFormat() {
        return "sddbssdsddds";
    }

    double bandwidth_tolerance;
    double sample_rate_tolerance;
    double gain;
    std::string stream_id;
};
};

inline bool operator>>= (const CORBA::Any& a, RDC_ns::frontend_tuner_status_struct_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("FRONTEND::tuner_status::allocation_id_csv")) {
        if (!(props["FRONTEND::tuner_status::allocation_id_csv"] >>= s.allocation_id_csv)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::bandwidth")) {
        if (!(props["FRONTEND::tuner_status::bandwidth"] >>= s.bandwidth)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::center_frequency")) {
        if (!(props["FRONTEND::tuner_status::center_frequency"] >>= s.center_frequency)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::enabled")) {
        if (!(props["FRONTEND::tuner_status::enabled"] >>= s.enabled)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::group_id")) {
        if (!(props["FRONTEND::tuner_status::group_id"] >>= s.group_id)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::rf_flow_id")) {
        if (!(props["FRONTEND::tuner_status::rf_flow_id"] >>= s.rf_flow_id)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::sample_rate")) {
        if (!(props["FRONTEND::tuner_status::sample_rate"] >>= s.sample_rate)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::tuner_type")) {
        if (!(props["FRONTEND::tuner_status::tuner_type"] >>= s.tuner_type)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::bandwidth_tolerance")) {
        if (!(props["FRONTEND::tuner_status::bandwidth_tolerance"] >>= s.bandwidth_tolerance)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::sample_rate_tolerance")) {
        if (!(props["FRONTEND::tuner_status::sample_rate_tolerance"] >>= s.sample_rate_tolerance)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::gain")) {
        if (!(props["FRONTEND::tuner_status::gain"] >>= s.gain)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::stream_id")) {
        if (!(props["FRONTEND::tuner_status::stream_id"] >>= s.stream_id)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const RDC_ns::frontend_tuner_status_struct_struct& s) {
    redhawk::PropertyMap props;
 
    props["FRONTEND::tuner_status::allocation_id_csv"] = s.allocation_id_csv;
 
    props["FRONTEND::tuner_status::bandwidth"] = s.bandwidth;
 
    props["FRONTEND::tuner_status::center_frequency"] = s.center_frequency;
 
    props["FRONTEND::tuner_status::enabled"] = s.enabled;
 
    props["FRONTEND::tuner_status::group_id"] = s.group_id;
 
    props["FRONTEND::tuner_status::rf_flow_id"] = s.rf_flow_id;
 
    props["FRONTEND::tuner_status::sample_rate"] = s.sample_rate;
 
    props["FRONTEND::tuner_status::tuner_type"] = s.tuner_type;
 
    props["FRONTEND::tuner_status::bandwidth_tolerance"] = s.bandwidth_tolerance;
 
    props["FRONTEND::tuner_status::sample_rate_tolerance"] = s.sample_rate_tolerance;
 
    props["FRONTEND::tuner_status::gain"] = s.gain;
 
    props["FRONTEND::tuner_status::stream_id"] = s.stream_id;
    a <<= props;
}

inline bool operator== (const RDC_ns::frontend_tuner_status_struct_struct& s1, const RDC_ns::frontend_tuner_status_struct_struct& s2) {
    if (s1.allocation_id_csv!=s2.allocation_id_csv)
        return false;
    if (s1.bandwidth!=s2.bandwidth)
        return false;
    if (s1.center_frequency!=s2.center_frequency)
        return false;
    if (s1.enabled!=s2.enabled)
        return false;
    if (s1.group_id!=s2.group_id)
        return false;
    if (s1.rf_flow_id!=s2.rf_flow_id)
        return false;
    if (s1.sample_rate!=s2.sample_rate)
        return false;
    if (s1.tuner_type!=s2.tuner_type)
        return false;
    if (s1.bandwidth_tolerance!=s2.bandwidth_tolerance)
        return false;
    if (s1.sample_rate_tolerance!=s2.sample_rate_tolerance)
        return false;
    if (s1.gain!=s2.gain)
        return false;
    if (s1.stream_id!=s2.stream_id)
        return false;
    return true;
}

inline bool operator!= (const RDC_ns::frontend_tuner_status_struct_struct& s1, const RDC_ns::frontend_tuner_status_struct_struct& s2) {
    return !(s1==s2);
}

#endif // STRUCTPROPS_H
