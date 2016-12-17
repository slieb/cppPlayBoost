#include <iostream>
#include <vector>
#include <set>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/date_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::property_tree;

int main() {
    //create an empty property tree object
    ptree ptJson;

    //parse the xml file into the property tree
    read_json("../sample.json", ptJson);

    //read values
    std::string target = ptJson.get<std::string>("header.target");
    int tradeQty = ptJson.get<int>("body.qty");
    std::string symbol = ptJson.get<std::string>("body.symbol");

    std::cout << "Target is " << target << ", qty is " << tradeQty << ", and symbol is " << symbol << std::endl;

    //add a value
    ptJson.put("body.orca-p1", "P1 did its thing");

    //change a value
    ptJson.put("header.target", "P2");

    target = ptJson.get<std::string>("header.target");
    std::string orca_p1 = ptJson.get<std::string>("body.orca-p1");

    std::cout << "New target is " << target << " and orca-p1 is " << orca_p1 << std::endl;

    ptree ptConfig;
    std::string currentRoute = "P2";
    std::string nextRoute;
    bool foundCurrent = 0;
    read_json("../sampleConfig.json", ptConfig);
    BOOST_FOREACH(ptree::value_type & v, ptConfig.get_child("orcaConfig.PM_ORDER.")) {
                    std::cout << v.second.data() << std::endl;
                    if (foundCurrent == 1) {
                        nextRoute = v.second.data();
                        std::cout << "Setting next route to " << nextRoute << std::endl;
                        break;
                    }
                    if (v.second.data() == currentRoute)
                        foundCurrent = 1;
                }

    std::string msgType;
    BOOST_FOREACH (ptree::value_type & v, ptConfig.get_child("orcaConfig")) {
                    msgType = v.first.data();
                    std::cout << "Message Type is " << msgType << std::endl;
                    BOOST_FOREACH (ptree::value_type & x, ptConfig.get_child("orcaConfig." + msgType)) {
                                    std::cout << "\tPath entry is " << x.second.data() << std::endl;
                                }
                }
    return 0;
}