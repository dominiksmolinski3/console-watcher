#ifndef OBSERVEDSTRING_H
#define OBSERVEDSTRING_H

#include <string>
#include <vector>
#include "IObserver.h"

class ObservedString {
    std::string m_str;
    std::vector<IObserver*> m_observers;
    void NotifyObservers() {
        for (auto& observer : m_observers) {
            observer->HandleEvent(*this);
        }
    }
public:
    void Subscribe(IObserver& ref) {
        m_observers.push_back(&ref);
    }

    const std::string& GetValue() const { return m_str; }
    void SetValue(const std::string& str) {
        m_str = str;
        NotifyObservers();
    }
};

#endif // OBSERVEDSTRING_H
