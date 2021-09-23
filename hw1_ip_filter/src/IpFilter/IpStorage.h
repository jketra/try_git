#pragma once

#include "IpFunctions.h"
#include "Restrictions.h"

#include <OperationResult/OperationResult.h>
#include <TypeTraits/TypeTraits.h>

#include <algorithm>
#include <iterator>
#include <set>
#include <tuple>

/// The second part of the first homework (ip filter).
namespace hw1
{

template <typename IpType>
class IpStorage
{
    using IpRestrictions = Restrictions<IpType>;
    using Byte = typename IpType::Byte;

  public:
    using Ip = IpType;
    using Container = std::multiset<Ip>;
    using Iterator = typename Container::iterator;
    using OperationResult = hw_libs::OperationResult;

    IpStorage() = default;

    OperationResult add(const std::string& ipStr)
    {
        Ip ip;
        auto result = stringToIp(ipStr, ip);

        if (!result)
        {
            return std::move(result);
        }

        return add(std::move(ip));
    }

    template <typename T, typename = hw_libs::sfinae::CheckType<Ip, T>>
    OperationResult addIp(T&& ip)
    {
        if (validateIp(ip))
        {
            m_storage.insert(std::forward<T>(ip));
            return OperationResult::Success();
        }

        return std::move(OperationResult::Fail("Invalid Ip: ") << ip);
    }


    const Container& allIps() const { return m_storage; }

    void partMethod()
    {
        std::vector<IpV4> vet;
        std::copy_if(std::begin(m_storage),
                     std::end(m_storage),
                     std::inserter(result, std::begin(result)),
                     [&byte](const IpV4& ip)
                     {
                         return ip.contains(byte);
                     });
    }

    Container getIpsWithByte(Byte byte) const
    {
        Container result;
        partMethod();

        return std::move(result);
    }

    bool empty() const { return m_storage.empty(); }


    void part2()
    {
        auto boundaries = IpRestrictions::boundaries;
        for (unsigned i = 0; i < filteringBytes.size(); ++i)
        {
            boundaries.min.byte(i) = filteringBytes[i];
            boundaries.max.byte(i) = filteringBytes[i];
        }

        auto begin = std::lower_bound(std::begin(m_storage), std::end(m_storage), boundaries.min);
        auto end = std::upper_bound(std::begin(m_storage), std::end(m_storage), boundaries.max);
    }

    template <typename... Args, typename = hw_libs::sfinae::CheckTypes<Byte, Args...>>
    std::tuple<Iterator, Iterator> filteredByBytes(Byte head, Args... tail)
    {
        static_assert(sizeof...(tail) < Ip::bytesNumber(), "The number of input bytes mustn't exceed 4");

        std::array<Byte, sizeof...(tail) + 1u> filteringBytes{head, tail...};

        part2();
    }

  private:
    Container m_storage;
};

}  // namespace hw1
