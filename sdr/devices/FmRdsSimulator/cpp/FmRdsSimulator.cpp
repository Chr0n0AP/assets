/**************************************************************************

    This is the device code. This file contains the child class where
    custom functionality can be added to the device. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "FmRdsSimulator.h"

PREPARE_LOGGING(FmRdsSimulator_i)

FmRdsSimulator_i::FmRdsSimulator_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    FmRdsSimulator_base(devMgr_ior, id, lbl, sftwrPrfl)
{
}

FmRdsSimulator_i::FmRdsSimulator_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    FmRdsSimulator_base(devMgr_ior, id, lbl, sftwrPrfl, compDev)
{
}

FmRdsSimulator_i::FmRdsSimulator_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    FmRdsSimulator_base(devMgr_ior, id, lbl, sftwrPrfl, capacities)
{
}

FmRdsSimulator_i::FmRdsSimulator_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    FmRdsSimulator_base(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev)
{
}

FmRdsSimulator_i::~FmRdsSimulator_i()
{
}

void FmRdsSimulator_i::constructor()
{
    /***********************************************************************************
     This is the RH constructor. All properties are properly initialized before this function is called 

     For a tuner device, the structure frontend_tuner_status needs to match the number
     of tuners that this device controls and what kind of device it is.
     The options for devices are: ANTENNA, RX, RX_ARRAY, DBOT, ABOT, ARDC, RDC, SRDC, DRDC, TX, TX_ARRAY, TDC
     
     An example of setting up this device as an ABOT would look like this:

     this->addChannels(1, "ABOT");
     
     The incoming request for tuning contains a string describing the requested tuner
     type. The string for the request must match the string in the tuner status.
    ***********************************************************************************/
    this->addChannels(1, "ABOT");
    std::string rds_name("RDC_1");
    RDC_ns::RDC_i* tmp = this->addChild<RDC_ns::RDC_i>(rds_name);
    RDCs.push_back(tmp);
}

// CF::Device::Allocations* FmRdsSimulator_i::allocate (const CF::Properties& capacities) {
//     CF::Device::Allocations_var result = new CF::Device::Allocations();
//     result = FmRdsSimulator_base::allocate(capacities);
//     /*
//      * Add data and control ports to response if length is greater than 0
//      */
//     return result._retn();
// }

CORBA::Boolean FmRdsSimulator_i::allocateCapacity(const CF::Properties & capacities) {

    std::string allocation_id;

    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(capacities);
    CF::Properties local_capacities;
    redhawk::PropertyMap& local_props = redhawk::PropertyMap::cast(local_capacities);
    local_props = props;

    if (local_props.find("FRONTEND::tuner_allocation") != local_props.end()) {
        redhawk::PropertyMap& tuner_alloc = redhawk::PropertyMap::cast(local_props["FRONTEND::tuner_allocation"].asProperties());
        if (tuner_alloc.find("FRONTEND::tuner_allocation::allocation_id") != tuner_alloc.end()) {
            std::string requested_alloc = tuner_alloc["FRONTEND::tuner_allocation::allocation_id"].toString();
            if (not requested_alloc.empty()) {
                if (_delegatedAllocations.find(requested_alloc) == _delegatedAllocations.end()) {
                    allocation_id = requested_alloc;
                } else {
                    throw frontend::AllocationAlreadyExists("ALLOCATION_ID ALREADY IN USE", capacities);
                }
                tuner_alloc["FRONTEND::tuner_allocation::allocation_id"] = allocation_id;
            }
        }
    }

    for (std::vector<RDC_ns::RDC_i*>::iterator it=RDCs.begin(); it!=RDCs.end(); it++) {
        bool result = (*it)->allocateCapacity(capacities);
        if (result) {
            CF::Device::Allocations_var alloc_response = new CF::Device::Allocations();
            alloc_response->length(1);
            alloc_response[0].device_ref = CF::Device::_duplicate((*it)->_this());
            _delegatedAllocations[allocation_id] = alloc_response;
            _usageState = updateUsageState();
            return true;
        }
    }
    
    return FmRdsSimulator_base::allocateCapacity(capacities);
}

