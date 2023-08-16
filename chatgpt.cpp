#include "ser.hpp"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void server::parse_response(std::string response, int client_fd)
{
    // response = "Bots response: \
    // {\
    // \"id\": \"chatcmpl-7oAsjq8WnEjOmsqu2jEGXPpeZKUmA\",\
    // \"object\": \"chat.completion\",\
    // \"created\": 1692192585,\
    // \"model\": \"gpt-3.5-turbo-0613\",\
    // \"choices\": [\
    //     {\
    //     \"index\": 0,\
    //     \"message\": {\
    //         \"role\": \"assistant\",\
    //         \"content\": \"In the realm of hearts, where feelings thrive,\nLove dances with grace, making souls come alive.\nIt whispers softly, in gentle melodic rhyme,\nA poem of love, enduring through time.\n\nLove is a tapestry, woven with care,\nThreads\"},\
    //     \"finish_reason\": \"stop\"\
    //     }\
    // ],\
    // \"usage\": {\
    //     \"prompt_tokens\": 9,\
    //     \"completion_tokens\": 9,\
    //     \"total_tokens\": 18\
    // }\
    // }";
    std::cout << response + "\n" << std::endl;
    // std::vector<std::string> vec = splite(response, ':');
    // std::vector<std::string> content;
    // for(size_t i = 0; i < vec.size(); i++)
    // {
    //     std::cout << "first \n";
    //     std::cout << vec[i] << std::endl;
    //     std::cout << "NEWLINE \n";
    //     content = splite(vec[i], ',');
    //     if (content[1] == "\"content\"")
    //     {
    //         std::cout << "here" << std::endl;
    //         content = splite(vec[i + 1], '}');
    //         std::cout << content[0] << std::endl;
    //         std::string resp = trim(content[0], "\"} \t");
    //         resp += "\r\n";
    //         const char *buff = resp.c_str();
    //         send(client_fd, buff, strlen(buff), 0);
    //         break;
    //     }
    // }
}

bool server::bot(std::vector<std::string> vec, int client_fd)
{
    std::string mssg;
    if (vec.size() > 1)
    {
        for (size_t i = 1; i < vec.size(); i++)
        {
            mssg += vec[i];
            if (i < vec.size() - 1)
                mssg += " ";
        }
        CURL *curl = curl_easy_init();
        std::string userMessage = mssg;
        // std::string conversation = "User: " + userMessage + "\nBot:";
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, API_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Content-Type: application/json"));
        headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // Set JSON data for the API request
        std::string json_data = "{"
                                "\"messages\": ["
                                "{"
                                "\"role\": \"user\"," //role
                                "\"content\": \"" + userMessage + "\""
                                "}"
                                "],"
                                "\"max_tokens\": 50,"
                                "\"model\": \"gpt-3.5-turbo\"" //model
                                "}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
        {
            // Handle the API response
            parse_response(response, client_fd);
        }
        else {
            std::cerr << "API request failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return true;
}