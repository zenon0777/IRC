#include "ser.hpp"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::string extractJsonValue(const std::string& json, const std::string& key) {
    size_t keyPos = json.find("\"" + key + "\":");
    if (keyPos != std::string::npos) {
        size_t valueStart = json.find("\"", keyPos + key.length() + 3) + 1;
        size_t valueEnd = json.find("\"", valueStart);
        return json.substr(valueStart, valueEnd - valueStart);
    }
    return "";
}

void server::parse_response(std::string response, int client_fd)
{
    std::cout << response + "\n" << std::endl;
    std::string resp = extractJsonValue(response, "content");
    // size_t linepos = resp.find("\n");
    // while(linepos != std::string::npos){
    //     resp.replace();
    // }
    resp += "\r\n";
    const char *buff = resp.c_str();
    send(client_fd, buff, strlen(buff), 0);
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