#ifndef IOBSERVER_H
#define IOBSERVER_H

class ObservedString;

class IObserver {
public:
    virtual void HandleEvent(const ObservedString& ref) = 0;
    virtual ~IObserver() = default;
};

#endif // IOBSERVER_H
