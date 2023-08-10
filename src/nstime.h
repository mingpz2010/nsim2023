/*
    Copyright © 2023 <Pingzhou Ming>

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the “Software”), to deal in 
    the Software without restriction, including without limitation the rights to use, 
    copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
    and to permit persons to whom the Software is furnished to do so, subject to the 
    following conditions:

    The above copyright notice and this permission notice shall be included in all copies 
    or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS 
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
    THE SOFTWARE.
*/

#ifndef TIME_H
#define TIME_H

#include "assert.h"
#include "attribute-helper.h"
#include "attribute.h"
#include "event-id.h"
#include "int64x64.h"
#include "type-name.h"

#include <cmath>
#include <limits>
#include <ostream>
#include <set>
#include <stdint.h>


namespace nsim2023
{

class TimeWithUnit;


// Management of virtual time in real world units.
class Time
{
  public:
    /**
     * The unit to use to interpret a number representing time
     */
    enum Unit
    {
        Y = 0,               //!< year, 365 days
        D = 1,              //!< day, 24 hours
        H = 2,              //!< hour, 60 minutes
        MIN = 3,         //!< minute, 60 seconds
        S = 4,              //!< second
        MS = 5,          //!< millisecond
        US = 6,             //!< microsecond
        NS = 7,             //!< nanosecond
        PS = 8,             //!< picosecond
        FS = 9,             //!< femtosecond
        LAST = 10,               //!< marker for last normal value
        AUTO = 11               //!< auto-scale output when using Time::As()
    };

    inline Time& operator=(const Time& o)
    {
        m_data = o.m_data;
        return *this;
    }


    inline Time()  : m_data()
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    inline Time(const Time& o)  : m_data(o.m_data)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    Time(Time&& o)
        : m_data(o.m_data)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(double v)
        : m_data(llround(v))
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(int v)
        : m_data(v)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(long int v)
        : m_data(v)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(long long int v)
        : m_data(v)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(unsigned int v)
        : m_data(v)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(unsigned long int v)
        : m_data(v)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(unsigned long long int v)
        : m_data(v)
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit inline Time(const int64x64_t& v)
        : m_data(v.Round())
    {
        if (g_markingTimes)
        {
            Mark(this);
        }
    }

    explicit Time(const std::string& s);


    static Time Min()
    {
        return Time(std::numeric_limits<int64_t>::min());
    }


    static Time Max()
    {
        return Time(std::numeric_limits<int64_t>::max());
    }


    ~Time()
    {
        if (g_markingTimes)
        {
            Clear(this);
        }
    }


    inline bool IsZero() const
    {
        return m_data == 0;
    }


    inline bool IsNegative() const
    {
        return m_data <= 0;
    }


    inline bool IsPositive() const
    {
        return m_data >= 0;
    }


    inline bool IsStrictlyNegative() const
    {
        return m_data < 0;
    }


    inline bool IsStrictlyPositive() const
    {
        return m_data > 0;
    }


    inline int Compare(const Time& o) const
    {
        return (m_data < o.m_data) ? -1 : (m_data == o.m_data) ? 0 : 1;
    }

    inline double GetYears() const
    {
        return ToDouble(Time::Y);
    }

    inline double GetDays() const
    {
        return ToDouble(Time::D);
    }

    inline double GetHours() const
    {
        return ToDouble(Time::H);
    }

    inline double GetMinutes() const
    {
        return ToDouble(Time::MIN);
    }

    inline double GetSeconds() const
    {
        return ToDouble(Time::S);
    }

    inline int64_t GetMilliSeconds() const
    {
        return ToInteger(Time::MS);
    }

    inline int64_t GetMicroSeconds() const
    {
        return ToInteger(Time::US);
    }

    inline int64_t GetNanoSeconds() const
    {
        return ToInteger(Time::NS);
    }

    inline int64_t GetPicoSeconds() const
    {
        return ToInteger(Time::PS);
    }

    inline int64_t GetFemtoSeconds() const
    {
        return ToInteger(Time::FS);
    }

    inline int64_t GetTimeStep() const
    {
        return m_data;
    }

    inline double GetDouble() const
    {
        return static_cast<double>(m_data);
    }

    inline int64_t GetInteger() const
    {
        return GetTimeStep();
    }

    static void SetResolution(enum Unit resolution);

    static enum Unit GetResolution();

    inline static Time From(const int64x64_t& value)
    {
        return Time(value);
    }

    // Create Times from values given in the indicated units.
    inline static Time FromInteger(uint64_t value, enum Unit unit)
    {
        struct Information* info = PeekInformation(unit);

        NS_ASSERT_MSG(info->isValid, "Attempted a conversion from an unavailable unit.");

        if (info->fromMul)
        {
            value *= info->factor;
        }
        else
        {
            value /= info->factor;
        }
        return Time(value);
    }

