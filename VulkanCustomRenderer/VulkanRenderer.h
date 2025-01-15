// Author: Xinjie Zhu
#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <vector>
#include <set>
#include <array>

#include "stb_image.h"

#include "Mesh.h"
#include "MeshModel.h"
#include "Utilities.h"
#include "VulkanValidation.h"


class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();
	void draw();
	void cleanup();
	int init(GLFWwindow* newWindow);

	int createMeshModel(std::string modelFile);

	void updateModel(int modelId, glm::mat4 newModel);

private:
	GLFWwindow* window;
	int currentFrame = 0;

	// Scene Objects
	// a scene can have multiple models 
	// each model with multiple nodes
	// each node with multiple meshes
	std::vector<MeshModel> modelList;

	// Scene Objects
//	std::vector<Mesh> meshList;

	/*
	struct MVP {
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
	} mvp;
	*/
	// Scene Settings
	struct UboViewProjection {
		glm::mat4 projection;
		glm::mat4 view;
	} uboViewProjection;


	struct LightingParameters {
		float La[4];
		float Ld[4];
		float Ls[4];
		float ka[4];
		float kd[4];
		float ks[4];
		float lightDirection[4];
		float alpha;	
		//float padding[3];

		// Constructor for initialization
		LightingParameters()
			: La{ 1.0f, 1.0f, 1.0f, 1.0f },
			Ld{ 1.2f, 1.2f, 1.2f, 1.0f },
			Ls{ 0.3f, 0.3f, 0.3f, 1.0f },
			ka{ 0.1f, 0.1f, 0.1f, 1.0f },
			kd{ 1.0f, 1.0f, 1.0f, 1.0f },
			ks{ 0.2f, 0.2f, 0.2f, 1.0f },
			lightDirection{ 0.0f, 1.0f, 0.0f, 0.0f },
			alpha(1.0f)

			
		{
		}
	};

	LightingParameters lightingParameters;

	//vulkan components
	VkInstance instance;
	VkDebugReportCallbackEXT callback;
	

	struct {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapchain;

	// These three list should be the same length
	std::vector<SwapchainImage> swapChainImages;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkImage depthBufferImage;
	VkDeviceMemory depthBufferImageMemory;
	VkImageView depthBufferImageView;

	VkSampler textureSampler;

	// - Descriptors
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayout lightingDescriptorSetLayout;
	VkDescriptorSetLayout samplerSetLayout; // texture descriptor layout
	VkPushConstantRange pushConstantRange;

	VkDescriptorPool descriptorPool;
	VkDescriptorPool samplerDescriptorPool;

	std::vector<VkDescriptorSet> descriptorSets;// the number of sets is equal to number of swap chain images ( also equal to number of command buffer)
	std::vector<VkDescriptorSet> lightingDescriptorSets;
	std::vector<VkDescriptorSet> samplerDescriptorSets; // the number of sets is equal to number of textures

	std::vector<VkBuffer> vpUniformBuffer;
	std::vector<VkDeviceMemory> vpUniformBufferMemory;

	std::vector<VkBuffer> lightingUniformBuffer;
	std::vector<VkDeviceMemory> lightingUniformBufferMemory;


	std::vector<VkBuffer> modelDUniformBuffer;
	std::vector<VkDeviceMemory> modelDUniformBufferMemory;

	VkDeviceSize minUniformBufferOffset;
	size_t modelUniformAlignment;
	//Model* modelTransferSpace;

	// - Assets
	std::vector<VkImage> textureImages;
	std::vector<VkDeviceMemory> textureImageMemory;
	std::vector<VkImageView> textureImageViews;


	// - Pipeline
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	// - Pools
	VkCommandPool graphicsCommandPool;

	// - Utility
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	// - Synchronisation
	std::vector<VkSemaphore> imageAvailable;
	std::vector<VkSemaphore> renderFinished;
	std::vector<VkFence> drawFences;

	// Vulkan Functions
	// - Create Functions
	void createInstance();
	void createDebugCallback();
	void createLogicalDevice();
	void createSurface();
	void createSwapChain();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createPushConstantRange();
	void createGraphicsPipeline();
	void createDepthBufferImage();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSynchronisation();
	void createTextureSampler();

	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();

	void updateUniformBuffers(uint32_t imageIndex);

	// - Record Functions
	void recordCommands();
	void recordCommands(uint32_t currentImage);

	// - Get Functions
	void getPhysicalDevice();

	// - Allocate Functions
	void allocateDynamicBufferTransferSpace();

	// - Support Functions
	// -- Checker Functions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device);
	bool checkValidationLayerSupport();

	// -- Getter Functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

	// -- Choose Functions
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
	VkFormat chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	// -- Create Functions
	VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags,
		VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	int createTextureImage(std::string fileName);
	int createTexture(std::string fileName);
	int createTextureDescriptor(VkImageView textureImage);

	// -- Loader Functions
	stbi_uc* loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize);

};

