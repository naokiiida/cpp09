#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
    public:
        BitcoinExchange(void);
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange &operator=(const BitcoinExchange &other);
        ~BitcoinExchange();

        static std::string readFile(const std::string &filename);
        static std::map<std::string, float> getRates(const std::string &fileContent);
        static std::map<std::string, float> getPrices(const std::string &fileContent);
        static void outputPrices(const std::string &fileContent);
        static bool isValidDate(const std::string &date);
        static bool isValidPrice(const float &price);
        static std::string getClosestDate(const std::string &date);
        static float getAmount(const std::string &amount);
        static float getRate(const std::string &date);
        static float calcValue(const float &rate, const float &amount);
        static void processTransactions(const std::map<std::string, float> &prices);

        static void initRates();

    private:
        static std::map<std::string, float> rates;
};

#endif
