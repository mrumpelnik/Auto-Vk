#pragma once
#include <avk/avk.hpp>

namespace avk
{
#if VK_HEADER_VERSION >= 135
	class top_level_acceleration_structure_t
	{
		friend class root;
		
	public:
		top_level_acceleration_structure_t() = default;
		top_level_acceleration_structure_t(top_level_acceleration_structure_t&&) noexcept = default;
		top_level_acceleration_structure_t(const top_level_acceleration_structure_t&) = delete;
		top_level_acceleration_structure_t& operator=(top_level_acceleration_structure_t&&) noexcept = default;
		top_level_acceleration_structure_t& operator=(const top_level_acceleration_structure_t&) = delete;
		~top_level_acceleration_structure_t() = default;

#if VK_HEADER_VERSION >= 162
		const auto& create_info() const	{ return mCreateInfo; }
		auto& create_info()				{ return mCreateInfo; }
		auto acceleration_structure_handle() { return mAccStructure.get(); }
		auto* acceleration_structure_handle_ptr() { return &mAccStructure.get(); }
		auto acceleration_structure_handle() const { return mAccStructure.get(); }
		const auto* acceleration_structure_handle_ptr() const { return &mAccStructure.get(); }
#else
		const auto& create_info() const { return mCreateInfo; }
		auto& create_info() { return mCreateInfo; }
		auto& acceleration_structure_handle() { return mAccStructure.mHandle; }
		auto* acceleration_structure_handle_ptr() { return &mAccStructure.mHandle; }
		auto& memory_handle() { return mMemory.get(); }
		auto* memory_handle_ptr() { return &mMemory.get(); }
		const auto& acceleration_structure_handle() const { return mAccStructure.mHandle; }
		const auto* acceleration_structure_handle_ptr() const { return &mAccStructure.mHandle; }
		const auto& memory_handle() const { return mMemory.get(); }
		const auto* memory_handle_ptr() const { return &mMemory.get(); }
#endif
		auto device_address() const { return mDeviceAddress; }

#if VK_HEADER_VERSION >= 162
		size_t required_acceleration_structure_size() const { return static_cast<size_t>(mMemoryRequirementsForAccelerationStructure); }
		size_t required_scratch_buffer_build_size() const { return static_cast<size_t>(mMemoryRequirementsForBuildScratchBuffer); }
		size_t required_scratch_buffer_update_size() const { return static_cast<size_t>(mMemoryRequirementsForScratchBufferUpdate); }
#else
		size_t required_acceleration_structure_size() const { return static_cast<size_t>(mMemoryRequirementsForAccelerationStructure.memoryRequirements.size); }
		size_t required_scratch_buffer_build_size() const { return static_cast<size_t>(mMemoryRequirementsForBuildScratchBuffer.memoryRequirements.size); }
		size_t required_scratch_buffer_update_size() const { return static_cast<size_t>(mMemoryRequirementsForScratchBufferUpdate.memoryRequirements.size); }
#endif

		const auto& descriptor_info() const
		{
			mDescriptorInfo = vk::WriteDescriptorSetAccelerationStructureKHR{}
				.setAccelerationStructureCount(1u)
				.setPAccelerationStructures(acceleration_structure_handle_ptr());
			return mDescriptorInfo;
		}

		auto descriptor_type() const			{ return vk::DescriptorType::eAccelerationStructureKHR; } 

		/** Build this top level acceleration structure using a vector of geometry instances.
		 *
		 *	@param	aGeometryInstances	Vector of geometry instances that will be used for creating the top-level acceleration structure.
		 *	@param	aScratchBuffer		Optional reference to a buffer to be used as scratch buffer. It must have the buffer usage flags
		 *								vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddressKHR set.
		 *								If no scratch buffer is supplied, one will be created internally.
		 *	@param	aSyncHandler		Sync handler which is to be deprecated
		 */
		void build(const std::vector<geometry_instance>& aGeometryInstances, std::optional<std::reference_wrapper<buffer_t>> aScratchBuffer = {}, sync aSyncHandler = sync::wait_idle());

		/** Update this top level acceleration structure using a vector of geometry instances.
		 *
		 *	@param	aGeometryInstances	Vector of geometry instances that will be used for creating the top-level acceleration structure.
		 *	@param	aScratchBuffer		Optional reference to a buffer to be used as scratch buffer. It must have the buffer usage flags
		 *								vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddressKHR set.
		 *								If no scratch buffer is supplied, one will be created internally.
		 *	@param	aSyncHandler		Sync handler which is to be deprecated
		 */
		void update(const std::vector<geometry_instance>& aGeometryInstances, std::optional<std::reference_wrapper<buffer_t>> aScratchBuffer = {}, sync aSyncHandler = sync::wait_idle());
		
