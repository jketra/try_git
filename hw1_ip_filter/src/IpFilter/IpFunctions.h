#pragma once

#include "GenericIp.h"
#include "Restrictions.h"

#include <OperationResult/OperationResult.h>
#include <StringAddOns/StringFunctions.h>

namespace hw1
{

template <typename IpType>
bool validateIp(const IpType& ip)
{
    for (unsigned i = 0; i < IpType::bytesNumber(); ++i)
    {
        if (ip.byte(i) < Restrictions<IpType>::boundaries.min.byte(i) ||
            ip.byte(i) > Restrictions<IpType>::boundaries.max.byte(i))
        {
            return false;
        }
    }

    return true;
}

template <typename IpType>
hw_libs::OperationResult stringToIp(const std::string& ipStr, IpType& ip)
{
    using OperationResult = hw_libs::OperationResult;

    auto bytes = hw_libs::split(ipStr, '.');

    if (bytes.size() == IpType::bytesNumber())
    {
        try
        {
            for (unsigned i = 0; i < IpType::bytesNumber(); ++i)
            {
                ip.byte(i) = std::stoi(bytes[i]);
            }

            return OperationResult::Success();
        }
        catch (std::invalid_argument const& e)
        {
            return std::move(OperationResult::Fail("Bad input: ") << std::string(e.what()) << ": " << ipStr);
        }
        catch (std::out_of_range const& e)
        {
            return std::move(OperationResult::Fail("Integer overflow : ") << e.what() << ": " << ipStr);
        }
    }

    return std::move(OperationResult::Fail("Invalid input: ") << ipStr);
}

}  // namespace hw1
