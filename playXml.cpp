#include <iostream>
#include <vector>
#include <set>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/date_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::gregorian;
using namespace boost::property_tree;

int main() {

    //create an empty property tree object
    ptree pt;

    //parse the xml file into the property tree
    read_xml("../sample.xml", pt);

    //iterate through processes
    for (const auto &i : pt.get_child("AMPSConfig.AMPSProcessTable")) {
        std::string name;
        ptree sub_pt;
        std::tie(name, sub_pt) = i;
        std::cout << "Process" << std::endl;
        std::cout << "\tName is " << sub_pt.get<std::string>("<xmlattr>.name") << std::endl;
        std::cout << "\tDescription is " << sub_pt.get<std::string>("<xmlattr>.descr") << std::endl;
        std::cout << "\tAllowMultiple is " << sub_pt.get<std::string>("<xmlattr>.allowMultiple") << std::endl;
    }

    using namespace boost::algorithm;

    //iterate through routing paths
    for (const auto &i : pt.get_child("AMPSConfig.AMPSRoutingTable")) {
        std::string name;
        ptree sub_pt;
        std::tie(name, sub_pt) = i;
        std::cout << "Message" << std::endl;
        std::cout << "\tTopic is " << sub_pt.get<std::string>("<xmlattr>.topic") << std::endl;
        std::string path = sub_pt.get<std::string>("<xmlattr>.path");
        std::cout << "\tPath is " << path << std::endl;

        std::vector<std::string> tokens;
        split(tokens, path, is_any_of("-"), token_compress_on);
        std::cout << "\tNumber of processes is " << tokens.size() << std::endl;
    }
}