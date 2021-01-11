/*
    Intention Repeater MAX v3.31 created by Thomas Sweet.
    Performance benchmark, enhancement and flags by Karteek Sheri.
    Small enhancements (for compilation ok in VS2019) by Mcode aka Ninja aka Tow3rbust3r (01/11/2021).
    Created 11/14/2020 for C++.
    Directions to compile on Windows: https://github.com/tsweet77/repeater-max/blob/main/Win_MAX_Compile_Directions.txt
    To compile on Linux (Requires 64-bit Compiler): g++ ./intention_repeater_max.cpp -O3 -o ./intention_repeater_max
    Repeats your intention up to 100 PHz to make things happen.
    For help: intention_repeater_max.exe --help
    Intention Repeater MAX is powered by a Servitor (20 Years / 2000+ hours in the making) [HR 6819 Black Hole System].
    Servitor Info: https://enlightenedstates.com/2017/04/07/servitor-just-powerful-spiritual-tool/
    Website: https://www.intentionrepeater.com/
    Forum: https://forums.intentionrepeater.com/
    Licensed under GNU General Public License v3.0
    This means you can modify, redistribute and even sell your own modified software, as long as it's open source too and released under this same license.
    https://choosealicense.com/licenses/gpl-3.0/
*/

#include <stdio.h>

#include <string>

#include <string.h>

#include <math.h>

#include <cmath>

#include <iostream>

#include <time.h>

#include <ctime>

#include <ratio>

#include <chrono>

#include <iomanip>

#include <locale.h>

#include <cstdint>

#include <vector>

#include <iterator>

#include <sstream>

#include <algorithm>

using namespace std;
using namespace std::chrono;

#define ONE_MINUTE 60
#define ONE_HOUR 3600

//#define SCALED //uncomment this if you want to use suffix or suffix_hz in your code

std::string PROCESS_STATEMENT = " REGULATE/INTEGRATE/OM";

class comma_numpunct : public std::numpunct < char > {
protected: virtual char do_thousands_sep() const {
    return ',';
}

         virtual std::string do_grouping() const {
             return "\03";
         }
};

std::string display_suffix(std::string num, int power, std::string designator)
{
    std::string s;
    if (designator == "Iterations")
    {
        char iterations_suffix_array[] = { ' ', 'k', 'M', 'B', 'T', 'q', 'Q', 's', 'S' };
        //cout << "Power: " << power << endl;
        s = iterations_suffix_array[int(power / 3)];
    }
    else //designator == "Frequency"
    {
        char frequency_suffix_array[] = { ' ', 'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y' };
        //cout << "Power: " << power << endl;
        s = frequency_suffix_array[int(power / 3)];
    }

    std::string str2 = num.substr(0, power % 3 + 1) + "." + num.substr(power % 3 + 1, 3) + s;

    /*
    cout << endl;
    cout << "Num: " << num << endl;
    cout << "Power: " << power << endl;
    cout << "str2: " << str2 << endl;
    */

    //cout << str2 << endl;
    return str2;
}

static const char* short_scale[] = {
    "",
    "k",
    "M",
    "B",
    "T",
    "q",
    "Q",
    "s",
    "S"
};

static const char* short_scale_hz[] = {
    "",
    "k",
    "M",
    "G",
    "T",
    "P",
    "E",
    "Z",
    "Y"
};

const char* scale(unsigned long long int n, int decimals = 1, const char* units[] = short_scale)
{
    /*
     * Number of digits in n is given by
     * 10^x = n ==> x = log(n)/log(10) = log_10(n).
     *
     * So 1000 would be 1 + floor(log_10(10^3)) = 4 digits.
     */
    int digits = n == 0 ? 0 : 1 + floor(log10l(fabs(n)));

    // determine base 10 exponential
    int exp = digits <= 4 ? 0 : 3 * ((digits - 1) / 3);

    // normalized number
    double m = n / powl(10, exp);

    // no decimals? then don't print any
    if (m - static_cast<long>(n) == 0)
        decimals = 0;

    // don't print unit for exp<3
    static char s[64];
    static const char* fmt[] = { "%1.*lf%s", "%1.*lf" };
    sprintf_s(s, fmt[exp < 3], decimals, m, units[exp / 3]);
    return s;
}

const char* suffix(unsigned long long int n, int decimals = 1)
{
    static char s[64];
    strcpy_s(s, scale(n, decimals, short_scale));
    return s;
}

const char* suffix_hz(unsigned long long int n, int decimals = 1)
{
    static char s[64];
    strcpy_s(s, scale(n, decimals, short_scale_hz));

    return s;
}

