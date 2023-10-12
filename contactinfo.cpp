#include "ContactInfo.hpp"


// ----- constructors -------------------------------------------------------

ContactInfo::ContactInfo(const std::string& sName,
        const std::string& sStreet, const std::string& sCity,
        const std::string& sState, const std::string& sZip)
    {
    setName(sName);
    setStreet(sStreet);
    setCity(sCity);
    setState(sState);
    setZip(sZip);
    }

ContactInfo::ContactInfo(const ContactInfo& that)
    {
    setName(that.getName());
    setStreet(that.getStreet());
    setCity(that.getCity());
    setState(that.getState());
    setZip(that.getZip());
    }

ContactInfo::ContactInfo()
    {
    }

// ----- accessors ----------------------------------------------------------

std::string ContactInfo::getName() const
    {
    return m_sName;
    }

void ContactInfo::setName(const std::string& sName)
    {
    m_sName = sName;
    }

std::string ContactInfo::getStreet() const
    {
    return m_sStreet;
    }

void ContactInfo::setStreet(const std::string& sStreet)
    {
    m_sStreet = sStreet;
    }

std::string ContactInfo::getCity() const
    {
    return m_sCity;
    }

void ContactInfo::setCity(const std::string& sCity)
    {
    m_sCity = sCity;
    }

std::string ContactInfo::getState() const
    {
    return m_sState;
    }

void ContactInfo::setState(const std::string& sState)
    {
    m_sState = sState;
    }

std::string ContactInfo::getZip() const
    {
    return m_sZip;
    }

void ContactInfo::setZip(const std::string& sZip)
    {
    m_sZip = sZip;
    }

// ----- free functions -----------------------------------------------------

std::ostream& operator<<(std::ostream& out, const ContactInfo& info)
    {
    out << "ContactInfo("
            << "Name="     << info.getName()
            << ", Street=" << info.getStreet()
            << ", City="   << info.getCity()
            << ", State="  << info.getState()
            << ", Zip="    << info.getZip()
            << ')';
    return out;
    }

bool operator==(const ContactInfo& infoA, const ContactInfo& infoB)
    {
    return infoA.getName()   == infoB.getName()   &&
           infoA.getStreet() == infoB.getStreet() &&
           infoA.getCity()   == infoB.getCity()   &&
           infoA.getState()  == infoB.getState()  &&
           infoA.getZip()    == infoB.getZip();
    }
 
size_t hash_value(const ContactInfo& info)
    {
    return size_t(&info); // identity hash (note: not suitable for cache keys)
    }
ContactInfoSerializer.cpp
Example A-5 C++ Code for the PortableContactInfo Application

/**
* This file defines serializers for the ContactInfo class and registers its
* managed form (Managed<ContactInfo>) with Coherence.
*/

#include "coherence/lang.ns"

#include "coherence/io/pof/PofWriter.hpp"
#include "coherence/io/pof/PofReader.hpp"
#include "coherence/io/pof/SystemPofContext.hpp"
#include "ContactInfo.hpp"

#include <string>

using namespace coherence::lang;

using coherence::io::pof::PofReader;
using coherence::io::pof::PofWriter;

COH_REGISTER_MANAGED_CLASS(1001, ContactInfo);
 
template<> void serialize<ContactInfo>(PofWriter::Handle hOut, const ContactInfo& info)
    {
    hOut->writeString(0, info.getName());
    hOut->writeString(1, info.getStreet());
    hOut->writeString(2, info.getCity());
    hOut->writeString(3, info.getState());
    hOut->writeString(4, info.getZip());
    }

template<> ContactInfo deserialize<ContactInfo>(PofReader::Handle hIn)
   {
    std::string sName   = hIn->readString(0);
    std::string sStreet = hIn->readString(1);
    std::string sCity   = hIn->readString(2);
    std::string sState  = hIn->readString(3);
    std::string sZip    = hIn->readString(4);
    return ContactInfo(sName, sStreet, sCity, sState, sZip);
    }
contacts.cpp
Example A-6 Code for the ContactInfo Data Object

