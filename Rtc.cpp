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
 * @param networkless If you don't want to connect to the internet for some reason
 */
Rtc::Rtc(int syncInterval, bool networkless)
{
    this->ms = 0;
    this->syncInterval = syncInterval;
    this->networkless = networkless;
    this->error = RtcErrors::NO_ERROR;
}

Rtc::~Rtc()
{
    this->ticker.detach();
    this->thread.terminate();
}

/**
 * @brief Start a thread to collect NTP timestamps
 * 
 */
RtcErrors Rtc::Start()
{
    // If the user don't want to connect to the internet
    // Then we early exit, though start the microsecond counter
    // This could be usefull for testing
    if (this->networkless) {
        this->ticker.attach(callback(this, &Rtc::Tick), microseconds(1000));

        return this->error;
    }
    
    this->net = NetworkInterface::get_default_instance();

    if (!this->net || this->net == nullptr) {
        this->error = RtcErrors::NO_INTERFACE;
        printf("Error! No network interface found.\n");

        return this->error;
    }

    if (this->net->get_connection_status() == NSAPI_STATUS_ERROR_UNSUPPORTED) {
        this->error = RtcErrors::NO_INTERFACE;
        printf("Error! Network interface not supported.\n");

        return this->error;
    }

    if (this->net->get_connection_status() == NSAPI_STATUS_DISCONNECTED) {
        nsapi_size_or_error_t result = this->net->connect();
        if (result != 0) {
            this->error = RtcErrors::CONNECTION_FAILED;
            printf("Error! net->connect() returned: %d\n", result);

            return this->error;
        }
    }

    // Block while the network is connecting
    while(this->net->get_connection_status() == NSAPI_STATUS_CONNECTING) {}

    this->ticker.attach(callback(this, &Rtc::Tick), microseconds(1000));
    this->thread.start(callback(this, &Bundsgaard::Rtc::Worker));

    return this->error;
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

        // If no internet connection
        if (this->net->get_connection_status() != NSAPI_STATUS_GLOBAL_UP) {
            break;
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
 * @brief Tick every millisecond
 * 
 */
void Rtc::Tick()
{
    this->ms++;

    if (this->ms > 1000) {
        this->ms = 0;
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

/**
 * @brief Get the current UNIX timestamp in MS
 * 
 * @return uint64_t
 */
uint64_t Rtc::GetTimestampMS()
{
    uint64_t msTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    msTime += this->ms;

    return msTime;
}
