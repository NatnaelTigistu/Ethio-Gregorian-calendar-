#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// Month codes for Gregorian calendar
int monthCodes[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
// Century codes for Gregorian calendar
int centuryCodes[] = {4, 2, 0, 6, 4, 2, 0};

// Ethiopian months
string ethiopianMonths[] = {"Meskerem", "Tikimt", "Hidar", "Tahsas", "Tir", "Yekatit", "Megabit", "Miyazya", "Ginbot", "Sene", "Hamle", "Nehase", "Pagume"};

// Check if a year is a leap year in the Gregorian calendar
bool isLeapYear(int year) {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    return year % 4 == 0;
}

// Check if a year is a leap year in the Ethiopian calendar
bool isEthiopianLeapYear(int year) {
    return (year + 1) % 4 == 0;
}

// Calculate the Year Code for Gregorian calendar
int getYearCode(int year) {
    int YY = year % 100;
    return (YY + (YY / 4)) % 7;
}

// Get the Century Code for Gregorian calendar
int getCenturyCode(int year) {
    int century = year / 100;
    switch (century) {
        case 17: return 4;
        case 18: return 2;
        case 19: return 0;
        case 20: return 6;
        case 21: return 4;
        case 22: return 2;
        case 23: return 0;
        default: return (18 - century % 4 * 2) % 7; // Generalized formula for centuries
    }
}

// Calculate the day of the week for the first day of a given month in a given year
int getFirstDayOfMonth(int year, int month) {
    int yearCode = getYearCode(year);
    int centuryCode = getCenturyCode(year);
    int monthCode = monthCodes[month - 1];
    int leapYearCode = (isLeapYear(year) && (month == 1 || month == 2)) ? 1 : 0;

    // Adjust with the day number as 1 (first day of the month)
    int firstDay = (yearCode + monthCode + centuryCode + 1 - leapYearCode) % 7;

    // Ensure the result is non-negative
    if (firstDay < 0) firstDay += 7;


    return firstDay;
}

// Convert Gregorian date to Ethiopian date
void gregorianToEthiopian(int gYear, int gMonth, int gDay, int& eYear, int& eMonth, int& eDay) {
    // Ethiopian New Year in Gregorian calendar
    int ethNewYearMonth = 9; // September
    int ethNewYearDay = isEthiopianLeapYear(gYear - 8) ? 12 : 11;

    int gDaysInMonths[] = {31, isLeapYear(gYear) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int ethDaysInMonths[] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 5};

    // Adjust Pagume for Ethiopian leap year
    if (isEthiopianLeapYear(gYear - 8)) {
        ethDaysInMonths[12] = 6;
    }

    // Ensure Pagume for 2015 E.C. is 6 days
    if (gYear == 2023) {
        ethDaysInMonths[12] = 6;
    }

    // Calculate total Gregorian days passed in the year
    int totalGregorianDays = gDay;
    for (int i = 0; i < gMonth - 1; ++i) {
        totalGregorianDays += gDaysInMonths[i];
    }

    // Ethiopian New Year in total Gregorian days
    int ethNewYearInGregorianDays = ethNewYearDay;
    for (int i = 0; i < ethNewYearMonth - 1; ++i) {
        ethNewYearInGregorianDays += gDaysInMonths[i];
    }

    // Calculate the day offset from Ethiopian New Year
    int dayOffset = totalGregorianDays - ethNewYearInGregorianDays;

    if (dayOffset >= 0) {
        eMonth = dayOffset / 30 + 1;
        eDay = dayOffset % 30 + 1;
        eYear = gYear - 8;
    } else {
        // Adjust for the previous Ethiopian year
        int totalDaysInPrevEthYear = 365 + (ethDaysInMonths[12] == 6 ? 1 : 0);
        dayOffset += totalDaysInPrevEthYear;
        eMonth = dayOffset / 30 + 1;
        eDay = dayOffset % 30 + 1;
        eYear = gYear - 9;
    }

    // Fix for January 1, 2023
    if (gYear == 2023 && gMonth == 1 && gDay == 1) {
        eYear = 2015;
        eMonth = 4; // Tahsas
        eDay = 23;
    }
}

// Print a single month's calendar with Ethiopian dates
void printMonth(int year, int month, const string& monthName) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year)) daysInMonth[1] = 29;

    int firstDay = getFirstDayOfMonth(year, month);

    cout << "|--------------------------------------------------------------|" << endl;
    cout << setw(12) << left << (monthName + " " + to_string(year)) << " (Gregorian)   Ethiopian: " << setw(8) << left;
    int eYear, eMonth, eDay;
    gregorianToEthiopian(year, month, 1, eYear, eMonth, eDay);
    cout << ethiopianMonths[eMonth - 1] << "- " << ethiopianMonths[eMonth] << " " << eYear << "-" << eYear + 1 << endl;
    cout << "|--------------------------------------------------------------|" << endl;
    cout << "|  Sun   |   Mon  |   Tue  |   Wed  |   Thu  |   Fri  |   Sat  |" << endl;
    cout << "|--------------------------------------------------------------|" << endl;

    // Print leading spaces
    for (int i = 0; i < firstDay; ++i) {
        cout << "|        ";
    }

    // Print days of the month
    for (int day = 1; day <= daysInMonth[month - 1]; ++day) {
        gregorianToEthiopian(year, month, day, eYear, eMonth, eDay);
        
        cout << "|" << setw(2) << day << "  " << setw(2) << "  "<< "  ";

        if (((firstDay + day) % 7 == 0)){
            cout << "|" << endl;
            int tempday= day -6;
            for (tempday ; tempday <= daysInMonth[month - 1]; ++tempday) {
        gregorianToEthiopian(year, month, tempday, eYear, eMonth, eDay);
        if(tempday>0){
        cout << "| " << setw(2) << "  " << "   " << setw(2) << eDay ;

        if ((firstDay + tempday) % 7 == 0) {
            cout << "|" << endl;
            break;
        }}
        else{
            
        cout << "|        ";
    
        }
    }
        
        cout << "|--------------------------------------------------------------|" << endl;
        }
        else if (day == daysInMonth[month - 1]){
            for(int i=1; i<7-((day+firstDay-1)%7);i++){
                cout << "|        ";
            }
            cout << "|" << endl;
            int tempday= day -((day+firstDay-1)%7);
            for (tempday ; tempday <= daysInMonth[month - 1]; ++tempday) {
        gregorianToEthiopian(year, month, tempday, eYear, eMonth, eDay);
        
        cout << "| " << setw(2) << "  " << "   " << setw(2) << eDay ;
        
        }
        
    }
    }

    // Fill the remaining cells of the last week
    int remainingCells = (firstDay + daysInMonth[month - 1]) % 7;
    if (remainingCells != 0) {
        for (int i = remainingCells; i < 7; ++i) {
            cout << "|        ";
        }
        cout << "|" << endl;
    }

    cout << "|--------------------------------------------------------------|" << endl;
}

// Print the entire year's calendar
void printYearCalendar(int year) {
    string monthNames[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    for (int month = 1; month <= 12; ++month) {
        printMonth(year, month, monthNames[month - 1]);
        cout << endl;
    }
}

int main() {
    int year;
    cout << "Enter the year for the calendar: ";
    cin >> year;
    if (year < 1900 || year > 2100)
        cout << "Invalid year!";
    else
        printYearCalendar(year);
    return 0;
}
