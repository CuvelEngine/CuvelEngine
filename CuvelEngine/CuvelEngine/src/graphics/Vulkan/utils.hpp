#pragma once

template <typename TargetType, typename SourceType>
TargetType checked_cast(SourceType value)
{
	static_assert(sizeof(TargetType) <= sizeof(SourceType), "No need to cast from smaller to larger type!");
	static_assert(std::numeric_limits<SourceType>::is_integer, "Only integer types supported!");
	static_assert(!std::numeric_limits<SourceType>::is_signed, "Only unsigned types supported!");
	static_assert(std::numeric_limits<TargetType>::is_integer, "Only integer types supported!");
	static_assert(!std::numeric_limits<TargetType>::is_signed, "Only unsigned types supported!");
	assert(value <= std::numeric_limits<TargetType>::max());
	return static_cast<TargetType>(value);
}

inline uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties)
{
	// get the first index into queueFamiliyProperties which supports graphics
	auto graphicsQueueFamilyProperty = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
		[](vk::QueueFamilyProperties const& qfp)
		{
			return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
		});
	assert(graphicsQueueFamilyProperty != queueFamilyProperties.end());
	return static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));
}

inline uint32_t findPresentQueueFamilyIndex(
	vk::raii::PhysicalDevice const& physicalDevice,
	uint32_t graphicsQueueFamilyIndex,
	vk::raii::SurfaceKHR const& surface,
	std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties
)
{
	uint32_t presentQueueFamilyIndex = physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, *surface)
		? graphicsQueueFamilyIndex
		: checked_cast<uint32_t>(queueFamilyProperties.size());
	if (presentQueueFamilyIndex == queueFamilyProperties.size())
	{
		// the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both
		// graphics and present
		for (size_t i = 0; i < queueFamilyProperties.size(); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
				physicalDevice.getSurfaceSupportKHR(checked_cast<uint32_t>(i), *surface))
			{
				graphicsQueueFamilyIndex = checked_cast<uint32_t>(i);
				presentQueueFamilyIndex = graphicsQueueFamilyIndex;
				break;
			}
		}
		if (presentQueueFamilyIndex == queueFamilyProperties.size())
		{
			// there's nothing like a single family index that supports both graphics and present -> look for an other
			// family index that supports present
			for (size_t i = 0; i < queueFamilyProperties.size(); i++)
			{
				if (physicalDevice.getSurfaceSupportKHR(checked_cast<uint32_t>(i), *surface))
				{
					presentQueueFamilyIndex = checked_cast<uint32_t>(i);
					break;
				}
			}
		}
	}
	if ((graphicsQueueFamilyIndex == queueFamilyProperties.size()) || (presentQueueFamilyIndex == queueFamilyProperties.size()))
	{
		throw std::runtime_error("Could not find a queue for graphics or present -> terminating");
	}

	return presentQueueFamilyIndex;
}

inline vk::raii::Device makeDevice(
	vk::raii::PhysicalDevice const& physicalDevice,
	uint32_t queueFamilyIndex,
	std::vector<std::string> const& extensions = {},
	vk::PhysicalDeviceFeatures const* physicalDeviceFeatures = nullptr,
	void const* pNext = nullptr)
{
	std::vector<char const*> enabledExtensions;
	enabledExtensions.reserve(extensions.size());
	for (auto const& ext : extensions)
	{
		enabledExtensions.push_back(ext.data());
	}

	float queuePriority = 0.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority);
	vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), deviceQueueCreateInfo, {}, enabledExtensions, physicalDeviceFeatures, pNext);
	return vk::raii::Device(physicalDevice, deviceCreateInfo);
}