void FmRdsSimulator_i::deallocateCapacity (const CF::Properties& capacities) {
    std::string allocation_id;

    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(capacities);
    CF::Properties local_capacities;
    redhawk::PropertyMap& local_props = redhawk::PropertyMap::cast(local_capacities);
    local_props = props;

    if (local_props.find("FRONTEND::tuner_allocation") != local_props.end()) {
        redhawk::PropertyMap& tuner_alloc = redhawk::PropertyMap::cast(local_props["FRONTEND::tuner_allocation"].asProperties());
        if (tuner_alloc.find("FRONTEND::tuner_allocation::allocation_id") != tuner_alloc.end()) {
            std::string requested_alloc = tuner_alloc["FRONTEND::tuner_allocation::allocation_id"].toString();
            if (not requested_alloc.empty()) {
                allocation_id = requested_alloc;
            }
        }
    }
    if (_delegatedAllocations.find(allocation_id) != _delegatedAllocations.end()) {
        _delegatedAllocations[allocation_id][0].device_ref->deallocateCapacity(capacities);
        _delegatedAllocations.erase(allocation_id);
        _usageState = updateUsageState();
        return;
    }
    
    FmRdsSimulator_base::deallocateCapacity(capacities);
    return;
}

CF::Device::Allocations* FmRdsSimulator_i::allocate(const CF::Properties& capacities) {
    CF::Device::Allocations_var result = new CF::Device::Allocations();

    if (capacities.length() == 0) {
        RH_TRACE(this->_baseLog, "no capacities to configure.");
        return result._retn();
    }

    std::string allocation_id = ossie::generateUUID();
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(capacities);

    // copy the const properties to something that is modifiable
    CF::Properties local_capacities;
    redhawk::PropertyMap& local_props = redhawk::PropertyMap::cast(local_capacities);
    local_props = props;

    if (local_props.find("FRONTEND::coherent_feeds") != local_props.end()) {
        /*redhawk::PropertyMap& tuner_alloc = redhawk::PropertyMap::cast(local_props["FRONTEND::tuner_allocation"].asProperties());
        if (tuner_alloc.find("FRONTEND::tuner_allocation::allocation_id") != tuner_alloc.end()) {
            std::string requested_alloc = tuner_alloc["FRONTEND::tuner_allocation::allocation_id"].toString();
            if (not requested_alloc.empty()) {
                if (_delegatedAllocations.find(requested_alloc) == _delegatedAllocations.end()) {
                    allocation_id = requested_alloc;
                } else {
                    allocation_id = "_"+allocation_id;
                    allocation_id = requested_alloc+allocation_id;
                }
                tuner_alloc["FRONTEND::tuner_allocation::allocation_id"] = allocation_id;
            }
        }*/
    }
    if (local_props.find("FRONTEND::tuner_allocation") != local_props.end()) {
        redhawk::PropertyMap& tuner_alloc = redhawk::PropertyMap::cast(local_props["FRONTEND::tuner_allocation"].asProperties());
        if (tuner_alloc.find("FRONTEND::tuner_allocation::allocation_id") != tuner_alloc.end()) {
            std::string requested_alloc = tuner_alloc["FRONTEND::tuner_allocation::allocation_id"].toString();
            if (not requested_alloc.empty()) {
                if (_delegatedAllocations.find(requested_alloc) == _delegatedAllocations.end()) {
                    allocation_id = requested_alloc;
                } else {
                    throw frontend::AllocationAlreadyExists("ALLOCATION_ID ALREADY IN USE", capacities);
                }
                tuner_alloc["FRONTEND::tuner_allocation::allocation_id"] = allocation_id;
            }
        }
    }
    /*if (local_props.find("FRONTEND::tuner_allocation") != local_props.end()) {
        redhawk::PropertyMap& tuner_alloc = redhawk::PropertyMap::cast(local_props["FRONTEND::tuner_allocation"].asProperties());
        if (tuner_alloc.find("FRONTEND::tuner_allocation::tuner_type") != tuner_alloc.end()) {
            std::string requested_device = tuner_alloc["FRONTEND::tuner_allocation::tuner_type"].toString();
            if (not requested_alloc.empty()) {
                if (_delegatedAllocations.find(requested_alloc) == _delegatedAllocations.end()) {
                    allocation_id = requested_alloc;
                } else {
                    allocation_id = "_"+allocation_id;
                    allocation_id = requested_alloc+allocation_id;
                }
                tuner_alloc["FRONTEND::tuner_allocation::allocation_id"] = allocation_id;
            }
        }
    }*/

    // Verify that the device is in a valid state
    if (!isUnlocked() || isDisabled() || isError()) {
        const char* invalidState;
        if (isLocked()) {
            invalidState = "LOCKED";
        } else if (isDisabled()) {
            invalidState = "DISABLED";
        } else if (isError()) {
            invalidState = "ERROR";
        } else {
            invalidState = "SHUTTING_DOWN";
        }
        throw CF::Device::InvalidState(invalidState);
    }
    
    for (std::vector<RDC_ns::RDC_i*>::iterator it=RDCs.begin(); it!=RDCs.end(); it++) {
        result = (*it)->allocate(local_capacities);
        if (result->length() > 0) {
            _delegatedAllocations[allocation_id] = result;
            _usageState = updateUsageState();
            return result._retn();
        }
    }
    
    result = FmRdsSimulator_base::allocate(capacities);

    return result._retn();
}

