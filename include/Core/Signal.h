#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <any>
#include <memory>

class Signal {
public:
    template<typename... Args>
    using Callback = std::function<void(Args...)>;


    static Signal& GetInstance() {
        static Signal instance;
        return instance;
    }

    template<typename... Args>
    void AddListener(const std::string& signalName, Callback<Args...> callback) {
        m_listeners[signalName].push_back([callback](std::any& args) {
            if (auto* tuple = std::any_cast<std::tuple<Args...>>(&args)) {
                std::apply(callback, *tuple);
            }
            });
    }

    template<typename... Args>
    void Dispatch(const std::string& signalName, Args... args) {
        if (m_listeners.find(signalName) != m_listeners.end()) {
            std::tuple<Args...> argTuple(args...);
            std::any argPack = argTuple;
            for (auto& callback : m_listeners[signalName]) {
                callback(argPack);
            }
        }
        else
        {
			std::cout << "Signal not found: " << signalName << std::endl;
        }
    }

    template<typename... Args>
    void RemoveListener(const std::string& signalName, Callback<Args...> callback) {
        // Implementar si usás ID para callback (actualmente no se puede comparar lambdas)
    }

private:
    std::unordered_map<std::string, std::vector<std::function<void(std::any&)>>> m_listeners;

    // static std::shared_ptr<Signal> _signal;

    Signal() = default;
};