std::string FormatTimeRun(int seconds_elapsed) {
    int hour, min, sec;

    std::string hour_formatted, min_formatted, sec_formatted;

    hour = seconds_elapsed / ONE_HOUR;
    seconds_elapsed -= hour * ONE_HOUR;
    min = seconds_elapsed / ONE_MINUTE;
    seconds_elapsed -= min * ONE_MINUTE;
    sec = seconds_elapsed;

    if (hour < 10) {
        hour_formatted = "0" + std::to_string(hour);
    }
    else {
        hour_formatted = std::to_string(hour);
    }

    if (min < 10) {
        min_formatted = "0" + std::to_string(min);
    }
    else {
        min_formatted = std::to_string(min);
    }

    if (sec < 10) {
        sec_formatted = "0" + std::to_string(sec);
    }
    else {
        sec_formatted = std::to_string(sec);
    }

    return hour_formatted + ":" + min_formatted + ":" + sec_formatted;
}

void print_help() {
    cout << "Intention Repeater MAX v3.31 (c)2020 Thomas Sweet aka Anthro Teacher And Mcode/Ninja/Tow3rbust3r." << endl;
    cout << "Performance benchmark, exponents and flags by Karteek Sheri." << endl;
    cout << "Intention multiplying by Thomas Sweet." << endl << endl;

    cout << "Optional Flags:" << endl;
    cout << "	a) --dur or -d" << endl;
    cout << "	b) --imem or -m" << endl;
    cout << "	c) --intent or -i" << endl;
    cout << "	d) --suffix or -s" << endl;
    cout << "	e) --help" << endl << endl;

    cout << "--dur = Duration in HH:MM:SS format. Example \"00:01:00\" to run for one minute. Default = \"Until Stopped\"." << endl;
    cout << "--imem = Specify how many GB of System RAM to use. Default = 1.0. Higher amount produces a faster repeat rate, but takes longer to load into memory." << endl;
    cout << "--intent = Intention. Default = Prompt the user for intention." << endl;
    cout << "--suffix = Specify Hz or Exp. Exp = Exponent (ex. 1.313x10^15). Hz (ex. 1.313PHz). Default = Hz" << endl;
    cout << "--help = Display this help." << endl << endl;

    cout << "Example automated usage: intention_repeater_max.exe --suffix Hz --dur \"00:01:00\" --imem 4.0 --intent \"I am calm.\"" << endl;
    cout << "Default usage: intention_repeater_max.exe" << endl << endl;

    cout << "gitHub Repository: https://github.com/tsweet77/repeater-max" << endl;
    cout << "Forum: https://forums.intentionrepeater.com" << endl;
    cout << "Website: https://www.intentionrepeater.com" << endl;
}

// Utility function to find the sum of two numbers represented as a string in CPP

std::string findsum(std::string a, std::string b) {

    std::vector<int>finalsum; // Stores the final sum of two number

    int carry = 0;  // Stores carry at each stage of calculation

    /* Step 1 starts here */

    int i = a.size() - 1, j = b.size() - 1; // Start adding from lowest significant bit
    while ((i >= 0) && (j >= 0)) {     // Loop until either of number exhausts first
        int x = (a[i] - '0') + (b[j] - '0') + carry; // Calculate the sum of digit in final sum by adding respective digits with previous carry.
        finalsum.push_back(x % 10);  // Store the respective digit of the final sum in a vector.
        carry = x / 10;                // update the carry. The carry for next step is the remaining number after forming the digit of final sum.
        i--;                        // Move one step towards the left in both the string(numbers)
        j--;
    }
    /*  Step 2 starts here */

    while (i >= 0) {          // If the number 1 was greater than number 2, then there must be some digits to be taken care off.
        int x = (a[i] - '0') + carry;  // Add the remaining digits to the carry one by one and store the unit digit.
        finalsum.push_back(x % 10);
        carry = x / 10;            // update the carry from each step.
        i--;
    }
    /* Step 3 starts here */

    while (j >= 0) {       // If the number 2 was greater than number 1, then there must be some digits to be taken care off.
        int x = (b[j] - '0') + carry; // Add the remaining digits to the carry one by one and store the unit digit.
        finalsum.push_back(x % 10);
        carry = x / 10;         // update the carry from each step.
        j--;
    }
    /* Step 4 starts here */

    while (carry) {   //If after finishing addition of the two numbers, if there is still carry/leftover then we need to take it into the final sum.
        finalsum.push_back(carry % 10); // Store digit one by one.
        carry = carry / 10;       // Reduce carry
    }
    /* Step 5 starts here */
    std::stringstream final_iter;
    // Since vector pushes value at last, the most significant digits starts at the end of the vector. Thus print reverse.

    std::copy(finalsum.rbegin(), finalsum.rend(), std::ostream_iterator<int>(final_iter, ""));

    return final_iter.str();
}