#include "coherence/lang.ns"

#include "coherence/net/CacheFactory.hpp"
#include "coherence/net/NamedCache.hpp"

#include "ContactInfo.hpp"

#include <iostream>
#include <sstream>

using namespace coherence::lang;

using coherence::net::CacheFactory;
using coherence::net::NamedCache;

// ----- function prototypes ------------------------------------------------

/**
* Create a contact from stdin.
*
* @return the contact
*/
ContactInfo readContact();

/**
* This Coherence for C++ example illustrates how to use non-Coherence data
* objects in the grid. This example operates on the ContactInfo class which
* is not Coherence aware.
*
* To run this against a remote cache, the proxy node must have the
* corresponding Java ContactInfo.class in its classpath.
*
* @argc  the number of command line arguments (including the process name)
* @argv  [cache-name]
*/
int main(int argc, char** argv)
    {
    try
        {
        String::View       vsCacheName = argc > 1 ? argv[1] : "dist-contacts";
        NamedCache::Handle hCache      = CacheFactory::getCache(vsCacheName);

        while (true)
            {
            // prompt for input
            std::cout << "contacts> " << std::flush;

            char achInput[256];
            std::cin.getline(achInput, 256);
            if (std::cin.fail())
                {
                std::cin.clear();
                continue;
                }

            std::stringstream ssInput(achInput);

            // process input
            std::string sCmd;
            ssInput >> sCmd;

            if (sCmd == "")
                {
                continue;
                }
            else if (sCmd == "bye")
                {
                // quit
                CacheFactory::shutdown();
                return 0;
                }
            else if (sCmd == "create")
                {
                ContactInfo ci = readContact();
                std::cout << "storing: " << ci << std::endl;
                hCache->put(String::create(ci.getName().c_str()),
                        Managed<ContactInfo>::create(ci));
                }
            else if (sCmd == "find")
                {
                std::string sPart;
                ssInput >> sPart;

                std::cout << "Name: " << std::flush;
                std::cin.getline(achInput, 256);
                String::View vsName = achInput;

               Managed<ContactInfo>::View vInfo =
                    cast<Managed<ContactInfo>::View>(hCache->get(vsName));

                if (NULL == vInfo)
                    {
                    std::cout << vsName << " not found" << std::endl;
                    continue;
                    }

                if (sPart == "" || sPart == "all")
                    {
                    std::cout << vInfo << std::endl;
                    }
                else if (sPart == "street")
                    {
                    std::cout << vInfo->getStreet() << std::endl;
                    }
                else if (sPart == "city")
                    {
                    std::cout << vInfo->getCity() << std::endl;
                    }
                else if (sPart == "state")
                    {
                    std::cout << vInfo->getState() << std::endl;
                    }
                else if (sPart == "zip")
                    {
                    std::cout << vInfo->getZip() << std::endl;
                    }
                else
                    {
                    std::cerr << "find must be followed by, street, city, "
                        << "state, or zip" << std::endl;
                    }
                }
            else // output help
                {
                std::cout << "commands are:"
                    << std::endl << "bye"
                    << std::endl << "create"
                    << std::endl << "find <street | city | state | zip | all>"
                    << std::endl;
                }
            }
        }
    catch (const std::exception& e)
        {
        std::cerr << e.what() << std::endl;
        }
    }

ContactInfo readContact()
    {
    char achInput[256];
    std::cout << "Name: " << std::flush;
    std::cin.getline(achInput, 256);
    std::string sName(achInput);

    std::cout << "Street: " << std::flush;
    std::cin.getline(achInput, 256);
    std::string sStreet(achInput);

    std::cout << "City: " << std::flush;
    std::cin.getline(achInput, 256);
    std::string sCity(achInput);

    std::cout << "State: " << std::flush;
    std::cin.getline(achInput, 256);
    std::string sState(achInput);

    std::cout << "Zip: " << std::flush;
    std::cin.getline(achInput, 256);
    std::string sZip(achInput);

    return ContactInfo(sName, sStreet, sCity, sState, sZip);
    }