void FmRdsSimulator_i::deallocate (const char* alloc_id) {
    std::string _alloc_id = ossie::corba::returnString(alloc_id);
    if (_delegatedAllocations.find(_alloc_id) != _delegatedAllocations.end()) {
        for (size_t i=0; i<_delegatedAllocations[_alloc_id]->length(); i++) {
            CF::Device_ptr dev = _delegatedAllocations[_alloc_id][i].device_ref;
            dev->deallocate(alloc_id);
            _usageState = updateUsageState();
            return;
        }
    }
    _usageState = updateUsageState();
    CF::Properties invalidProps;
    throw CF::Device::InvalidCapacity("Capacities do not match allocated ones in the child devices", invalidProps);
}

/***********************************************************************************************

    Basic functionality:

        The service function is called by the serviceThread object (of type ProcessThread).
        This call happens immediately after the previous call if the return value for
        the previous call was NORMAL.
        If the return value for the previous call was NOOP, then the serviceThread waits
        an amount of time defined in the serviceThread's constructor.
        
    SRI:
        To create a StreamSRI object, use the following code:
                std::string stream_id = "testStream";
                BULKIO::StreamSRI sri = bulkio::sri::create(stream_id);

        To create a StreamSRI object based on tuner status structure index 'idx' and collector center frequency of 100:
                std::string stream_id = "my_stream_id";
                BULKIO::StreamSRI sri = this->create(stream_id, this->frontend_tuner_status[idx], 100);

    Time:
        To create a PrecisionUTCTime object, use the following code:
                BULKIO::PrecisionUTCTime tstamp = bulkio::time::utils::now();

        
    Ports:

        Data is passed to the serviceFunction through by reading from input streams
        (BulkIO only). The input stream class is a port-specific class, so each port
        implementing the BulkIO interface will have its own type-specific input stream.
        UDP multicast (dataSDDS and dataVITA49) ports do not support streams.

        The input stream from which to read can be requested with the getCurrentStream()
        method. The optional argument to getCurrentStream() is a floating point number that
        specifies the time to wait in seconds. A zero value is non-blocking. A negative value
        is blocking.  Constants have been defined for these values, bulkio::Const::BLOCKING and
        bulkio::Const::NON_BLOCKING.

        More advanced uses of input streams are possible; refer to the REDHAWK documentation
        for more details.

        Input streams return data blocks that automatically manage the memory for the data
        and include the SRI that was in effect at the time the data was received. It is not
        necessary to delete the block; it will be cleaned up when it goes out of scope.

        To send data using a BulkIO interface, create an output stream and write the
        data to it. When done with the output stream, the close() method sends and end-of-
        stream flag and cleans up.

        NOTE: If you have a BULKIO dataSDDS or dataVITA49  port, you must manually call 
              "port->updateStats()" to update the port statistics when appropriate.

        Example:
            // This example assumes that the device has two ports:
            //  An input (provides) port of type bulkio::InShortPort called dataShort_in
            //  An output (uses) port of type bulkio::OutFloatPort called dataFloat_out
            // The mapping between the port and the class is found
            // in the device base class header file

            bulkio::InShortStream inputStream = dataShort_in->getCurrentStream();
            if (!inputStream) { // No streams are available
                return NOOP;
            }

            // Get the output stream, creating it if it doesn't exist yet
            bulkio::OutFloatStream outputStream = dataFloat_out->getStream(inputStream.streamID());
            if (!outputStream) {
                outputStream = dataFloat_out->createStream(inputStream.sri());
            }

            bulkio::ShortDataBlock block = inputStream.read();
            if (!block) { // No data available
                // Propagate end-of-stream
                if (inputStream.eos()) {
                   outputStream.close();
                }
                return NOOP;
            }

            if (block.sriChanged()) {
                // Update output SRI
                outputStream.sri(block.sri());
            }

            // Get read-only access to the input data
            redhawk::shared_buffer<short> inputData = block.buffer();

            // Acquire a new buffer to hold the output data
            redhawk::buffer<float> outputData(inputData.size());

            // Transform input data into output data
            for (size_t index = 0; index < inputData.size(); ++index) {
                outputData[index] = (float) inputData[index];
            }

            // Write to the output stream; outputData must not be modified after
            // this method call
            outputStream.write(outputData, block.getStartTime());

            return NORMAL;

        If working with complex data (i.e., the "mode" on the SRI is set to
        true), the data block's complex() method will return true. Data blocks
        provide a cxbuffer() method that returns a complex interpretation of the
        buffer without making a copy:

            if (block.complex()) {
                redhawk::shared_buffer<std::complex<short> > inData = block.cxbuffer();
                redhawk::buffer<std::complex<float> > outData(inData.size());
                for (size_t index = 0; index < inData.size(); ++index) {
                    outData[index] = inData[index];
                }
                outputStream.write(outData, block.getStartTime());
            }

        Interactions with non-BULKIO ports are left up to the device developer's discretion
        
    Messages:
    
        To receive a message, you need (1) an input port of type MessageEvent, (2) a message prototype described
        as a structure property of kind message, (3) a callback to service the message, and (4) to register the callback
        with the input port.
        
        Assuming a property of type message is declared called "my_msg", an input port called "msg_input" is declared of
        type MessageEvent, create the following code:
        
        void FmRdsSimulator_i::my_message_callback(const std::string& id, const my_msg_struct &msg){
        }
        
        Register the message callback onto the input port with the following form:
        this->msg_input->registerMessage("my_msg", this, &FmRdsSimulator_i::my_message_callback);
        
        To send a message, you need to (1) create a message structure, (2) a message prototype described
        as a structure property of kind message, and (3) send the message over the port.
        
        Assuming a property of type message is declared called "my_msg", an output port called "msg_output" is declared of
        type MessageEvent, create the following code:
        
        ::my_msg_struct msg_out;
        this->msg_output->sendMessage(msg_out);

    Accessing the Device Manager and Domain Manager:
    
        Both the Device Manager hosting this Device and the Domain Manager hosting
        the Device Manager are available to the Device.
        
        To access the Domain Manager:
            CF::DomainManager_ptr dommgr = this->getDomainManager()->getRef();
        To access the Device Manager:
            CF::DeviceManager_ptr devmgr = this->getDeviceManager()->getRef();
    
    Properties:
        
        Properties are accessed directly as member variables. For example, if the
        property name is "baudRate", it may be accessed within member functions as
        "baudRate". Unnamed properties are given the property id as its name.
        Property types are mapped to the nearest C++ type, (e.g. "string" becomes
        "std::string"). All generated properties are declared in the base class
        (FmRdsSimulator_base).
    
        Simple sequence properties are mapped to "std::vector" of the simple type.
        Struct properties, if used, are mapped to C++ structs defined in the
        generated file "struct_props.h". Field names are taken from the name in
        the properties file; if no name is given, a generated name of the form
        "field_n" is used, where "n" is the ordinal number of the field.
        
        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A boolean called scaleInput
              
            if (scaleInput) {
                dataOut[i] = dataIn[i] * scaleValue;
            } else {
                dataOut[i] = dataIn[i];
            }
            
        Callback methods can be associated with a property so that the methods are
        called each time the property value changes.  This is done by calling 
        addPropertyListener(<property>, this, &FmRdsSimulator_i::<callback method>)
        in the constructor.

        The callback method receives two arguments, the old and new values, and
        should return nothing (void). The arguments can be passed by value,
        receiving a copy (preferred for primitive types), or by const reference
        (preferred for strings, structs and vectors).

        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A struct property called status
            
        //Add to FmRdsSimulator.cpp
        FmRdsSimulator_i::FmRdsSimulator_i(const char *uuid, const char *label) :
            FmRdsSimulator_base(uuid, label)
        {
            addPropertyListener(scaleValue, this, &FmRdsSimulator_i::scaleChanged);
            addPropertyListener(status, this, &FmRdsSimulator_i::statusChanged);
        }

        void FmRdsSimulator_i::scaleChanged(float oldValue, float newValue)
        {
            RH_DEBUG(this->_baseLog, "scaleValue changed from" << oldValue << " to " << newValue);
        }
            
        void FmRdsSimulator_i::statusChanged(const status_struct& oldValue, const status_struct& newValue)
        {
            RH_DEBUG(this->_baseLog, "status changed");
        }
            
        //Add to FmRdsSimulator.h
        void scaleChanged(float oldValue, float newValue);
        void statusChanged(const status_struct& oldValue, const status_struct& newValue);

    Logging:

        The member _baseLog is a logger whose base name is the component (or device) instance name.
        New logs should be created based on this logger name.

        To create a new logger,
            rh_logger::LoggerPtr my_logger = this->_baseLog->getChildLogger("foo");

        Assuming component instance name abc_1, my_logger will then be created with the 
        name "abc_1.user.foo".

    Allocation:
    
        Allocation callbacks are available to customize the Device's response to 
        allocation requests. For example, if the Device contains the allocation 
        property "my_alloc" of type string, the allocation and deallocation
        callbacks follow the pattern (with arbitrary function names
        my_alloc_fn and my_dealloc_fn):
        
        bool FmRdsSimulator_i::my_alloc_fn(const std::string &value)
        {
            // perform logic
            return true; // successful allocation
        }
        void FmRdsSimulator_i::my_dealloc_fn(const std::string &value)
        {
            // perform logic
        }
        
        The allocation and deallocation functions are then registered with the Device
        base class with the setAllocationImpl call. Note that the variable for the property is used rather
        than its id:
        
        this->setAllocationImpl(my_alloc, this, &FmRdsSimulator_i::my_alloc_fn, &FmRdsSimulator_i::my_dealloc_fn);
        
        

************************************************************************************************/
int FmRdsSimulator_i::serviceFunction()
{
    RH_TRACE(this->_baseLog, "serviceFunction() example log message");
    
    return NOOP;
}

