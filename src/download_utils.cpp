#include <curl/curl.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace download_utils
{
    void downloadFile(const char *url, const char *fname)
    {
        CURL *curl;
        FILE *fp;
        CURLcode res;
        curl = curl_easy_init();
        if (curl)
        {
            fp = fopen(fname, "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
        }
    }

    int create_dirs(std::string out_folder, int z, int x)
    {
        fs::path dir0(out_folder);
        fs::path dir1(std::to_string(z));
        fs::path dir2(std::to_string(x));

        if (!fs::exists(dir0))
        {
            fs::create_directory(dir0);
        }
        if (!fs::exists(dir0 / dir1))
        {
            fs::create_directory(dir0 / dir1);
        }
        if (!fs::exists(dir0 / dir1 / dir2))
        {
            fs::create_directory(dir0 / dir1 / dir2);
        }
    }

} // namespace download_utils