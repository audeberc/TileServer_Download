namespace string_utils
{
    bool replace(std::string &str, const std::string &from, const std::string &to);
    void parse_bounds(std::string bounds_string, std::vector<float> &bounds_values);

} // namespace string_utils