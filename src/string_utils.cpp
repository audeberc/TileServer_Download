#include <string>
#include <vector>
#include <sstream>

namespace string_utils
{
    bool replace(std::string &str, const std::string &from, const std::string &to)
    {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    void parse_bounds(std::string bounds_string, std::vector<float> &bounds_values)
    {
        std::stringstream ss(bounds_string);

        while (ss.good())
        {
            std::string substr;
            std::getline(ss, substr, ',');
            bounds_values.push_back(std::stof(substr));
        }
        if (bounds_values.size() != 4)
        {
            throw std::invalid_argument("You must provide 4 comma separated bounds");
        }
    }

} // namespace string_utils
