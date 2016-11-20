#include "boost/date_time/gregorian/gregorian.hpp"
#include <set>

void print_date(boost::gregorian::date d) {
    using namespace boost::gregorian;
    std::cout << "\t" << d << std::endl;
}

int main() {
    using namespace boost::gregorian;

    //setting the output date format
    date_facet* facet(new date_facet("%A %B %d, %Y"));
    std::cout.imbue(std::locale(std::cout.getloc(),facet));

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

    return 0;
}