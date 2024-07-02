/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__direction_client_glue_h__proxy__H__
#define __sdbuscpp__direction_client_glue_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace gspine {

class Gesture_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.gspine.Gesture";

protected:
    Gesture_proxy(sdbus::IProxy& proxy)
        : proxy_(proxy)
    {
    }

    Gesture_proxy(const Gesture_proxy&) = delete;
    Gesture_proxy& operator=(const Gesture_proxy&) = delete;
    Gesture_proxy(Gesture_proxy&&) = delete;
    Gesture_proxy& operator=(Gesture_proxy&&) = delete;

    ~Gesture_proxy() = default;

    virtual void onTouchEvent() = 0;

    void registerProxy()
    {
        proxy_.uponSignal("TOP_TO_BOTTOM").onInterface(INTERFACE_NAME).call([this](){ this->onTouchEvent(); });
    }

private:
    sdbus::IProxy& proxy_;
};

}} // namespaces

#endif