int main(int argc, char** argv) {

    std::string intention, process_intention, intention_value, duration, param_duration, param_intention, runtime_formatted, ref_rate, suffix_value = "HZ";
    unsigned long long int iterations = 0, cpu_benchmark_count = 0;
    int seconds = 0; unsigned long long multiplier = 0; float ram_size_value = 1;

    ///parse command line arguments
    param_duration = "Until Stopped"; param_intention = "";

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            print_help();
            exit(0);
        }
        else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dur")) {

            param_duration = argv[i + 1];

        }
        else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--imem")) {

            ram_size_value = atof(argv[i + 1]);

        }
        else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--rate")) {

            ref_rate = argv[i + 1];

        }
        else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--intent")) {

            param_intention = argv[i + 1];

        }
        else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--suffix")) {

            suffix_value = argv[i + 1];
            std::transform(suffix_value.begin(), suffix_value.end(), suffix_value.begin(), ::toupper);

        }
        else {
            if (i == argc - 1) {
                break;
            }
            std::cout << "ERROR: Invalid Command Line Option Found: " << argv[i] << " Error " << std::endl;
        } i++;
    }

    unsigned long long int INTENTION_MULTIPLIER = (ram_size_value * 1024 * 1024 * 512);

    std::locale comma_locale(std::locale(), new comma_numpunct());
    std::cout.imbue(comma_locale);

    cout << "Intention Repeater MAX v3.3 created by Thomas Sweet." << endl;
    cout << "Performance benchmark, enhancements and flags by Karteek Sheri." << endl;
    cout << "Intention multiplier by Thomas Sweet." << endl;
    cout << "This software comes with no guarantees or warranty of any kind and is for entertainment purposes only." << endl;
    cout << "Press Ctrl-C to quit." << endl << endl;

    if ((param_intention) == "") {
        cout << "Intention: ";
        std::getline(std::cin, intention);

    }
    else {
        intention = param_intention;
    }

    cout << "Loading Intention into Memory." << std::flush;

    //Repeat string till it is more than INTENTION_MULTIPLIER characters long.
    while (intention_value.length() < INTENTION_MULTIPLIER) {
        intention_value += intention;
        ++multiplier;
    }
    --multiplier; //Account for having to reduce at the end.

    //Now, remove enough characters at the end to account for the process statement to limit to less than 1024 characters.
    long long int intention_value_length = intention_value.length();
    long long int intention_length = intention.length();
    int process_statement_length = PROCESS_STATEMENT.length();
    long long int intention_length_val = intention_value_length - intention_length - process_statement_length;

    intention_value = intention_value.substr(0, intention_length_val);
    intention_value += PROCESS_STATEMENT;

    intention_value = intention + PROCESS_STATEMENT;

    duration = param_duration;
    //This code is to benchmark cpu iterations. By Karteek Sheri.
    auto b_start = std::chrono::system_clock::now();
    auto b_end = std::chrono::system_clock::now();

    while ((std::chrono::duration_cast <std::chrono::seconds> (b_end - b_start).count() != 1))
    {
        process_intention = intention_value; //The Intention Repeater Statement
        ++cpu_benchmark_count;
        b_end = std::chrono::system_clock::now();
    }
    //Benchmark ends here

    cout << endl;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::string iterations_string = "0", iterations_string_freq = "0";
    int digits = 0, freq_digits = 0;

    do {
        start = std::chrono::system_clock::now();
        while ((std::chrono::duration_cast <std::chrono::seconds> (end - start).count() != 1)) {
            for (int i = 0; i < cpu_benchmark_count; ++i) {
                process_intention = intention_value; //This is the Intention Repeater call that actually does the work with the Servitor.
                ++iterations;
            }
            end = std::chrono::system_clock::now();
        }
        ++seconds;
        runtime_formatted = FormatTimeRun(seconds);
        iterations_string_freq = to_string(iterations * multiplier);
        iterations_string = findsum(iterations_string, iterations_string_freq);
        digits = iterations_string.length();

        freq_digits = iterations_string_freq.length();

        if (suffix_value == "EXP")
        {
            std::cout << "[" + runtime_formatted + "]" << " (" << setprecision(3) << fixed << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / " << (stoull(iterations_string_freq.substr(0, 4)) / pow(10, 3)) << "x10^" << freq_digits - 1 << " Hz): " << intention << "     \r" << std::flush;
            //std::cout<< "[" + runtime_formatted + "]" << " (" <<setprecision(3)<<fixed<<(stoull(iterations_string.substr(0,4)))/pow(10,3)<<"x10^"<<digits-1<<" / "<<(stoull(iterations_string_freq.substr(0,4))/pow(10,3))<<"x10^"<<freq_digits-1<<" Hz): "<<intention<<"     \r" << std::flush;
        }
        else //suffix_value = "HZ"
        {
            std::cout << "[" + runtime_formatted + "]" << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / " << display_suffix(iterations_string_freq, freq_digits - 1, "Frequency") << "Hz): " << intention << "     \r" << std::flush;
        }
        iterations = 0;
        if (runtime_formatted == duration) {
            std::cout << endl << std::flush;
            exit(0);
        }
    } while (1);
    std::cout << endl << std::flush;
    return 0;
}
