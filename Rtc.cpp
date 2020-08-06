/**
 * @file Rtc.cpp
 * @author Rasmus Sørensen (bundsgaard.rasmus@gmail.com)
 * @brief Rtc class - this will automatically sync your board time with the world!
 * @version 0.1
 * @date 2020-08-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "mbed.h"
#include "./ntp-client/NTPClient.h"
#include "./Rtc.h"

using namespace Bundsgaard;
using namespace std::chrono;

/**
 * @brief Construct a new Rtc:: Rtc object
 * 
 * @param syncInterval Interval is in MINUTES
 */
Rtc::Rtc(int syncInterval)
{
    this->syncInterval = syncInterval;
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

/**
 * @brief Start a thread to collect NTP timestamps
 * 
 */
void Rtc::Start()
{
    this->_thread.start(callback(this, &Bundsgaard::Rtc::Worker));
}

/**
 * @brief Thread worker callback
 * 
 */
void Rtc::Worker()
{
    NTPClient ntp(this->net);
    bool firstIteration = true;

    while(1) {
        if (firstIteration == false) {
            ThisThread::sleep_for(minutes(this->syncInterval));
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

/**
 * @brief Returns the latest time gotten from the NTP server
 * 
 * @return time_t 
 */
time_t Rtc::GetNTPTime()
{
    return this->ntpTime;
}

/**
 * @brief Get the current time (UTC)
 * 
 * @return time_t 
 */
time_t Rtc::GetTime()
{
    return time(NULL);
}

/**
 * @brief Get the current UNIX timestamp
 * 
 * @return unsigned int 
 */
unsigned int Rtc::GetTimestamp()
{
    return (unsigned int)time(NULL);
}