/*************************************************************
Functions supporting tuning allocation
*************************************************************/
void FmRdsSimulator_i::deviceEnable(frontend_tuner_status_struct_struct &fts, size_t tuner_id){
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to set the 'enabled' member of fts to indicate that tuner as enabled
    ************************************************************/
    fts.enabled = true;
    return;
}
void FmRdsSimulator_i::deviceDisable(frontend_tuner_status_struct_struct &fts, size_t tuner_id){
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to reset the 'enabled' member of fts to indicate that tuner as disabled
    ************************************************************/
    fts.enabled = false;
    return;
}
bool FmRdsSimulator_i::deviceSetTuning(const frontend::frontend_tuner_allocation_struct &request, frontend_tuner_status_struct_struct &fts, size_t tuner_id){
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
      At a minimum, bandwidth, center frequency, and sample_rate have to be set
      If the device is tuned to exactly what the request was, the code should be:
        fts.bandwidth = request.bandwidth;
        fts.center_frequency = request.center_frequency;
        fts.sample_rate = request.sample_rate;

    return true if the tuning succeeded, and false if it failed
    ************************************************************/
    return true;
}
bool FmRdsSimulator_i::deviceDeleteTuning(frontend_tuner_status_struct_struct &fts, size_t tuner_id) {
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    return true if the tune deletion succeeded, and false if it failed
    ************************************************************/
    return true;
}
/*************************************************************
Functions servicing the tuner control port
*************************************************************/
std::string FmRdsSimulator_i::getTunerType(const std::string& allocation_id) {
    return frontend_tuner_status[0].tuner_type;
}

