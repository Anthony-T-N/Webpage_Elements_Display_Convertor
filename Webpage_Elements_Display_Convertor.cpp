#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

std::vector<std::string> html_extractor(const char* target_url);

int generate_HTML_file(std::vector<std::string> input_file_line_vec)
{
    std::vector<std::string> extracted_strings;
    std::ifstream input_file;
    input_file.open("elements_webpage_template.html");
    std::ofstream output_file;
    std::cout << "\n" << "[!] Creating/opening List.csv;" << "\n";
    output_file.open("elements_list_output.html");
    std::cout << "[+] Opened List.csv successfully;" << "\n\n";
    std::string input_file_line;
    while (std::getline(input_file, input_file_line))
    {
        if (input_file_line.find("<div class=\"row\">") != std::string::npos)
        {
            std::cout << "<div class=\"row\"> found" << "\n";
            output_file << input_file_line << "\n";
            for (int i = 0; i <= input_file_line_vec.size() - 1; i++)
            {
                extracted_strings = html_extractor(input_file_line_vec[i].c_str());
                std::string color;
                if (i % 2 == 0)
                {
                    color = "#34495E";
                }
                else
                {
                    color = "#273746";
                }
                output_file << "<div class=\"column\" style=\"background-color:" + color + ";display:flex;\">" << "\n"
                    << "<img style=\"margin-right:10px;display:block;height:445px;\" src=\"" + extracted_strings[1] + "\">" << "\n" 
                    << "<div class=\"text\"><h3 style=\"color:white;\"><a href=" + input_file_line_vec[i] + ">" + extracted_strings[0] + "</a></h2><p style=\"color:white;\">" + extracted_strings[2] + "</p>" << "\n" 
                    << "</div>" << "\n" << "</div>" << "\n\n";
            }
        }
        else
        {
            output_file << input_file_line << "\n";
        }
        std::cout << input_file_line << "\n";

    }
    input_file.close();
    output_file.close();
    return 0;
}

std::vector<std::string> read_file()
{
    std::ifstream input_file;
    input_file.open("List.csv");

    std::vector<std::string> input_file_line_vec;
    std::string input_file_line;

    std::cout << "URL List:" << "\n";
    while (std::getline(input_file, input_file_line))
    {
        input_file_line = input_file_line.substr(input_file_line.find(',') + 1);
        std::cout << input_file_line << "\n";
        input_file_line_vec.push_back(input_file_line);
    }
    input_file.close();
    return input_file_line_vec;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<std::string> html_extractor(const char* target_url)
{
    std::cout << target_url << "\n";

    CURL* curl;
    CURLcode res;
    std::string stored_webpage;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, target_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stored_webpage);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    std::string target_tag = stored_webpage.substr(stored_webpage.find("<h1"), stored_webpage.find("</h1>") - stored_webpage.find("<h1"));
    target_tag = target_tag.substr(target_tag.find("<strong>") + 8, target_tag.find("</strong>") - target_tag.find("<strong>") - 8);
    std::cout << "Target_Tag: " << target_tag << "\n";

    std::string target_image = stored_webpage.substr(stored_webpage.find("lazyload"));
    target_image = target_image.substr(target_image.find("data-src=\"") + 10, target_image.find("\" alt=") - target_image.find("data-src=\"") - 10);
    std::cout << "Target_Image: " << target_image << "\n";

    std::string target_desc = stored_webpage.substr(stored_webpage.find("<p itemprop=\"description\">"));
    target_desc = target_desc.substr(target_desc.find("<p itemprop=\"description\">") + 26, target_desc.find("</p>") - target_desc.find("<p itemprop=\"description\">") - 22);
    std::cout << "Target_Desc: " << target_desc << "\n\n";
    
    std::vector<std::string> extracted_strings = { target_tag , target_image, target_desc };
    
    return extracted_strings;
}

int main()
{
    std::cout << "=======================================" << "\n";
    std::cout << "- Webpage_Elements_Display_Convertor console application" << "\n";
    std::cout << "- Console Application Version: 1.0" << "\n";
    std::cout << "- Created By: Anthony-T-N." << "\n";
    std::cout << "- Current location of executable: " << std::filesystem::current_path() << "\n";
    std::cout << "=======================================" << "\n\n";

    generate_HTML_file(read_file());
    
    return 0;

    // 1) Read text file with URLS
    // 2) Iterate through each URL to collect page elements.
    // 3) Generate HTML page with sorted elements.
}
