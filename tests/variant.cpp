#include <gdbus-c++/variant.hpp>

#include <gtest/gtest.h>
#include <limits>
#include <random>

struct random_value
{
    random_value()
        : m_mt(m_random_device())
    {}

    explicit operator bool()
    {
        return static_cast<bool>(static_cast<uint8_t>(*this) % 2);
    }

    template<typename IntegerT>
    explicit operator IntegerT()
    {
        IntegerT min = std::numeric_limits<IntegerT>::min();
        IntegerT max = std::numeric_limits<IntegerT>::max();

        return std::uniform_int_distribution<IntegerT>(min, max)(m_mt);
    }

    explicit operator double()
    {
        double min = std::numeric_limits<double>::min();
        double max = std::numeric_limits<double>::max();

        return std::uniform_real_distribution<double>(min, max)(m_mt);
    }

    explicit operator std::string()
    {
        static const char alphanum[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string string;
        size_t size = std::uniform_int_distribution<size_t>(0, 10)(m_mt);

        for (size_t i = 0; i < size; i++) {
            size_t position = std::uniform_int_distribution<size_t>(0, sizeof(alphanum) - 2)(m_mt);
            string += alphanum[position];
        }

        return string;
    }

    template<typename T>
    explicit operator std::vector<T>()
    {
        size_t size = std::uniform_int_distribution<size_t>(0, 10)(m_mt);

        std::vector<T> vector;
        vector.reserve(size);

        for (size_t i = 0; i < size; i++) {
            vector.push_back(static_cast<T>(*this));
        }

        return vector;
    }

    template<typename K, typename V>
    explicit operator std::unordered_map<K, V>()
    {
        size_t size = std::uniform_int_distribution<size_t>(0, 10)(m_mt);

        std::unordered_map<K, V> map;
        map.reserve(size);

        for (size_t i = 0; i < size; i++) {
            map.insert({static_cast<K>(*this), static_cast<V>(*this)});
        }

        return map;
    }

    template<typename... T>
    explicit operator std::tuple<T...>()
    {
        return {static_cast<T>(*this)...};
    }

    //    explicit operator gdbus::message()
    //    {
    //        uint8_t index = static_cast<uint8_t>(*this) % 21;
    //
    //        switch(index) {
    //        case 0:
    //            return static_cast<bool>(*this);
    //        case 1:
    //            return static_cast<uint8_t>(*this);
    //        case 2:
    //            return static_cast<int16_t>(*this);
    //        case 3:
    //            return static_cast<uint16_t>(*this);
    //        case 4:
    //            return static_cast<int32_t>(*this);
    //        case 5:
    //            return static_cast<uint32_t>(*this);
    //        case 6:
    //            return static_cast<int64_t>(*this);
    //        case 7:
    //            return static_cast<uint64_t>(*this);
    //        case 8:
    //            return static_cast<double>(*this);
    //        case 9:
    //            return static_cast<std::string>(*this);
    //        case 10:
    //            return static_cast<std::vector<bool>>(*this);
    //        case 11:
    //            return static_cast<std::vector<uint8_t>>(*this);
    //        case 12:
    //            return static_cast<std::vector<int16_t>>(*this);
    //        case 13:
    //            return static_cast<std::vector<uint16_t>>(*this);
    //        case 14:
    //            return static_cast<std::vector<int32_t>>(*this);
    //        case 15:
    //            return static_cast<std::vector<uint32_t>>(*this);
    //        case 16:
    //            return static_cast<std::vector<int64_t>>(*this);
    //        case 17:
    //            return static_cast<std::vector<uint64_t>>(*this);
    //        case 18:
    //            return static_cast<std::vector<double>>(*this);
    //        case 19:
    //            return static_cast<std::vector<std::string>>(*this);
    //        case 20:
    //        default:
    //            return static_cast<std::unordered_map<std::string, gdbus::message>>(*this);
    //        }
    //    }

private:
    std::random_device m_random_device;
    std::mt19937 m_mt;
};

#define TYPE(...) __VA_ARGS__
#define DECLARE_TEST_FOR_TYPE(TEST_NAME, T)                                 \
    TEST(Message, TEST_NAME)                                                \
    {                                                                       \
        for (int i = 0; i < 10; i++) {                                      \
            T value = static_cast<T>(random_value());                       \
            EXPECT_EQ(gdbus::variant(value).get<decltype(value)>(), value); \
        }                                                                   \
    }

DECLARE_TEST_FOR_TYPE(Bool, TYPE(bool))
DECLARE_TEST_FOR_TYPE(Uint8, TYPE(uint8_t))
DECLARE_TEST_FOR_TYPE(Int16, TYPE(int16_t))
DECLARE_TEST_FOR_TYPE(Int32, TYPE(int32_t))
DECLARE_TEST_FOR_TYPE(Int64, TYPE(int64_t))
DECLARE_TEST_FOR_TYPE(Uint16, TYPE(uint16_t))
DECLARE_TEST_FOR_TYPE(Uint32, TYPE(uint32_t))
DECLARE_TEST_FOR_TYPE(Uint64, TYPE(uint64_t))
DECLARE_TEST_FOR_TYPE(Double, TYPE(double))
DECLARE_TEST_FOR_TYPE(String, TYPE(std::string))
//DECLARE_TEST_FOR_TYPE(Variant, gdbus::message)
DECLARE_TEST_FOR_TYPE(VectorOfBool, TYPE(std::vector<bool>))
DECLARE_TEST_FOR_TYPE(VectorOfUint8, TYPE(std::vector<uint8_t>))
DECLARE_TEST_FOR_TYPE(VectorOfInt16, TYPE(std::vector<int16_t>))
DECLARE_TEST_FOR_TYPE(VectorOfInt32, TYPE(std::vector<int32_t>))
DECLARE_TEST_FOR_TYPE(VectorOfInt64, TYPE(std::vector<int64_t>))
DECLARE_TEST_FOR_TYPE(VectorOfUint16, TYPE(std::vector<uint16_t>))
DECLARE_TEST_FOR_TYPE(VectorOfUint32, TYPE(std::vector<uint32_t>))
DECLARE_TEST_FOR_TYPE(VectorOfUint64, TYPE(std::vector<uint64_t>))
DECLARE_TEST_FOR_TYPE(VectorOfDouble, TYPE(std::vector<double>))
DECLARE_TEST_FOR_TYPE(VectorOfString, TYPE(std::vector<std::string>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndBool, TYPE(std::unordered_map<uint8_t, bool>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndUint8, TYPE(std::unordered_map<uint8_t, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndUint16, TYPE(std::unordered_map<uint8_t, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndUint32, TYPE(std::unordered_map<uint8_t, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndUint64, TYPE(std::unordered_map<uint8_t, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndInt16, TYPE(std::unordered_map<uint8_t, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndInt32, TYPE(std::unordered_map<uint8_t, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndInt64, TYPE(std::unordered_map<uint8_t, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndDouble, TYPE(std::unordered_map<uint8_t, double>))
DECLARE_TEST_FOR_TYPE(MapOfUint8AndString, TYPE(std::unordered_map<uint8_t, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndBool, TYPE(std::unordered_map<int16_t, bool>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndUint8, TYPE(std::unordered_map<int16_t, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndUint16, TYPE(std::unordered_map<int16_t, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndUint32, TYPE(std::unordered_map<int16_t, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndUint64, TYPE(std::unordered_map<int16_t, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndInt16, TYPE(std::unordered_map<int16_t, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndInt32, TYPE(std::unordered_map<int16_t, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndInt64, TYPE(std::unordered_map<int16_t, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndDouble, TYPE(std::unordered_map<int16_t, double>))
DECLARE_TEST_FOR_TYPE(MapOfInt16AndString, TYPE(std::unordered_map<int16_t, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndBool, TYPE(std::unordered_map<int32_t, bool>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndUint8, TYPE(std::unordered_map<int32_t, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndUint16, TYPE(std::unordered_map<int32_t, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndUint32, TYPE(std::unordered_map<int32_t, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndUint64, TYPE(std::unordered_map<int32_t, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndInt16, TYPE(std::unordered_map<int32_t, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndInt32, TYPE(std::unordered_map<int32_t, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndInt64, TYPE(std::unordered_map<int32_t, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndDouble, TYPE(std::unordered_map<int32_t, double>))
DECLARE_TEST_FOR_TYPE(MapOfInt32AndString, TYPE(std::unordered_map<int32_t, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndBool, TYPE(std::unordered_map<int64_t, bool>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndUint8, TYPE(std::unordered_map<int64_t, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndUint16, TYPE(std::unordered_map<int64_t, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndUint32, TYPE(std::unordered_map<int64_t, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndUint64, TYPE(std::unordered_map<int64_t, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndInt16, TYPE(std::unordered_map<int64_t, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndInt32, TYPE(std::unordered_map<int64_t, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndInt64, TYPE(std::unordered_map<int64_t, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndDouble, TYPE(std::unordered_map<int64_t, double>))
DECLARE_TEST_FOR_TYPE(MapOfInt64AndString, TYPE(std::unordered_map<int64_t, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndBool, TYPE(std::unordered_map<uint16_t, bool>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndUint8, TYPE(std::unordered_map<uint16_t, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndUint16, TYPE(std::unordered_map<uint16_t, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndUint32, TYPE(std::unordered_map<uint16_t, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndUint64, TYPE(std::unordered_map<uint16_t, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndInt16, TYPE(std::unordered_map<uint16_t, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndInt32, TYPE(std::unordered_map<uint16_t, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndInt64, TYPE(std::unordered_map<uint16_t, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndDouble, TYPE(std::unordered_map<uint16_t, double>))
DECLARE_TEST_FOR_TYPE(MapOfUint16AndString, TYPE(std::unordered_map<uint16_t, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndBool, TYPE(std::unordered_map<uint32_t, bool>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndUint8, TYPE(std::unordered_map<uint32_t, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndUint16, TYPE(std::unordered_map<uint32_t, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndUint32, TYPE(std::unordered_map<uint32_t, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndUint64, TYPE(std::unordered_map<uint32_t, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndInt16, TYPE(std::unordered_map<uint32_t, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndInt32, TYPE(std::unordered_map<uint32_t, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndInt64, TYPE(std::unordered_map<uint32_t, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndDouble, TYPE(std::unordered_map<uint32_t, double>))
DECLARE_TEST_FOR_TYPE(MapOfUint32AndString, TYPE(std::unordered_map<uint32_t, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndBool, TYPE(std::unordered_map<uint64_t, bool>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndUint8, TYPE(std::unordered_map<uint64_t, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndUint16, TYPE(std::unordered_map<uint64_t, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndUint32, TYPE(std::unordered_map<uint64_t, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndUint64, TYPE(std::unordered_map<uint64_t, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndInt16, TYPE(std::unordered_map<uint64_t, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndInt32, TYPE(std::unordered_map<uint64_t, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndInt64, TYPE(std::unordered_map<uint64_t, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndDouble, TYPE(std::unordered_map<uint64_t, double>))
DECLARE_TEST_FOR_TYPE(MapOfUint64AndString, TYPE(std::unordered_map<uint64_t, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndBool, TYPE(std::unordered_map<double, bool>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndUint8, TYPE(std::unordered_map<double, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndUint16, TYPE(std::unordered_map<double, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndUint32, TYPE(std::unordered_map<double, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndUint64, TYPE(std::unordered_map<double, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndInt16, TYPE(std::unordered_map<double, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndInt32, TYPE(std::unordered_map<double, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndInt64, TYPE(std::unordered_map<double, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndDouble, TYPE(std::unordered_map<double, double>))
DECLARE_TEST_FOR_TYPE(MapOfDoubleAndString, TYPE(std::unordered_map<double, std::string>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndBool, TYPE(std::unordered_map<std::string, bool>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndUint8, TYPE(std::unordered_map<std::string, uint8_t>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndUint16, TYPE(std::unordered_map<std::string, uint16_t>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndUint32, TYPE(std::unordered_map<std::string, uint32_t>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndUint64, TYPE(std::unordered_map<std::string, uint64_t>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndInt16, TYPE(std::unordered_map<std::string, int16_t>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndInt32, TYPE(std::unordered_map<std::string, int32_t>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndInt64, TYPE(std::unordered_map<std::string, int64_t>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndDouble, TYPE(std::unordered_map<std::string, double>))
DECLARE_TEST_FOR_TYPE(MapOfStringAndString, TYPE(std::unordered_map<std::string, std::string>))
DECLARE_TEST_FOR_TYPE(ComplexTuple,
                      TYPE(std::tuple<bool,
                                      uint8_t,
                                      int16_t,
                                      uint16_t,
                                      int32_t,
                                      uint32_t,
                                      int64_t,
                                      uint64_t,
                                      double,
                                      std::string,
                                      std::vector<bool>,
                                      std::vector<uint8_t>,
                                      std::vector<int16_t>,
                                      std::vector<uint16_t>,
                                      std::vector<int32_t>,
                                      std::vector<uint32_t>,
                                      std::vector<int64_t>,
                                      std::vector<uint64_t>,
                                      std::vector<double>,
                                      std::vector<std::string>,
                                      std::unordered_map<uint8_t, bool>,
                                      std::unordered_map<uint8_t, uint8_t>,
                                      std::unordered_map<uint8_t, uint16_t>,
                                      std::unordered_map<uint8_t, uint32_t>,
                                      std::unordered_map<uint8_t, uint64_t>,
                                      std::unordered_map<uint8_t, int16_t>,
                                      std::unordered_map<uint8_t, int32_t>,
                                      std::unordered_map<uint8_t, int64_t>,
                                      std::unordered_map<uint8_t, double>,
                                      std::unordered_map<uint8_t, std::string>,
                                      std::unordered_map<int16_t, bool>,
                                      std::unordered_map<int16_t, uint8_t>,
                                      std::unordered_map<int16_t, uint16_t>,
                                      std::unordered_map<int16_t, uint32_t>,
                                      std::unordered_map<int16_t, uint64_t>,
                                      std::unordered_map<int16_t, int16_t>,
                                      std::unordered_map<int16_t, int32_t>,
                                      std::unordered_map<int16_t, int64_t>,
                                      std::unordered_map<int16_t, double>,
                                      std::unordered_map<int16_t, std::string>,
                                      std::unordered_map<int32_t, bool>,
                                      std::unordered_map<int32_t, uint8_t>,
                                      std::unordered_map<int32_t, uint16_t>,
                                      std::unordered_map<int32_t, uint32_t>,
                                      std::unordered_map<int32_t, uint64_t>,
                                      std::unordered_map<int32_t, int16_t>,
                                      std::unordered_map<int32_t, int32_t>,
                                      std::unordered_map<int32_t, int64_t>,
                                      std::unordered_map<int32_t, double>,
                                      std::unordered_map<int32_t, std::string>,
                                      std::unordered_map<int64_t, bool>,
                                      std::unordered_map<int64_t, uint8_t>,
                                      std::unordered_map<int64_t, uint16_t>,
                                      std::unordered_map<int64_t, uint32_t>,
                                      std::unordered_map<int64_t, uint64_t>,
                                      std::unordered_map<int64_t, int16_t>,
                                      std::unordered_map<int64_t, int32_t>,
                                      std::unordered_map<int64_t, int64_t>,
                                      std::unordered_map<int64_t, double>,
                                      std::unordered_map<int64_t, std::string>,
                                      std::unordered_map<uint16_t, bool>,
                                      std::unordered_map<uint16_t, uint8_t>,
                                      std::unordered_map<uint16_t, uint16_t>,
                                      std::unordered_map<uint16_t, uint32_t>,
                                      std::unordered_map<uint16_t, uint64_t>,
                                      std::unordered_map<uint16_t, int16_t>,
                                      std::unordered_map<uint16_t, int32_t>,
                                      std::unordered_map<uint16_t, int64_t>,
                                      std::unordered_map<uint16_t, double>,
                                      std::unordered_map<uint16_t, std::string>,
                                      std::unordered_map<uint32_t, bool>,
                                      std::unordered_map<uint32_t, uint8_t>,
                                      std::unordered_map<uint32_t, uint16_t>,
                                      std::unordered_map<uint32_t, uint32_t>,
                                      std::unordered_map<uint32_t, uint64_t>,
                                      std::unordered_map<uint32_t, int16_t>,
                                      std::unordered_map<uint32_t, int32_t>,
                                      std::unordered_map<uint32_t, int64_t>,
                                      std::unordered_map<uint32_t, double>,
                                      std::unordered_map<uint32_t, std::string>,
                                      std::unordered_map<uint64_t, bool>,
                                      std::unordered_map<uint64_t, uint8_t>,
                                      std::unordered_map<uint64_t, uint16_t>,
                                      std::unordered_map<uint64_t, uint32_t>,
                                      std::unordered_map<uint64_t, uint64_t>,
                                      std::unordered_map<uint64_t, int16_t>,
                                      std::unordered_map<uint64_t, int32_t>,
                                      std::unordered_map<uint64_t, int64_t>,
                                      std::unordered_map<uint64_t, double>,
                                      std::unordered_map<uint64_t, std::string>,
                                      std::unordered_map<double, bool>,
                                      std::unordered_map<double, uint8_t>,
                                      std::unordered_map<double, uint16_t>,
                                      std::unordered_map<double, uint32_t>,
                                      std::unordered_map<double, uint64_t>,
                                      std::unordered_map<double, int16_t>,
                                      std::unordered_map<double, int32_t>,
                                      std::unordered_map<double, int64_t>,
                                      std::unordered_map<double, double>,
                                      std::unordered_map<double, std::string>,
                                      std::unordered_map<std::string, bool>,
                                      std::unordered_map<std::string, uint8_t>,
                                      std::unordered_map<std::string, uint16_t>,
                                      std::unordered_map<std::string, uint32_t>,
                                      std::unordered_map<std::string, uint64_t>,
                                      std::unordered_map<std::string, int16_t>,
                                      std::unordered_map<std::string, int32_t>,
                                      std::unordered_map<std::string, int64_t>,
                                      std::unordered_map<std::string, double>,
                                      std::unordered_map<std::string, std::string>,
                                      std::tuple<bool,
                                                 uint8_t,
                                                 int16_t,
                                                 uint16_t,
                                                 int32_t,
                                                 uint32_t,
                                                 int64_t,
                                                 uint64_t,
                                                 double,
                                                 std::string,
                                                 std::vector<bool>,
                                                 std::vector<uint8_t>,
                                                 std::vector<int16_t>,
                                                 std::vector<uint16_t>,
                                                 std::vector<int32_t>,
                                                 std::vector<uint32_t>,
                                                 std::vector<int64_t>,
                                                 std::vector<uint64_t>,
                                                 std::vector<double>,
                                                 std::vector<std::string>,
                                                 std::unordered_map<uint8_t, bool>,
                                                 std::unordered_map<uint8_t, uint8_t>,
                                                 std::unordered_map<uint8_t, uint16_t>,
                                                 std::unordered_map<uint8_t, uint32_t>,
                                                 std::unordered_map<uint8_t, uint64_t>,
                                                 std::unordered_map<uint8_t, int16_t>,
                                                 std::unordered_map<uint8_t, int32_t>,
                                                 std::unordered_map<uint8_t, int64_t>,
                                                 std::unordered_map<uint8_t, double>,
                                                 std::unordered_map<uint8_t, std::string>,
                                                 std::unordered_map<int16_t, bool>,
                                                 std::unordered_map<int16_t, uint8_t>,
                                                 std::unordered_map<int16_t, uint16_t>,
                                                 std::unordered_map<int16_t, uint32_t>,
                                                 std::unordered_map<int16_t, uint64_t>,
                                                 std::unordered_map<int16_t, int16_t>,
                                                 std::unordered_map<int16_t, int32_t>,
                                                 std::unordered_map<int16_t, int64_t>,
                                                 std::unordered_map<int16_t, double>,
                                                 std::unordered_map<int16_t, std::string>,
                                                 std::unordered_map<int32_t, bool>,
                                                 std::unordered_map<int32_t, uint8_t>,
                                                 std::unordered_map<int32_t, uint16_t>,
                                                 std::unordered_map<int32_t, uint32_t>,
                                                 std::unordered_map<int32_t, uint64_t>,
                                                 std::unordered_map<int32_t, int16_t>,
                                                 std::unordered_map<int32_t, int32_t>,
                                                 std::unordered_map<int32_t, int64_t>,
                                                 std::unordered_map<int32_t, double>,
                                                 std::unordered_map<int32_t, std::string>,
                                                 std::unordered_map<int64_t, bool>,
                                                 std::unordered_map<int64_t, uint8_t>,
                                                 std::unordered_map<int64_t, uint16_t>,
                                                 std::unordered_map<int64_t, uint32_t>,
                                                 std::unordered_map<int64_t, uint64_t>,
                                                 std::unordered_map<int64_t, int16_t>,
                                                 std::unordered_map<int64_t, int32_t>,
                                                 std::unordered_map<int64_t, int64_t>,
                                                 std::unordered_map<int64_t, double>,
                                                 std::unordered_map<int64_t, std::string>,
                                                 std::unordered_map<uint16_t, bool>,
                                                 std::unordered_map<uint16_t, uint8_t>,
                                                 std::unordered_map<uint16_t, uint16_t>,
                                                 std::unordered_map<uint16_t, uint32_t>,
                                                 std::unordered_map<uint16_t, uint64_t>,
                                                 std::unordered_map<uint16_t, int16_t>,
                                                 std::unordered_map<uint16_t, int32_t>,
                                                 std::unordered_map<uint16_t, int64_t>,
                                                 std::unordered_map<uint16_t, double>,
                                                 std::unordered_map<uint16_t, std::string>,
                                                 std::unordered_map<uint32_t, bool>,
                                                 std::unordered_map<uint32_t, uint8_t>,
                                                 std::unordered_map<uint32_t, uint16_t>,
                                                 std::unordered_map<uint32_t, uint32_t>,
                                                 std::unordered_map<uint32_t, uint64_t>,
                                                 std::unordered_map<uint32_t, int16_t>,
                                                 std::unordered_map<uint32_t, int32_t>,
                                                 std::unordered_map<uint32_t, int64_t>,
                                                 std::unordered_map<uint32_t, double>,
                                                 std::unordered_map<uint32_t, std::string>,
                                                 std::unordered_map<uint64_t, bool>,
                                                 std::unordered_map<uint64_t, uint8_t>,
                                                 std::unordered_map<uint64_t, uint16_t>,
                                                 std::unordered_map<uint64_t, uint32_t>,
                                                 std::unordered_map<uint64_t, uint64_t>,
                                                 std::unordered_map<uint64_t, int16_t>,
                                                 std::unordered_map<uint64_t, int32_t>,
                                                 std::unordered_map<uint64_t, int64_t>,
                                                 std::unordered_map<uint64_t, double>,
                                                 std::unordered_map<uint64_t, std::string>,
                                                 std::unordered_map<double, bool>,
                                                 std::unordered_map<double, uint8_t>,
                                                 std::unordered_map<double, uint16_t>,
                                                 std::unordered_map<double, uint32_t>,
                                                 std::unordered_map<double, uint64_t>,
                                                 std::unordered_map<double, int16_t>,
                                                 std::unordered_map<double, int32_t>,
                                                 std::unordered_map<double, int64_t>,
                                                 std::unordered_map<double, double>,
                                                 std::unordered_map<double, std::string>,
                                                 std::unordered_map<std::string, bool>,
                                                 std::unordered_map<std::string, uint8_t>,
                                                 std::unordered_map<std::string, uint16_t>,
                                                 std::unordered_map<std::string, uint32_t>,
                                                 std::unordered_map<std::string, uint64_t>,
                                                 std::unordered_map<std::string, int16_t>,
                                                 std::unordered_map<std::string, int32_t>,
                                                 std::unordered_map<std::string, int64_t>,
                                                 std::unordered_map<std::string, double>,
                                                 std::unordered_map<std::string, std::string>>>))

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
