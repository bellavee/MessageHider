#pragma once

#include <memory>
#include <string>

class AppManager
{
public:
    static AppManager& GetInstance();

    std::string GetUserInput() const { return m_userInput; }
    void SetUserInput(std::string input) { m_userInput = input; }

private:
    static std::unique_ptr<AppManager> m_instance;

    std::string m_userInput;

    AppManager() {};
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};