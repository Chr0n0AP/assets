/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This file is part of REDHAWK Basic Components autocorrelate.
 *
 * REDHAWK Basic Components autocorrelate is free software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * REDHAWK Basic Components autocorrelate is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 */
#ifndef AUTOCORRELATE_IMPL_BASE_H
#define AUTOCORRELATE_IMPL_BASE_H

#include <boost/thread.hpp>
#include <ossie/Resource_impl.h>
#include <ossie/ThreadedComponent.h>

#include <bulkio/bulkio.h>

class autocorrelate_base : public Resource_impl, protected ThreadedComponent
{
    public:
        autocorrelate_base(const char *uuid, const char *label);
        ~autocorrelate_base();

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
         * @throw CF::LIfeCycle::ReleaseError
         * @throw CORBA::SystemException
         */
        void releaseObject();

        void loadProperties();

    protected:
        // Member variables exposed as properties
        CORBA::ULong correlationSize;
        CORBA::Long inputOverlap;
        CORBA::ULong numAverages;
        std::string outputType;
        bool zeroMean;
        bool zeroCenter;

        // Ports
        bulkio::InFloatPort *dataFloat_in;
        bulkio::OutFloatPort *dataFloat_out;

    private:
};
#endif // AUTOCORRELATE_IMPL_BASE_H
