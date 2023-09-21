#ifndef I2CPROXY_H
#define I2CPROXY_H

#include <vector>
#include <mutex>

void i2cProxy(const std::vector<int> &motorData, std::mutex &motorMutex);

#endif // I2CPROXY_H