    inline static Time FromDouble(double value, enum Unit unit)
    {
        return From(int64x64_t(value), unit);
    }

    inline static Time From(const int64x64_t& value, enum Unit unit)
    {
        struct Information* info = PeekInformation(unit);

        NS_ASSERT_MSG(info->isValid, "Attempted a conversion from an unavailable unit.");

        // DO NOT REMOVE this temporary variable. It's here
        // to work around a compiler bug in gcc 3.4
        int64x64_t retval = value;
        if (info->fromMul)
        {
            retval *= info->timeFrom;
        }
        else
        {
            retval.MulByInvert(info->timeFrom);
        }
        return Time(retval);
    }

    inline int64_t ToInteger(enum Unit unit) const
    {
        struct Information* info = PeekInformation(unit);

        NS_ASSERT_MSG(info->isValid, "Attempted a conversion to an unavailable unit.");

        int64_t v = m_data;
        if (info->toMul)
        {
            v *= info->factor;
        }
        else
        {
            v /= info->factor;
        }
        return v;
    }

    inline double ToDouble(enum Unit unit) const
    {
        return To(unit).GetDouble();
    }

    inline int64x64_t To(enum Unit unit) const
    {
        struct Information* info = PeekInformation(unit);

        NS_ASSERT_MSG(info->isValid, "Attempted a conversion to an unavailable unit.");

        int64x64_t retval = int64x64_t(m_data);
        if (info->toMul)
        {
            retval *= info->timeTo;
        }
        else
        {
            retval.MulByInvert(info->timeTo);
        }
        return retval;
    }


    Time RoundTo(enum Unit unit) const
    {
        return From(this->To(unit).Round(), unit);
    }

    // Attach a unit to a Time, to facilitate output in a specific unit.
    TimeWithUnit As(const enum Unit unit = Time::AUTO) const;

    typedef void (*TracedCallback)(Time value);

  private:
    /** How to convert between other units and the current unit. */
    struct Information
    {
        bool toMul;          //!< Multiply when converting To, otherwise divide
        bool fromMul;        //!< Multiple when converting From, otherwise divide
        int64_t factor;      //!< Ratio of this unit / current unit
        int64x64_t timeTo;   //!< Multiplier to convert to this unit
        int64x64_t timeFrom; //!< Multiplier to convert from this unit
        bool isValid;        //!< True if the current unit can be used
    };

    /** Current time unit, and conversion info. */
    struct Resolution
    {
        struct Information info[LAST]; //!<  Conversion info from current unit
        enum Time::Unit unit;          //!<  Current time unit
    };

    static inline struct Resolution* PeekResolution()
    {
        static struct Time::Resolution& resolution{SetDefaultNsResolution()};
        return &resolution;
    }

    static inline struct Information* PeekInformation(enum Unit timeUnit)
    {
        return &(PeekResolution()->info[timeUnit]);
    }

    static struct Resolution& SetDefaultNsResolution();

    static void SetResolution(enum Unit unit,
                              struct Resolution* resolution,
                              const bool convert = true);

    /**
     *  Record all instances of Time, so we can rescale them when
     *  the resolution changes.
     */
    typedef std::set<Time*> MarkedTimes;

    static MarkedTimes* g_markingTimes;

  public:

    static bool StaticInit();

  private:

    friend class Simulator;

    static void ClearMarkedTimes();

    static void Mark(Time* const time);

    static void Clear(Time* const time);

    static void ConvertTimes(const enum Unit unit);


    friend bool operator==(const Time& lhs, const Time& rhs);
    friend bool operator!=(const Time& lhs, const Time& rhs);
    friend bool operator<=(const Time& lhs, const Time& rhs);
    friend bool operator>=(const Time& lhs, const Time& rhs);
    friend bool operator<(const Time& lhs, const Time& rhs);
    friend bool operator>(const Time& lhs, const Time& rhs);
    friend bool operator<(const Time& time, const EventId& event);


    friend Time operator+(const Time& lhs, const Time& rhs);
    friend Time operator-(const Time& lhs, const Time& rhs);
    friend Time operator*(const Time& lhs, const int64x64_t& rhs);
    friend Time operator*(const int64x64_t& lhs, const Time& rhs);
    friend int64x64_t operator/(const Time& lhs, const Time& rhs);
    friend Time operator/(const Time& lhs, const int64x64_t& rhs);
    friend Time operator%(const Time& lhs, const Time& rhs);
    friend int64_t Div(const Time& lhs, const Time& rhs);
    friend Time Rem(const Time& lhs, const Time& rhs);

