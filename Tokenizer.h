#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <sstream>
#include <vector>

namespace jackal {
namespace util {

class Tokenizer {
    public:
        typedef std::vector<std::string> TokenList;
        typedef TokenList::const_iterator const_iterator;
        typedef TokenList::reference reference;
        typedef TokenList::const_reference const_reference;
    private:
        TokenList m_Tokens;
    public:
        Tokenizer(const std::string &str, char delim, int max = 0) {
            std::stringstream ss(str);
            std::string token = "";

            int cur = 0;

            while (true) {
                if (max > 0 && ++cur >= max) {
                    std::getline(ss, token);
                    m_Tokens.push_back(token);
                    break;
                }

                if (std::getline(ss, token, delim)) {
                    m_Tokens.push_back(token);
                } else {
                    // add an empty token if the last chunk was empty
                    if (str.at(str.length() - 1) == ':')
                        m_Tokens.push_back("");
                    break;
                }
            }
        }

        const_iterator begin() const { return m_Tokens.begin(); }
        const_iterator end() const { return m_Tokens.end(); }
        reference operator[](TokenList::size_type i) { return m_Tokens[i]; }
        const_reference operator[](TokenList::size_type i) const { return m_Tokens[i]; }
        TokenList::size_type size() const { return m_Tokens.size(); }
};

}
}
#endif
