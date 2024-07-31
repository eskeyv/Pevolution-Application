#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <windows.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;

void setConsoleTextColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetConsoleTextColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

bool isValidURL(const string& url) {
    const vector<string> validPrefixes = {
        "https://flux.li/android/external/start.php?HWID=",
        "https://gateway.platoboost.com/a/8?id=",
        "https://linkvertise.com"
    };

    for (const auto& prefix : validPrefixes) {
        if (url.find(prefix) == 0) {
            return true;
        }
    }
    return false;
}

void clearScreen() {
    system("cls");
}

void displayMenu() {
    setConsoleTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Pevolution Bypasser" << endl;
    resetConsoleTextColor();
    cout << "Supported Bypasses ( Last Checked 30 July, 2024 ) " << endl;
    cout << "  [+] Fluxus ( https://flux.li/android/external/start.php?HWID= )" << endl;
    cout << "  [+] Delta ( https://gateway.platoboost.com/a/8?id= )" << endl;
    cout << "  [+] Linkvertise ( https://linkvertise.com )" << endl;
    cout << endl;
    cout << "Enter Your Link : ";
}

bool handleResponse(http_response response) {
    try {
        if (response.status_code() != status_codes::OK) {
            setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Request failed with status code " << response.status_code() << endl;
            resetConsoleTextColor();
            return false;
        }

        web::json::value body = response.extract_json().get();
        if (body.has_field(U("result"))) {
            setConsoleTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            cout << "Result : " << conversions::to_utf8string(body[U("result")].as_string()) << endl;
            resetConsoleTextColor();
            return true;
        }
        else {
            setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Bypass error, please try again" << endl;
            resetConsoleTextColor();
            return false;
        }
    }
    catch (const std::exception& e) {
        setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error: " << e.what() << endl;
        resetConsoleTextColor();
        return false;
    }
}

int main() {
    while (true) {
        clearScreen();
        displayMenu();

        string link;
        getline(cin, link);

        if (!isValidURL(link)) {
            setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: Invalid URL." << endl;
            resetConsoleTextColor();
            continue;
        }

        try {
            http_client client(U("https://byte.goatbypassers.xyz"));
            uri_builder builder(U("/api/bypass"));
            builder.append_query(U("link"), conversions::to_string_t(link));

            http_request request(methods::GET);
            request.set_request_uri(builder.to_string());
            request.headers().add(U("Authorization"), U("Your API Key"));

            bool success = client.request(request).then([](http_response response) {
                return handleResponse(response);
                }).get();

                char choice;
                do {
                    cout << endl;
                    cout << "Choose an option:" << endl;
                    cout << "  [1] Request again" << endl;
                    cout << "  [2] Exit" << endl;
                    cout << "Enter your choice: ";
                    cin >> choice;

                    if (choice == '1') {
                        break;
                    }
                    else if (choice == '2') {
                        return 0;
                    }
                    else {
                        setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                        cout << "Invalid choice. Please enter 1 or 2." << endl;
                        resetConsoleTextColor();
                    }
                } while (true);

                cin.ignore(10000, '\n');
        }
        catch (const std::exception& e) {
            setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: " << e.what() << endl;
            resetConsoleTextColor();
        }
    }
}
