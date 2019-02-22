#include <cstdlib>
#include <string>
#include <iostream>
#include <limits>
#include <ccrtp/rtp.h>
class Sender: public RTPSession, public TimerPort
{
public:
    Sender(const unsigned char* data, const InetHostAddress& ia,
    tpport_t port, uint32 tstamp, uint16 count):
    RTPSession(InetHostAddress("0.0.0.0")), packetsPerSecond(10)
    {
        uint32 timestamp = tstamp? tstamp : 0;
        defaultApplication().setSDESItem(SDESItemTypeTOOL, "rtpsend demo app.");
        setSchedulingTimeout(10000);
        setExpireTimeout(1000000);
        if ( !addDestination(ia,port) ) {
            cerr << "Could not connect" << endl;
            exit();
        }
        setPayloadFormat(StaticPayloadFormat(sptPCMU));
        startRunning();
        uint16 tstampInc = getCurrentRTPClockRate()/packetsPerSecond;
        uint32 period = 1000/packetsPerSecond;
        TimerPort::setTimer(period);
        for ( int i = 0; i < count ; i++ ) {
            putData(timestamp + i*tstampInc,
                data,strlen((char *)data) + 1);
            Thread::sleep(TimerPort::getTimer());
            TimerPort::incTimer(period);
        }
    }

private:
    const uint16 packetsPerSecond;
};
