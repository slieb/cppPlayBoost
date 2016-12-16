#include <iostream>
#include <vector>
#include <set>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/date_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::gregorian;
using namespace boost::property_tree;

void print_date(boost::gregorian::date d) {
    std::cout << "\t" << d << std::endl;
}

int main() {

    //
    //Explore some date logic with a holiday calendar
    //

    boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();

    //setting the output date format
    date_facet *facet(new date_facet("%A %B %d, %Y"));
    std::cout.imbue(std::locale(std::cout.getloc(), facet));

    //define a collection of holidays fixed by month and day
    std::vector<year_based_generator *> holidays;
    holidays.push_back(new partial_date(1, Jan)); //Western New Year
    holidays.push_back(new partial_date(4, Jul)); //US Independence Day
    holidays.push_back(new partial_date(25, Dec));//Christmas day

    //define a shorthand for the nth_day_of_the_week_in_month function object
    typedef nth_day_of_the_week_in_month nth_dow;

    //US Labor Day
    holidays.push_back(new nth_dow(nth_dow::first, Monday, Sep));
    //MLK Day
    holidays.push_back(new nth_dow(nth_dow::third, Monday, Jan));
    //Pres day
    holidays.push_back(new nth_dow(nth_dow::second, Tuesday, Feb));
    //Thanksgiving
    holidays.push_back(new nth_dow(nth_dow::fourth, Thursday, Nov));

    typedef std::set<date> date_set;
    date_set all_holidays;

    ushort theYear = 2016;
    for (std::vector<year_based_generator *>::iterator it = holidays.begin();
         it != holidays.end(); ++it) {
        all_holidays.insert((*it)->get_date(theYear));
    }

    //print the holidays to the screen
    std::cout << "Number Holidays in " << theYear << ": " << all_holidays.size() << std::endl;
    std::for_each(all_holidays.begin(), all_holidays.end(), print_date);

    std::cout << "----------" << std::endl;

    //
    // Here we're playing around with BOOST's ability to parse XML files
    //

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
    std::cout << "----------" << std::endl;

    //
    // Here we're playing around with BOOST's ability to parse JSON files
    //

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
    read_json("../orcaConfig.json", ptConfig);
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

    boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration tDiff = t2 - t1;
    std::cout << "This ran in " << tDiff.total_microseconds() << " microseconds" << std::endl;
    std::cout << "This ran in " << tDiff.total_milliseconds() << " milliseconds" << std::endl;

    boost::posix_time::ptime b2 = boost::posix_time::microsec_clock::local_time();
    tDiff = b2 - t2;
    std::cout << "STDOUT cost us " << tDiff.total_microseconds() << " microseconds" << std::endl;

    return 0;
}