#ifndef UARTPROXY_H
#define UARTPROXY_H

#include <vector>
#include <mutex>

void i2cProxy(const std::vector<int> &motorData, std::mutex &motorMutex);

#endif // UARTPROXY_H