bool FmRdsSimulator_i::getTunerDeviceControl(const std::string& allocation_id) {
    return true;
}

std::string FmRdsSimulator_i::getTunerGroupId(const std::string& allocation_id) {
    return frontend_tuner_status[0].group_id;
}

std::string FmRdsSimulator_i::getTunerRfFlowId(const std::string& allocation_id) {
    return frontend_tuner_status[0].rf_flow_id;
}

void FmRdsSimulator_i::setTunerCenterFrequency(const std::string& allocation_id, double freq) {
    if (freq<0) throw FRONTEND::BadParameterException("Center frequency cannot be less than 0");
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[0].center_frequency = freq;
}

double FmRdsSimulator_i::getTunerCenterFrequency(const std::string& allocation_id) {
    return frontend_tuner_status[0].center_frequency;
}

void FmRdsSimulator_i::setTunerBandwidth(const std::string& allocation_id, double bw) {
    if (bw<0) throw FRONTEND::BadParameterException("Bandwidth cannot be less than 0");
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[0].bandwidth = bw;
}

double FmRdsSimulator_i::getTunerBandwidth(const std::string& allocation_id) {
    return frontend_tuner_status[0].bandwidth;
}

void FmRdsSimulator_i::setTunerAgcEnable(const std::string& allocation_id, bool enable)
{
    throw FRONTEND::NotSupportedException("setTunerAgcEnable not supported");
}

