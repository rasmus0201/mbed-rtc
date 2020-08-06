#include "mbed.h"
#include "../ntp-client/NTPClient.h"
#include "./Rtc.h"

using namespace Bundsgaard;
using namespace std::chrono;

Rtc::Rtc(int refreshInterval)
{
    this->refreshInterval = refreshInterval;
    this->error = RtcErrors::NO_ERROR;
    this->net = NetworkInterface::get_default_instance();

    if (!this->net) {
        this->error = RtcErrors::NO_INTERFACE;
        printf("Error! No network interface found.\n");
        
        return;
    }

    nsapi_size_or_error_t result = this->net->connect();
    if (result != 0) {
        this->error = RtcErrors::CONNECTION_FAILED;
        printf("Error! net->connect() returned: %d\n", result);

        return;
    }
}

void Rtc::Start()
{
    this->_thread.start(callback(this, &Bundsgaard::Rtc::Run));
}

void Rtc::Run()
{
    NTPClient ntp(this->net);
    bool firstIteration = true;

    while(1) {
        if (firstIteration == false) {
            ThisThread::sleep_for(minutes(this->refreshInterval));
        }
        
        time_t currentTime = ntp.get_timestamp();
        if (currentTime < 0) {
            printf("An error occurred when getting the time. Code: %u\n", currentTime);

            continue;
        }

        set_time(currentTime);
        this->ntpTime = currentTime;
        firstIteration = false;
    }
}

time_t Rtc::GetNTPTime()
{
    return this->ntpTime;
}

time_t Rtc::GetTime()
{
    return time(NULL);
}

unsigned int Rtc::GetTimestamp()
{
    return (unsigned int)time(NULL);
}

