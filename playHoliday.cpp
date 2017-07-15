#include <iostream>
#include <vector>
#include <set>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/date_time.hpp>

using namespace boost::gregorian;

void print_date(boost::gregorian::date d) {
    std::cout << "\t" << d << std::endl;
}

int main() {

    //setting the output date format
    date_facet *facet(new date_facet("%A %B %d, %Y"));
    std::cout.imbue(std::locale(std::cout.getloc(), facet));

    //define a shorthand for the nth_day_of_the_week_in_month function object
    typedef nth_day_of_the_week_in_month nth_dow;

    //define a collection of holidays fixed by month and day
    std::vector<year_based_generator *> holidays;
    holidays.push_back(new partial_date(1, Jan));       //Western New Year
    holidays.push_back(new partial_date(14, Feb));      //Valentine's Day
    holidays.push_back(new partial_date(4, Jul));       //US Independence Day
    holidays.push_back(new partial_date(25, Dec));      //Christmas day

    //define holidays based on relative day of month
    holidays.push_back(new nth_dow(nth_dow::first, Monday, Sep));       //Labor Day
    holidays.push_back(new nth_dow(nth_dow::third, Monday, Jan));       //MLK Day
    holidays.push_back(new nth_dow(nth_dow::second, Tuesday, Feb));     //President's Day
    holidays.push_back(new nth_dow(nth_dow::fourth, Thursday, Nov));    //Thanksgiving

    //define birthdays of interest
    holidays.push_back (new partial_date(22, Aug));     //Steve
    holidays.push_back (new partial_date(19, Sep));     //Jeannie
    holidays.push_back (new partial_date(25, Aug));     //Ezra
    holidays.push_back (new partial_date(19, Mar));     //Maxine

    typedef std::set<date> date_set;
    date_set all_holidays;

    ushort theYear = 2017;
    for (std::vector<year_based_generator *>::iterator it = holidays.begin();
         it != holidays.end(); ++it) {
        all_holidays.insert((*it)->get_date(theYear));
    }

    //print the holidays to the screen
    std::cout << "Number Holidays in " << theYear << ": " << all_holidays.size() << std::endl;
    std::for_each(all_holidays.begin(), all_holidays.end(), print_date);
    return 0;
}