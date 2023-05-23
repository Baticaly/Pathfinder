#ifndef UARTPROXY_H
#define UARTPROXY_H

#include <vector>
#include <mutex>

void uartProxy(const std::vector<int> &motorData, std::mutex &motorMutex);

#endif // UARTPROXY_H
