#ifndef FMRDSSIMULATOR_BASE_IMPL_BASE_H
#define FMRDSSIMULATOR_BASE_IMPL_BASE_H

#include <boost/thread.hpp>
#include <frontend/frontend.h>
#include <CF/AggregateDevices.h>
#include <ossie/AggregateDevice_impl.h>
#include <ossie/ThreadedComponent.h>
#include <ossie/DynamicComponent.h>

#include <frontend/frontend.h>
#include "port_impl.h"
#include <bulkio/bulkio.h>
#include "struct_props.h"
#include "RDC/RDC.h"

#define BOOL_VALUE_HERE 0

class FmRdsSimulator_base : public frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>, public virtual POA_CF::AggregatePlainDevice, public AggregateDevice_impl, public virtual frontend::digital_tuner_delegation, public virtual frontend::rfinfo_delegation, protected ThreadedComponent, public virtual DynamicComponent
{
    friend class CF_DeviceStatus_Out_i;

    public:
        FmRdsSimulator_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl);
        FmRdsSimulator_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev);
        FmRdsSimulator_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities);
        FmRdsSimulator_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev);
        ~FmRdsSimulator_base();

        /**
         * @throw CF::Resource::StartError
         * @throw CORBA::SystemException
         */
        void start();

        /**
         * @throw CF::Resource::StopError
         * @throw CORBA::SystemException
         */
        void stop();

        /**
         * @throw CF::LifeCycle::ReleaseError
         * @throw CORBA::SystemException
         */
        void releaseObject();

        void loadProperties();
        void removeAllocationIdRouting(const size_t tuner_id);

        virtual CF::Properties* getTunerStatus(const std::string& allocation_id);
        virtual void assignListener(const std::string& listen_alloc_id, const std::string& allocation_id);
        virtual void removeListener(const std::string& listen_alloc_id);
        void frontendTunerStatusChanged(const std::vector<frontend_tuner_status_struct_struct>* oldValue, const std::vector<frontend_tuner_status_struct_struct>* newValue);

    protected:

        // Ports
        /// Port: RFInfo_in
        frontend::InRFInfoPort *RFInfo_in;
        /// Port: DigitalTuner_in
        frontend::InDigitalTunerPort *DigitalTuner_in;
        /// Port: DeviceStatus_out
        CF_DeviceStatus_Out_i *DeviceStatus_out;
        /// Port: dataFloat_out
        bulkio::OutFloatPort *dataFloat_out;

        std::map<std::string, std::string> listeners;

    private:
        void construct();
};
#endif // FMRDSSIMULATOR_BASE_IMPL_BASE_H