    template <class T>
    friend typename std::enable_if<std::is_integral<T>::value, Time>::type operator*(
        const Time& lhs,
        T rhs);

    template <class T>
    friend typename std::enable_if<std::is_arithmetic<T>::value, Time>::type operator*(
        T lhs,
        const Time& rhs);

    template <class T>
    friend typename std::enable_if<std::is_integral<T>::value, Time>::type operator/(
        const Time& lhs,
        T rhs);

    friend Time Abs(const Time& time);
    friend Time Max(const Time& timeA, const Time& timeB);
    friend Time Min(const Time& timeA, const Time& timeB);

    template <class T>
    friend typename std::enable_if<std::is_floating_point<T>::value, Time>::type operator*(
        const Time& lhs,
        T rhs);
    template <class T>
    friend typename std::enable_if<std::is_floating_point<T>::value, Time>::type operator/(
        const Time& lhs,
        T rhs);

    friend Time& operator+=(Time& lhs, const Time& rhs);
    friend Time& operator-=(Time& lhs, const Time& rhs);

    int64_t m_data;

}; // class Time

namespace TracedValueCallback
{

typedef void (*Time)(Time oldValue, Time newValue);

}

// Force static initialization order of Time in each compilation unit.
static bool g_TimeStaticInit [[maybe_unused]] = Time::StaticInit();


inline bool operator==(const Time& lhs, const Time& rhs)
{
    return lhs.m_data == rhs.m_data;
}


inline bool operator!=(const Time& lhs, const Time& rhs)
{
    return lhs.m_data != rhs.m_data;
}


inline bool
operator<=(const Time& lhs, const Time& rhs)
{
    return lhs.m_data <= rhs.m_data;
}


inline bool
operator>=(const Time& lhs, const Time& rhs)
{
    return lhs.m_data >= rhs.m_data;
}


inline bool 
operator<(const Time& lhs, const Time& rhs)
{
    return lhs.m_data < rhs.m_data;
}


inline bool
operator>(const Time& lhs, const Time& rhs)
{
    return lhs.m_data > rhs.m_data;
}

/**
 * Compare a Time to an EventId.
 *
 * This is useful when you have cached a previously scheduled event:
 *
 *     m_event = Schedule (...);
 *
 * and later you want to know the relationship between that event
 * and some other Time `when`:
 *
 *     if (when < m_event) ...
 */
inline bool
operator<(const Time& time, const EventId& event)
{
    // Negative Time is less than any possible EventId, which are all >= 0.
    if (time.m_data < 0)
    {
        return true;
    }
    // Time must be >= 0 so casting to unsigned is safe.
    return static_cast<uint64_t>(time.m_data) < event.GetTs();
}

inline Time
operator+(const Time& lhs, const Time& rhs)
{
    return Time(lhs.m_data + rhs.m_data);
}


inline Time
operator-(const Time& lhs, const Time& rhs)
{
    return Time(lhs.m_data - rhs.m_data);
}


inline Time
operator*(const Time& lhs, const int64x64_t& rhs)
{
    int64x64_t res = lhs.m_data;
    res *= rhs;
    return Time(res);
}


inline Time
operator*(const int64x64_t& lhs, const Time& rhs)
{
    return rhs * lhs;
}


template <class T>
typename std::enable_if<std::is_integral<T>::value, Time>::type
operator*(const Time& lhs, T rhs)
{
    static_assert(!std::is_same<T, bool>::value,
                  "Multiplying a Time by a boolean is not supported");

    return Time(lhs.m_data * rhs);
}

// Leave undocumented
template <class T>
typename std::enable_if<std::is_floating_point<T>::value, Time>::type
operator*(const Time& lhs, T rhs)
{
    return lhs * int64x64_t(rhs);
}

// Scale a Time by a numeric value
template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, Time>::type
operator*(T lhs, const Time& rhs)
{
    return rhs * lhs;
}

// Exact division, returning a dimensionless fixed point number
inline int64x64_t
operator/(const Time& lhs, const Time& rhs)
{
    int64x64_t num = lhs.m_data;
    int64x64_t den = rhs.m_data;
    return num / den;
}

inline Time
operator/(const Time& lhs, const int64x64_t& rhs)
{
    int64x64_t res = lhs.m_data;
    res /= rhs;
    return Time(res);
}

template <class T>
typename std::enable_if<std::is_integral<T>::value, Time>::type
operator/(const Time& lhs, T rhs)
{
    static_assert(!std::is_same<T, bool>::value, "Dividing a Time by a boolean is not supported");

    return Time(lhs.m_data / rhs);
}

// Leave undocumented
template <class T>
typename std::enable_if<std::is_floating_point<T>::value, Time>::type
operator/(const Time& lhs, T rhs)
{
    return lhs / int64x64_t(rhs);
}

inline Time
operator%(const Time& lhs, const Time& rhs)
{
    return Time(lhs.m_data % rhs.m_data);
}

inline Time
Rem(const Time& lhs, const Time& rhs)
{
    return Time(lhs.m_data % rhs.m_data);
}


inline int64_t
Div(const Time& lhs, const Time& rhs)
{
    return lhs.m_data / rhs.m_data;
}


inline Time&
operator+=(Time& lhs, const Time& rhs)
{
    lhs.m_data += rhs.m_data;
    return lhs;
}


inline Time&
operator-=(Time& lhs, const Time& rhs)
{
    lhs.m_data -= rhs.m_data;
    return lhs;
}


inline Time
Abs(const Time& time)
{
    return Time((time.m_data < 0) ? -time.m_data : time.m_data);
}


inline Time
Max(const Time& timeA, const Time& timeB)
{
    return Time((timeA.m_data < timeB.m_data) ? timeB : timeA);
}


inline Time
Min(const Time& timeA, const Time& timeB)
{
    return Time((timeA.m_data > timeB.m_data) ? timeB : timeA);
}


std::ostream& operator<<(std::ostream& os, const Time& time);

std::istream& operator>>(std::istream& is, Time& time);

inline Time
Years(double value)
{
    return Time::FromDouble(value, Time::Y);
}

inline Time
Years(int64x64_t value)
{
    return Time::From(value, Time::Y);
}

inline Time
Days(double value)
{
    return Time::FromDouble(value, Time::D);
}

inline Time
Days(int64x64_t value)
{
    return Time::From(value, Time::D);
}

inline Time
Hours(double value)
{
    return Time::FromDouble(value, Time::H);
}

inline Time
Hours(int64x64_t value)
{
    return Time::From(value, Time::H);
}

inline Time
Minutes(double value)
{
    return Time::FromDouble(value, Time::MIN);
}

inline Time
Minutes(int64x64_t value)
{
    return Time::From(value, Time::MIN);
}

inline Time
Seconds(double value)
{
    return Time::FromDouble(value, Time::S);
}

inline Time
Seconds(int64x64_t value)
{
    return Time::From(value, Time::S);
}

inline Time
MilliSeconds(uint64_t value)
{
    return Time::FromInteger(value, Time::MS);
}

inline Time
MilliSeconds(int64x64_t value)
{
    return Time::From(value, Time::MS);
}

inline Time
MicroSeconds(uint64_t value)
{
    return Time::FromInteger(value, Time::US);
}

inline Time
MicroSeconds(int64x64_t value)
{
    return Time::From(value, Time::US);
}

inline Time
NanoSeconds(uint64_t value)
{
    return Time::FromInteger(value, Time::NS);
}

inline Time
NanoSeconds(int64x64_t value)
{
    return Time::From(value, Time::NS);
}

inline Time
PicoSeconds(uint64_t value)
{
    return Time::FromInteger(value, Time::PS);
}

inline Time
PicoSeconds(int64x64_t value)
{
    return Time::From(value, Time::PS);
}

inline Time
FemtoSeconds(uint64_t value)
{
    return Time::FromInteger(value, Time::FS);
}

inline Time
FemtoSeconds(int64x64_t value)
{
    return Time::From(value, Time::FS);
}


inline Time
TimeStep(uint64_t ts)
{
    return Time(ts);
}

ATTRIBUTE_VALUE_DEFINE(Time);
ATTRIBUTE_ACCESSOR_DEFINE(Time);


Ptr<const AttributeChecker> MakeTimeChecker(const Time min, const Time max);


inline Ptr<const AttributeChecker>
MakeTimeChecker()
{
    return MakeTimeChecker(Time::Min(), Time::Max());
}


inline Ptr<const AttributeChecker>
MakeTimeChecker(const Time min)
{
    return MakeTimeChecker(min, Time::Max());
}


class TimeWithUnit
{
  public:
    // Attach a unit to a Time
    TimeWithUnit(const Time time, const Time::Unit unit)
        : m_time(time),
          m_unit(unit)
    {
    }

  private:
    Time m_time;                //!< The time
    Time::Unit m_unit;      //!< The unit to use in output

    friend std::ostream& operator<<(std::ostream& os, const TimeWithUnit& timeU);

}; // class TimeWithUnit


TYPENAMEGET_DEFINE(Time);

}

#endif /* TIME_H */