bool FmRdsSimulator_i::getTunerAgcEnable(const std::string& allocation_id)
{
    throw FRONTEND::NotSupportedException("getTunerAgcEnable not supported");
}

void FmRdsSimulator_i::setTunerGain(const std::string& allocation_id, float gain)
{
    throw FRONTEND::NotSupportedException("setTunerGain not supported");
}

float FmRdsSimulator_i::getTunerGain(const std::string& allocation_id)
{
    throw FRONTEND::NotSupportedException("getTunerGain not supported");
}

void FmRdsSimulator_i::setTunerReferenceSource(const std::string& allocation_id, long source)
{
    throw FRONTEND::NotSupportedException("setTunerReferenceSource not supported");
}

long FmRdsSimulator_i::getTunerReferenceSource(const std::string& allocation_id)
{
    throw FRONTEND::NotSupportedException("getTunerReferenceSource not supported");
}

void FmRdsSimulator_i::setTunerEnable(const std::string& allocation_id, bool enable) {
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[0].enabled = enable;
}

bool FmRdsSimulator_i::getTunerEnable(const std::string& allocation_id) {
    return frontend_tuner_status[0].enabled;
}

void FmRdsSimulator_i::setTunerOutputSampleRate(const std::string& allocation_id, double sr) {
    if (sr<0) throw FRONTEND::BadParameterException("Sample rate cannot be less than 0");
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[0].sample_rate = sr;
}

double FmRdsSimulator_i::getTunerOutputSampleRate(const std::string& allocation_id){
    return frontend_tuner_status[0].sample_rate;
}

void FmRdsSimulator_i::configureTuner(const std::string& allocation_id, const CF::Properties& tunerSettings){
    // set the appropriate tuner settings
}

CF::Properties* FmRdsSimulator_i::getTunerSettings(const std::string& allocation_id){
    // return the tuner settings
    redhawk::PropertyMap* tuner_settings = new redhawk::PropertyMap();
    return tuner_settings;
}

/*************************************************************
Functions servicing the RFInfo port(s)
- port_name is the port over which the call was received
*************************************************************/
std::string FmRdsSimulator_i::get_rf_flow_id(const std::string& port_name)
{
    return std::string("none");
}

void FmRdsSimulator_i::set_rf_flow_id(const std::string& port_name, const std::string& id)
{
}

frontend::RFInfoPkt FmRdsSimulator_i::get_rfinfo_pkt(const std::string& port_name)
{
    frontend::RFInfoPkt pkt;
    return pkt;
}

void FmRdsSimulator_i::set_rfinfo_pkt(const std::string& port_name, const frontend::RFInfoPkt &pkt)
{
}