		/** Build this top level acceleration structure using a vector of geometry instances.
		 *
		 *	@param	aGeometryInstancesBuffer	Buffer containing one or multiple geometry instances. The buffer must have the appropriate
		 *										meta data set, which is geometry_instance_buffer_meta.
		 *	@param	aScratchBuffer				Optional reference to a buffer to be used as scratch buffer. It must have the buffer usage flags
		 *										vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddressKHR set.
		 *										If no scratch buffer is supplied, one will be created internally.
		 *	@param	aSyncHandler				Sync handler which is to be deprecated
		 */
		void build(const buffer& aGeometryInstancesBuffer, std::optional<std::reference_wrapper<buffer_t>> aScratchBuffer = {}, sync aSyncHandler = sync::wait_idle());

		/** Build this top level acceleration structure using a vector of geometry instances.
		 *
		 *	@param	aGeometryInstancesBuffer	Buffer containing one or multiple geometry instances. The buffer must have the appropriate
		 *										meta data set, which is geometry_instance_buffer_meta.
		 *	@param	aScratchBuffer				Optional reference to a buffer to be used as scratch buffer. It must have the buffer usage flags
		 *										vk::BufferUsageFlagBits::eRayTracingKHR | vk::BufferUsageFlagBits::eShaderDeviceAddressKHR set.
		 *										If no scratch buffer is supplied, one will be created internally.
		 *	@param	aSyncHandler				Sync handler which is to be deprecated
		 */
		void update(const buffer& aGeometryInstancesBuffer, std::optional<std::reference_wrapper<buffer_t>> aScratchBuffer = {}, sync aSyncHandler = sync::wait_idle());
		
	private:
		enum struct tlas_action { build, update };
		std::optional<command_buffer> build_or_update(const std::vector<geometry_instance>& aGeometryInstances, std::optional<std::reference_wrapper<buffer_t>> aScratchBuffer, sync aSyncHandler, tlas_action aBuildAction);
		std::optional<command_buffer> build_or_update(const buffer& aGeometryInstancesBuffer, std::optional<std::reference_wrapper<buffer_t>> aScratchBuffer, sync aSyncHandler, tlas_action aBuildAction);
		buffer_t& get_and_possibly_create_scratch_buffer();

#if VK_HEADER_VERSION >= 162
		vk::DeviceSize mMemoryRequirementsForAccelerationStructure;
		vk::DeviceSize mMemoryRequirementsForBuildScratchBuffer;
		vk::DeviceSize mMemoryRequirementsForScratchBufferUpdate;
		buffer mAccStructureBuffer;
#else
		vk::MemoryRequirements2KHR mMemoryRequirementsForAccelerationStructure;
		vk::MemoryRequirements2KHR mMemoryRequirementsForBuildScratchBuffer;
		vk::MemoryRequirements2KHR mMemoryRequirementsForScratchBufferUpdate;
		vk::MemoryAllocateInfo mMemoryAllocateInfo;
		vk::UniqueHandle<vk::DeviceMemory, DISPATCH_LOADER_CORE_TYPE> mMemory;
#endif

#if VK_HEADER_VERSION >= 162
		std::vector<vk::AccelerationStructureGeometryKHR> mAccStructureGeometries;
		std::vector<uint32_t> mBuildPrimitiveCounts;
		vk::AccelerationStructureBuildGeometryInfoKHR mBuildGeometryInfo;
#else
		std::vector<vk::AccelerationStructureCreateGeometryTypeInfoKHR> mGeometryInfos;
#endif
		
		vk::AccelerationStructureCreateInfoKHR mCreateInfo;
		vk::BuildAccelerationStructureFlagsKHR mFlags;
		const root* mRoot;
#if VK_HEADER_VERSION >= 162
		//avk::handle_wrapper<vk::AccelerationStructureKHR> mAccStructure;
		vk::UniqueHandle<vk::AccelerationStructureKHR, DISPATCH_LOADER_EXT_TYPE> mAccStructure;
#else
		//vk::ResultValueType<vk::UniqueHandle<vk::AccelerationStructureKHR, DISPATCH_LOADER_EXT_TYPE>>::type mAccStructure;
		avk::handle_wrapper<vk::AccelerationStructureKHR> mAccStructure;
#endif
		vk::DeviceAddress mDeviceAddress = 0;

		std::optional<buffer> mScratchBuffer;
		
		mutable vk::WriteDescriptorSetAccelerationStructureKHR mDescriptorInfo;
	};

	using top_level_acceleration_structure = avk::owning_resource<top_level_acceleration_structure_t>;
#endif
}
