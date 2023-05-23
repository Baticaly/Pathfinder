#ifndef TCPSOCKETLISTENER_H
#define TCPSOCKETLISTENER_H

#include <vector>
#include <mutex>

void tcpSocketListener(std::vector<int> &motorData, std::mutex &motorMutex);

#endif // TCPSOCKETLISTENER_H
