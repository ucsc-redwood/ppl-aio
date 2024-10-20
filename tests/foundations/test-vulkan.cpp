#define VOLK_IMPLEMENTATION
#include "volk.h"
// #include <vulkan/vulkan.h>
#include <gtest/gtest.h>

TEST(VulkanTest, InitializeVulkanInstance) {
  // Initialize Volk
  VkResult volkResult = volkInitialize();
  ASSERT_EQ(volkResult, VK_SUCCESS) << "Failed to initialize Volk.";

  VkInstance instance;

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Test";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Test Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion =
      VK_API_VERSION_1_0;  // Use VK_API_VERSION_1_2 if available

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  // Optionally, check for validation layers here

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
  EXPECT_EQ(result, VK_SUCCESS) << "Failed to create Vulkan instance.";

  if (result == VK_SUCCESS) {
    // Load instance-level Vulkan functions using Volk
    volkLoadInstance(instance);

    vkDestroyInstance(instance, nullptr);
  }
}

TEST(VulkanTest, EnumeratePhysicalDevices) {
  // Initialize Volk
  VkResult volkResult = volkInitialize();
  ASSERT_EQ(volkResult, VK_SUCCESS) << "Failed to initialize Volk.";

  VkInstance instance;

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Test";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Test Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
  ASSERT_EQ(result, VK_SUCCESS) << "Failed to create Vulkan instance.";

  // Load instance-level Vulkan functions using Volk
  volkLoadInstance(instance);

  uint32_t deviceCount = 0;
  result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  EXPECT_EQ(result, VK_SUCCESS) << "Failed to enumerate physical devices.";
  EXPECT_GT(deviceCount, 0u) << "No physical devices found.";

  if (deviceCount > 0) {
    std::vector<VkPhysicalDevice> devices(deviceCount);
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    EXPECT_EQ(result, VK_SUCCESS) << "Failed to enumerate physical devices.";
  }

  vkDestroyInstance(instance, nullptr);
}

TEST(VulkanTest, PhysicalDeviceProperties) {
  // Initialize Volk
  VkResult volkResult = volkInitialize();
  ASSERT_EQ(volkResult, VK_SUCCESS) << "Failed to initialize Volk.";

  VkInstance instance;

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Test";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Test Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
  ASSERT_EQ(result, VK_SUCCESS) << "Failed to create Vulkan instance.";

  // Load instance-level Vulkan functions using Volk
  volkLoadInstance(instance);

  uint32_t deviceCount = 0;
  result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  ASSERT_EQ(result, VK_SUCCESS) << "Failed to enumerate physical devices.";
  ASSERT_GT(deviceCount, 0u) << "No physical devices found.";

  std::vector<VkPhysicalDevice> devices(deviceCount);
  result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
  ASSERT_EQ(result, VK_SUCCESS) << "Failed to enumerate physical devices.";

  for (const auto& device : devices) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    EXPECT_STRNE(deviceProperties.deviceName, "") << "Device name is empty.";
    EXPECT_GT(deviceProperties.limits.maxImageDimension2D, 0u)
        << "Invalid max image dimension.";
  }

  vkDestroyInstance(instance, nullptr);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}