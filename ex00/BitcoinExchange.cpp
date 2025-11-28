#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

// Define the static member variable
std::map<std::string, float> BitcoinExchange::rates;

// Constructor with data file parameter
BitcoinExchange::BitcoinExchange(const std::string &dataFile)
{
	std::cout << "Constructor called with file: " << dataFile << "\n";
	initRates(dataFile);
}

// Copy constructor
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	std::cout << "Copy constructor called\n";
	(void)other;
}

// Assignment operator overload
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	std::cout << "Assignment operator called\n";
	if (this != &other)
	{
		(void)other;
	}
	return (*this);
}

// Destructor
BitcoinExchange::~BitcoinExchange(void)
{
	std::cout << "Destructor called\n";
}

std::string BitcoinExchange::readFile(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + filename);
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

// std::map<std::string, float> BitcoinExchange::parseFile(const std::string &fileContent)
std::map<std::string, float> BitcoinExchange::getRates(const std::string &fileContent)
{
	std::string line;
	std::map<std::string, float> rates;

	std::istringstream iss(fileContent);
	std::getline(iss, line); // Skip header

	while (std::getline(iss, line))
	{
		std::string date;
		float rate;
		std::istringstream lineStream(line);
		std::getline(lineStream, date, ',');
		lineStream >> rate;
		rates[date] = rate;
	}

	return rates;
}

void BitcoinExchange::initRates(const std::string &dataFile)
{
	try
	{
		const std::string fileContent = readFile(dataFile);
		rates = getRates(fileContent);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error initializing rates: " << e.what() << "\n";
	}
}

static int days_in_month(int year, int month) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap && month == 2) daysInMonth[1] = 29;
    return daysInMonth[month - 1];
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
	std::istringstream iss(date);
	int year;
	int month;
	int day;
	char dash;

	if (!(iss >> year >> dash >> month >> dash >> day))
	{
		throw std::invalid_argument("bad input");
	}
	// std::cout << "Year: " << year << ", Month: " << month << ", Day: " << day << "\n";
	if (year < 1900 || year > 2025)
	{
		throw std::invalid_argument("Year out of range");
	}
	if (month < 1 || month > 12)
	{
		throw std::invalid_argument("Month out of range");
	}
	if (day < 1 ||  day > days_in_month(year, month))
	{
		throw std::invalid_argument("Day out of range");
	}
	return true;
}

bool BitcoinExchange::isValidPrice(const float &price)
{
	// std::cout << "Price: " << price << "\n";
	if (price < 0.0)
	{
		throw std::invalid_argument("Not a positive number");
	}
	if (price > 1000.0)
	{
		throw std::invalid_argument("Too large a number");
	}
	return price >= 0.0 && price <= 1000.0;
}

void BitcoinExchange::outputPrices(const std::string &fileContent)
{
    std::string line;
    std::istringstream iss(fileContent);
    std::getline(iss, line); // Skip header

    while (std::getline(iss, line))
    {
        std::string date;
        float amount;
        std::istringstream lineStream(line);
        std::getline(lineStream, date, '|');

        if (!(lineStream >> amount))
        {
            std::cerr << "Error: bad input => " << line << "\n";
            continue;
        }

        try
        {
            isValidDate(date);
            isValidPrice(amount);

            std::string closestDate = getClosestDate(date);
            float rate = getRate(closestDate);
            float value = calcValue(rate, amount);

            std::cout << date << " => " << amount << " = " << value << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << date << " => " << "Error: " << e.what() << "\n";
        }
    }
}

std::string BitcoinExchange::getClosestDate(const std::string &date)
{
    std::map<std::string, float>::const_iterator it = rates.lower_bound(date);
    if (it == rates.end() || it->first != date)
    {
        if (it == rates.begin()) {
            throw std::runtime_error("No rate available for date");
        }
        --it;
    }
    return it->first;
}


float BitcoinExchange::getRate(const std::string &date)
{
    std::map<std::string, float>::const_iterator it = rates.find(date);
    if (it == rates.end()) {
        throw std::runtime_error("Rate not found");
    }
    return it->second;
}

float BitcoinExchange::calcValue(const float &rate, const float &amount)
{
    return rate * amount;
}
