

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>

using namespace std;

double calculatePrimes(vector<int> &, int);     // also returns elapsed time to calculate
string formatWithCommas(int);                   // helper function to print numbers with comma separators
bool checkAnswer(string);

int main(int argc, char *argv[]) {

    // default to 1 ~ 1000, unless specified
    int max = 1000;
    if (argc == 2) { max = atoi(argv[1]); }

    // initialize empty vector of ints
    vector<int>* primes = new vector<int>;

    cout << "\nCalculating prime numbers between 1 and " << formatWithCommas(max) << " . . .";

    // calculate primes, which will be added to the vector
    // function also returns elapsed time in seconds
    double calc_time = calculatePrimes(*primes, max);
    
    // print results
    cout << "\n\n";
    cout << "There are " << formatWithCommas(primes->size()) << " primes between 1 and "
         << formatWithCommas(max) << "!";

    cout << "\n\n";
    cout << "Would you like to print all the primes? ";
    string print_answer;
    cin >> print_answer;
    bool print = checkAnswer(print_answer);

    // use to track numbers printed per row
    int print_count = 0;
    double print_time;

    // variables for tracking print time
    timespec print_start, print_end;
    
    if (print) {
        // get start time for printing
        clock_gettime(CLOCK_MONOTONIC, &print_start);

        // convert largest prime to string to determine column width
        string max_prime = to_string(primes->back());
        // add 1 to column width for padding
        int width = max_prime.length() + 1;
        // max 120 character width per row
        int columns = 120 / width;

        cout << "\n\n";
        for (int p : *primes) {
            printf("%*d", width, p);
            if (++print_count % columns == 0) { cout << endl; }
        }
        cout << "\n\n";
        // get end time for printing

        clock_gettime(CLOCK_MONOTONIC, &print_end);

        // calculate elapsed time for printing
        print_time = (print_end.tv_sec - print_start.tv_sec) * 1e9;
        print_time = (print_time + (print_end.tv_nsec - print_start.tv_nsec)) * 1e-9;
    }

    // calculate memory used by vector
    int memory = sizeof(*primes) + (primes->capacity() * sizeof(int));

    // print statistics
    cout << "\n";
    cout << "Approximate memory used: " << formatWithCommas(memory) << " bytes\n";
    cout << fixed << setprecision(6);
    cout << "Calculation time:        " << calc_time << " seconds\n";
    if (print) {
        cout << "Printing time:           " << print_time << " seconds\n";
    }
    cout << "Total elapsed time:      " << calc_time + print_time << " seconds\n" << endl;

    // deallocate memory for vector
    delete primes;

    return 0;
}

double calculatePrimes(vector<int> &vec, int max) {

    if (max <= 2) { return 0.0; }
    
    // initialize vector with the first prime number, 2
    vec.push_back(2);

    bool is_prime = true;   // flag for when a prime is found
    int time_count = 0;     // counter for seconds elapsed
    double time_passed;     // counter for elapsed time between seconds

    // get start time for elapsed time calculation
    timespec calc_start, calc_end, last_mid, mid;
    clock_gettime(CLOCK_MONOTONIC, &calc_start);
    // use last_mid to calculate when 1 second has elapsed
    last_mid = calc_start;

    for (int i = 3; i < max; i += 2) {

        // get current time
        clock_gettime(CLOCK_MONOTONIC, &mid);
        // calculate elapsed time from last_mid
        time_passed = (mid.tv_sec - last_mid.tv_sec) * 1e9;
        time_passed = (time_passed + (mid.tv_nsec - last_mid.tv_nsec)) * 1e-9;
        // if 1 second has passed, print a dot
        if (time_passed > 1) {
            // wrap dots after 60
            if (time_count++ % 60 == 0) { cout << "\n"; }

            // print one dot per second
            cout << ". ";

            // reset last_mid to current time
            last_mid = mid;
        }

        // no number higher than this can be a factor
        int limit = sqrt(i) + 1;

        // check for prime factors
        for (int p : vec) {
            // break when largest possible factor surpassed
            if (p > limit) { break; }

            // check if p is a factor of i
            if (i % p == 0) {
                is_prime = false;
                break;
            }
        }

        // add prime numbers to vector
        if (is_prime) { vec.push_back(i); }
        // else reset is_prime to true
        else { is_prime = true; }
    }
    // get end time for elapsed time calculation
    clock_gettime(CLOCK_MONOTONIC, &calc_end);

    // calculate elapsed time
    double calc_time;
    calc_time = (calc_end.tv_sec - calc_start.tv_sec) * 1e9;
    calc_time = (calc_time + (calc_end.tv_nsec - calc_start.tv_nsec)) * 1e-9;

    return calc_time;
}

string formatWithCommas(int num) {
    string unformatted = to_string(num);

    int len = unformatted.length();

    if (len <= 3) { return unformatted; }

    int commas = (unformatted.length() - 1) / 3;
    int new_len = len + commas;

    string formatted(new_len, ' ');

    int i = len - 1;
    int j = new_len - 1;
    while (j >= 0) {
        formatted[j--] = unformatted[i--];
        if ((j > 0) && ((new_len - j) % 4 == 0)) {
            formatted[j--] = ',';
        }
    }

    return formatted;
}

bool checkAnswer(string ans) {

    for_each(ans.begin(), ans.end(), [](char &c) { c = tolower(c);});

    if (ans.length() == 1 && ans[0] == 'y') { return true; }
    if (ans.length() == 3 && ans[0] == 'y' && ans[1] == 'e' && ans[2] == 's') { return true; }

    return false; 

}


